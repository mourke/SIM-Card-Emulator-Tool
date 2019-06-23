#ifndef APDU_COMMAND_H
#define APDU_COMMAND_H

#include <vector>
#include <cstdint>

class QString;

/** A wrapper around ISO 7816-4 APDU commands.
  *
  * @b Description
  * This class provides helper functions for interacting
  * with APDU commands.
  */
class APDUCommand {

public:

	enum FileType {
		IO,
		SIMToolkit,
		Authentication
	};

	enum Type : uint8_t {
		Command,
		Response = 0xFF
	};


	/** The type of the APDU command.
	  *
	  * @retval	Command or Response.
	  */
	Type type() const;


	/** The filetype of the data the command points to.
	  *
	  * @retval	FileIO, STK/CAT or Authentication.
	  */
	FileType fileType() const { return m_fileType; }

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

	/** Command processing status.
	  *
	  * @retval	SW1.
	  */
	uint8_t firstStatusByte() const { return m_status[0]; }

	/** Command processing qualifier.
	  *
	  * @retval	SW2.
	  */
	uint8_t secondStatusByte() const { return m_status[1]; }

	/** String of bytes sent in the data field 
	  * of the command.
	  *
	  * @retval	Data.
	  */
	const std::vector<uint8_t>& data() const { return m_data; }

	/** Returns a combination of all the parameters as 
	  * a QString.
	  *
	  * @retval	CLA INS P1 P2 Lc Data (SW1 SW2).
	  */
	QString string() const;
private:
	void setInstructionCode(const uint8_t instructionCode);

	FileType m_fileType = IO;
	uint8_t m_instructionClass = 0;
	uint8_t m_instructionCode = 0;
	uint8_t m_parameters[2];
	std::vector<uint8_t> m_data;
	uint8_t m_status[2];

	friend class APDUSplitter;
};

#endif // APDU_COMMAND_H