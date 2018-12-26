/*
 * CAN_HSOW2_Driver.h
 *
 *  Created on: 17 дек. 2018 г.
 *      Author: user
 */

#ifndef CAN_HSOW2_DRIVER_H_
#define CAN_HSOW2_DRIVER_H_

#include "main.h"


#define Acc_Frame_StdId          (uint32_t)0x0cc
#define Bumps_Frame_StdId        (uint32_t)0x111
#define DPP_Frame_StdId          (uint32_t)0x0dd



void HSOW2_CAN_Transmit(uint8_t *AccGyroData, uint16_t Size);
void HSOW2_CAN_Init(void);








#endif /* CAN_HSOW2_DRIVER_H_ */
