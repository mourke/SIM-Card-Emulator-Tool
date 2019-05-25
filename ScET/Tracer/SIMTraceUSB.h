#ifndef SIMTRACE_USB_H
#define SIMTRACE_USB_H

#include "libusb.h"
#include <stdint.h>

enum SIMTraceCommand : uint8_t {
	Null,
	Data,
	Reset
};

enum SIMTraceFlag : uint8_t {
	ATR = 0x01,
	WaitTimeExpired = 0x04,
	PPSFiDi = 0x08
};

#define PACK( __Declaration__ ) __pragma( pack(push, 1) ) __Declaration__ __pragma( pack(pop) ) // workaround for packed not being supported on GCC. Fix in VS2019

PACK(struct SIMTraceHeader {
	SIMTraceCommand command;
	SIMTraceFlag flags;
	uint8_t reset[2];
	uint8_t data[0];
});



static const uint16_t SIMTRACE_PID = 0x0762;
static const uint16_t SIMTRACE_VID = 0x16c0;
static const uint16_t SIMTRACE_OUT = (LIBUSB_ENDPOINT_OUT | LIBUSB_TRANSFER_TYPE_ISOCHRONOUS);
static const uint16_t SIMTRACE_IN_BULK = (LIBUSB_ENDPOINT_IN | LIBUSB_TRANSFER_TYPE_BULK);
static const uint16_t SIMTRACE_IN_INTERRUPT = (LIBUSB_ENDPOINT_IN | LIBUSB_TRANSFER_TYPE_INTERRUPT);


#endif /* SIMTRACE_USB_H */
