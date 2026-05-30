#include "cpp_main.hpp"
#include "stm32_spi.hpp"
#include "stm32h7xx_hal_spi.h"
#include <utility>

SPI_HandleTypeDef spi_handler;

void cpp_spi_init(SPI_HandleTypeDef hspi) {
	spi_handler = hspi;
	return;
}

void application_entry(void) {
	while (1) {
		// BSP_LED_Toggle(LED1);
		// HAL_Delay(100);
	}
}
