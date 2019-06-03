#include "APDUCommand.h"
#include <QString>
#include <iostream>
#include <cstdint>


QString APDUCommand::string() const {
	static char buffer[UINT16_MAX];
	buffer[0] = '\0';

	sprintf(buffer, "%02x %02x %02x %02x %02x", m_instructionClass, m_instructionCode, firstParameter(), secondParameter(), dataLength());
	for (uint8_t character : m_data) {
		sprintf(buffer + strlen(buffer), " %02x", character);
	}

	buffer[sizeof(buffer) - 1] = '\0';
	return QString(buffer);
}