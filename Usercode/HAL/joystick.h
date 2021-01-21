/*
 * joystick.h
 *
 *  Created on: 18 Jan 2021
 *      Author: tkrax
 */

#ifndef HAL_JOYSTICK_H_
#define HAL_JOYSTICK_H_

#include<stdint.h>

typedef enum {
	JOYSTICK_NOTHING,
	JOYSTICK_UP,
	JOYSTICK_DOWN,
	JOYSTICK_LEFT,
	JOYSTICK_RIGHT,
	JOYSTICK_CENTER
} joystick_press_t;

typedef struct {
	void (*callbackUp)();
	void (*callbackDown)();
	void (*callbackRight)();
	void (*callbackLeft)();
	void (*callbackCenter)();
}joystick_callbacks_t;

#define	JOYSTICK_WAIT_FOREVER		(-1)

void joystick_Init();
uint32_t joystick_WaitForPress(joystick_press_t *pressed, uint32_t timeOutTicks);

#endif /* HAL_JOYSTICK_H_ */
