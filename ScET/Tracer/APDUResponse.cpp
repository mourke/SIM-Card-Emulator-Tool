#include "APDUResponse.h"
#include <QString>


QString APDUResponse::string() const {
	QString string;

	for (uint8_t character : m_data) {
		string += QString::asprintf("%02x ", character);
	}
	string += QString::asprintf("(%02x %02x)", firstStatusByte(), secondStatusByte());

	return string;
}