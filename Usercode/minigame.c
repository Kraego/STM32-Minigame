/*
 * minigame.c
 *
 *  Created on: 17 Jan 2021
 *      Author: tkrax
 */
#include <minigame.h>
#include "joystick.h"
#include "compass.h"
#include "display.h"
#include "debug.h"
#include "heading_calc.h"

void minigame_Init(void) {
	uint32_t ret;
	DEBUG_PRINTF("starting minigame!");
	display_Init();
	joystick_init(); // zum Beispiel up/down selection, left right für sequencen, center bestätigen
	ret = compass_Init();

	if (ret != 0) {
		DEBUG_PRINTF("No calibration found ...");
		display_Write("CALIB");
		compass_Calibrate();
	}
	display_Clear();
}

void minigame_Run(void) {
	display_ScrollText("   SELECT");

	// hier statemachine siehe flussdiagram


	double magn[3];

//	compass_GetRawValues(magn_vals);
//	DEBUG_PRINTF("Raw Values x: %i, y: %i, z: %i", magn_vals[0], magn_vals[1], magn_vals[2]);
    compass_GetValues(magn);
    DEBUG_PRINTF("Magn Values x: %i, y: %i, z: %i", magn[0], magn[1], magn[2]);
    DEBUG_PRINTF("heading is %d  !",(int32_t) heading_Calc_getHeading(magn[0], magn[1]));
    DEBUG_PRINTF("\033[3A");
    HAL_Delay(1000);
}
