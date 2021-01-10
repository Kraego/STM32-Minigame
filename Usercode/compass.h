/*
 * compass.h
 *
 *  Created on: Jan 10, 2021
 *      Author: tkrax
 */

#ifndef COMPASS_H_
#define COMPASS_H_

#include <stdint.h>

void compass_init(void);
void compass_getRawValues(int16_t *pDataXYZ);

#endif /* COMPASS_H_ */
