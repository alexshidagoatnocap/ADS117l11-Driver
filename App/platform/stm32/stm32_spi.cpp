#include "platform/stm32/stm32_spi.hpp"
#include "abstractions/spi_interface.hpp"
#include "stm32h7xx_hal_def.h"
#include "stm32h7xx_hal_gpio.h"
#include "stm32h7xx_hal_spi.h"

namespace platform::stm32hal {

Spi::Spi(SPI_HandleTypeDef *hspi,
		 const std::pair<GPIO_TypeDef *, uint16_t> &csPin)
	: m_hspi(hspi), m_gpioPort(csPin.first), m_csPin(csPin.second) {}

abstractions::Status Spi::setChipSelectImpl(abstractions::PinState state) {
	using namespace abstractions;

	switch (state) {
	case PinState::HIGH:
		HAL_GPIO_WritePin(m_gpioPort, m_csPin, GPIO_PIN_SET);
		break;
	// when low
	default:
		HAL_GPIO_WritePin(m_gpioPort, m_csPin, GPIO_PIN_RESET);
		break;
	}

	return Status::ABSTRACT_OK;
}

abstractions::Status Spi::transmitReceiveImpl(std::span<const uint8_t> tx_data,
											  std::span<uint8_t> rx_data) {
	if (HAL_SPI_TransmitReceive(m_hspi, tx_data.data(), rx_data.data(),
								tx_data.size(), HAL_MAX_DELAY) != HAL_OK) {
		return abstractions::Status::ABSTRACT_ERROR;
	}
	return abstractions::Status::ABSTRACT_OK;
}

} // namespace platform::stm32hal
