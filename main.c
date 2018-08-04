/*
 *  Created on: 30/07/2018
 *      Author: andru
 *
 *      nRF24LE1 BME280 driver
 *
 *		based on great nRF24LE1 SDK https://github.com/DeanCording/nRF24LE1_SDK
 */

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "gpio.h"
#include "delay.h"
#include "memory.h"

#include "main.h"

#if DEBUG
#define EN_UART			1	// use UART for debugging
#define UARTTXPIN		GPIO_PIN_ID_P0_3		// P0.3 - UART TX
#define UARTRXPIN		GPIO_PIN_ID_P0_4		// P0.4	- UART RX
#else
#define EN_UART			0	// use UART for debugging
#endif

#define LEDPIN	GPIO_PIN_ID_P0_1		// P0.1 - LED

#if EN_UART
#include "uart.h"
#endif

#include "bme280.h"

// main
void main(void) {

	// variable definition

	// program code
	gpio_pin_configure(LEDPIN,
		GPIO_PIN_CONFIG_OPTION_DIR_OUTPUT
		| GPIO_PIN_CONFIG_OPTION_OUTPUT_VAL_CLEAR
		| GPIO_PIN_CONFIG_OPTION_PIN_MODE_OUTPUT_BUFFER_NORMAL_DRIVE_STRENGTH
		);

#if EN_UART
	// Setup UART pins
	gpio_pin_configure(GPIO_PIN_ID_FUNC_RXD,
		GPIO_PIN_CONFIG_OPTION_DIR_INPUT |
		GPIO_PIN_CONFIG_OPTION_PIN_MODE_INPUT_BUFFER_ON_NO_RESISTORS
		);

	gpio_pin_configure(GPIO_PIN_ID_FUNC_TXD,
		GPIO_PIN_CONFIG_OPTION_DIR_OUTPUT |
		GPIO_PIN_CONFIG_OPTION_OUTPUT_VAL_SET |
		GPIO_PIN_CONFIG_OPTION_PIN_MODE_OUTPUT_BUFFER_NORMAL_DRIVE_STRENGTH
		);

	uart_configure_8_n_1_38400();
#endif

	bme280_init();

	while(1) {

		gpio_pin_val_complement(LEDPIN);

		stream_sensor_data_forced_mode();

		delay_s(1);
	} // main loop
}
