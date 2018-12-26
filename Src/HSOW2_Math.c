/*
 * HSOW2_Math.c
 *
 *  Created on: 21 дек. 2018 г.
 *      Author: IlyaO
 *
 *      This file calculate min, max and averages values. Also it integrate angular speed for bumps.
 */
#include "HSOW2_Math.h"
//#define float_type float
#define float_type double
//float        LastMinAccSample = 0xffffff;
//float        LastMaxAccSample = -0xffffff;
extern AveragesAccTypeDef AverageAcc;
extern BumpsTypeDef       Bumps;
extern DppTimSampleTypeDef DppStruct;

float    SumAccSamples   = 0;
int      CalculateCount  = 0;

uint16_t SizeArray = 5;
int16_t GyroArray[5] = {0};
uint32_t GyroCount = 0;
float_type AvrGyro = 0; float_type Bump_impuls = 0;

void IntegrateGyroData(int16_t GyroValue)
{
	HAL_GPIO_WritePin(YELLOW_GPIO_Port,YELLOW_Pin, ENABLE);
	GyroArray[GyroCount++ % SizeArray] = GyroValue; // ring buffer
	if (GyroCount >= SizeArray) {
		AvrGyro = 0;
		for (int i = 0; i < SizeArray; i++)
			AvrGyro += (float_type) GyroArray[i]; // when array is full, calc average
		AvrGyro /= (float_type) SizeArray;

		Bump_impuls += GyroArray[(GyroCount + SizeArray / 2) % SizeArray] - AvrGyro;   // mid point - average
	}

	HAL_GPIO_WritePin(YELLOW_GPIO_Port,YELLOW_Pin, DISABLE);
	Bumps.MaxBump = Bump_impuls;
	HSOW2_CAN_Transmit((uint8_t*)&Bumps, sizeof(Bumps));

//	GyroArray[GyroCount++%SizeArray] = GyroValue; // ring buffer
//	if (GyroCount==SizeArray) for (int i=0; i<SizeArray; i++) AvrGyro += (float)GyroArray[i]/(float)SizeArray; // when array is full, calc average
//	if (GyroCount>SizeArray) {
//		AvrGyro += (float)(GyroValue - GyroArray[GyroCount%SizeArray])/(float)SizeArray; // move window and change average
//		Bump_impuls += GyroArray[(GyroCount+SizeArray/2)%SizeArray] - AvrGyro;   // mid point - average
//	}
//	DppStruct.DPP=(int32_t)Bump_impuls;
//	DppStruct.NowTime = TIM2 -> CNT;
//	HSOW2_CAN_Transmit((uint8_t*)&DppStruct, sizeof(DppStruct));
}


void CalculateAccSample(int16_t AccValue)
{
	float AccSample = 0;
	AccSample = ((float)AccValue)/LSM_Acc_Sensitivity_16g/LSM_Acc_Divider; // change value to G-scale

	if (AverageAcc.MinAcc>(int16_t)AccSample) AverageAcc.MinAcc = (int16_t)AccSample;
	if (AverageAcc.MaxAcc<(int16_t)AccSample) AverageAcc.MaxAcc = (int16_t)AccSample;

	SumAccSamples+=AccSample;
	CalculateCount++;
}
