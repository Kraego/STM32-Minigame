/*
 * compass.h
 *
 *  Created on: Jan 10, 2021
 *      Author: tkrax
 */

#ifndef COMPASS_H_
#define COMPASS_H_

#include <stdint.h>

void compass_Init(void);
void compass_Calibrate(void);
int compass_GetValues(double *pDataXYZ);
void compass_GetRawValues(int16_t *pDataXYZ);

#endif /* COMPASS_H_ */
