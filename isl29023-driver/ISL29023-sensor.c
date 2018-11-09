/**	Vinh Long, 09/10/2018, Do Ngoc Nhuan
 *	ISL29032 driver for nRF5 SDK version 14.2.0
 */

#include "ISL29023-sensor.h"

/* Indicates if operation on TWI has ended. */
static volatile bool xfer_tx_done = false;
static volatile bool xfer_rx_done = false;

/* TWI instance. */
static const nrf_drv_twi_t isl29023_twi = NRF_DRV_TWI_INSTANCE(ISL29023_TWI_INSTANCE_ID);

/**
 * @brief ISL29023 TWI events handler.
 */
void isl29023_twi_handler(nrf_drv_twi_evt_t const * p_event, void * p_context)
{
    switch (p_event->type)
    {
        case NRF_DRV_TWI_EVT_DONE:
            if (p_event->xfer_desc.type == NRF_DRV_TWI_XFER_RX)
            {
                xfer_rx_done = true;
            }
            if (p_event->xfer_desc.type == NRF_DRV_TWI_XFER_TX) 
						{
								xfer_tx_done = true;
						}
            break;
        default:
            break;
    }
}

/**
 * @brief Two wire interface (I2C) initialization.
 */
void isl29023_twi_init(void)
{
    ret_code_t err_code;

    const nrf_drv_twi_config_t isl29023_twi_config = {
       .scl                = ISL29023_SCL,
       .sda                = ISL29023_SDA,
       .frequency          = NRF_TWI_FREQ_100K,
       .interrupt_priority = APP_IRQ_PRIORITY_HIGH,
       .clear_bus_init     = false
    };

    err_code = nrf_drv_twi_init(&isl29023_twi, &isl29023_twi_config, isl29023_twi_handler, NULL);
    APP_ERROR_CHECK(err_code);

    nrf_drv_twi_enable(&isl29023_twi);
}

/**
 * @brief Write data to ISL29023 sensor.
 */
void isl29023_write(uint8_t *a_data, uint8_t a_length)
{
		ret_code_t err_code;
		xfer_tx_done = false;
	
		err_code = nrf_drv_twi_tx(&isl29023_twi, ISL29023_ADDR, a_data, a_length, false);
		APP_ERROR_CHECK(err_code);
}

/**
 * @brief Read data from ISL29023 sensor.
 */
void isl29023_read(uint8_t *a_data, uint8_t a_length, uint8_t reg_addr)
{
		ret_code_t err_code;
	
		xfer_rx_done = false;
		err_code = nrf_drv_twi_tx(&isl29023_twi, ISL29023_ADDR, &reg_addr, 1, true);
		APP_ERROR_CHECK(err_code);
		nrf_delay_ms(2); //(important)if not have this line, system will have reset error
		err_code = nrf_drv_twi_rx(&isl29023_twi, ISL29023_ADDR, a_data, a_length);
		APP_ERROR_CHECK(err_code);
		nrf_delay_ms(2);
}
/**
 * @brief Read light intensity in lux unit
 * ISL29032 configuration registers in default mode (1000 lux in range & 16-bit resolution) 
 * and external resistor (R_ext) is 499kOhm.
 */
void isl29023_read_lux(uint16_t *real_light)
{
		uint8_t w_data[2] = {CMD1, CMD1_ONE_READ}; 				//writing_data
		uint8_t r_data[2];																//reading_data
		uint16_t d_light;																	//data_light is |DATA_H|DATA_L|
		float real_light_1;																//real_light in float

		isl29023_write(w_data, sizeof(w_data)); 					//(data to write, length of this data)
		nrf_delay_ms(ISL29023_TIME_16BITS);								//(note to time unit: mili or micro second)
		isl29023_read(r_data, 2, DATA_L); 								//(buffer for read, length of that data, address of registor to read)
		while (!xfer_rx_done);														//waiting for two wire interface (I2C) reading complete. 
		d_light = r_data[1];															//(1)
		d_light<<=8; 																			//(2)
		d_light |= r_data[0];															//(3) d_light is |DATA_H|DATA_L|
		real_light_1 = d_light*1000/65536;								// For this formula, refer at page 10 of ISL29023 datasheet 
		*real_light = (uint16_t)real_light_1;							// casting float to integer for simple.
}
