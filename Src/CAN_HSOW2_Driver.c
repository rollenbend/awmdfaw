/*
 * CAN_HSOW2_Driver.c
 *
 *  Created on: 17 дек. 2018 г.
 *      Author: user
 */

#include "CAN_HSOW2_Driver.h"
extern CAN_HandleTypeDef hcan;
extern DppTimSampleTypeDef DppStruct;
extern AveragesAccTypeDef  AverageAcc;
extern BumpsTypeDef        Bumps;
uint32_t CAN_LED_Turn_Count=0;

void HSOW2_CAN_Transmit(uint8_t *AccGyroData, uint16_t Size)
{
	CAN_TxHeaderTypeDef txheader;
	uint32_t can_tx_mailbox = CAN_TX_MAILBOX0;//[3] = {CAN_TX_MAILBOX0, CAN_TX_MAILBOX1, CAN_TX_MAILBOX2};

	if (AccGyroData == (uint8_t*)&DppStruct) {
		txheader.StdId = DPP_Frame_StdId;
		can_tx_mailbox = CAN_TX_MAILBOX0;
	}
	else if (AccGyroData == (uint8_t*)&AverageAcc) {
		txheader.StdId = Acc_Frame_StdId;
		can_tx_mailbox = CAN_TX_MAILBOX1;
	}
	else if (AccGyroData == (uint8_t*)&Bumps) {
		txheader.StdId = Bumps_Frame_StdId;
		can_tx_mailbox = CAN_TX_MAILBOX2;
	}
	else txheader.StdId = 0x1;

	txheader.DLC = Size % (MAX_BTS_IN_CAN_MSG + 1);
	txheader.IDE = CAN_ID_STD;
	txheader.RTR = CAN_RTR_DATA;
	txheader.ExtId = 0;
	txheader.TransmitGlobalTime = DISABLE;

	HAL_CAN_AddTxMessage(&hcan, &txheader, AccGyroData, &can_tx_mailbox);
}
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	uint32_t rxfifo =    CAN_RX_FIFO0;
	CAN_RxHeaderTypeDef  rxheader;
	uint8_t rxdata[8] =  {0};
	HAL_CAN_GetRxMessage(hcan, rxfifo, &rxheader, rxdata);
	if (rxheader.StdId == 0x03)
		switch (rxdata[0])
		{
		case 0: HSOW2_Start_Encoder_Work();
		break;
		case 1: HSOW2_Start_Imitation_DPP(36); // 250 km/h
		break;
		case 2: HSOW2_Start_Imitation_DPP(72); // 125 km/h
		break;
		case 3: HSOW2_Start_Imitation_DPP(144); // 62.5 km/h
		break;
		case 4: HSOW2_Start_Imitation_DPP(288); // 31.25 km/h
		break;
		case 5: HSOW2_Start_Imitation_DPP(577); // 15.6 km/h
		break;
		case 6: HSOW2_Start_Imitation_DPP(1125); // 8 km/h
		break;
		case 7: HSOW2_Start_Imitation_DPP(2250); // 4 km/h
		break;
		}
}
void HAL_CAN_ErrorCallback(CAN_HandleTypeDef *hcan)
{
	if (hcan->ErrorCode)
		HAL_GPIO_TogglePin(WARNING_GPIO_Port, WARNING_Pin);
}
void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef *hcan)
{
//	HAL_GPIO_TogglePin(GREEN_GPIO_Port, GREEN_Pin);
}
void HAL_CAN_TxMailbox1CompleteCallback(CAN_HandleTypeDef *hcan)
{
//	HAL_GPIO_TogglePin(GREEN_GPIO_Port, GREEN_Pin);
}
void HAL_CAN_TxMailbox2CompleteCallback(CAN_HandleTypeDef *hcan)
{
//	HAL_GPIO_TogglePin(GREEN_GPIO_Port, GREEN_Pin);
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
	HSOW2_CAN_InitFilter();
	HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING | CAN_IT_TX_MAILBOX_EMPTY);
	HAL_CAN_Start(&hcan);

}

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

//	HAL_CAN_StateTypeDef canstate = HAL_CAN_GetState(&hcan);
//canstate = HAL_CAN_GetState(&hcan);
//	if (canstate-1) HAL_GPIO_TogglePin(YELLOW_GPIO_Port, YELLOW_Pin);
//	canstate = HAL_CAN_GetState(&hcan);
//	if (canstate-2) HAL_GPIO_TogglePin(YELLOW_GPIO_Port, YELLOW_Pin);
