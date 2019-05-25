#ifndef TRACERMANAGER_H
#define TRACERMANAGER_H

#include <QObject>
#include <optional>

class Tracer;
class libusb_context;

class TracerManager : QObject {
	Q_OBJECT

public:
	static TracerManager & sharedManager() {
		static TracerManager instance;
		return instance;
	}

	// prevent accidental copies
	TracerManager(TracerManager const&) = delete;
	void operator=(TracerManager const&) = delete;

	std::optional<Tracer *>findTracer();

private:
	TracerManager();
	~TracerManager();
	libusb_context *context;

Q_SIGNALS:

	void tracerHotplugChanged(Tracer *tracer, bool connected);
};

#endif // TRACERMANAGER_H
