/*
 * compass.h
 *
 *  Created on: Jan 10, 2021
 *      Author: tkrax
 */

#ifndef COMPASS_H_
#define COMPASS_H_

#include <stdint.h>

uint32_t compass_Init(void);
void compass_Calibrate(void);
uint32_t compass_GetValues(double *pDataXYZ);
void compass_GetRawValues(int16_t *pDataXYZ);

#endif /* COMPASS_H_ */
