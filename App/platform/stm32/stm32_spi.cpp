#include "platform/stm32/stm32_spi.hpp"
#include "abstractions/spi_interface.hpp"
#include "abstractions/status_codes.hpp"
#include "stm32h7xx_hal_def.h"
#include "stm32h7xx_hal_gpio.h"
#include "stm32h7xx_hal_spi.h"

namespace platform::stm32hal {

DAL::Status Spi::initImpl(SPI_HandleTypeDef *hspi,
								   GPIO_TypeDef *csGpioPort, uint16_t csPin) {
	using namespace DAL;
	using enum Status;

	if (!hspi || !csGpioPort) {
		return ABSTRACT_INIT_FAIL;
	}

	// WARN: There could be a case of a silent failure if a GPIO pin was set
	// to the wrong pin or not initialized by the user.

	m_hspi = hspi;
	m_gpioPort = csGpioPort;
	m_csPin = csPin;

	m_isInit = true;

	return ABSTRACT_OK;
}

DAL::Status Spi::csHighImpl() {
	using namespace DAL;
	using enum Status;

	if (!m_isInit) {
		return ABSTRACT_NOT_INIT;
	}

	HAL_GPIO_WritePin(m_gpioPort, m_csPin, GPIO_PIN_SET);

	return ABSTRACT_OK;
}

DAL::Status Spi::csLowImpl() {
	using namespace DAL;
	using enum Status;

	if (!m_isInit) {
		return ABSTRACT_NOT_INIT;
	}

	HAL_GPIO_WritePin(m_gpioPort, m_csPin, GPIO_PIN_RESET);

	return ABSTRACT_OK;
}

DAL::Status Spi::transmitImpl(std::span<const uint8_t> tx_data) {
	using namespace DAL;
	using enum Status;

	if (!m_isInit) {
		return ABSTRACT_NOT_INIT;
	}

	if (HAL_SPI_Transmit(m_hspi, tx_data.data(), tx_data.size(),
						 HAL_MAX_DELAY) != HAL_OK) {
		return ABSTRACT_ERROR;
	}
	return ABSTRACT_OK;
}

DAL::Status Spi::receiveImpl(std::span<uint8_t> rx_data) {
	using namespace DAL;
	using enum Status;

	if (!m_isInit) {
		return ABSTRACT_NOT_INIT;
	}

	if (HAL_SPI_Receive(m_hspi, rx_data.data(), rx_data.size(),
						HAL_MAX_DELAY) != HAL_OK) {
		return ABSTRACT_ERROR;
	}
	return ABSTRACT_OK;
}

DAL::Status Spi::transmitReceiveImpl(std::span<const uint8_t> tx_data,
											  std::span<uint8_t> rx_data) {
	using namespace DAL;
	using enum Status;

	if (!m_isInit) {
		return ABSTRACT_NOT_INIT;
	}

	if (HAL_SPI_TransmitReceive(m_hspi, tx_data.data(), rx_data.data(),
								tx_data.size(), HAL_MAX_DELAY) != HAL_OK) {
		return ABSTRACT_ERROR;
	}
	return ABSTRACT_OK;
}

} // namespace platform::stm32hal
