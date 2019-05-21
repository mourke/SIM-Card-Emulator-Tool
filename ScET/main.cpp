#include "Windows/MainWindow.h"
#include <QtWidgets/QApplication>
#include "libusb.h"


// TODO get actual PID and VID of tracer
static const uint16_t PID;
static const uint16_t VID;

static void printDevices(libusb_device **devices) {
	libusb_device *device;
	uint8_t portNumbers[8];

	int i = 0;
	while ((device = devices[i++]) != NULL) {
		libusb_device_descriptor deviceDescriptor;
		libusb_get_device_descriptor(device, &deviceDescriptor);
		printf("%04x:%04x (bus %d, device %d)", deviceDescriptor.idVendor, deviceDescriptor.idProduct,
			libusb_get_bus_number(device), libusb_get_device_address(device));
		int portNumbersSize = libusb_get_port_numbers(device, portNumbers, sizeof(portNumbers));
		if (portNumbersSize != LIBUSB_ERROR_OVERFLOW) {
			printf("path: %d", portNumbers[0]);
			for (int i = 1; i < portNumbersSize; ++i) {
				printf(".%d", portNumbers[i]);
			}
		}
		
		printf("\n");
	}
}

static libusb_device * getTracer(libusb_device **devices) {
	libusb_device *device;

	int i = 0;
	while ((device = devices[i++]) != nullptr) {
		libusb_device_descriptor deviceDescriptor;
		libusb_get_device_descriptor(device, &deviceDescriptor);
		if (deviceDescriptor.idVendor == VID && deviceDescriptor.idProduct == PID) {
			break;
		}
	}
	return device;
}

static bool isTracerConnected(libusb_device **devices) {
	return getTracer(devices) != nullptr;
}

int main(int argc, char *argv[]) {
	QApplication application(argc, argv);

	QCoreApplication::setOrganizationName("CardCentric");
	QCoreApplication::setOrganizationDomain("com.cardcentric");
	QCoreApplication::setApplicationName("ScET");
	
	MainWindow *mainWindow = new MainWindow();
	mainWindow->show();

	libusb_context *context;
	libusb_init(&context);

	if (context != nullptr) {
		libusb_device **devices;
		ssize_t numberOfDevices = libusb_get_device_list(context, &devices);
		printDevices(devices);
		if (isTracerConnected(devices)) {
			libusb_device *tracer = getTracer(devices);
			libusb_device_handle *handle = libusb_open_device_with_vid_pid(context, VID, PID);
			if (handle != nullptr) {
				// do stuff with tracer
			}
		}

		libusb_free_device_list(devices, 1);
		libusb_exit(context);
	}

	return application.exec();
}
