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
#include <math.h>


#define SizeArray       			     (uint16_t)49
#define TrolleyLength  				     (float)2000000
#define LSM_Gyro_Sensitivity_245dps	     (float)(245 / 32767)
#define LSM_Gyro_Datarate_1k66         	 (float)1660
#define Grad_in_radian   				 (float)57.295779
#define BumpCoefficient 				 (float)(0.000000078615)//(245 / 32767 / 1660 / 57.295779) sensetivety/datarate/(grad in radian)

void CalculateAccSample(int16_t AccValue);
void IntegrateGyroData(int16_t GyroValue);




#endif /* HSOW2_MATH_H_ */
