#ifndef APDUSPLITTER_H
#define APDUSPLITTER_H

#include <stdint.h>
#include <QObject>

class ADPUSplitter : public QObject {
	Q_OBJECT

	enum State {
		ClearAll,
		Insert,
		P1,
		P2,
		P3,
		Data,
		DataSingle,
		SW1,
		SW2
	};

Q_SIGNALS:
	void callback(uint8_t *buffer, unsigned int bufferSize, void *userData);

public:
	ADPUSplitter(void *userData = nullptr);

	void reset();
	void input(uint8_t *buffer, unsigned int bufferSize);
	void boundary();

private:
	void *userData;

	State state = ClearAll;
	uint8_t insert = 0;
	unsigned int bufferSize = 0;
	unsigned int dataRemaining = 0;
	uint8_t buffer[(2 << 16)];

	void setState(State newState);
	void insertByte(uint8_t character);
	void appendCharacterToBuffer(uint8_t character);
};

#endif // APDUSPLITTER_H
