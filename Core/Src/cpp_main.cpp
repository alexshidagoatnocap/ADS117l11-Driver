#include "cpp_main.hpp"
#include "abstractions/spi_interface.hpp"
#include "main.h"
#include "stm32_spi.hpp"
#include "stm32h7xx_hal_gpio.h"
#include "stm32h7xx_hal_spi.h"
#include <cstdint>

SPI_HandleTypeDef spi_handler;

void application_entry(void) {
	platform::stm32hal::Spi spiObj;
	spiObj.init(&hspi1, GPIOD, GPIO_PIN_14);

	uint8_t txBuffer[] = {"I'm new bro"};
	uint8_t rxBuffer[sizeof(txBuffer)];

	while (1) {
		spiObj.csLow();
		spiObj.transmitReceiveImpl(txBuffer, rxBuffer);
		spiObj.csHigh();
	}
}
