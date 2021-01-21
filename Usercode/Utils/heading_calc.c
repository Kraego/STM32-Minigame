/*
 * Heading_calc.c
 *
 *  Created on: Jan 13, 2021
 *      Author: tkrax
 */
#include <math.h>
#include "compass.h"

#ifndef M_PI
#define M_PI            (3.14159265358979323846264338327950288)
#endif

#define M_GN            (0.58) // see datasheet lsm303 page 11

/**
 * Calculate heading for given x & y axis values (of MEMS magnetic field sensor).
 *
 * @param x value on x axis
 * @param y value on x axis
 * @return the heading angle in degrees (no tilt compensation)
 */
double _heading_CalcHeading(double x, double y) {
	double heading = (atan2(y * M_GN, x * M_GN) * 180) / M_PI;
	return heading < 0 ? heading + 360 : heading;
}

/**
 * Get current heading.
 *
 * @return the relative heading in degrees
 */
uint32_t heading_GetHeading() {
	double dataXYZ[3];
	compass_GetValues(dataXYZ);
	double heading = _heading_CalcHeading(dataXYZ[0], dataXYZ[1]);
	return (uint32_t) heading;
}
