/*
 * led.c
 *
 *  Created on: 20 Jan 2021
 *      Author: tkrax
 */
#include <stdbool.h>
#include "led.h"
#include "stm32l476g_discovery.h"

/**
 * Init green and red led
 */
void led_Init() {
	BSP_LED_Init(LED_RED);
	BSP_LED_Init(LED_GREEN);
}

/**
 * Switch green led
 *
 * @param on true is on, false is off
 */
void led_SwitchGreen(bool on) {
	if (on) {
		BSP_LED_On(LED_GREEN);
	} else {
		BSP_LED_Off(LED_GREEN);
	}
}

/**
 * Switch green led
 *
 * @param on true is on, false is off
 */
void led_SwitchRed(bool on) {
	if (on) {
		BSP_LED_On(LED_RED);
	} else {
		BSP_LED_Off(LED_RED);
	}
}
