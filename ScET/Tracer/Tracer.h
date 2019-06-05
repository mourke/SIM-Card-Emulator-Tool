#ifndef TRACER_H
#define TRACER_H

#include <cstdint>
#include <chrono>
#include <thread>
#include <optional>

struct libusb_device;
struct libusb_device_handle;
struct libusb_transfer;
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
	~Tracer();
	Tracer(libusb_device *device, libusb_context *context = nullptr);
	libusb_device *device;
	libusb_context *context; // will be freed by manager
	libusb_device_handle *handle;
	libusb_transfer *transfer;
	uint8_t interface = 0; // the interface that the handle has claimed
	APDUSplitter *splitter;
	std::optional<std::chrono::time_point<std::chrono::steady_clock>> firstAPDUTime;
	std::thread eventsThread;
	bool sniffing = false;

	void finishedSniffing();
	void processInput(uint8_t *buffer, int bufferSize);
	
	friend class TracerManager;
};


#endif // TRACER_H
