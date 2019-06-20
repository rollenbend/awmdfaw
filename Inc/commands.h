/*
 * commands.h
 *
 *  Created on: 6 θών. 2019 γ.
 *      Author: user
 */

#ifndef COMMANDS_H_
#define COMMANDS_H_

#pragma pack(push,1)
typedef struct{
	uint8_t Syncro;
	uint8_t Size;
	uint8_t Code;
	uint8_t cs;
} SENSOR_COMMANDS;
#pragma pack(pop)

void start_cmd_receiving(UART_HandleTypeDef *huart);

#endif /* COMMANDS_H_ */
