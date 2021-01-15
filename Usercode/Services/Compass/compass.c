/*
 *  compass.c
 *
 *  Created on: Jan 10, 2021
 *      Author: tkrax
 */
#include <limits.h>
#include <stdbool.h>
#include "debug.h"
#include "compass.h"
#include "stm32l476g_discovery_compass.h"

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
	double biasX;
	double rangeX;
	double biasY;
	double rangeY;
	double biasZ;
	double rangeZ;
} compass_calibration_t;

static compass_calibration_t calibration = { };

static bool isCalibrated() {
	return calibration.biasX != 0 && calibration.rangeX && calibration.biasY != 0 && calibration.rangeY
			&& calibration.biasZ != 0 && calibration.rangeZ;
}

/**
 * Initialize compass
 */
void compass_Init() {
	DEBUG_PRINTF("Initializing Compass");
	BSP_COMPASS_Init();
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
int compass_GetValues(double *pDataXYZ) {
	int16_t rawValues[3];

	if (!isCalibrated()) {
		return -1;
	}
	BSP_COMPASS_MagGetXYZ(rawValues);
	pDataXYZ[0] = ((double) rawValues[0] - calibration.biasX) / calibration.rangeX;
	pDataXYZ[1] = ((double) rawValues[1] - calibration.biasY) / calibration.rangeY;
	pDataXYZ[2] = ((double) rawValues[2] - calibration.biasZ) / calibration.rangeZ;

	return 0;
}

void compass_Calibrate() {
	int16_t magBuffer[3];
	int16_t toDo = CALIBRATION_SAMPLES;
	int16_t xMax = 0;
	int16_t yMax = 0;
	int16_t zMax = 0;
	int16_t xMin = INT16_MAX;
	int16_t yMin = INT16_MAX;
	int16_t zMin = INT16_MAX;

	DEBUG_PRINTF("Calibrating compass ...");

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

	DEBUG_PRINTF("Calibration done");

	// Todo: store calibration data qspi
}
