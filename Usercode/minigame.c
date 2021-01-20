/*
 * minigame.c
 *
 *  Created on: 17 Jan 2021
 *      Author: tkrax
 */
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "minigame.h"
#include "joystick.h"
#include "compass.h"
#include "display.h"
#include "debug.h"
#include "heading_calc.h"

#define PLAYER_CNT_2			("PL 2")
#define PLAYER_CNT_4			("PL 4")
#define PLAY_TXT				("PLAY")
#define CALIBRATE_TXT			("CAL")
#define DIFF_LEVEL_CNT			(4)
#define DIFF_LEVE_STRLEN		(6)
#define DIFF_EASY_TXT			("DIF E")
#define DIFF_MEDIUM_TXT			("DIF M")
#define DIFF_ADVANCED_TXT		("DIF A")
#define DIFF_HARDCORE_TXT		("DIF H")
#define DIFF_EASY_FLASH_CNT		(3)
#define DIFF_MEDIUM_FLASH_CNT	(5)
#define DIFF_ADVANCED_FLASH_CNT	(7)
#define DIFF_HARDCORE_FLASH_CNT	(15)


#define DIFF_INCR_CURRENT	((_gameConfig.difficulty + 1) % DIFF_LEVEL_CNT)
#define DIFF_DECR_CURRENT	(_gameConfig.difficulty == DIFF_EASY ? DIFF_HARDCORE : (_gameConfig.difficulty - 1) % DIFF_LEVEL_CNT)

typedef enum {
	MENU_CALIBRATE, MENU_PLAY
} menuSelection_t;

typedef enum {
	STATE_INITIAL_SELECTION, STATE_CONFIGURE, STATE_CALCULATE_FLASHES, STATE_PLAY
} gameState_t;

typedef enum {
	DIFF_EASY = 0, DIFF_MEDIUM = 1, DIFF_ADVANCED = 2, DIFF_HARDCORE = 3
} difficulty_t;

typedef struct {
	uint32_t players;
	difficulty_t difficulty;
} gameConfig_t;

typedef enum {
	GREEN, RED
} gameFlash_t;

static char difficulty_map[DIFF_LEVEL_CNT][DIFF_LEVE_STRLEN] = { DIFF_EASY_TXT, DIFF_MEDIUM_TXT, DIFF_ADVANCED_TXT,
		DIFF_HARDCORE_TXT };
static menuSelection_t _menuSelection = MENU_PLAY;
static gameState_t _gameState = STATE_INITIAL_SELECTION;
static gameConfig_t _gameConfig = { };

static void _minigame_mainMenu() {
	joystick_press_t input = NOTHING;

	display_ScrollText("         MINIGAME");
	HAL_Delay(1500);

	display_Write(PLAY_TXT);

	while (input != CENTER) {
		input = joystick_waitForPress();

		if (input == UP || input == DOWN) {
			if (_menuSelection == MENU_PLAY) {
				_menuSelection = MENU_CALIBRATE;
				display_Write(CALIBRATE_TXT);
			} else if (_menuSelection == MENU_CALIBRATE) {
				_menuSelection = MENU_PLAY;
				display_Write(PLAY_TXT);
			}
		}
	}

	if (_menuSelection == MENU_CALIBRATE) {
		compass_Calibrate();
	} else if (_menuSelection == MENU_PLAY) {
		_gameState = STATE_CONFIGURE;
	}
}

static void _minigame_configPlayers() {
	joystick_press_t input = NOTHING;
	uint32_t playerCount = 2;

	display_Write(PLAYER_CNT_2);
	_gameConfig.players = playerCount;

	while (input != CENTER) {
		input = joystick_waitForPress();

		if (input == UP || input == DOWN) {
			if (_gameConfig.players == 2) {
				display_Write(PLAYER_CNT_4);
				_gameConfig.players = 4;
			} else if (_gameConfig.players == 4) {
				display_Write(PLAYER_CNT_2);
				_gameConfig.players = 2;
			}
		}
	}
}

static void _minigame_configDifficulty() {
	joystick_press_t input = NOTHING;

	display_Write(DIFF_MEDIUM_TXT);
	_gameConfig.difficulty = DIFF_MEDIUM;

	while (input != CENTER) {
		input = joystick_waitForPress();

		if (input == UP) {
			_gameConfig.difficulty = DIFF_INCR_CURRENT;
		} else if (input == DOWN) {
			_gameConfig.difficulty = DIFF_DECR_CURRENT;
		}
		display_Write(difficulty_map[_gameConfig.difficulty]);
	}
}

static void _minigame_config() {
	_minigame_configPlayers();
	_minigame_configDifficulty();
	DEBUG_PRINTF("Start game for %d player: difficulty is '%s'", _gameConfig.players,
			difficulty_map[_gameConfig.difficulty]);
	_gameState = STATE_CALCULATE_FLASHES;
}

static void _minigame_generateRandomFlash(gameFlash_t *flashSequence, uint32_t flashCount) {

}

static void _minigame_calculateFlashSequence(gameFlash_t **flashSequence) {
	switch (_gameConfig.difficulty) {
	case DIFF_EASY:
		*flashSequence = (gameFlash_t *) calloc(DIFF_EASY_FLASH_CNT, sizeof(gameFlash_t));
		assert(*flashSequence != NULL);
		_minigame_generateRandomFlash(*flashSequence, DIFF_EASY_FLASH_CNT);
		break;
	case DIFF_MEDIUM:
		*flashSequence = (gameFlash_t *) calloc(DIFF_MEDIUM_FLASH_CNT, sizeof(gameFlash_t));
		assert(*flashSequence != NULL);
		_minigame_generateRandomFlash(*flashSequence, DIFF_MEDIUM_FLASH_CNT);
		break;
	case DIFF_ADVANCED:
		*flashSequence = (gameFlash_t *) calloc(DIFF_ADVANCED_FLASH_CNT, sizeof(gameFlash_t));
		assert(*flashSequence != NULL);
		_minigame_generateRandomFlash(*flashSequence, DIFF_ADVANCED_FLASH_CNT);
		break;
	case DIFF_HARDCORE:
		*flashSequence = (gameFlash_t *) calloc(DIFF_HARDCORE_FLASH_CNT, sizeof(gameFlash_t));
		assert(*flashSequence != NULL);
		_minigame_generateRandomFlash(*flashSequence, DIFF_HARDCORE_FLASH_CNT);
		break;
	default:
		DEBUG_PRINTF("Invalid difficulty level for calculate flash!!!");
		break;
	}
	_gameState = STATE_PLAY;
}

static void _minigame_play() {
	display_Write("GO");

	joystick_waitForPress();
}

void minigame_Init(void) {
	uint32_t ret;
	DEBUG_PRINTF("starting minigame!");
	display_Init();
	joystick_init();
	ret = compass_Init();

	if (ret != 0) {
		DEBUG_PRINTF("No calibration found ...");
		display_Write("CALIB");
		compass_Calibrate();
	}
	display_Clear();
}

void minigame_Run(void) {
	gameFlash_t *flashSequence = NULL;

	DEBUG_PRINTF("Starting game ... ");

	while (true) {
		switch (_gameState) {
		case STATE_INITIAL_SELECTION:
			DEBUG_PRINTF("In Initial Selection State");
			_minigame_mainMenu();
			break;
		case STATE_CONFIGURE:
			DEBUG_PRINTF("In Configure State");
			_minigame_config();
			break;
		case STATE_CALCULATE_FLASHES:
			DEBUG_PRINTF("In Calculate Flashes State");
			_minigame_calculateFlashSequence(&flashSequence);
			break;
		case STATE_PLAY:
			DEBUG_PRINTF("In Play State");
			_minigame_play();
			break;
		default:
			DEBUG_PRINTF("Wrong state for state machine!!!");
			break;
		}
	}

	double magn[3];

//	compass_GetRawValues(magn_vals);
//	DEBUG_PRINTF("Raw Values x: %i, y: %i, z: %i", magn_vals[0], magn_vals[1], magn_vals[2]);
	compass_GetValues(magn);
	DEBUG_PRINTF("Magn Values x: %i, y: %i, z: %i", magn[0], magn[1], magn[2]);
	DEBUG_PRINTF("heading is %d  !", (int32_t ) heading_Calc_getHeading(magn[0], magn[1]));
	DEBUG_PRINTF("\033[3A");
	HAL_Delay(1000);
}
