#include "Tracer.h"
#include "libusb.h"
#include "SIMTraceUSB.h"
#include <iostream>
#include "APDUSplitter.h"
#include "APDUCommand.h"
#include "APDUResponse.h"
#include <QString>

Tracer::Tracer(libusb_device *device, libusb_context *context) {
	this->device = device;
	this->context = context;
}

Tracer::~Tracer() {
	libusb_unref_device(device); // tracer class is responsable for deallocating the device.
}

int Tracer::startSniffing() {
	int error;
	libusb_device_handle *handle;

	error = libusb_open(device, &handle);
	if (error != LIBUSB_SUCCESS) return error;
	
	libusb_config_descriptor *configDescriptor;
	libusb_get_config_descriptor(device, 0, &configDescriptor);

	uint8_t interface = 0;
	while (true) {
		error = libusb_claim_interface(handle, interface++);
		if (error == LIBUSB_SUCCESS) {
			break;			
		} 

		if (interface == configDescriptor->bNumInterfaces) {
			libusb_free_config_descriptor(configDescriptor);
			goto returnError;
		}
	}

	this->splitter = new APDUSplitter([this](APDUCommand &command, APDUResponse &response) {
		printf("APDU: (HH:mm:ss:SSS:): %s %s\n", command.string().toStdString().c_str(), response.string().toStdString().c_str());
	});

	uint8_t buffer[16 * 265];
	int bufferSize;
	sniffing = true;

	while (sniffing) {
		error = libusb_bulk_transfer(handle, SIMTRACE_IN_BULK, buffer, sizeof(buffer), &bufferSize, 100000);
		if ((error == LIBUSB_SUCCESS || error == LIBUSB_ERROR_TIMEOUT) && bufferSize > 0) {
			processMessage(buffer, bufferSize);
		}
	}

	delete splitter;

	libusb_release_interface(handle, 0);

returnError:
	libusb_close(handle);
	return error;
}

void Tracer::processMessage(uint8_t *buffer, int bufferSize) {
	SIMTraceHeader *header = (SIMTraceHeader *)buffer;

	if (header->totalBufferSize != bufferSize) { // multiple traces have been mangled together, we must separate them
		uint8_t *demangledBuffer = buffer + header->totalBufferSize;
		size_t demangledBufferSize = bufferSize - header->totalBufferSize;
		processMessage(buffer, bufferSize - demangledBufferSize); // process the original trace and then the demangled one
		return processMessage(demangledBuffer, demangledBufferSize); // recursively call this in case there are more than two traces mangled together
	}

	size_t headerSize = sizeof(*header);
	uint8_t *payload = buffer + headerSize; // point to the first element of the data
	int payloadSize = bufferSize - headerSize;

	if (payloadSize <= 0) return;

	switch (header->command) {
	case SIMTraceCommand::Data:
		if (header->flags == SIMTraceFlag::ATR) {
			printf("ATR ");
			printf("APDU: %s\n", hexdump(payload, payloadSize));
		} else if (header->flags == SIMTraceFlag::PPSFiDi) {
			printf("PPS(Fi=%u/Di=%u) \n", header->reset[0], header->reset[1]);
		} else if (header->flags == SIMTraceFlag::WaitTimeExpired) {
			printf("Wait time expired\n");
			splitter->setBoundary();
		} else {
			splitter->splitInput(payload, payloadSize);
			//printf("Raw output: %s\n", hexdump(payload, payloadSize));
		}
		break;
	default:
		printf("unknown simtrace msg type 0x%02x\n", header->command);
		break;
	}
}

void Tracer::stopSniffing() {
	sniffing = false;
}

bool Tracer::isConnected() {
	return false;
}

const char * Tracer::hexdump(const uint8_t *data, unsigned int len) {
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

