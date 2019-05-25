#ifndef TRACER_H
#define TRACER_H

struct libusb_device;
class ADPUSplitter;

#include <QObject>

class Tracer : public QObject {
	Q_OBJECT

public:
	Tracer(libusb_device *device);
	~Tracer();

	int startSniffing();
	void stopSniffing();
	bool isSniffing();
	bool isConnected();

private:
	libusb_device *device;
	ADPUSplitter *splitter;
	bool sniffing = false;
	void processMessage(uint8_t *buffer, int bufferSize);
	const char * hexdump(const void *data, unsigned int len);

private Q_SLOTS:
	void adpuSplit(uint8_t *buffer, unsigned int bufferSize, void *userData);
};


#endif // TRACER_H
