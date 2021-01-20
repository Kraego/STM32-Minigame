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

/**
 * Scroll text.
 *
 * @param text the text
 */
void display_ScrollText(char *text) {
	BSP_LCD_GLASS_Clear();
	BSP_LCD_GLASS_ScrollSentence((uint8_t *) text, SCROLL_REPEATS, SCROLL_SPEED);
}

/**
 * Arrow in the middle of display pointing west (relative to the board)
 */
void display_ArrowWest() {
	BSP_LCD_GLASS_Clear();
	BSP_LCD_GLASS_DisplayArrowWest();
}

/**
 * Arrow in the middle of display pointing east (relative to the board)
 */
void display_ArrowEast() {
	BSP_LCD_GLASS_Clear();
	BSP_LCD_GLASS_DisplayArrowEast();
}

/**
 * Arrow in the middle of display pointing north (relative to the board)
 */
void display_ArrowNorth() {
	BSP_LCD_GLASS_Clear();
	BSP_LCD_GLASS_DisplayArrowNorth();
}

/**
 * Arrow in the middle of display pointing south (relative to the board)
 */
void display_ArrowSouth() {
	BSP_LCD_GLASS_Clear();
	BSP_LCD_GLASS_DisplayArrowSouth();
}

/**
 *
 * Show Bars right at the lcd
 *
 * @param level 0 is empty 4 is full
 */
void display_ShowBars(uint32_t level) {
	BSP_LCD_GLASS_BarLevelConfig(level);
}
