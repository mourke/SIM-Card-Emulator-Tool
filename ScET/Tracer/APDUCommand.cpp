#include "APDUCommand.h"
#include <QString>


QString APDUCommand::string() const {
	QString string = QString::asprintf("%02x %02x %02x %02x %02x", m_instructionClass, m_instructionCode, firstParameter(), secondParameter(), dataLength());
	
	for (uint8_t character : m_data) {
		string += QString::asprintf(" %02x", character);
	}

	string += QString::asprintf(" (%02x %02x)", firstStatusByte(), secondStatusByte());

	return string.toUpper();
}

APDUCommand::Type APDUCommand::type() const {
	return m_instructionCode == 0xC0 ? Response : Command;
}

void APDUCommand::setInstructionCode(const uint8_t instructionCode) {
	m_instructionCode = instructionCode;
	if (type() == Command) { // Response will have the same filetype as the command
		if (instructionCode == 0x88 || instructionCode == 0x82) {
			m_fileType = Authentication;
		} else if (false) { // TODO: figure out what makes it an STK command
			m_fileType = SIMToolkit;
		} else {
			m_fileType = IO;
		}
	}
}

