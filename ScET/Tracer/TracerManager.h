#ifndef TRACERMANAGER_H
#define TRACERMANAGER_H

#include <optional>
#include <vector>

class Tracer;
class libusb_context;

/** A manager class for all tracers connected to
  * the system.
  *
  * @b Description
  * The manager class will send notifications when  
  * tracers are hotplugged.
  *
  */
class TracerManager {

public:
	/** The shared manager instance */
	static TracerManager & sharedManager() {
		static TracerManager instance;
		return instance;
	}

	// prevent accidental copies
	TracerManager(TracerManager const&) = delete;
	void operator=(TracerManager const&) = delete;

	/**
	 * This function finds a tracer connected to the system
	 * and wraps the libusb_device in the `Tracer` class.
	 *
	 * @retval	A Tracer object if a tracer is found. Otherwise,
	 *			an empty optional.
	 */
	std::optional<Tracer *>findTracer();

private:
	TracerManager();
	~TracerManager();
	std::vector<Tracer *>tracers;
	libusb_context *context;
};

#endif // TRACERMANAGER_H
