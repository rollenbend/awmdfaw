/*
 * HSOW2_Way.c
 *
 *  Created on: 21 дек. 2018 г.
 *      Author: user
 */
#include "HSOW2_Way.h"

DppTimSampleTypeDef DppStruct;
AveragesAccTypeDef  AverageAcc;
BumpsTypeDef        Bumps;

extern float  SumAccSamples;
extern int    CalculateCount;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim == &htim1){
		HAL_GPIO_TogglePin(YELLOW_GPIO_Port, YELLOW_Pin);

		DppStruct.DPP++;
		DppStruct.NowTime = TIM2 -> CNT;
		HSOW2_CAN_Transmit((uint8_t*)&DppStruct, sizeof(DppStruct));

		if (CalculateCount) AverageAcc.AvrAcc = (int16_t)(SumAccSamples/(float)CalculateCount);
		HSOW2_CAN_Transmit((uint8_t*)&AverageAcc, sizeof(AverageAcc));
		Reset_AverageAcc();

		HSOW2_CAN_Transmit((uint8_t*)&Bumps, sizeof(Bumps));
		Reset_Bumps();
	}
}



void HSOW2_Way_Init(void)
{
	DppStruct.DPP = 0;
	DppStruct.NowTime = 0;

	Reset_AverageAcc();
	Reset_Bumps();

	HAL_TIM_Base_Start(&htim2);
	HAL_TIM_Base_Start_IT(&htim1);
}



void Reset_AverageAcc(void)
{
	AverageAcc.MinAcc = 32767;
	AverageAcc.MaxAcc = -32768;
	AverageAcc.AvrAcc = 0;
	CalculateCount = 0;
	SumAccSamples  = 0;
}
void Reset_Bumps(void)
{
	Bumps.MinBump = 32767;
	Bumps.MaxBump = -32768;
}
