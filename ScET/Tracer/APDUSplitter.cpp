#include "APDUSplitter.h"
#include <iostream>
#include <assert.h>

static const unsigned int MAX_RESPONSE_LENGTH = 256;


void APDUSplitter::incrementState() {
    state = State(state + 1);
}

void APDUSplitter::insertByte(uint8_t character) {
	switch (state) {
	case State::Class:
		start = std::make_optional(std::chrono::high_resolution_clock::now());
		this->command.m_instructionClass = character;
		incrementState();
		break;
	case State::Instruction:
		this->command.setInstructionCode(character);
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
	case State::Data:
		this->command.m_data.push_back(character);
		dataRemaining--;
		if (dataRemaining == 0) {
			state = StatusByte1;
		}
		break;
	case State::StatusByte1:
		if (character == 0x60) {
			break; // waiting time extension. Do nothing
		} else if (character == command.m_instructionCode ||
			character == (command.m_instructionCode + 1) ||
			character == ~(command.m_instructionCode + 1))
		{
			state = Data; // data will be prepended by some variant of the instruction code
		} else if (character == ~command.m_instructionCode) {
			this->command.m_data.push_back(character);
			dataRemaining--;
		} else {
			this->command.m_status[0] = character; // data empty. character must be SW1
			incrementState();
		}
		break;
	case State::StatusByte2:
		this->command.m_status[1] = character;
		this->command.updateApplicationMap();
        std::invoke(m_callback, command, *start);
		reset();
		break;
	}
		
}

void APDUSplitter::reset() {
	state = State::Class;
	this->command.m_data.clear();
	start.reset();
}

void APDUSplitter::splitInput(const uint8_t *buffer, const int bufferSize) {
    for (int i = 0; i < bufferSize; ++i) {
		insertByte(buffer[i]);
	}
}
