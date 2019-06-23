#include "Tracer.h"
#include <libusb.h>
#include "SIMTraceUSB.h"
#include <iostream>
#include "APDUSplitter.h"
#include "APDUCommand.h"
#include <QString>
#include <QApplication>


// The max value in milliseconds that the transfer will wait before failing. (100 seconds)
static const unsigned int BULK_TRANSFER_TIMEOUT = 100000;

Tracer::Tracer(libusb_device *device, libusb_context *context) : QObject(nullptr), device(device), context(context) {
}

Tracer::~Tracer() {
	libusb_unref_device(device); // TracerManager has increased the reference count of the device and we need to decrement it
}

libusb_error Tracer::startSniffing() {
	int error;

	error = libusb_open(device, &handle);
	if (error != LIBUSB_SUCCESS) return (libusb_error)error;
	
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
			return (libusb_error)error;
		}
	}

	libusb_free_config_descriptor(configDescriptor);

	this->splitter = new APDUSplitter([this](APDUCommand &command, std::chrono::steady_clock::time_point &end) {
		auto start = firstAPDUTime.value();

		unsigned long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
		unsigned long long milliseconds = microseconds / 1000;
		microseconds = microseconds - (milliseconds * 1000);
		unsigned long long seconds = milliseconds / 1000;
		milliseconds = milliseconds - (seconds * 1000);
		unsigned long long minutes = seconds / 60;
		seconds = seconds - (minutes * 60);
		unsigned long long hours = minutes / 60;
		minutes = minutes - (hours * 60);

		QString output = QString::asprintf("APDU: (%02lld:%02lld:%02lld:%03lld:%03lld): ", hours, minutes, seconds, milliseconds, microseconds);
		output += command.string();

		QMetaObject::invokeMethod(QApplication::instance(), [this, output, command]() {
			emit apduCommandReceived(output, command);
		});
		
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
		default:
			QMetaObject::invokeMethod(QApplication::instance(), [self]() {
				self->sniffing = false;
				self->finishedSniffing();
			});
			
			break;
		}
	}, this, BULK_TRANSFER_TIMEOUT);

	error = libusb_submit_transfer(transfer);
	sniffing = true;

	if (error != LIBUSB_SUCCESS) {
		stopSniffing();
		finishedSniffing();
		return (libusb_error)error;
	}
	
	return (libusb_error)error;
}

void Tracer::processInput(uint8_t *buffer, int bufferSize) {
	SIMTraceHeader *header = (SIMTraceHeader *)buffer;

	switch (header->command) {
	case SIMTraceCommand::Data: {
		if (header->flags & SIMTraceFlag::PPSFiDi) {
			printf("PPS(Fi=%u/Di=%u) \n", header->reset[0], header->reset[1]);
		}

		if (header->flags & SIMTraceFlag::WaitTimeExpired) {
			printf("Wait time expired\n");
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
				QMetaObject::invokeMethod(QApplication::instance(), [this]() {
					emit traceStartedMidSession();
				});
			}
		}

		if (header->flags & SIMTraceFlag::AnswerToReset) {
			QString string;
			uint8_t *data = payload;
			for (int i = 0; i < payloadSize; ++i) {
				string += QString::asprintf("%02x ", *data++);
			}

			QString atr = QString::asprintf("ATR:  (00:00:00:000:000): ") + string.toUpper();
			QMetaObject::invokeMethod(QApplication::instance(), [this, atr]() {
				emit atrCommandReceived(atr);
			});
			break;
		}

		splitter->splitInput(payload, payloadSize);
		break;
	}
	default:
		printf("Unknown SIMTraceCommand Type 0x%02x\n", header->command);
		break;
	}
}

void Tracer::stopSniffing() {
	if (sniffing == false) return;

	libusb_cancel_transfer(transfer);
}

void Tracer::finishedSniffing() {
	libusb_transfer_status status = transfer->status;
	libusb_free_transfer(transfer);
	libusb_release_interface(handle, interface);
	libusb_close(handle);
	delete splitter;
	firstAPDUTime.reset();
	emit stoppedSniffing(status);
}

bool Tracer::isConnected() const {
	libusb_device_handle *handle;
	if (libusb_open(device, &handle) != LIBUSB_ERROR_NO_DEVICE) { // libusb_device->attached is hidden. we must work around with this hack
		libusb_close(handle);
		return true;
	}
	return false;
}

