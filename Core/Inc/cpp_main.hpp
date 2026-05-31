#pragma once

#ifdef __cplusplus
extern "C" {
#include "main.h"
#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_spi.h"
#endif

void application_init();
void application_loop(void);

#ifdef __cplusplus
}
#endif
