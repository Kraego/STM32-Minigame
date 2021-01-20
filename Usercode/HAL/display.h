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
void display_ArrowWest(void);
void display_ArrowEast(void);
void display_ArrowNorth(void);
void display_ArrowSouth(void);
void display_ShowBars(uint32_t level);
void display_ScrollText(char *text);
int32_t display_Write(char *text);

#endif /* SERVICES_DISPLAY_DISPLAY_H_ */
