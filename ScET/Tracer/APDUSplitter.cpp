#include "APDUSplitter.h"
#include <iostream>
#include <assert.h>

static const unsigned int MAX_RESPONSE_LENGTH = 256;


void APDUSplitter::incrementState() {
	state = State((int)state + 1);
}

void APDUSplitter::insertByte(uint8_t character) {
	switch (state) {
	case State::Class:
		this->command.m_instructionClass = character;
		incrementState();
		break;
	case State::Instruction:
		this->command.m_instructionCode = character;
		incrementState();
		break;
	case State::Parameter1:
		this->command.m_parameters[0] = character;
		incrementState();
		break;
	case State::Parameter2:
		this->command.m_parameters[1] = character;
		incrementState();
		break;
	case State::CommandLength:
		dataRemaining = character == 0 ? MAX_RESPONSE_LENGTH : character;
		state = StatusByte1;
		break;
	case State::Command:
		this->command.m_data.push_back(character);
		dataRemaining--;
		if (dataRemaining == 0) {
			// TODO: figure out why there's no response
			state = StatusByte1;
		}
		break;
	case State::EstimatedResponseLength:
		dataRemaining = character == 0 ? MAX_RESPONSE_LENGTH : character;
		incrementState();
		break;
	case State::Response:
		if (dataRemaining == 0) incrementState();
		this->response.m_data.push_back(character);
		dataRemaining--;
		break;
	case State::StatusByte1:
		if (character == 0x60) {
			break; // waiting time extension. Do nothing
		} else if (character == command.m_instructionCode ||
			character == (command.m_instructionCode + 1) ||
			character == ~(command.m_instructionCode + 1))
		{
			state = Command; // data will be prepended by some variant of the instruction code
		} else if (character == ~command.m_instructionCode) {
			this->command.m_data.push_back(character);
			dataRemaining--;
		} else {
			this->response.m_status[0] = character; // data empty. character must be SW1
			incrementState();
		}
		break;
	case State::StatusByte2:
		this->response.m_status[1] = character;
		std::invoke(m_callback, command, response);
		reset();
		break;
	}
		
}

void APDUSplitter::reset() {
	state = State::Class;
	this->dataRemaining = 0;
	this->command = APDUCommand();
	this->response = APDUResponse();
}

void APDUSplitter::setBoundary() {
	std::invoke(m_callback, command, response);
	reset();
}

void APDUSplitter::splitInput(uint8_t *buffer, unsigned int bufferSize) {
	while (bufferSize--) {
		insertByte(*buffer++);
	}
}
