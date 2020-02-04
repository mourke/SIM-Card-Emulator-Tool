#ifndef ERRORS_H
#define ERRORS_H

#include <libusb.h>

class QString;

QString libUSBErrorToString(libusb_error);

#endif // ERRORS_H
