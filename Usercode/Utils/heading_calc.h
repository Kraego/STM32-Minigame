/*
 * heading_calc.h
 *
 *  Created on: Jan 13, 2021
 *      Author: tkrax
 */

#ifndef UTILS_HEADING_CALC_H_
#define UTILS_HEADING_CALC_H_

#include <stdint.h>

uint32_t heading_GetHeading();
uint32_t heading_NormalizeDegree(uint32_t degrees);

#endif /* UTILS_HEADING_CALC_H_ */
