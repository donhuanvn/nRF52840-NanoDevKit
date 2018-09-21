#include <stdio.h>
#include "boards.h"
#include "app_util_platform.h"
#include "app_error.h"
#include "nrf_drv_twi.h"
#include "nrf_delay.h"
#include "ISL29023-sensor.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

static uint16_t light;

int main(void)
{
    APP_ERROR_CHECK(NRF_LOG_INIT(NULL));
    NRF_LOG_DEFAULT_BACKENDS_INIT();

    NRF_LOG_INFO("\r\n\n\nTWI ISL29023 sensor example");
    NRF_LOG_FLUSH();
	
		nrf_gpio_cfg_output(NRF_GPIO_PIN_MAP(1,15));			//(1)
		nrf_gpio_pin_clear(NRF_GPIO_PIN_MAP(1,15));				//(2) enable I2C bus on nRF52840 NanoDevKit version NT-1.0, dismiss it on version NT-1.1
	
		isl29023_twi_init();
	
    while (true)
    {
				NRF_LOG_INFO("\r\nStart Reading");
				isl29023_read_lux(&light);
				NRF_LOG_INFO("\r\nLight intensity: %d lux", light);
        NRF_LOG_FLUSH();
			
				nrf_delay_ms(500);
    }
}

