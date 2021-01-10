/*
 * debug.c
 *
 *  Created on: Jan 10, 2021
 *      Author: tkrax
 */
#include "debug.h"
#include <string.h>
#include <stdio.h>

void debugPrintln(UART_HandleTypeDef *huart, const char *fmt, ...) {
	char buffer[256];

	va_list args;
	va_start(args, fmt);
	vsprintf(buffer, fmt, args);
	HAL_UART_Transmit(huart, (uint8_t*) buffer, strlen(buffer), 50);
	va_end(args);
	char newline[2] = "\r\n";
	HAL_UART_Transmit(huart, (uint8_t*) newline, 2, 10);
}
