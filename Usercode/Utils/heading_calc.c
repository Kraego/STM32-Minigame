/*
 * Heading_calc.c
 *
 *  Created on: Jan 13, 2021
 *      Author: tkrax
 */

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
double heading_Calc_getHeading(double x, double y){
	double heading = (atan2(y * M_GN, x * M_GN) * 180) / M_PI;
	return heading < 0 ? heading + 360 : heading;
}
