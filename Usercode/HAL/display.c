/*
 * display.c
 *
 *  Created on: 15 Jan 2021
 *      Author: tkrax
 */
#include <string.h>
#include "display.h"
#include "stm32l476g_discovery_glass_lcd.h"

#define SCROLL_SPEED	(400)
#define SCROLL_REPEATS	(1)
#define DISPLAY_SIZE	(6)

/**
 * Init the lcd display
 */
void display_Init() {
	BSP_LCD_GLASS_Init();
}

/**
 * Clear the lcd display
 */
void display_Clear() {
	BSP_LCD_GLASS_Clear();
}

/**
 * Write text to display.
 *
 * @param text the text to write
 * @return -1 if text to long, 0 otherwise
 */
int32_t display_Write(char *text) {
	if (strlen(text) > DISPLAY_SIZE){
		return -1;
	}

	BSP_LCD_GLASS_Clear();
	BSP_LCD_GLASS_DisplayChar((uint8_t *) "A", 0, 0, 1);
	BSP_LCD_GLASS_DisplayString((uint8_t *) text);
	return 0;
}

void display_ScrollText(char *text) {
	BSP_LCD_GLASS_Clear();
	BSP_LCD_GLASS_ScrollSentence((uint8_t *) text, SCROLL_REPEATS, SCROLL_SPEED);
}

void display_arrayWest() {
	BSP_LCD_GLASS_Clear();
	BSP_LCD_GLASS_DisplayArrowWest();
}

void display_arrayEast() {
	BSP_LCD_GLASS_Clear();
	BSP_LCD_GLASS_DisplayArrowEast();
}

void display_arrayNorth() {
	BSP_LCD_GLASS_Clear();
	BSP_LCD_GLASS_DisplayArrowNorth();
}

void display_arraySouth() {
	BSP_LCD_GLASS_Clear();
	BSP_LCD_GLASS_DisplayArrowSouth();
}
