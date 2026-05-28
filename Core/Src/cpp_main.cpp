#include "cpp_main.hpp"

void application_entry(void) {
	while (1) {
		BSP_LED_Toggle(LED1);
		HAL_Delay(100);
	}
}
