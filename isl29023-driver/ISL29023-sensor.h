/**	Vinh Long, 09/10/2018, Do Ngoc Nhuan
 *	ISL29032 driver for nRF5 SDK version 14.2.0
 */
 
#ifndef __ISL_29023_SENSOR_H__
#define __ISL_29023_SENSOR_H__

#include "app_util_platform.h"
#include "app_error.h"
#include "nrf_gpio.h"
#include "nrf_drv_twi.h"
#include "nrf_delay.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

/* TWI instance ID. */
#define ISL29023_TWI_INSTANCE_ID     0

/* TWI0 definition for light sensor. */
#define ISL29023_SCL				NRF_GPIO_PIN_MAP(1,8)
#define ISL29023_SDA				NRF_GPIO_PIN_MAP(1,9)
#define ISL29023_ADDR       0x44U			//refer page 6 of ISL29023 datasheet.

/* ISL29023 Register Address */
#define CMD1			0x00
#define CMD2			0x01
#define DATA_L		0x02
#define DATA_H		0x03
#define INT_LT_L	0x04
#define INT_LT_H	0x05
#define INT_HT_L	0x06
#define INT_HT_H	0x07

/* Mode for ISL29023. */
#define CMD1_ONE_READ 	0x20U			//refer p.8&p.9 of ISL29023 datasheet.
#define CMD2_16BIT			0x03U
/* Integration time of ADC in ISL29032*/
#define ISL29023_TIME_16BITS	90	//in milisecond
#define ISL29023_TIME_12BITS	6		//in milisecond
#define ISL29023_TIME_8BITS		352	//in microsecond
#define ISL29023_TIME_4BITS		22	//in microsecond

void isl29023_twi_handler(nrf_drv_twi_evt_t const * p_event, void * p_context);

void isl29023_twi_init(void);

void isl29023_write(uint8_t *a_data, uint8_t a_length);

void isl29023_read(uint8_t *a_data, uint8_t a_length, uint8_t reg_addr);

void isl29023_read_lux(uint16_t *real_light);
#endif /*__ISL_29023_SENSOR_H__*/
