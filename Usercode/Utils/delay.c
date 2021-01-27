/*
 * delay.c
 *
 *  Created on: 27 Jan 2021
 *      Author: tkrax
 */

#include "delay.h"
#include "stm32l4xx_hal.h"

/**
 * Sleep for given milliseconds
 *
 * @param ms sleep time
 */
void delay_sleep(uint32_t ms){
	HAL_Delay(ms);
}
