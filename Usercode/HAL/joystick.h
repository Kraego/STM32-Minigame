/*
 * joystick.h
 *
 *  Created on: 18 Jan 2021
 *      Author: tkrax
 */

#ifndef HAL_JOYSTICK_H_
#define HAL_JOYSTICK_H_

typedef enum {
	NOTHING,
	UP,
	DOWN,
	LEFT,
	RIGHT,
	CENTER
} joystick_press_t;

typedef struct {
	void (*callbackUp)();
	void (*callbackDown)();
	void (*callbackRight)();
	void (*callbackLeft)();
	void (*callbackCenter)();
}joystick_callbacks_t;

void joystick_Init();
joystick_press_t joystick_WaitForPress();

#endif /* HAL_JOYSTICK_H_ */
