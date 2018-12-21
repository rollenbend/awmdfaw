/*
 * HSOW2_Math.h
 *
 *  Created on: 21 дек. 2018 г.
 *      Author: IlyaO
 *
 *   This file calculate min, max and averages values. Also it integrate angular speed for bumps.
 */

#ifndef HSOW2_MATH_H_
#define HSOW2_MATH_H_

#include "main.h"

void CalculateAccSample(int16_t AccValue);
void IntegrateGyroData(int16_t GyroValue);




#endif /* HSOW2_MATH_H_ */
