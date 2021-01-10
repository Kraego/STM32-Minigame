/*
 * debug.h
 *
 * Debugging Helper
 *
 *  Created on: Jan 10, 2021
 *      Author: tkrax
 */

#ifndef UTILS_DEBUG_H_
#define UTILS_DEBUG_H_

#include <stdarg.h>
#include "stm32l4xx_hal.h"

#define DEBUG_PRINT

#ifdef DEBUG_PRINT
	void debugPrintln(UART_HandleTypeDef *huart, const char *fmt, ...);
	#define DEBUG_PRINTF(huart, fmt, args...) debugPrintln(huart, fmt, ## args)
#else
#define DEBUG_PRINTF(huart, fmt, args...)
#endif

#endif /* UTILS_DEBUG_H_ */
