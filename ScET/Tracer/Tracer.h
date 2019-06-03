#ifndef TRACER_H
#define TRACER_H

#include <cstdint>

struct libusb_device;
class libusb_context;
class APDUSplitter;

/** A wrapper class around libusb_device.
  *
  * @b Description
  * This class provides helper functions for interacting
  * with the tracer device.
  *
  */
class Tracer {

public:
	~Tracer();

	/**
	 * Starts sniffing SIM requests on the current tracer.
	 *
	 * @retval	A libusb_error status code. Returns `0` for success.
	 */
	int startSniffing();

	/** Stops sniffing requests. */
	void stopSniffing();

	/**
	 * A boolean value indicating whether the current tracer is 
	 * sniffing SIM requests.
	 *
	 * @retval	`true` if the tracer is currently sniffing.
	 */
	bool isSniffing() { return sniffing; }

	/**
	 * A boolean value indicating whether the current tracer is
	 * still connected to the system.
	 *
	 * @retval	`true` if the tracer is connected to the system.
	 */
	bool isConnected();

private:
	Tracer(libusb_device *device, libusb_context *context = nullptr);
	libusb_device *device;
	libusb_context *context; // will be freed by manager
	APDUSplitter *splitter;
	bool sniffing = false;
	void processMessage(uint8_t *buffer, int bufferSize);
	const char * hexdump(const uint8_t *data, unsigned int len);

	friend class TracerManager;
};


#endif // TRACER_H
