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

typedef enum {
	WantSyncro,
	WantSize,
	WantCode,
	WantCs
} PROTOCOL;

#define SyncroByte 	 	0x53
#define SizeByte  		0x01

uint8_t Get_command_code(void);
void start_cmd_receiving(UART_HandleTypeDef *huart);
void DecodingCmd(uint8_t Bt);
void Execute_cmd(uint8_t Code);

#endif /* COMMANDS_H_ */
