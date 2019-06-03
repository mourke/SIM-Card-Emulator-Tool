#include "APDUResponse.h"
#include <QString>
#include <iostream>


QString APDUResponse::string() const {
	static char buffer[UINT16_MAX];
	buffer[0] = '\0';
	for (uint8_t character : m_data) {
		sprintf(buffer + strlen(buffer), "%02x ", character);
	}
	sprintf(buffer, "(%02x %02x)", firstStatusByte(), secondStatusByte());

	buffer[sizeof(buffer) - 1] = '\0';
	return QString(buffer);
}