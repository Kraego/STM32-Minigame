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
			cb();
		}
	}
}

void EXTI0_IRQHandler(void) {
	handleInterrupt(&EXTI0_lastCalled, _callbacks.callbackCenter);
}

void EXTI1_IRQHandler(void) {
	handleInterrupt(&EXTI1_lastCalled, _callbacks.callbackLeft);
}

void EXTI2_IRQHandler(void) {
	handleInterrupt(&EXTI2_lastCalled, _callbacks.callbackRight);
}

void EXTI3_IRQHandler(void) {
	handleInterrupt(&EXTI3_lastCalled, _callbacks.callbackUp);
}

void EXTI9_5_IRQHandler(void) {
	handleInterrupt(&EXTI9_5_lastCalled, _callbacks.callbackDown);
}

void joystick_init(){
	BSP_JOY_Init(JOY_MODE_EXTI);
}

void joystick_register_up_cb(void (*cb)()) {
	_callbacks.callbackUp = cb;
}

void joystick_register_down_cb(void (*cb)()) {
	_callbacks.callbackDown = cb;
}

void joystick_register_left_cb(void (*cb)()) {
	_callbacks.callbackLeft = cb;
}

void joystick_register_right_cb(void (*cb)()) {
	_callbacks.callbackRight = cb;
}

void joystick_register_center_cb(void (*cb)()) {
	_callbacks.callbackCenter = cb;
}


