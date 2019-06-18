#ifndef ERRORS_H
#define ERRORS_H

class QString;
enum libusb_error;

QString libUSBErrorToString(libusb_error);

#endif // ERRORS_H