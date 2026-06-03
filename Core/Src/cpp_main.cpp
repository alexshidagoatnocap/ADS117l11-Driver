#include "cpp_main.hpp"
#include "drivers/ads117l11/ads117l11.hpp"
#include "drivers/ads117l11/ads117l11_cmds.hpp"
#include "main.h"
#include "stm32_spi.hpp"
#include "stm32h7xx_hal_gpio.h"
#include "stm32h7xx_hal_spi.h"

SPI_HandleTypeDef spi_handler;

void application_entry(void) {
	platform::stm32hal::Spi spi1;
	spi1.init(&hspi1, GPIOD, GPIO_PIN_14);

	drivers::ADS117L11::Driver<platform::stm32hal::Spi> ads(spi1);

	ads.getDeviceId();
	ads.getRevisionId();
	// Clear status reg first
	ads.readReg(drivers::ADS117L11::Register::STATUS);
	ads.writeReg(drivers::ADS117L11::Register::STATUS, 0x60);

	while (1) {
		ads.readReg(drivers::ADS117L11::Register::STATUS);
	}
}
