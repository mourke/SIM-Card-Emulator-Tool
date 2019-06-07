#include "APDUCommand.h"
#include <QString>


QString APDUCommand::string() const {
	QString string = QString::asprintf("%02x %02x %02x %02x %02x", m_instructionClass, m_instructionCode, firstParameter(), secondParameter(), dataLength());
	
	for (uint8_t character : m_data) {
		string += QString::asprintf(" %02x", character);
	}

	return string;
}