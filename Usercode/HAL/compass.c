/*
 *  compass.c
 *
 *  Created on: Jan 10, 2021
 *      Author: tkrax
 */
#include <limits.h>
#include <stdbool.h>
#include "compass.h"
#include "display.h"
#include "debug.h"
#include "stm32l476g_discovery_qspi.h"
#include "stm32l476g_discovery_compass.h"

#define CALIBRATION_MAGIC		(0xDEADBEEF)
#define CALIBRATION_ADDRESS		(0x1000)
#define CALIBRATION_SAMPLES		(3000)
#define SAMPLES_DELAY_MS		(10)

#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

#define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

typedef struct {
	uint32_t magic; // magic number to check for loading at init
	double biasX;
	double rangeX;
	double biasY;
	double rangeY;
	double biasZ;
	double rangeZ;
} compass_calibration_t;

static compass_calibration_t calibration = { };

static bool isCalibrated() {
	return calibration.magic == CALIBRATION_MAGIC;
}

/**
 * Initialize compass
 *
 * @retval 0 if success, -1 if no calibration data available - run compass_Calibrate
 */
uint32_t compass_Init() {
	DEBUG_PRINTF("I: Initializing Compass");

	BSP_QSPI_Init();
	BSP_QSPI_Read((uint8_t*) &calibration, CALIBRATION_ADDRESS, sizeof(compass_calibration_t));
	BSP_COMPASS_Init();

	if (isCalibrated()) {
		DEBUG_PRINTF("I: Loaded stored calibration conifg");
	} else {
		return -1;
	}

	return 0;
}

/**
 * Get raw values from compass
 *
 * @param  pDataXYZ Pointer on 3 magnetometer values table with
 *                  pDataXYZ[0] = X axis, pDataXYZ[1] = Y axis, pDataXYZ[2] = Z axis
 * @retval None
 */
void compass_GetRawValues(int16_t *pDataXYZ) {
	BSP_COMPASS_MagGetXYZ(pDataXYZ);
}

/**
 * Get calibrated values from compass
 *
 * @param  pDataXYZ Pointer on 3 magnetometer values table with
 *                  pDataXYZ[0] = X axis, pDataXYZ[1] = Y axis, pDataXYZ[2] = Z axis
 * @retval 0 if success, -1 if not calibrated
 */
uint32_t compass_GetValues(double *pDataXYZ) {
	int16_t rawValues[3];
	BSP_COMPASS_MagGetXYZ(rawValues);

	if (!isCalibrated()) {
		return -1;
	}
	pDataXYZ[0] = ((double) rawValues[0] - calibration.biasX) / calibration.rangeX;
	pDataXYZ[1] = ((double) rawValues[1] - calibration.biasY) / calibration.rangeY;
	pDataXYZ[2] = ((double) rawValues[2] - calibration.biasZ) / calibration.rangeZ;

	return 0;
}

/**
 * Run compass calibration - storing calibration data in flash
 */
void compass_Calibrate() {
	int16_t magBuffer[3];
	int16_t toDo = CALIBRATION_SAMPLES;
	int16_t xMax = 0;
	int16_t yMax = 0;
	int16_t zMax = 0;
	int16_t xMin = INT16_MAX;
	int16_t yMin = INT16_MAX;
	int16_t zMin = INT16_MAX;

	display_Write("DO CAL");
	DEBUG_PRINTF("I: Calibrating compass ...");
	while (toDo) {
		compass_GetRawValues(magBuffer);
		xMax = max(xMax, magBuffer[0]);
		yMax = max(yMax, magBuffer[1]);
		zMax = max(zMax, magBuffer[2]);
		xMin = min(xMin, magBuffer[0]);
		yMin = min(yMin, magBuffer[1]);
		zMin = min(zMin, magBuffer[2]);
		toDo--;
		HAL_Delay(SAMPLES_DELAY_MS);
	}
	calibration.biasX = (xMax + xMin) / 2;
	calibration.rangeX = (xMax - xMin) / 2;
	calibration.biasY = (yMax + yMin) / 2;
	calibration.rangeY = (yMax - yMin) / 2;
	calibration.biasZ = (zMax + zMin) / 2;
	calibration.rangeZ = (zMax - zMin) / 2;

	DEBUG_PRINTF("I: Calibration done");

	calibration.magic = CALIBRATION_MAGIC;
	BSP_QSPI_Erase_Chip();
	BSP_QSPI_Write((uint8_t*) &calibration, CALIBRATION_ADDRESS, sizeof(compass_calibration_t));
	DEBUG_PRINTF("I: Calibration stored");
}
