/*
 * HSOW2_Way.h
 *
 *  Created on: 21 дек. 2018 г.
 *      Author: user
 */

#ifndef HSOW2_WAY_H_
#define HSOW2_WAY_H_

#include "main.h"

typedef struct {
		uint32_t DPP;      // DPP counter
		uint32_t NowTime;  // 32-bit Timer
} DppTimSampleTypeDef;

typedef struct {
		int16_t MinAcc;   // minimal acc sample by 40 pulses encoder
		int16_t MaxAcc;   // maximum acc sample by 40 pulses encoder
		int16_t AvrAcc;   // average acc sample by 40 pulses encoder
} AveragesAccTypeDef;

typedef struct {
		int16_t MinBump;   // minimal vertical motion relatively rail level (in micrometers)
		int16_t MaxBump;   // maximum vertical motion relatively rail level (in micrometers)
} BumpsTypeDef;

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
void HSOW2_Way_Init(void);
void Reset_AverageAcc(void);
void Reset_Bumps(void);

#endif /* HSOW2_WAY_H_ */
