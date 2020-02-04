#ifndef APDU_COMMAND_H
#define APDU_COMMAND_H

#include <vector>
#include <cstdint>
#include <optional>
#include <QMap>
#include <QVariant>

/** A wrapper around ISO 7816-4 APDU commands.
  *
  * @b Description
  * This class provides helper functions for interacting
  * with APDU commands.
  */
class APDUCommand {

public:

	enum Type : uint8_t {
		SIMToolkit = 1 << 0,
		IO = 1 << 1,
		Authentication = 1 << 2,
		Unknown = 1 << 3
	};

	enum InstructionCode : uint8_t {
		// File Management Commands
		Select = 0xA4,
		Status = 0xF2,
		ReadBinary = 0xB0,
		UpdateBinary = 0xD6,
		ReadRecord = 0xB2,
		UpdateRecord = 0xDC,
		Seek = 0xA2,
		Increase = 0x32,
		Invalidate = 0x04,
		Rehabilitate = 0x44,
		GetResponse = 0xC0,
		VerifyCHV = 0x20,
		ChangeCHV = 0x24,
		DisableCHV = 0x26,
		EnableCHV = 0x28,
		UnblockCHV = 0x2C,

		// SIM Toolkit Commands
		TerminalProfile = 0x10,
		Envelope = 0xC2,
		Fetch = 0x12,
		TerminalResponse = 0x14,
		ManageChannel = 0x70,

		// Authentication Commands
		RunGSMAlgorithm = 0x88
	};


	/** The type of the command.
	  *
	  * @retval	FileIO, STK/CAT, Authentication or Unknown.
	  */
	Type type() const { return m_type; }

	/** Class of instruction.
	  * 
	  * @retval	CLA.
	  */
	uint8_t instructionClass() const { return m_instructionClass; }

	/** Instruction code.
	  *
	  * @retval	INS.
	  */
	InstructionCode instructionCode() const { return static_cast<InstructionCode>(m_instructionCode); }

	/** Instruction code.
	  *
	  * @retval	INS as a human-readable string.
	  */
	QString instructionCodeString() const;

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
    uint8_t dataLength() const { return static_cast<uint8_t>(m_data.size()); }

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
	QString protocolString() const;

	/** A higher level interpretation of the data.
	  *
	  * @retval	A textual representation of the raw 
	  *			protocol string split into key-value pairs, if available.
	  */
	const std::optional<QMap<QString, QVariant>> & applicationMap() const { return m_applicationMap; }
private:
	void setInstructionCode(const uint8_t instructionCode);
	QString getStatusWordString();
	void updateApplicationMap();

	Type m_type = Unknown;
	uint8_t m_instructionClass = 0;
	uint8_t m_instructionCode = 0;
	uint8_t m_parameters[2];
	std::vector<uint8_t> m_data;
	uint8_t m_status[2];
	std::optional<QMap<QString, QVariant>> m_applicationMap;

	friend class APDUSplitter;
};

#endif // APDU_COMMAND_H
