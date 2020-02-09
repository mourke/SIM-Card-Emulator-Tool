#include "Errors.h"
#include <QString>
#include <QObject>

QString libUSBErrorToString(libusb_error error) {
	QString errorReason;
	switch (error) {
	case LIBUSB_ERROR_IO:
		errorReason = QObject::tr("Input/output error");
		break;
	case LIBUSB_ERROR_INVALID_PARAM:
		errorReason = QObject::tr("Invalid parameter");
		break;
	case LIBUSB_ERROR_ACCESS:
		errorReason = QObject::tr("Access denied (insufficient permissions)");
		break;
	case LIBUSB_ERROR_NO_DEVICE:
		errorReason = QObject::tr("No such device (it may have been disconnected)");
		break;
	case LIBUSB_ERROR_NOT_FOUND:
		errorReason = QObject::tr("Entity not found");
		break;
	case LIBUSB_ERROR_BUSY:
		errorReason = QObject::tr("Resource busy");
		break;
	case LIBUSB_ERROR_TIMEOUT:
		errorReason = QObject::tr("Operation timed out");
		break;
	case LIBUSB_ERROR_OVERFLOW:
		errorReason = QObject::tr("Overflow");
		break;
	case LIBUSB_ERROR_PIPE:
		errorReason = QObject::tr("Pipe error");
		break;
	case LIBUSB_ERROR_INTERRUPTED:
		errorReason = QObject::tr("System call interrupted (perhaps due to signal)");
		break;
	case LIBUSB_ERROR_NO_MEM:
		errorReason = QObject::tr("Insufficient memory");
		break;
	case LIBUSB_ERROR_NOT_SUPPORTED:
		errorReason = QObject::tr("Operation not supported or unimplemented on this platform");
		break;
	case LIBUSB_ERROR_OTHER:
		errorReason = QObject::tr("An unknown error occurred");
		break;
	case LIBUSB_SUCCESS:
		throw std::invalid_argument("libUSB command completed successfully. There is no error code");
	}
	return errorReason;
}
