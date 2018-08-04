/*
 * bme280.h
 *
 *	nRF24LE1 BME280 driver
 *
 *	based on great nRF24LE1 SDK https://github.com/DeanCording/nRF24LE1_SDK
 *
 *  Created on: 30/07/2018
 *      Author: andru
 */

#ifndef NRF24LE1_BME280_H_
#define NRF24LE1_BME280_H_

#define EN_PRESSURE		1
#define EN_HUMIDITY		1

/*! CPP guard */
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "bme280_defs.h"

bme280_error_t bme280_get_regs(uint8_t reg_addr, uint8_t *reg_data, uint16_t len);
bme280_error_t bme280_set_regs(uint8_t reg_addr, uint8_t *reg_data, uint8_t len);
bme280_error_t bme280_soft_reset(void);
bme280_error_t bme280_init(void);
bme280_error_t write_power_mode(uint8_t sensor_mode);
bme280_error_t bme280_get_sensor_mode(uint8_t *sensor_mode);
bme280_error_t bme280_set_sensor_mode(uint8_t sensor_mode);
bme280_error_t set_osr_settings(uint8_t osr_t, uint8_t osr_p, uint8_t osr_h);
bme280_error_t set_filter_settings(uint8_t filter);
bme280_error_t set_standby_settings(uint8_t standby_time);
bme280_error_t bme280_get_temperature(int32_t *temperature);
#if EN_PRESSURE
uint32_t bme280_Pa_to_mmHg(uint32_t pressure);
bme280_error_t bme280_get_pressure(uint32_t *pressure);
#endif
#if EN_HUMIDITY
bme280_error_t bme280_get_humidity(uint32_t *humidity);
#endif
bme280_error_t stream_sensor_data_forced_mode(void);

#ifdef __cplusplus
}
#endif /* End of CPP guard */

#endif /* NRF24LE1_BME280_H_ */
