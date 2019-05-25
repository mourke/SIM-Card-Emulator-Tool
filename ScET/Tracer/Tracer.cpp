#include "Tracer.h"
#include "libusb.h"
#include "SIMTraceUSB.h"
#include <iostream>
#include "ADPUSplitter.h"

Tracer::Tracer(libusb_device *device) : QObject(Q_NULLPTR) {
	this->device = device;
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

	this->splitter = new ADPUSplitter();
	QObject::connect(splitter, SIGNAL(callback(uint8_t *, unsigned int , void *)), this, SLOT(adpuSplit(uint8_t *, unsigned int, void *)));

	uint8_t buffer[16 * 265];
	int bufferSize;
	sniffing = true;

	while (sniffing) {
		error = libusb_bulk_transfer(handle, SIMTRACE_IN_BULK, buffer, sizeof(buffer), &bufferSize, 100000);
		if (error == LIBUSB_SUCCESS) {
			processMessage(buffer, bufferSize);
		}
	}

	delete splitter;

	libusb_release_interface(handle, 0);

returnError:
	libusb_close(handle);
	return error;
}

void Tracer::adpuSplit(uint8_t *buffer, unsigned int bufferSize, void *userData) {
	printf("APDU: %s\n", hexdump(buffer, bufferSize));
}

void Tracer::processMessage(uint8_t *buffer, int bufferSize) {
	SIMTraceHeader *header = (SIMTraceHeader *)buffer;
	uint8_t *payload = buffer += sizeof(*header);
	int payloadSize = bufferSize - sizeof(*header);

	if (payloadSize < 0) return;

	switch (header->command) {
	case SIMTraceCommand::Data:
		if (header->flags == SIMTraceFlag::ATR) {
			printf("ATR ");
			adpuSplit(buffer, bufferSize, nullptr);
		} else if (header->flags == SIMTraceFlag::PPSFiDi) {
			printf("PPS(Fi=%u/Di=%u) ", header->reset[0], header->reset[1]);
		} else if (header->flags == SIMTraceFlag::WaitTimeExpired) {
			splitter->boundary();
		} else {
			splitter->input(payload, payloadSize);
		}
		
		break;
	case SIMTraceCommand::Reset:
		break;
	default:
		printf("unknown simtrace msg type 0x%02x\n", header->command);
		break;
	}
}

void Tracer::stopSniffing() {
	sniffing = false;
}

bool Tracer::isSniffing() {
	return this->sniffing;
}

bool Tracer::isConnected() {
	return false;
}

const char * Tracer::hexdump(const void *data, unsigned int len) {
	static char string[65535];
	unsigned char *d = (unsigned char *)data;
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

