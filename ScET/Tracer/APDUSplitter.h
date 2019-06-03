#ifndef APDUSPLITTER_H
#define APDUSPLITTER_H

#include <cstdint>
#include <functional>
#include "APDUCommand.h"
#include "APDUResponse.h"

/** A class that splits input from a tracer into 
  * correct APDU commands and responses.
  *
  * @b Description
  * This class allows the combining oif APDU commands
  * across multiple input buffers recieved from the
  * tracer or the splitting of multiple APDU commands
  * received from the tracer.
  *
  */
class APDUSplitter {

public:

	/** The function called after an input from the tracer 
	  * has been successfully split into a command and response
	  * pair.
	  */
	typedef std::function<void(APDUCommand &, APDUResponse &)> Callback;

	APDUSplitter(Callback callback) : m_callback(callback) {};
	
	/** Splits an input buffer recieved from a tracer into an
	  * APDU command response pair. The callback function will
	  * be called once the command and response have been split
	  * correctly from the input buffers passed into this function.
	  *
	  * @param buffer		A pointer to the first element of the buffer.
	  * @param bufferSize	The size of the buffer.
	  */
	void splitInput(uint8_t *buffer, unsigned int bufferSize);

	/** Stops splitting input no matter what stage, forces
	  * a call to the callback and resets the splitting process.
	  *
	  * Call this function if the wait time for a tracer transger
	  * has expired.
	  */
	void setBoundary();

private:
	enum State {
		Class, // CLA
		Instruction, // INS
		Parameter1, // P1
		Parameter2, // P2
		CommandLength, // Lc
		Command,
		EstimatedResponseLength, // Le
		Response,
		StatusByte1, // SW1
		StatusByte2 // SW2
	};

	Callback m_callback;
	APDUCommand command;
	APDUResponse response;
	State state = Class;
	uint8_t insert = 0;
	unsigned int dataRemaining = 0;

	void reset();
	void incrementState();
	void insertByte(uint8_t character);
};

#endif // APDUSPLITTER_H
