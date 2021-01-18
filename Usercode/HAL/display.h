/*
 * display.h
 *
 *  Created on: 15 Jan 2021
 *      Author: tkrax
 */

#ifndef SERVICES_DISPLAY_DISPLAY_H_
#define SERVICES_DISPLAY_DISPLAY_H_

#include <stdint.h>

void display_Init(void);
void display_Clear(void);
void display_arrayWest(void);
void display_arrayEast(void);
void display_arrayNorth(void);
void display_arraySouth(void);
void display_ScrollText(char *text);
int32_t display_Write(char *text);

#endif /* SERVICES_DISPLAY_DISPLAY_H_ */
