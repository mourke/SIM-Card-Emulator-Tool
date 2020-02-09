#ifndef SIMTRACE_USB_H
#define SIMTRACE_USB_H

#include "libusb.h"
#include <cstdint>

enum SIMTraceCommand : uint8_t {
	Null,
	Data,
	Reset,
	Stats,
	CommandAPDU
};

enum SIMTraceFlag : uint8_t {
	AnswerToReset = 0x01, // ATR
	WaitTimeExpired = 0x04,
	PPSFiDi = 0x08
};

struct SIMTraceHeader {
	SIMTraceCommand command;
	SIMTraceFlag flags;
	uint8_t reset[2];
	uint16_t sequenceNumber;
	uint16_t offset; 
	uint16_t totalBufferSize; // header size + data size
};


// Product ID of the tracer.
static const uint16_t SIMTRACE_PID = 0x0762;

// Vendor ID of the tracer.
static const uint16_t SIMTRACE_VID = 0x16C0;

// The output endpoint of the tracer.
static const uint16_t SIMTRACE_OUT = (LIBUSB_ENDPOINT_OUT | LIBUSB_TRANSFER_TYPE_ISOCHRONOUS);

// The bulk transfer endpoint of the tracer.
static const uint16_t SIMTRACE_IN_BULK = (LIBUSB_ENDPOINT_IN | LIBUSB_TRANSFER_TYPE_BULK);

// The interrupt transfer endpoint of the tracer.
static const uint16_t SIMTRACE_IN_INTERRUPT = (LIBUSB_ENDPOINT_IN | LIBUSB_TRANSFER_TYPE_INTERRUPT);


#endif /* SIMTRACE_USB_H */
