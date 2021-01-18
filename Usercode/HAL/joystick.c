/*
 * joystick.c
 *
 *  Created on: 18 Jan 2021
 *      Author: tkrax
 */
#include "joystick.h"
#include "stm32l476g_discovery.h"

/* Joystick Exception line Mapping joystick BSP
 * 	center - exti0
 *	down   - exti9_5
 *	up     - exti3
 *	left   - exti1
 *	rigth  - exti2
 *
 */

joystick_callbacks_t _callbacks = {};

void joystick_init(joystick_callbacks_t callbacks){
	BSP_JOY_Init(JOY_MODE_EXTI);
	_callbacks = callbacks;
}

// TODO: implement extis


