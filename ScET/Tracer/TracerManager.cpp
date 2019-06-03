#include "TracerManager.h"
#include "libusb.h"
#include "SIMTraceUSB.h"
#include "Tracer.h"
#include <cassert>

TracerManager::TracerManager() {
	libusb_init(&this->context);
	assert(context != nullptr && "Failed to initialise libusb library");
}

TracerManager::~TracerManager() {
	libusb_exit(this->context);
	for (Tracer *tracer : tracers) {
		delete tracer;
	}
}

std::optional<Tracer *> TracerManager::findTracer() {
	libusb_device **devices;
	ssize_t numberOfDevices = libusb_get_device_list(context, &devices);
	libusb_device *device;

	int i = 0;
	while ((device = devices[i++]) != nullptr) {
		libusb_device_descriptor deviceDescriptor;
		libusb_get_device_descriptor(device, &deviceDescriptor);
		if (deviceDescriptor.idVendor == SIMTRACE_VID && deviceDescriptor.idProduct == SIMTRACE_PID) {
			libusb_ref_device(device); // stop from being deallocated when free_device_list is called. Tracer will remove this reference when it has finished.
			break;
		}
	}

	libusb_free_device_list(devices, 1);
	
	if (device != nullptr) {
		Tracer *tracer = new Tracer(device, context);
		tracers.push_back(tracer);
		return std::make_optional(tracer);
	}

	return std::nullopt;
}