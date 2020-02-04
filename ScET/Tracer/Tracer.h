#ifndef TRACER_H
#define TRACER_H

#include <cstdint>
#include <chrono>
#include <optional>
#include <QObject>
#include <libusb.h>

class APDUCommand;
class APDUSplitter;

/** A wrapper class around libusb_device.
  *
  * @b Description
  * This class provides helper functions for interacting
  * with the tracer device.
  *
  */
class Tracer : public QObject {
	Q_OBJECT

signals:
	/**
	 * Emitted when the tracer stops sniffing for whatever
	 * reason. This is only emmitted if the trace session has 
	 * started and then failed. If the trace session hasn't been
	 * successfully started, `startSniffing()` will return the
	 * error code for this. If a non-zero error code is returned, 
	 * the trace session has been stopped due to an error.
	 *
	 * @param errorCode		libusb_transfer_status code if any.
	 */
	void stoppedSniffing(libusb_transfer_status errorCode);

	/**
	 * Emitted when a trace has begun and the SIM card has 
	 * already been sending commands to the tracer, but 
	 * because we have not been listening for them, we find 
	 * ourselves at an arbitrary point in the trace. This may
	 * make the APDUSplitter misbehave and thus return incorrect
	 * values.
	 */
	void traceStartedMidSession();

	/**
	 * Emitted when an APDU command has been
	 * successfully received and split.
	 *
	 * @param output	The output string that should be presented to the user.
	 * @param command	The APDU command.
	 */
	void apduCommandReceived(const QString &output, const APDUCommand &command);

	/**
	 * Emitted when an ATR command has been received from the tracer.
	 *
	 * @param output	The output string that should be presented to the user.
	 */
	void atrCommandReceived(const QString &output);

	/**
	 * Emitted when the tracer communicates with the computer.
	 *
	 * @param input	The raw hexadecimal input from the tracer.
	 */
	void simTraceCommandReceived(const QString &input);

public:
	/**
	 * A boolean value indicating whether the current tracer is 
	 * sniffing SIM requests.
	 *
	 * @retval	`true` if the tracer is currently sniffing.
	 */
	bool isSniffing() const { return sniffing; }

	/**
	 * A boolean value indicating whether the current tracer is
	 * still connected to the system.
	 *
	 * @retval	`true` if the tracer is connected to the system.
	 */
	bool isConnected() const;

	~Tracer();
private:
	/**
	 * Starts sniffing SIM requests on the current tracer.
	 *
	 * @retval	A libusb_error status code. Returns `0` for success.
	 */
	libusb_error startSniffing();

	/** Stops sniffing requests. */
	void stopSniffing();

	Tracer(libusb_device *device = nullptr, libusb_context *context = nullptr);
	Tracer(QObject *parent) = delete; 
	libusb_device *device;
	libusb_context *context;
	libusb_device_handle *handle;
	libusb_transfer *transfer;
	uint8_t interface = 0; // the interface that the handle has claimed
	APDUSplitter *splitter;
	std::optional<std::chrono::time_point<std::chrono::steady_clock>> firstAPDUTime;
	bool sniffing = false;
	uint8_t buffer[16 * 265];

	void finishedSniffing();
	void processInput(const uint8_t *buffer, const int bufferSize);
	
	friend class TracerManager;
	friend class MainWindow;
};


#endif // TRACER_H
