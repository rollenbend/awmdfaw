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
		int16_t MinAcc; 		  // minimal acc sample by 40 pulses encoder == 25 sm
		int16_t MaxAcc; 		  // maximum acc sample by 40 pulses encoder == 25 sm
		int16_t AvrAcc;  		  // average acc sample by 40 pulses encoder == 25 sm
		int16_t WagonSpeed; 	  // smooth speed (average for 200ms)
} AveragesAccTypeDef;

typedef struct {
		int16_t MinBump;		// minimal vertical motion relatively rail level (in micrometers)
		int16_t MaxBump;		// maximum vertical motion relatively rail level (in micrometers)
		int16_t AvrAccYaxis;  	// average Y axis acc sample by 40 pulses encoder == 25 sm
		int16_t AvrAccZaxis;  	// average Z axis acc sample by 40 pulses encoder == 25 sm
} BumpsTypeDef;

#pragma pack(push,1)
typedef struct {
	uint8_t syncro;
	uint8_t size;
	uint32_t DPP;

	int16_t speed;
	uint32_t timer;
	uint8_t status;
	uint8_t reply;

	int16_t MinAccXaxis;  		 // minimal acc sample by 40 pulses encoder
	int16_t MaxAccXaxis;  		 // maximum acc sample by 40 pulses encoder
	int16_t AvrAccXaxis;  		 // average acc sample by 40 pulses encoder
	int16_t AvrAccYaxis;  	// average Y axis acc sample by 40 pulses encoder == 25 sm
	int16_t AvrAccZaxis;  	// average Z axis acc sample by 40 pulses encoder == 25 sm

	int16_t MinBump;   // minimal vertical motion relatively rail level (in micrometers)
	int16_t MaxBump;   // maximum vertical motion relatively rail level (in micrometers)

	uint8_t cs;
} SENSOR_DPP_FRAME; // size 29-3=26 = 0x1a
#pragma pack(pop)

extern UART_HandleTypeDef huart1;
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;

void HSOW2_Way_Init(void);
void Reset_AverageAcc(void);
void Reset_Bumps(void);
void Get_Speed(void);
void HSOW2_Start_Imitation_DPP(uint16_t Speed);
void HSOW2_Start_Encoder_Work(void);

void Reset_Parameters(void);
void Send_DPP(void);
void Incr_DPP(void);
#endif /* HSOW2_WAY_H_ */
