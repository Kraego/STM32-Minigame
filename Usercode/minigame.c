/*
 * minigame.c
 *
 *  Created on: 17 Jan 2021
 *      Author: tkrax
 */
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>
#include "minigame.h"
#include "arrowRotator.h"
#include "led.h"
#include "joystick.h"
#include "compass.h"
#include "display.h"
#include "debug.h"
#include "heading_calc.h"

#define PLAYER_CNT_2			    ("PL 2")
#define PLAYER_CNT_4			    ("PL 4")
#define PLAY_TXT				    ("PLAY")
#define CALIBRATE_TXT			    ("CAL")
#define DIFF_LEVEL_CNT			    (4)
#define DIFF_LEVE_STRLEN		    (6)
#define DIFF_EASY_TXT			    ("DIF E")
#define DIFF_MEDIUM_TXT			    ("DIF M")
#define DIFF_ADVANCED_TXT		    ("DIF A")
#define DIFF_HARDCORE_TXT		    ("DIF H")
#define DIFF_EASY_FLASH_CNT			(3)
#define DIFF_MEDIUM_FLASH_CNT		(5)
#define DIFF_ADVANCED_FLASH_CNT		(7)
#define ROTATION_TOLERANCE_DEG		(15)
#define DIFF_HARDCORE_FLASH_CNT		(15)
#define FLASH_TIME_MS				(500)
#define WAIT_DELAY_MS				(250)
#define FLASH_ENTER_TIMEOUT_TICKS	(4000)

#define DIFF_DECR_CURRENT	(_gameConfig.difficulty == DIFF_EASY ? DIFF_HARDCORE : (_gameConfig.difficulty - 1) % DIFF_LEVEL_CNT)

typedef enum {
	MENU_CALIBRATE, MENU_PLAY
} menuSelection_t;

typedef enum {
	STATE_INITIAL_SELECTION,
	STATE_CONFIGURE,
	STATE_CALCULATE_FLASHES,
	STATE_WAIT_FOR_CENTER,
	STATE_RUN_FLASHES,
	STATE_ROTATE_ARROW,
	STATE_ROTATE_BOARD,
	STATE_ENTER_FLASH_SEQUENCE,
	STATE_VICTORY,
	STATE_FAILED,
	STATE_DONE
} gameState_t;

typedef enum {
	DIFF_EASY = 0, DIFF_MEDIUM = 1, DIFF_ADVANCED = 2, DIFF_HARDCORE = 3
} difficulty_t;

typedef struct {
	uint32_t players;
	difficulty_t difficulty;
	uint32_t current_Player;
} gameData_t;

typedef enum {
	GREEN = 0, RED = 1
} gameFlash_t;

typedef struct {
	gameFlash_t *sequence;
	uint32_t flashCount;
} gameFlashSequence_t;

static const gameFlash_t flashMap[] = { GREEN, RED };
static char difficulty_map[DIFF_LEVEL_CNT][DIFF_LEVE_STRLEN] = { DIFF_EASY_TXT, DIFF_MEDIUM_TXT, DIFF_ADVANCED_TXT,
DIFF_HARDCORE_TXT };

static gameState_t _gameState = STATE_INITIAL_SELECTION;

static void _minigame_MainMenu() {
	joystick_press_t input = JOYSTICK_NOTHING;
	menuSelection_t _menuSelection = MENU_PLAY;

	display_ScrollText("         MINIGAME");
	HAL_Delay(WAIT_DELAY_MS);

	display_Write(PLAY_TXT);

	while (input != JOYSTICK_CENTER) {
		joystick_WaitForPress(&input, JOYSTICK_WAIT_FOREVER);

		if (input == JOYSTICK_UP || input == JOYSTICK_DOWN) {
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

static void _minigame_ConfigPlayers(gameData_t *gameConfig) {
	joystick_press_t input = JOYSTICK_NOTHING;
	uint32_t playerCount = 2;

	display_Write(PLAYER_CNT_2);
	gameConfig->players = playerCount;

	while (input != JOYSTICK_CENTER) {
		joystick_WaitForPress(&input, JOYSTICK_WAIT_FOREVER);

		if (input == JOYSTICK_UP || input == JOYSTICK_DOWN) {
			if (gameConfig->players == 2) {
				display_Write(PLAYER_CNT_4);
				gameConfig->players = 4;
			} else if (gameConfig->players == 4) {
				display_Write(PLAYER_CNT_2);
				gameConfig->players = 2;
			}
		}
	}
}

static void _minigame_IncrDifficulty(gameData_t *gameConfig) {
	gameConfig->difficulty = (gameConfig->difficulty + 1) % DIFF_LEVEL_CNT;
}

static void _minigame_DecrDifficulty(gameData_t *gameConfig) {
	gameConfig->difficulty =
			gameConfig->difficulty == DIFF_EASY ? DIFF_HARDCORE : (gameConfig->difficulty - 1) % DIFF_LEVEL_CNT;
}

static void _minigame_ConfigDifficulty(gameData_t *gameConfig) {
	joystick_press_t input = JOYSTICK_NOTHING;

	display_Write(DIFF_MEDIUM_TXT);
	gameConfig->difficulty = DIFF_MEDIUM;

	while (input != JOYSTICK_CENTER) {
		joystick_WaitForPress(&input, JOYSTICK_WAIT_FOREVER);

		if (input == JOYSTICK_UP) {
			_minigame_IncrDifficulty(gameConfig);
		} else if (input == JOYSTICK_DOWN) {
			_minigame_DecrDifficulty(gameConfig);
		}
		display_Write(difficulty_map[gameConfig->difficulty]);
	}
}

static void _minigame_Config(gameData_t *gameConfig) {
	gameConfig->current_Player = 0;
	_minigame_ConfigPlayers(gameConfig);
	_minigame_ConfigDifficulty(gameConfig);
	DEBUG_PRINTF("I: Start game for %d player: difficulty is '%s'", gameConfig->players,
			difficulty_map[gameConfig->difficulty]);
}

static void _minigame_GenerateRandomFlash(gameFlash_t *flashSequence, uint32_t flashCount) {
	uint32_t flashsTodo = flashCount;
	uint32_t idx = 0;
	srand(HAL_GetTick());

	while (flashsTodo) {
		uint32_t flashIdx = rand() % 2;
		flashSequence[idx++] = flashMap[flashIdx];
		flashsTodo--;
	}
}

/**
 * Generate a random flash sequence. Attention: flashSequence must be freed by caller.
 *
 * @param flashSequence the generated Flashsequence
 * @param difficulty difficulty of the flashsequence
 *
 * @return the flash count of the sequence
 */
static uint32_t _minigame_CalculateFlashSequence(gameFlash_t **flashSequence, difficulty_t difficulty) {
	uint32_t flashCount;

	switch (difficulty) {
	case DIFF_EASY:
		*flashSequence = (gameFlash_t*) calloc(DIFF_EASY_FLASH_CNT, sizeof(gameFlash_t));
		_minigame_GenerateRandomFlash(*flashSequence, DIFF_EASY_FLASH_CNT);
		flashCount = DIFF_EASY_FLASH_CNT;
		break;
	case DIFF_MEDIUM:
		*flashSequence = (gameFlash_t*) calloc(DIFF_MEDIUM_FLASH_CNT, sizeof(gameFlash_t));
		assert(*flashSequence != NULL);
		_minigame_GenerateRandomFlash(*flashSequence, DIFF_MEDIUM_FLASH_CNT);
		flashCount = DIFF_MEDIUM_FLASH_CNT;
		break;
	case DIFF_ADVANCED:
		*flashSequence = (gameFlash_t*) calloc(DIFF_ADVANCED_FLASH_CNT, sizeof(gameFlash_t));
		assert(*flashSequence != NULL);
		_minigame_GenerateRandomFlash(*flashSequence, DIFF_ADVANCED_FLASH_CNT);
		flashCount = DIFF_ADVANCED_FLASH_CNT;
		break;
	case DIFF_HARDCORE:
		*flashSequence = (gameFlash_t*) calloc(DIFF_HARDCORE_FLASH_CNT, sizeof(gameFlash_t));
		assert(*flashSequence != NULL);
		_minigame_GenerateRandomFlash(*flashSequence, DIFF_HARDCORE_FLASH_CNT);
		flashCount = DIFF_HARDCORE_FLASH_CNT;
		break;
	default:
		DEBUG_PRINTF("E: Invalid difficulty level for calculate flash!!!");
		break;
	}
	return flashCount;
}

static void _minigame_FlashGreen() {
	led_SwitchGreen(true);
	HAL_Delay(FLASH_TIME_MS);
	led_SwitchGreen(false);
}

static void _minigame_FlashRed() {
	led_SwitchRed(true);
	HAL_Delay(FLASH_TIME_MS);
	led_SwitchRed(false);
}

static void _minigame_RunFlashSequence(gameFlashSequence_t *flashSequence) {
	uint32_t flashsTodo = flashSequence->flashCount;
	uint32_t idx = 0;

	while (flashsTodo) {
		if (flashSequence->sequence[idx++] == GREEN) {
			_minigame_FlashGreen();
		} else {
			_minigame_FlashRed();
		}
		HAL_Delay(FLASH_TIME_MS);
		flashsTodo--;
	}
}

/**
 * Rotate arrow random
 *
 * @param players players of the game
 *
 * @return the offset to the next player
 */
static uint32_t _minigame_RotateArrow(uint32_t players) {
	uint32_t delay_ms = 0;
	uint32_t randomness;

	srand(HAL_GetTick());
	randomness = rand() % players;
	DEBUG_PRINTF("I: Add Randomness %d", randomness);

	while (delay_ms <= 250) {
		arrowRotator_FullRoation(delay_ms);
		delay_ms += 25;
	}

	arrowRotator_Rotate(delay_ms + 50, randomness);
	HAL_Delay(750);
	return randomness;
}

static void _minigame_WaitForCenter() {
	joystick_press_t input = JOYSTICK_NOTHING;
	display_Write("GO");

	while (input != JOYSTICK_CENTER) {
		joystick_WaitForPress(&input, JOYSTICK_WAIT_FOREVER);
	};
	HAL_Delay(WAIT_DELAY_MS);
}

static bool _minigame_CheckHeading(uint32_t target, uint32_t tolerance){
	uint32_t current = heading_GetHeading();

	if (current > target){
		return (current - target) < tolerance;
	} else {
		return (target - current) < tolerance;
	}
}

static bool _minigame_RunCheckRotationTo(uint32_t toPlayer, uint32_t players){
	uint32_t startHeading = heading_GetHeading();
	uint32_t degreesBetweenPlayers = players == 4 ? 90 : 180;
	int32_t destinationHeading = startHeading - toPlayer * degreesBetweenPlayers;
	uint32_t startTicks;

	destinationHeading = destinationHeading < 0 ? destinationHeading + 360 : destinationHeading;

	DEBUG_PRINTF("I: Start heading is %d target is %d (to player: %d)! ", startHeading, destinationHeading, toPlayer);
	display_Write("ROTATE");
	display_ShowBars(4);

	startTicks = HAL_GetTick();
	while (!_minigame_CheckHeading(destinationHeading, ROTATION_TOLERANCE_DEG)){
		uint32_t diff = HAL_GetTick() - startTicks;

		if (diff > 6000){
			DEBUG_PRINTF("I: Heading at end was %d", heading_GetHeading());
			return false;
		}

		if (diff < 1500) {
			display_ShowBars(3);
		} else if (diff < 3000) {
			display_ShowBars(2);
		}else if (diff <= 4500) {
			display_ShowBars(1);
		}
		HAL_Delay(50);
	}
	display_ShowBars(0);
	return true;
}

static bool _minigame_CheckSequence(gameFlashSequence_t *flashSequence){
	display_Write("FLASH");
	uint32_t flashsCheck = flashSequence->flashCount;
	joystick_press_t input;
	uint32_t idx = 0;

	while (flashsCheck) {
		joystick_WaitForPress(&input, FLASH_ENTER_TIMEOUT_TICKS);

		if (flashSequence->sequence[idx++] == GREEN) {
			if (input != JOYSTICK_RIGHT){
				DEBUG_PRINTF("I: Failure expected JOYSTICK_RIGHT for flash %d", idx);
				return false;
			}
		} else {
			if (input != JOYSTICK_LEFT){
				DEBUG_PRINTF("I: Failure expected JOYSTICK_LEFT for flash %d", idx);
				return false;
			}
		}
		flashsCheck--;
	}
	return true;
}

/**
 * Initialize minigame
 */
void minigame_Init(void) {
	uint32_t ret;
	DEBUG_PRINTF("I: starting mini game!");
	display_Init();
	led_Init();
	joystick_Init();
	ret = compass_Init();

	if (ret != 0) {
		DEBUG_PRINTF("W: No calibration found ...");
		display_Write("CALIB");
		compass_Calibrate();
	}
	display_Clear();
}

/**
 * Run the minigame. Function never returns.
 */
void minigame_Run(void) {
	gameFlash_t *flashes = NULL;
	gameFlashSequence_t flashSequence = { };
	gameData_t gameData = { };
	bool success;

	while (true) {
		switch (_gameState) {
		case STATE_INITIAL_SELECTION:
			DEBUG_PRINTF("I: In Initial Selection State");
			_minigame_MainMenu();
			break;
		case STATE_CONFIGURE:
			DEBUG_PRINTF("I: In Configure State");
			_minigame_Config(&gameData);
			_gameState = STATE_CALCULATE_FLASHES;
			break;
		case STATE_CALCULATE_FLASHES:
			DEBUG_PRINTF("I: In Calculate Flashes State");
			uint32_t flashCount = _minigame_CalculateFlashSequence(&flashes, gameData.difficulty);
			flashSequence.sequence = flashes;
			flashSequence.flashCount = flashCount;
			_gameState = STATE_WAIT_FOR_CENTER;
			break;
		case STATE_WAIT_FOR_CENTER:
			DEBUG_PRINTF("I: In Wait for Center State");
			_minigame_WaitForCenter();
			_gameState = STATE_RUN_FLASHES;
			break;
		case STATE_RUN_FLASHES:
			DEBUG_PRINTF("I: In Run Flashes State");
			_minigame_RunFlashSequence(&flashSequence);
			_gameState = STATE_ROTATE_ARROW;
			break;
		case STATE_ROTATE_ARROW:
			DEBUG_PRINTF("I: In rotate Arrow State");
			gameData.current_Player += _minigame_RotateArrow(gameData.players);
			_gameState = STATE_ROTATE_BOARD;
			break;
		case STATE_ROTATE_BOARD:
			DEBUG_PRINTF("I: In checkRotation State");
			success = _minigame_RunCheckRotationTo(gameData.current_Player, gameData.players);
			_gameState = success ? STATE_ENTER_FLASH_SEQUENCE : STATE_FAILED;
			break;
		case STATE_ENTER_FLASH_SEQUENCE:
			DEBUG_PRINTF("I: In Enter Sequence State");
			success = _minigame_CheckSequence(&flashSequence);
			_gameState = success ? STATE_VICTORY : STATE_FAILED;
			break;
		case STATE_FAILED:
			display_ScrollText("      YOU FAILED");
			HAL_Delay(WAIT_DELAY_MS);
			_gameState = STATE_DONE;
			break;
		case STATE_VICTORY:
			display_ScrollText("      VICTORY");
			HAL_Delay(WAIT_DELAY_MS);
			_gameState = STATE_DONE;
			break;
		case STATE_DONE:
			DEBUG_PRINTF("I: In Done State ... Cleanup");
			free(flashes);
			flashes = NULL;
			gameData.current_Player = 0;
			_gameState = STATE_CALCULATE_FLASHES;
			break;
		default:
			DEBUG_PRINTF("E: Wrong state for state machine!!!");
			break;
		}
	}
}
