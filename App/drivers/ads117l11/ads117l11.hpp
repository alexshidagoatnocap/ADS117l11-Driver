#pragma once

#include "abstractions/spi_concepts.hpp"
#include "abstractions/status_codes.hpp"
#include "drivers/ads117l11/ads117l11_cmds.hpp"
#include "drivers/ads117l11/ads117l11_regs.hpp"
#include <array>
#include <cstdint>

namespace drivers::ADS117L11 {

template <DAL::SpiImpl Spi> class Driver {
	Spi m_spiObj{};

	std::array<uint8_t, 2> m_txBuffer{};
	std::array<uint8_t, 2> m_rxBuffer{};

  public:
	drivers::ADS117L11::Status status;

	explicit Driver(Spi spiObj) : m_spiObj(spiObj) {};

	DAL::Status getDeviceId() { return readReg(Register::DEV_ID); }
	DAL::Status getRevisionId() { return readReg(Register::REV_ID); }

	// TODO: Make these private when other functions have been implemented

	DAL::Status readReg(drivers::ADS117L11::Register reg) {
		// TODO: Add CRC and STATUS bytes if needed

		m_txBuffer[0] = Command::READ_REG + reg;
		return m_spiObj.transmitReceive(m_txBuffer, m_rxBuffer);
	}

	DAL::Status writeReg(drivers::ADS117L11::Register reg,
						 uint8_t dataToWrite) {
		// TODO: Add CRC and STATUS bytes if needed

		m_txBuffer[0] = Command::WRITE_REG + reg;
		m_txBuffer[1] = dataToWrite;

		return m_spiObj.transmit(m_txBuffer);
	}
};

template <DAL::SpiImpl Spi> Driver(Spi) -> Driver<Spi>;

} // namespace drivers::ADS117L11
