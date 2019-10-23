/*
 * commands.c
 *
 *  Created on: 6 θών. 2019 γ.
 *      Author: user
 */

#include "main.h"

SENSOR_COMMANDS cmd;
int ProtocolCmd = WantSyncro;
uint8_t Cs;
uint8_t Byte;
uint8_t Code;
#define SyncroByte  0x53

void start_cmd_receiving(UART_HandleTypeDef *huart)
{
//	HAL_UART_Receive_DMA(huart, (uint8_t*)&cmd, sizeof(cmd));
	HAL_UART_Receive_IT(huart, &Byte, sizeof(Byte));
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	DecodingCmd(Byte);
	HAL_UART_Receive_IT(huart, &Byte, sizeof(Byte));
}

void DecodingCmd(uint8_t Bt)
{
	switch (ProtocolCmd)
	{
	case WantSyncro:
		if (Bt == SyncroByte)
		{
			ProtocolCmd = WantSize;
			Cs = Bt;
		}
		break;
	case WantSize:
		if (Bt == SizeByte)
		{
			Cs += Bt;
			ProtocolCmd = WantCode;
		}
		else
			ProtocolCmd = WantSyncro;
		break;
	case WantCode:
		Cs += Bt;
		Code = Bt;
		ProtocolCmd = WantCs;
		break;
	case WantCs:
		if (Cs == Bt)
		{
			Execute_cmd(Code);
		}
		ProtocolCmd = WantSyncro;
		break;
	default:
		ProtocolCmd = WantSyncro;
		break;
	}
}

void Execute_cmd(uint8_t Code)
{
//	uint8_t cs = cmd.Syncro + cmd.Size + cmd.Code;
//	if (cmd.Syncro == 0x53 && cmd.Size == 1 && cmd.cs == cs)
//	{
		switch (Code)
		{
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
		case 8:
			HAL_TIM_Base_Stop_IT(&htim3);
			HAL_TIM_Base_Stop_IT(&htim1);
			HAL_TIM_Encoder_Stop(&htim1, TIM_CHANNEL_ALL);
			break;
		default:
			break;
		}
//	}
}
uint8_t Get_command_code(void)
{
	return cmd.Code;
}
