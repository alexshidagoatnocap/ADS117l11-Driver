#include "cpp_main.hpp"
#include "abstractions/spi_interface.hpp"
#include "main.h"
#include "stm32_spi.hpp"
#include "stm32h7xx_hal_gpio.h"
#include "stm32h7xx_hal_spi.h"
#include <cstdint>

SPI_HandleTypeDef spi_handler;

void application_init() {}

void application_loop(void) {
	platform::stm32hal::Spi spiObj;
	spiObj.init(&hspi1, GPIOC, GPIO_PIN_8);

	uint8_t tx_buffer[] = {'H', 'I'};
	uint8_t rx_buffer[32];

	while (1) {
		spiObj.setChipSelect(abstractions::PinState::LOW);
		spiObj.transmitReceiveImpl(tx_buffer, rx_buffer);
		spiObj.setChipSelect(abstractions::PinState::HIGH);
	}
}
