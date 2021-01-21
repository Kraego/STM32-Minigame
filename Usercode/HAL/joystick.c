/*
 * joystick.c
 *
 *  Created on: 18 Jan 2021
 *      Author: tkrax
 */
#include "joystick.h"
#include "exti.h"
#include "stm32l476g_discovery.h"

/* Joystick Exception line Mapping joystick BSP
 * 	center - exti0
 *	down   - exti9_5
 *	up     - exti3
 *	left   - exti1
 *	rigth  - exti2
 *
 */
#define DEBOUNCE_TIME_MS	(50)

static volatile joystick_press_t currentPressed = JOYSTICK_NOTHING;
static joystick_callbacks_t _callbacks = {};
static uint32_t EXTI0_lastCalled;
static uint32_t EXTI1_lastCalled;
static uint32_t EXTI2_lastCalled;
static uint32_t EXTI3_lastCalled;
static uint32_t EXTI9_5_lastCalled;


static void handleInterrupt(uint32_t *lastCalled, void (*cb)()){
	volatile exti_PR1_cmsis_t *pr1 = EXTI_PR1_BASE_ADDRESS;

	if (pr1->val.val0 &= 0x1) {
		uint32_t now = HAL_GetTick();

		pr1->val.val0 |= 0x0;
		if (now - *lastCalled >= DEBOUNCE_TIME_MS) {
			*lastCalled = now;
			if (cb != NULL){
				cb();
			}
		}
	}
}

void EXTI0_IRQHandler(void) {
	handleInterrupt(&EXTI0_lastCalled, _callbacks.callbackCenter);
	currentPressed = JOYSTICK_CENTER;
}

void EXTI1_IRQHandler(void) {
	handleInterrupt(&EXTI1_lastCalled, _callbacks.callbackLeft);
	currentPressed = JOYSTICK_LEFT;
}

void EXTI2_IRQHandler(void) {
	handleInterrupt(&EXTI2_lastCalled, _callbacks.callbackRight);
	currentPressed = JOYSTICK_RIGHT;
}

void EXTI3_IRQHandler(void) {
	handleInterrupt(&EXTI3_lastCalled, _callbacks.callbackUp);
	currentPressed = JOYSTICK_UP;
}

void EXTI9_5_IRQHandler(void) {
	handleInterrupt(&EXTI9_5_lastCalled, _callbacks.callbackDown);
	currentPressed = JOYSTICK_DOWN;
}

void joystick_Init(){
	BSP_JOY_Init(JOY_MODE_EXTI);
}

/**
 * Wait for joystick press. !Attention Function is blocking!
 *
 *
 * @param timeOutTicks max wait ticks (-1 wait forever)
 * @param pressed the pressed joystick direction.
 *
 * @return in case of success, JOYSTICK_WAIT_FOREVER in case of timeout
 */
uint32_t joystick_WaitForPress(joystick_press_t *pressed, uint32_t timeOutTicks) {
	joystick_press_t previousPressed = currentPressed;
	uint32_t startTicks = HAL_GetTick();

	while (previousPressed == currentPressed){
		if (timeOutTicks != JOYSTICK_WAIT_FOREVER && (HAL_GetTick() - startTicks >= timeOutTicks))
		{
			*pressed = JOYSTICK_NOTHING;
			return -1;
		}
		HAL_Delay(1);
	}
	*pressed = currentPressed;
	currentPressed = JOYSTICK_NOTHING;
	return 0;
}

void joystick_RegisterUpCb(void (*cb)()) {
	_callbacks.callbackUp = cb;
}

void joystick_RegisterDownCb(void (*cb)()) {
	_callbacks.callbackDown = cb;
}

void joystick_RegisterLeftCb(void (*cb)()) {
	_callbacks.callbackLeft = cb;
}

void joystick_RegisterRightCb(void (*cb)()) {
	_callbacks.callbackRight = cb;
}

void joystick_RegisterCenterCb(void (*cb)()) {
	_callbacks.callbackCenter = cb;
}
