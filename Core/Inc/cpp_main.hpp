#pragma once

#ifdef __cplusplus
extern "C" {
#include "main.h"
#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_spi.h"
#endif

void application_entry(void);
void cpp_spi_init(SPI_HandleTypeDef *hspi);

#ifdef __cplusplus
}
#endif
