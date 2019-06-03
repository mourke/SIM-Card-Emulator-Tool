#ifndef APDU_COMMAND_H
#define APDU_COMMAND_H

#include <vector>

class QString;

/** A wrapper around ISO 7816-4 APDU commands.
  *
  * @b Description
  * This class provides helper functions for interacting
  * with APDU commands.
  */
class APDUCommand {

public:

	/** Class of instruction.
	  * 
	  * @retval	CLA.
	  */
	uint8_t instructionClass() const { return m_instructionClass; }

	/** Instruction code.
	  *
	  * @retval	INS.
	  */
	uint8_t instructionCode() const { return m_instructionCode; }

	/** Instruction parameter 1.
	  *
	  * @retval	P1.
	  */
	uint8_t firstParameter() const { return m_parameters[0]; }

	/** Instruction parameter 2.
	  *
	  * @retval	P2.
	  */
	uint8_t secondParameter() const { return m_parameters[1]; }

	/** Number of bytes present in the data field 
	  * of the command.
	  *
	  * @retval	Lc.
	  */
	uint8_t dataLength() const { return (uint8_t)m_data.size(); }

	/** String of bytes sent in the data field 
	  * of the command.
	  *
	  * @retval	Data.
	  */
	const std::vector<uint8_t>& data() const { return m_data; }

	/** Returns a combination of all the parameters as 
	  * a QString.
	  *
	  * @retval	CLA INS P1 P2 Lc Data.
	  */
	QString string() const;
private:
	uint8_t m_instructionClass = 0;
	uint8_t m_instructionCode = 0;
	uint8_t m_parameters[2];
	std::vector<uint8_t> m_data;

	friend class APDUSplitter;
};

#endif // APDU_COMMAND_H