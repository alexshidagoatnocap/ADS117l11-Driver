#include <cstdint>

namespace drivers::ADS117L11 {

class Status {
	uint8_t m_rData;

	enum Bit : uint8_t {
		CS_MODE = 7,
		ALV_FLAG = 6,
		POR_FLAG = 5,
		SPI_ERR = 4,
		REG_ERR = 3,
		ADC_ERR = 2,
		MOD_FLAG = 1,
		DRDY = 0,
	};

  public:
	void clearFlagsAndErrors() {
		m_rData |= 1 << ALV_FLAG;
		m_rData |= 1 << POR_FLAG;
		m_rData |= 1 << SPI_ERR;
		m_rData |= 1 << REG_ERR;
	}

	inline uint8_t data() { return m_rData; }
	inline bool getCSMode() { return (m_rData &= 1 << CS_MODE) >> CS_MODE; }
	inline bool getALVFlag() { return (m_rData &= 1 << ALV_FLAG) >> ALV_FLAG; }
	inline bool getPORFlag() { return (m_rData &= 1 << POR_FLAG) >> POR_FLAG; }
	inline bool getSPIError() { return (m_rData &= 1 << SPI_ERR) >> SPI_ERR; }
	inline bool getREGError() { return (m_rData &= 1 << REG_ERR) >> REG_ERR; }
	inline bool getADCError() { return (m_rData &= 1 << ADC_ERR) >> ADC_ERR; }
	inline bool getMODFlag() { return (m_rData &= 1 << MOD_FLAG) >> MOD_FLAG; }
	inline bool getDRDY() { return (m_rData &= 1 << DRDY) >> DRDY; }
};

} // namespace drivers::ADS117L11
