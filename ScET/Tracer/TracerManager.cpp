#include "TracerManager.h"
#include <libusb.h>
#include "SIMTraceUSB.h"
#include "Tracer.h"
#include <cassert>
#include "APDUCommand.h"
#include "Errors.h"
#include <QApplication>

TracerManager::TracerManager() : QObject(nullptr) {
	int error = libusb_init(&this->context);
	assert(context != nullptr && "Failed to initialise libusb library");
	assert(libusb_has_capability(LIBUSB_CAP_HAS_HOTPLUG) != 0 && "This platform is not supported");
	
	error = libusb_hotplug_register_callback(context, libusb_hotplug_event(LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED |
		LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT), LIBUSB_HOTPLUG_NO_FLAGS, SIMTRACE_VID, SIMTRACE_PID,
		LIBUSB_HOTPLUG_MATCH_ANY, [](libusb_context *context, libusb_device *device, libusb_hotplug_event event, void *userData) -> int {
        TracerManager *self = reinterpret_cast<TracerManager *>(userData);

		if (event == LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED) {
			libusb_ref_device(device); // stop from being deallocated when labda returns
			Tracer *tracer = new Tracer(device, context);
			self->manageTracer(tracer);
			QMetaObject::invokeMethod(QApplication::instance(), [self, tracer]() {
				emit self->tracerConnected(tracer);
			});
		} else {
			for (Tracer *tracer : self->tracers) {
				if (tracer->device == device && self->isManagingTracer(tracer)) {
					QMetaObject::invokeMethod(QApplication::instance(), [self, tracer]() {
						emit self->tracerDisconnected(tracer);
						self->stopManagingTracer(tracer);
					});
				}
			}
		}
		
		return 0;
	}, this, &hotplugHandle);

	assert(error == LIBUSB_SUCCESS && "Failed to create hotplug callback");

    handlingEvents = true;

    libUSBEventsThread = std::thread([this]() {
        while (handlingEvents) {
            int error = libusb_handle_events_completed(context, nullptr);
            if (error != LIBUSB_SUCCESS) {
                printf("Failed to handle events: %s", libUSBErrorToString(static_cast<libusb_error>(error)).toStdString().c_str());
			}
		}
	});
}

TracerManager::~TracerManager() {
    for (Tracer *tracer : tracers) {
        stopManagingTracer(tracer);
    }

    handlingEvents = false;
    libusb_hotplug_deregister_callback(context, hotplugHandle);
    if (libUSBEventsThread.joinable()) libUSBEventsThread.join();

    libusb_exit(this->context);
}

std::optional<Tracer *> TracerManager::findTracer() {
	libusb_device **devices;
    libusb_get_device_list(context, &devices);
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

	QObject::connect(tracer, &Tracer::stoppedSniffing, this, [this, tracer](libusb_transfer_status errorCode) {
		emit tracerStoppedSniffing(tracer, errorCode);
	});

	QObject::connect(tracer, &Tracer::traceStartedMidSession, this, [this, tracer]() {
		emit traceStartedMidSession(tracer);
	});

	QObject::connect(tracer, &Tracer::apduCommandReceived, this, [this, tracer](const QString &output, const APDUCommand &command) {
		emit apduCommandRecieved(tracer, output, command);
	});

	QObject::connect(tracer, &Tracer::atrCommandReceived, this, [this, tracer](const QString &output) {
		emit atrCommandReceived(tracer, output);
	});

	QObject::connect(tracer, &Tracer::simTraceCommandReceived, this, [this, tracer](const QString &input) {
		emit simTraceCommandReceived(tracer, input);
	});
}

void TracerManager::stopManagingTracer(Tracer *tracer) {
	if (!isManagingTracer(tracer)) {
		throw std::invalid_argument("The tracer passed in is not managed by this manager.");
	}

    unsigned long int index;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wsometimes-uninitialized" // we have already made sure that the Manager is managing the tracer so index will always be assigned a value in the for loop

    for (unsigned long int i = 0; i < tracers.size(); ++i) {
        if (tracer == tracers[i]) {
            index = i;
            break;
        }
    }

#pragma clang diagnostic pop

	tracers.erase(tracers.begin() + index);
	QObject::disconnect(tracer);
	delete tracer;
}

bool TracerManager::isManagingTracer(Tracer *tracer) {
	return std::find(tracers.begin(), tracers.end(), tracer) != tracers.end();
}

libusb_error TracerManager::startSniffing(Tracer *tracer) {
	libusb_error error = tracer->startSniffing();
	if (error == LIBUSB_SUCCESS) {
		emit tracerStartedSniffing(tracer);
	}
	return error;
}

void TracerManager::stopSniffing(Tracer *tracer) {
	tracer->stopSniffing();
}
