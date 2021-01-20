/*
 * arrowRotator.c
 *
 *  Created on: 20 Jan 2021
 *      Author: tkrax
 */
#include "arrowRotator.h"
#include "display.h"
#include "stm32l4xx_hal.h"

#define ONE_ROTATION_CNT	(5)

static void (*rotation[ONE_ROTATION_CNT]) () = {display_arrayNorth, display_arrayEast, display_arraySouth, display_arrayWest, display_arrayNorth};


/**
 * Rotate the display arrow - starting north til north
 *
 * @param delay_ms
 */
void arrowRotator_FullRoation(uint32_t delay_ms) {
	arrowRotator_Rotate(delay_ms, ONE_ROTATION_CNT);
}

void arrowRotator_Rotate(uint32_t delay_ms, uint32_t times) {
	uint32_t count = times + 1;
	uint32_t idx = 0;

	while (count){
		rotation[idx]();
		idx = (idx + 1) % ONE_ROTATION_CNT;
		HAL_Delay(delay_ms);
		count--;
	}
}
