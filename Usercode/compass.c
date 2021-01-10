/*
 *  compass.c
 *
 *  Created on: Jan 10, 2021
 *      Author: tkrax
 */
#include "compass.h"
#include "stm32l476g_discovery_compass.h"

/**
 * Initialize compass
 */
void compass_init(){
	BSP_COMPASS_Init();
}

/**
  * Get raw values from compass
  *
  * @param  pDataXYZ Pointer on 3 magnetometer values table with
  *                  pDataXYZ[0] = X axis, pDataXYZ[1] = Y axis, pDataXYZ[2] = Z axis
  * @retval None
  */
void compass_getRawValues(int16_t *pDataXYZ){
	BSP_COMPASS_MagGetXYZ(pDataXYZ);
}
