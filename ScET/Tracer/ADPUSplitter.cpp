/* simtrace - main program for the host PC
 *
 * (C) 2010 by Harald Welte <hwelte@hmw-consulting.de>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 
 *  as published by the Free Software Foundation
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "ADPUSplitter.h"
#include <iostream>
#include <assert.h>

/* wrapper function to catch apdu_buf overflows */
void ADPUSplitter::appendCharacterToBuffer(uint8_t character) {
	assert(bufferSize < sizeof(buffer));
	buffer[bufferSize++] = character;
}

void ADPUSplitter::setState(State newState) {
	if (newState == State::ClearAll) {
		this->bufferSize = 0;
		memset(buffer, 0, sizeof(buffer));
	}

	this->state = newState;
}

void ADPUSplitter::insertByte(uint8_t character) {
	switch (state) {
	case State::Insert:
		insert = character;
	case State::ClearAll:
	case State::P1:
	case State::P2:
		appendCharacterToBuffer(character);
		setState(State((int)state + 1));
		break;
	case State::P3:
		appendCharacterToBuffer(character);
		dataRemaining = character;
		setState(State::SW1);
		break;
	case State::Data:
		appendCharacterToBuffer(character);
		dataRemaining--;
		if (dataRemaining == 0) setState(State::SW1);
		break;
	case State::DataSingle:
		appendCharacterToBuffer(character);
		dataRemaining--;
		setState(State::SW1);
		break;
	case State::SW1:
		if (character == 0x60) { // check for NULL / waiting time extension
			//printf("NULL");
		} else if (character == insert || character == insert + 1 ||
			character == ~(insert + 1))  { // check for 'all remaining' type ACK
			//printf("ACK");
			setState(State::Data);
		} else if (character == ~insert) { // check for 'only next byte' type ACK
			setState(State::DataSingle);
		} else { // must be SW1
			appendCharacterToBuffer(character);
			setState(State::SW2);
		}
		break;
	case State::SW2:
		appendCharacterToBuffer(character);
		//printf("APDU: %s\n", hexdump(as->apdu_buf, as->apdu_len));
		emit callback(buffer, bufferSize, userData);
		setState(State::ClearAll);
		break;
	}
		
}

ADPUSplitter::ADPUSplitter(void *userData) : QObject(Q_NULLPTR) {
	this->userData = userData;
}

void ADPUSplitter::reset() {
	setState(State::ClearAll);
}

void ADPUSplitter::boundary() {
	printf("BOUNDARY\n");
	emit callback(buffer, bufferSize, userData);
	setState(State::ClearAll);
}

void ADPUSplitter::input(uint8_t *buffer, unsigned int bufferSize) {
	while (bufferSize--) {
		insertByte(*buffer++);
	}
}
