#include "Tracer.h"
#include "libusb.h"
#include "SIMTraceUSB.h"
#include <iostream>
#include "APDUSplitter.h"
#include "APDUCommand.h"
#include "APDUResponse.h"
#include <QString>


// The max value in milliseconds that the transfer will wait before failing. (100 seconds)
static const unsigned int BULK_TRANSFER_TIMEOUT = 100000;

Tracer::Tracer(libusb_device *device, libusb_context *context) {
	this->device = device;
	this->context = context;
}

Tracer::~Tracer() {
	libusb_unref_device(device); // TracerManager has increased the reference count of the device and we need to decrement it
}


const char * hexdump(const uint8_t *data, unsigned int len) {
	static char string[UINT16_MAX];
	uint8_t *d = (uint8_t *)data;
	unsigned int i, left, ofs;

	string[0] = '\0';
	ofs = snprintf(string, sizeof(string) - 1, "(%u): ", len);

	left = sizeof(string) - ofs;
	for (i = 0; len--; i += 3) {
		if (i >= sizeof(string) - 4)
			break;
		snprintf(string + ofs + i, 4, " %02x", *d++);
	}
	string[sizeof(string) - 1] = '\0';
	return string;
}

int Tracer::startSniffing() {
	int error;

	error = libusb_open(device, &handle);
	if (error != LIBUSB_SUCCESS) return error;
	
	libusb_config_descriptor *configDescriptor;
	libusb_get_config_descriptor(device, 0, &configDescriptor);

	interface = 0;
	while (true) {
		error = libusb_claim_interface(handle, interface++);
		if (error == LIBUSB_SUCCESS) {
			break;			
		} 

		if (interface == configDescriptor->bNumInterfaces) {
			libusb_free_config_descriptor(configDescriptor);
			libusb_close(handle);
			return error;
		}
	}

	libusb_free_config_descriptor(configDescriptor);

	this->splitter = new APDUSplitter([this](APDUCommand &command, APDUResponse &response) {
		auto start = firstAPDUTime.value();
		auto end = std::chrono::high_resolution_clock::now();

		auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
		int milliseconds = microseconds / 1000;
		microseconds = microseconds - (milliseconds * 1000);
		int seconds = milliseconds / 1000;
		milliseconds = milliseconds - (seconds * 1000);
		int minutes = seconds / 60;
		seconds = seconds - (minutes * 60);
		int hours = minutes / 60;
		minutes = minutes - (hours * 60);

		printf("APDU:\t(%02d:%02d:%02d:%03d:%03lld): %s %s\n", hours, minutes, seconds, milliseconds, microseconds, command.string().toStdString().c_str(), response.string().toStdString().c_str());
	});

	uint8_t buffer[16 * 265];

	transfer = libusb_alloc_transfer(0);
	libusb_fill_bulk_transfer(transfer, handle, SIMTRACE_IN_BULK, buffer, sizeof(buffer), [](libusb_transfer *transfer) {
		Tracer *self = (Tracer *)transfer->user_data;

		switch (transfer->status) {
		case LIBUSB_TRANSFER_COMPLETED:
			self->processInput(transfer->buffer, transfer->actual_length);
		case LIBUSB_TRANSFER_TIMED_OUT: {
			int error = libusb_submit_transfer(transfer); // submit next transfer
			if (error == LIBUSB_SUCCESS) break;
		}
		case LIBUSB_TRANSFER_CANCELLED:
		case LIBUSB_TRANSFER_ERROR:
			self->sniffing = false;
			break;
		}
	}, this, BULK_TRANSFER_TIMEOUT);

	error = libusb_submit_transfer(transfer);
	sniffing = true;

	if (error != LIBUSB_SUCCESS) {
		stopSniffing();
		finishedSniffing();
		return error;
	}
	

	eventsThread = std::thread([this]() {
		while (sniffing) {
			int error = libusb_handle_events_completed(context, nullptr);
			if (error != LIBUSB_SUCCESS) break;
		}
		finishedSniffing();
	});
	
	return error;
}

void Tracer::processInput(uint8_t *buffer, int bufferSize) {
	SIMTraceHeader *header = (SIMTraceHeader *)buffer;

	switch (header->command) {
	case SIMTraceCommand::Data: {
		if (header->flags & SIMTraceFlag::PPSFiDi) {
			printf("PPS(Fi=%u/Di=%u) \n", header->reset[0], header->reset[1]);
		}

		if (header->flags & SIMTraceFlag::WaitTimeExpired) {
			printf("Wait time expired\n"); // TODO: fix this idk if discarding is the best thing to do.
			splitter->setBoundary();
			break;
		}

		if (header->totalBufferSize != bufferSize) { // multiple traces have been mangled together, we must separate them
			uint8_t *demangledBuffer = buffer + header->totalBufferSize;
			size_t demangledBufferSize = bufferSize - header->totalBufferSize;
			processInput(buffer, bufferSize - demangledBufferSize); // process the original trace and then the demangled one
			return processInput(demangledBuffer, demangledBufferSize); // recursively call this in case there are more than two traces mangled together
		}

		size_t headerSize = sizeof(*header);
		uint8_t *payload = buffer + headerSize; // point to the first element of the data
		int payloadSize = bufferSize - headerSize;

		if (payloadSize <= 0) break;

		if (!firstAPDUTime.has_value()) {
			firstAPDUTime = std::make_optional(std::chrono::high_resolution_clock::now());
			if (header->sequenceNumber > 10) { // first sequence number is never 0 it's usually around 6 so we say 10 just to be sure.
				// TODO: Add warning about starting a trace from the middle
				printf("WARNING: Attaching to a previously runnning trace may yeild unexpected results.\n");
			}
		}

		if (header->flags & SIMTraceFlag::ATR) {
			printf("ATR:\t(00:00:00:000:000): %s\n", hexdump(payload, payloadSize));
			break;
		}

		splitter->splitInput(payload, payloadSize);
		//printf("Raw output: %s\n", hexdump(payload, payloadSize));
		break;
	}
	default:
		printf("Unknown SIMTraceCommand Type 0x%02x\n", header->command);
		break;
	}
}

void Tracer::stopSniffing() {
	if (sniffing == false) return;

	sniffing = false;
	if (eventsThread.joinable()) {
		eventsThread.join(); // Finished sniffing is called by this thread
	}
}

void Tracer::finishedSniffing() {
	if (transfer->status != LIBUSB_TRANSFER_CANCELLED) libusb_cancel_transfer(transfer);
	libusb_free_transfer(transfer);
	libusb_release_interface(handle, interface);
	libusb_close(handle);
	delete splitter;
	firstAPDUTime.reset();
}

bool Tracer::isConnected() {
	return false;
}


