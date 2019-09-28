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

void APDUCommand::setInstructionCode(const uint8_t instructionCode) {
	m_instructionCode = instructionCode;
	if (instructionCode != GetResponse) { // Response will have the same filetype as the command
		switch (instructionCode) {
		case RunGSMAlgorithm:
			m_type = Authentication;
			break;
		case TerminalProfile:
		case Envelope:
		case Fetch:
		case TerminalResponse:
			m_type = SIMToolkit;
			break;
		case Select:
		case Status:
		case ReadBinary:
		case UpdateBinary:
		case ReadRecord:
		case UpdateRecord:
		case Seek:
		case Increase:
		case Invalidate:
		case Rehabilitate:
		case GetResponse:
		case VerifyCHV:
		case ChangeCHV:
		case DisableCHV:
		case EnableCHV:
		case UnblockCHV:
			m_type = IO;
			break;
		default:
			m_type = Unknown;
		}
	}
}

