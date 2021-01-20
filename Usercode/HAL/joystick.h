/*
 * joystick.h
 *
 *  Created on: 18 Jan 2021
 *      Author: tkrax
 */

#ifndef HAL_JOYSTICK_H_
#define HAL_JOYSTICK_H_

typedef struct {
	void (*callbackUp)();
	void (*callbackDown)();
	void (*callbackRight)();
	void (*callbackLeft)();
	void (*callbackCenter)();
}joystick_callbacks_t;

void joystick_init();

#endif /* HAL_JOYSTICK_H_ */
