/*
 * display.c
 *
 *  Created on: 15 Jan 2021
 *      Author: tkrax
 */
#include "display.h"
#include "stm32l476g_discovery_glass_lcd.h"

void display_Init() {
	BSP_LCD_GLASS_Init();
}

void display_Write(char *text) {
	BSP_LCD_GLASS_Clear();
	BSP_LCD_GLASS_DisplayString((uint8_t *) text);
}

void display_ScrollText(char *text) {

}

