/*
 * commands.c
 *
 *  Created on: 6 θών. 2019 γ.
 *      Author: user
 */

#include "main.h"

SENSOR_COMMANDS cmd;
void start_cmd_receiving(UART_HandleTypeDef *huart)
{
	HAL_UART_Receive_DMA(huart, (uint8_t*)&cmd, sizeof(cmd));
//	HAL_UART_Receive_IT(huart, (uint8_t*)&cmd, sizeof(cmd));
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	uint8_t cs = cmd.Syncro + cmd.Size + cmd.Code;
	if (cmd.Syncro == 0x53 && cmd.Size == 1 && cmd.cs == cs)
	{
		switch (cmd.Code) {
		case 0:
			HSOW2_Start_Encoder_Work();
			break;
		case 1:
			HSOW2_Start_Imitation_DPP(4); // 250 km/h
			break;
		case 2:
			HSOW2_Start_Imitation_DPP(8); // 125 km/h
			break;
		case 3:
			HSOW2_Start_Imitation_DPP(15); // 62.5 km/h
			break;
		case 4:
			HSOW2_Start_Imitation_DPP(29); // 31.25 km/h
			break;
		case 5:
			HSOW2_Start_Imitation_DPP(57); // 15.6 km/h
			break;
		case 6:
			HSOW2_Start_Imitation_DPP(112); // 8 km/h
			break;
		case 7:
			HSOW2_Start_Imitation_DPP(225); // 4 km/h
			break;
		default:
			break;
		}
	}
}
