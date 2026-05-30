#include "platform/stm32/stm32_spi.hpp"
#include "abstractions/spi_interface.hpp"
#include "stm32h7xx_hal_def.h"
#include "stm32h7xx_hal_gpio.h"
#include "stm32h7xx_hal_spi.h"

namespace platform::stm32hal {

abstractions::Status Spi::initImpl(SPI_HandleTypeDef *hspi,
								   GPIO_TypeDef *csGpioPort, uint16_t csPin) {
	using namespace abstractions;
	using enum Status;

	if (!hspi || !csGpioPort) {
		return ABSTRACT_INIT_FAIL;
	}

	// WARN: There could be a case of a silent failure if a GPIO was set
	// incorrectly

	m_hspi = hspi;
	m_gpioPort = csGpioPort;
	m_csPin = csPin;

	isInit = true;

	return ABSTRACT_OK;
}

abstractions::Status Spi::setChipSelectImpl(abstractions::PinState state) {
	using namespace abstractions;
	using enum Status;

	if (!isInit) {
		return ABSTRACT_NOT_INIT;
	}

	switch (state) {
	case PinState::HIGH:
		HAL_GPIO_WritePin(m_gpioPort, m_csPin, GPIO_PIN_SET);
		break;
	// when low
	default:
		HAL_GPIO_WritePin(m_gpioPort, m_csPin, GPIO_PIN_RESET);
		break;
	}

	return ABSTRACT_OK;
}

abstractions::Status Spi::transmitReceiveImpl(std::span<const uint8_t> tx_data,
											  std::span<uint8_t> rx_data) {
	using namespace abstractions;
	using enum Status;

	if (!isInit) {
		return ABSTRACT_NOT_INIT;
	}

	if (HAL_SPI_TransmitReceive(m_hspi, tx_data.data(), rx_data.data(),
								tx_data.size(), HAL_MAX_DELAY) != HAL_OK) {
		return ABSTRACT_ERROR;
	}
	return ABSTRACT_OK;
}

} // namespace platform::stm32hal
