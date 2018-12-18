/*
 * CAN_HSOW2_Driver.c
 *
 *  Created on: 17 дек. 2018 г.
 *      Author: user
 */

#include "CAN_HSOW2_Driver.h"
extern CAN_HandleTypeDef hcan;
uint32_t can_tx_mailbox[3] = {CAN_TX_MAILBOX0, CAN_TX_MAILBOX1, CAN_TX_MAILBOX2};
uint32_t CAN_LED_Turn_Count=0;

void HSOW2_CAN_Transmit(uint8_t *AccGyroData, uint16_t Size)
{
	CAN_TxHeaderTypeDef txheader;
	txheader.StdId = 0x0cc;
	txheader.DLC = 8;
	txheader.IDE = CAN_ID_STD;
	txheader.RTR = CAN_RTR_DATA;
	txheader.ExtId = 0;
	txheader.TransmitGlobalTime = ENABLE;

	uint32_t canmai=1;
	HAL_CAN_AddTxMessage(&hcan, &txheader, AccGyroData, &canmai);

	//HAL_StatusTypeDef status;
	//	int freetx = 0;
	//	int txpending = 0;
//	if (Size>6) {
//		for (uint16_t s=0; s<Size; s+=6 ) {
//			txheader.StdId = 0x111;
//			txheader.DLC = 6;
//			HAL_CAN_AddTxMessage(&hcan, &txheader, AccGyroData+s, can_tx_mailbox+((s/6)%3));
//		}
//		txheader.StdId = 0x111;
//		txheader.DLC = Size%6;
//		if (txheader.DLC != 0) HAL_CAN_AddTxMessage(&hcan, &txheader, AccGyroData+(Size/6), can_tx_mailbox);
//	} else {
//		txheader.StdId = 0x111;
//		txheader.DLC = Size;
//		HAL_CAN_AddTxMessage(&hcan, &txheader, AccGyroData, can_tx_mailbox);
//	}

//	txpending = HAL_CAN_IsTxMessagePending(&hcan, can_tx_mailbox[0]);
//	freetx = HAL_CAN_GetTxMailboxesFreeLevel(&hcan);
//	status = HAL_CAN_AddTxMessage(&hcan, &txheader, AccGyroData, can_tx_mailbox);
//	status = HAL_CAN_AddTxMessage(&hcan, &txheader, AccGyroData+6, can_tx_mailbox+1);
////	status = HAL_CAN_AddTxMessage(&hcan, &txheader, AccGyroData+6, can_tx_mailbox+2);
//	if (status) Got_Error();
//	freetx = HAL_CAN_GetTxMailboxesFreeLevel(&hcan);
//	if (!freetx) HAL_GPIO_TogglePin(WARNING_GPIO_Port, WARNING_Pin);
//	txpending = HAL_CAN_IsTxMessagePending(&hcan, can_tx_mailbox[0]);
//	if (txpending) HAL_GPIO_TogglePin(YELLOW_GPIO_Port, YELLOW_Pin);
}

void HAL_CAN_ErrorCallback(CAN_HandleTypeDef *hcan)
{
	hcan->ErrorCode;
	HAL_GPIO_TogglePin(WARNING_GPIO_Port, WARNING_Pin);
}
void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef *hcan)
{
	if (++CAN_LED_Turn_Count%50==0) HAL_GPIO_TogglePin(GREEN_GPIO_Port, GREEN_Pin);
}
void HAL_CAN_TxMailbox1CompleteCallback(CAN_HandleTypeDef *hcan)
{
	HAL_GPIO_TogglePin(GREEN_GPIO_Port, GREEN_Pin);
}
void HAL_CAN_TxMailbox0AbortCallback(CAN_HandleTypeDef *hcan)
{
	HAL_GPIO_TogglePin(WARNING_GPIO_Port, WARNING_Pin);
}

void HAL_CAN_TxMailbox1AbortCallback(CAN_HandleTypeDef *hcan)
{
	HAL_GPIO_TogglePin(WARNING_GPIO_Port, WARNING_Pin);
}

void HSOW2_CAN_InitFilter(void)
{
	CAN_FilterTypeDef filterconfig;
	filterconfig.FilterIdHigh = 0;
	filterconfig.FilterIdLow = 0;
	filterconfig.FilterMaskIdHigh = 0;
	filterconfig.FilterMaskIdLow = 0;
	filterconfig.FilterFIFOAssignment = CAN_FILTER_FIFO0;
	filterconfig.FilterBank = 0;
	filterconfig.FilterMode = CAN_FILTERMODE_IDMASK;
	filterconfig.FilterScale = CAN_FILTERSCALE_32BIT;
	filterconfig.FilterActivation = CAN_FILTER_ENABLE;

	HAL_CAN_ConfigFilter(&hcan, &filterconfig);
}
void HSOW2_CAN_Init(void)
{
	HAL_CAN_StateTypeDef canstate = HAL_CAN_GetState(&hcan);

	HSOW2_CAN_InitFilter();
	HAL_CAN_ActivateNotification(&hcan, CAN_IT_TX_MAILBOX_EMPTY); // |CAN_IT_BUSOFF
	canstate = HAL_CAN_GetState(&hcan);
	if (canstate-1) HAL_GPIO_TogglePin(YELLOW_GPIO_Port, YELLOW_Pin);
	HAL_CAN_Start(&hcan);

	canstate = HAL_CAN_GetState(&hcan);
	if (canstate-2) HAL_GPIO_TogglePin(YELLOW_GPIO_Port, YELLOW_Pin);
}


