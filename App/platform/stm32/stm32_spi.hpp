#pragma once

#include "abstractions/spi_interface.hpp"
#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_gpio.h"
#include <cstdint>

namespace platform::stm32hal {
class Spi : public abstractions::SpiInterface {
  private:
	SPI_HandleTypeDef *m_hspi;
	GPIO_TypeDef *m_gpioPort;
	uint16_t m_csPin;
	bool m_isInit{false};

  public:
	Spi() = default;

	abstractions::Status initImpl(SPI_HandleTypeDef *hspi,
								  GPIO_TypeDef *csGpioPort, uint16_t csPin);

	abstractions::Status deinitImpl();

	abstractions::Status setChipSelectImpl(abstractions::PinState state);

	abstractions::Status transmitImpl(std::span<const uint8_t> tx_data);

	abstractions::Status receiveImpl(std::span<uint8_t> rx_data);

	abstractions::Status transmitReceiveImpl(std::span<const uint8_t> tx_data,
											 std::span<uint8_t> rx_data);
};
} // namespace platform::stm32hal
