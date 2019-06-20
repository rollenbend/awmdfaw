/*
 * HSOW2_Way.c
 *
 *  Created on: 21 дек. 2018 г.
 *      Author: user
 */
#include "HSOW2_Way.h"

//#define USE_CAN
#define USE_RS485
#define IMITATION

//#ifdef USE_CAN
	DppTimSampleTypeDef DppStruct;
	AveragesAccTypeDef  AverageAcc;
	BumpsTypeDef        Bumps;
//#endif
#ifdef USE_RS485
	SENSOR_DPP_FRAME DppFrame;
#endif

extern float  SumAccSamples[3];
extern int    CalculateCount;
int           SmoothSpeedCount = 0;
uint32_t      McSecTimePrevious = 0;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if ((htim == &htim1)||(htim == &htim3)){
		HAL_GPIO_TogglePin(YELLOW_GPIO_Port, YELLOW_Pin);

		Incr_DPP();
		Send_DPP();
		Reset_Parameters();

	}
}


void Incr_DPP(void)
{
	Get_Speed();
#ifdef USE_CAN
	DppStruct.DPP += 5;
	DppStruct.NowTime = TIM2 -> CNT;
	if (CalculateCount > 0)
	{
		AverageAcc.AvrAcc = (int16_t)(SumAccSamples[0]/(float)CalculateCount);
		// min and max global in math.c file
		Bumps.AvrAccYaxis = (int16_t)(SumAccSamples[1]/(float)CalculateCount);
		Bumps.AvrAccZaxis = (int16_t)(SumAccSamples[2]/(float)CalculateCount);
		// bumps global in math.c file
	}
#endif
#ifdef USE_RS485
	DppFrame.syncro = 0x53;
	DppFrame.size = sizeof(DppFrame)-3;
	DppFrame.DPP += 5;
	DppFrame.speed = AverageAcc.WagonSpeed;
	DppFrame.timer = TIM2 -> CNT;
	DppFrame.status = 0;
	DppFrame.reply = 0;
	DppFrame.MinAccXaxis = AverageAcc.MinAcc;
	DppFrame.MaxAccXaxis = AverageAcc.MaxAcc;
	if (CalculateCount > 0)
	{
		DppFrame.AvrAccXaxis = (int16_t)(SumAccSamples[0]/(float)CalculateCount);
		DppFrame.AvrAccYaxis = (int16_t)(SumAccSamples[1]/(float)CalculateCount);
		DppFrame.AvrAccZaxis = (int16_t)(SumAccSamples[2]/(float)CalculateCount);
	}
	DppFrame.MinBump = Bumps.MinBump;
	DppFrame.MaxBump = Bumps.MaxBump;
	DppFrame.cs = 0;
	for (int i=0; i<sizeof(DppFrame)-1; i++) DppFrame.cs += *(uint8_t*)(&DppFrame.syncro+i);
#endif
}

void Send_DPP(void)
{
#ifdef USE_CAN
	HSOW2_CAN_Transmit((uint8_t*)&DppStruct, sizeof(DppStruct));
	HSOW2_CAN_Transmit((uint8_t*)&AverageAcc, sizeof(AverageAcc));
	HSOW2_CAN_Transmit((uint8_t*)&Bumps, sizeof(Bumps));
#endif
#ifdef USE_RS485
	HAL_UART_Transmit_DMA(&huart1, (uint8_t*)&DppFrame, sizeof(DppFrame));
#endif
}

void Reset_Parameters(void)
{
	Reset_AverageAcc();
	Reset_Bumps();
}
void Get_Speed(void)
{
	int PassedTime;
	SmoothSpeedCount++;
	if (McSecTimePrevious > TIM2->CNT) McSecTimePrevious = 0;
	PassedTime = (int) ( TIM2->CNT - McSecTimePrevious);
	if (PassedTime > 200000)
	{
		if (TIM1->CR1 & (1 << 4))
			AverageAcc.WagonSpeed = (short) (10440000 * SmoothSpeedCount / PassedTime); // Direction Bit
		else
			AverageAcc.WagonSpeed = (short) (-10440000 * SmoothSpeedCount / PassedTime);
		McSecTimePrevious = TIM2->CNT;
		SmoothSpeedCount = 0;
	}
}

void HSOW2_Way_Init(void)
{
	DppStruct.DPP = 0;
	DppStruct.NowTime = 0;

	HAL_TIM_Base_Start(&htim2);

#ifdef IMITATION
//	HSOW2_Start_Imitation_DPP(100);
#else
	HSOW2_Start_Encoder_Work();
#endif
}



void HSOW2_Start_Imitation_DPP(uint16_t Speed)
{
	HAL_TIM_Base_Stop_IT(&htim3);
	HAL_TIM_Base_DeInit(&htim3);
	HAL_TIM_Base_Stop_IT(&htim1);
	HAL_TIM_Encoder_Stop(&htim1, TIM_CHANNEL_ALL);
	HAL_TIM_Base_DeInit(&htim1);
	DppStruct.DPP = 0;

	ini_imit_tim();
	if (Speed > 2) TIM3->ARR = Speed - 1;
	else TIM3->ARR = 1;
	HAL_TIM_Base_Start_IT(&htim3);
}

void HSOW2_Start_Encoder_Work(void)
{
	HAL_TIM_Base_Stop_IT(&htim3);
	HAL_TIM_Base_DeInit(&htim3);
	HAL_TIM_Base_Stop_IT(&htim1);
	HAL_TIM_Encoder_Stop(&htim1, TIM_CHANNEL_ALL);
	HAL_TIM_Base_DeInit(&htim1);
	DppStruct.DPP = 0;

	ini_enc_tim();
	HAL_TIM_Base_Start_IT(&htim1);
	HAL_TIM_Encoder_Start(&htim1, TIM_CHANNEL_ALL);
}
void Reset_AverageAcc(void)
{
	AverageAcc.MinAcc = 32767;
	AverageAcc.MaxAcc = -32768;
	AverageAcc.AvrAcc = 0;
	CalculateCount = 0;
	SumAccSamples[0] = 0;
	SumAccSamples[1] = 0;
	SumAccSamples[2] = 0;
}
void Reset_Bumps(void)
{
	Bumps.MinBump = 32767;
	Bumps.MaxBump = -32768;
}

