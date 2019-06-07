#ifndef APDU_RESPONSE_H
#define APDU_RESPONSE_H

#include <vector>
#include <cstdint>

class QString;

/** A wrapper around ISO 7816-4 APDU responses.
  *
  * @b Description
  * This class provides helper functions for interacting
  * with APDU responses.
  */
class APDUResponse {

public:

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

	/** Number of bytes present in the data field 
	  * of the response.
	  *
	  * @retval	Lr.
	  */
	uint8_t dataLength() const { return (uint8_t)m_data.size(); }

	/** String of bytes received in the data field 
	  * of the response.
	  *
	  * @retval	Data.
	  */
	const std::vector<uint8_t>& data() const { return m_data; }

	/** Returns a combination of all the parameters as
	  * a QString.
	  *
	  * @retval	Data (SW1 SW2).
	  */
	QString string() const;
private:
	uint8_t m_status[2];
	std::vector<uint8_t> m_data;

	friend class APDUSplitter;
};

#endif // APDU_RESPONSE_H