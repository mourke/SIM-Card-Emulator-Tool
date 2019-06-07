#include "TracerManager.h"
#include "libusb.h"
#include "SIMTraceUSB.h"
#include "Tracer.h"
#include <cassert>
#include "APDUCommand.h"
#include "APDUResponse.h"

TracerManager::TracerManager() : QObject(nullptr) {
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
		manageTracer(tracer);
		return std::make_optional(tracer);
	}

	return std::nullopt;
}

void TracerManager::manageTracer(Tracer *tracer) {
	if (isManagingTracer(tracer)) return;
	tracers.push_back(tracer);

	auto connection = QObject::connect(tracer, &Tracer::stoppedSniffing, this, [this, tracer](libusb_transfer_status errorCode) {
		emit tracerStoppedSniffing(tracer, errorCode);
	});

	QObject::connect(tracer, &Tracer::traceStartedMidSession, this, [this, tracer]() {
		emit traceStartedMidSession(tracer);
	});

	QObject::connect(tracer, &Tracer::apduCommandReceived, this, [this, tracer](const QString &output, const APDUCommand &command, const APDUResponse &response) {
		emit apduCommandRecieved(tracer, output, command, response);
	});

	QObject::connect(tracer, &Tracer::atrCommandReceived, this, [this, tracer](const QString &output) {
		emit atrCommandReceived(tracer, output);
	});
}

void TracerManager::stopManagingTracer(Tracer *tracer) {
	if (!isManagingTracer(tracer)) {
		throw std::invalid_argument("The tracer passed in is not managed by this manager.");
	}

	int index;

	for (int i = 0; i < tracers.size(); ++i) {
		if (tracer == tracers[i]) {
			index = i;
		}
	}

	tracers.erase(tracers.begin() + index);
	QObject::disconnect(tracer);
}

bool TracerManager::isManagingTracer(Tracer *tracer) {
	return std::find(tracers.begin(), tracers.end(), tracer) != tracers.end();
}

int TracerManager::startSniffing(Tracer *tracer) {
	int error = tracer->startSniffing();
	if (error < 0) {
		emit tracerStartedSniffing(tracer);
	}
	return error;
}

void TracerManager::stopSniffing(Tracer *tracer) {
	tracer->stopSniffing();
}