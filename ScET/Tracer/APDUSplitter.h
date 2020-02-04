#ifndef APDUSPLITTER_H
#define APDUSPLITTER_H

#include <cstdint>
#include <functional>
#include "APDUCommand.h"
#include <chrono>
#include <optional>

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
	  * has been successfully split into an APDU command.
	  */
	typedef std::function<void(APDUCommand, std::chrono::steady_clock::time_point)> Callback;

    APDUSplitter(Callback callback) : m_callback(callback) {}
	
	/** Splits an input buffer recieved from a tracer into an
	  * APDU command. The callback function will be called 
	  * once the command and response have been split correctly
	  * from the input buffers passed into this function.
	  *
	  * @param buffer		A pointer to the first element of the buffer.
	  * @param bufferSize	The size of the buffer.
	  */
    void splitInput(const uint8_t *buffer, const int bufferSize);

	/**
	  * Resets the splitter back to the start. i.e. next expected state will be CLA
	  */
	void reset();

private:
	enum State {
		////////////
		// HEADER //
		////////////
		Class, // CLA
		Instruction, // INS
		Parameter1, // P1
		Parameter2, // P2
		CommandLength, // Lc

		//////////
		// DATA	//
		//////////
		Data,

		//////////
		//	SW	//
		//////////
		StatusByte1, // SW1
		StatusByte2 // SW2
	};

	

	Callback m_callback;
	APDUCommand command;
	State state = Class;
	unsigned int dataRemaining = 0;
	std::optional <std::chrono::steady_clock::time_point> start;

	void incrementState();
	void insertByte(uint8_t character);
};

#endif // APDUSPLITTER_H
