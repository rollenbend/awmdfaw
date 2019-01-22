/*
 * HSOW2_Math.c
 *
 *  Created on: 21 дек. 2018 г.
 *      Author: IlyaO
 *
 *      This file calculate min, max and averages values. Also it integrate angular speed for bumps.
 */
#include "HSOW2_Math.h"

extern AveragesAccTypeDef AverageAcc;
extern BumpsTypeDef       Bumps;

float    SumAccSamples   = 0;
int      CalculateCount  = 0;

int16_t GyroArray[SizeArray] = {0};
uint32_t GyroCount = 0;
float AvrGyro = 0;
float Bump_impuls = 0; float Bump_impuls_SI_scale = 0; int16_t Bump_impuls_SI_scale_16int = 0;

void IntegrateGyroData(int16_t GyroValue)
{
	HAL_GPIO_WritePin(YELLOW_GPIO_Port,YELLOW_Pin, ENABLE);
	GyroArray[GyroCount++ % SizeArray] = GyroValue; // ring buffer
	if ((GyroValue>GyroEdgeWagonRest||GyroValue<-GyroEdgeWagonRest) && (Bump_impuls == 0)) GyroCount=0;

	if (GyroCount >= SizeArray) {
		AvrGyro = 0;
		for (int i = 0; i < SizeArray; i++)
			AvrGyro += (float) GyroArray[i]; // when array is full, calc average
		AvrGyro /= (float) SizeArray;

		Bump_impuls += (float)GyroArray[(GyroCount + SizeArray / 2) % SizeArray] - AvrGyro;   // mid point - average

		Bump_impuls_SI_scale = TrolleyLength * sinf(Bump_impuls * BumpCoefficient); // vertical moving equally wagon cart length * sin(a), where "a" is integr*coeff
		Bump_impuls_SI_scale_16int = (int16_t)Bump_impuls_SI_scale;
		if (Bumps.MaxBump < Bump_impuls_SI_scale_16int) Bumps.MaxBump = Bump_impuls_SI_scale_16int;
		if (Bumps.MinBump > Bump_impuls_SI_scale_16int) Bumps.MinBump = Bump_impuls_SI_scale_16int;

//		if ((AvrGyro>200||AvrGyro<-200)&&(GyroCount==SizeArray))  {
//			GyroCount=0;
//			Bump_impuls=0;
//			Reset_Bumps();
//		}
//		if (Bumps.MaxBump < (int16_t)Bump_impuls_SI_scale) Bumps.MaxBump = (int16_t)Bump_impuls_SI_scale;
//		if (Bumps.MinBump > (int16_t)Bump_impuls_SI_scale) Bumps.MinBump = (int16_t)Bump_impuls_SI_scale;
	}

	//HSOW2_CAN_Transmit((uint8_t*)&Bumps, sizeof(Bumps));

	HAL_GPIO_WritePin(YELLOW_GPIO_Port,YELLOW_Pin, DISABLE);
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


//float        LastMinAccSample = 0xffffff;
//float        LastMaxAccSample = -0xffffff;
//#define float_type float
//#define float_type double
//	GyroArray[GyroCount++%SizeArray] = GyroValue; // ring buffer
//	if (GyroCount==SizeArray) for (int i=0; i<SizeArray; i++) AvrGyro += (float)GyroArray[i]/(float)SizeArray; // when array is full, calc average
//	if (GyroCount>SizeArray) {
//		AvrGyro += (float)(GyroValue - GyroArray[GyroCount%SizeArray])/(float)SizeArray; // move window and change average
//		Bump_impuls += GyroArray[(GyroCount+SizeArray/2)%SizeArray] - AvrGyro;   // mid point - average
//	}
//	DppStruct.DPP=(int32_t)Bump_impuls;
//	DppStruct.NowTime = TIM2 -> CNT;
//	HSOW2_CAN_Transmit((uint8_t*)&DppStruct, sizeof(DppStruct));
