/*
 * HSOW2_Math.c
 *
 *  Created on: 21 дек. 2018 г.
 *      Author: IlyaO
 *
 *      This file calculate min, max and averages values. Also it integrate angular speed for bumps.
 */
#include "HSOW2_Math.h"

//float        LastMinAccSample = 0xffffff;
//float        LastMaxAccSample = -0xffffff;
extern AveragesAccTypeDef AverageAcc;
extern PumpsTypeDef       Pumps;

float    SumAccSamples   = 0;
int      CalculateCount  = 0;


void CalculateAccSample(int16_t AccValue)
{
	float AccSample = 0;
	AccSample = ((float)AccValue)/LSM_Acc_Sensitivity_16g/LSM_Acc_Divider; // change value to G-scale

	if (AverageAcc.MinAcc>(int16_t)AccSample) AverageAcc.MinAcc = (int16_t)AccSample;
	if (AverageAcc.MaxAcc<(int16_t)AccSample) AverageAcc.MaxAcc = (int16_t)AccSample;

	SumAccSamples+=AccSample;
	CalculateCount++;
}

void IntegrateGyroData(int16_t GyroValue)
{
	Pumps.MinPump += GyroValue;
	Pumps.MaxPump -= GyroValue;
}


