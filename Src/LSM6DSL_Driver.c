/*
 * LSM6DSL_Driver.c
 *
 *  Created on: 14 дек. 2018 г.
 *      Author: user
 */
#include "LSM6DSL_Driver.h"

uint32_t LED_Turn_Count=0;


void LSM6DSL_Read_data(void)
{
	uint8_t Addrreadaccgyro = GYRO_OUT_Z_L|ReadCommand; // начинаем читать с GYRO_OUT_Z_L (26h), бит чтения = 1(0x80)
	AccGyroSampleTypeDef accgyro;

	CS_OFF;
	HAL_SPI_Transmit(&hspi1, &Addrreadaccgyro, 1, 0x100);
	HAL_SPI_Receive(&hspi1, (uint8_t*)&accgyro, sizeof(accgyro), 0x100);
	CS_ON;

	CalculateAccSample(accgyro.accdata);
	IntegrateGyroData(accgyro.gyrodata);

	if (++LED_Turn_Count%10000==0) HAL_GPIO_TogglePin(BLUE_IMU_GPIO_Port, BLUE_IMU_Pin);
}




void LSM6DSL_Init(void)
{
	LSM6DSL_Software_Reset();
	LSM6DSL_Set_ODR(ACC_ODR_833|ACC_SCALE_16G, GYRO_ODR_833|GYRO_SCALE_500dps);
	//LSM6DSL_FIFO_CTRL();
}

void LSM6DSL_Set_ODR(uint8_t Acc_ODR, uint8_t Gyro_ODR)
{
	uint8_t CR1_2[3]={0x10, Acc_ODR, Gyro_ODR}; // ставим выдачу в 833Гц для аксела и гиро. чувств 16g и 500dps

	CS_OFF;
	HAL_SPI_Transmit(&hspi1, CR1_2, sizeof(CR1_2), 0x1000);
	CS_ON;
}

void LSM6DSL_Software_Reset(void)
{
	uint8_t CR3[2]={0x12, 0x01};  // SW_RESET=1 сброс настроек датчика, чтобы предыдущие действия не мешались

	CS_OFF;
	HAL_SPI_Transmit(&hspi1, CR3, sizeof(CR3), 0x1000);
	CS_ON;
	HAL_Delay(1); // ждем пока сбросятся настройки
}

void Is_this_LSM6DSL(void)
{
	uint8_t LSM6DSL_Answer=0;
	uint8_t WHO_AM_I_Addr_read=0x8F;
	CS_OFF;
//	HAL_SPI_TransmitReceive_DMA(&hspi1, &WHO_AM_I_Addr_read, &LSM6DSL_Answer, 1);
//	HAL_SPI_Transmit_DMA(&hspi1, &WHO_AM_I_Addr_read, 1);
//	HAL_Delay(1);
//	HAL_SPI_TransmitReceive_DMA(&hspi1,  &LSM6DSL_Answer,  &LSM6DSL_Answer, 1);
//	HAL_SPI_Transmit(&hspi1, &WHO_AM_I_Addr_read, 1, 0x100);
//	HAL_SPI_Receive_DMA(&hspi1, &LSM6DSL_Answer, 1);
	//HAL_SPI_TransmitReceive(&hspi1, &WHO_AM_I_Addr_read, &LSM6DSL_Answer, 2, 0x200);
//	LSM6DSL_Answer=0;
	HAL_SPI_Transmit(&hspi1, &WHO_AM_I_Addr_read, 1, 0x100);
//	for (int i=0; i<1000; i++) {;}
	HAL_SPI_Receive(&hspi1, &LSM6DSL_Answer, 1, 0x100);
//	HAL_Delay(1);
	CS_ON;
	if (LSM6DSL_Answer!=WHO_AM_I_Answer)
		Got_Error();
}


void Got_Error(void)
{
	for (int i=0; i<15; i++){
		HAL_GPIO_TogglePin(WARNING_GPIO_Port, WARNING_Pin);
		HAL_Delay(200);
	}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (GPIO_Pin == GPIO_PIN_2) // LSM6DSL int2
		HAL_GPIO_TogglePin(YELLOW_GPIO_Port, YELLOW_Pin);
	if (GPIO_Pin == GPIO_PIN_3) // LSM6DSL int1
			HAL_GPIO_TogglePin(YELLOW_GPIO_Port, YELLOW_Pin);
}

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
//	HAL_GPIO_TogglePin(YELLOW_GPIO_Port, YELLOW_Pin);
//	HAL_SPI_Receive(&hspi1, &LSM6DSL_Answer, 1,0x100);
//	HAL_SPI_Receive_DMA(&hspi1, &LSM6DSL_Answer, 1);
}
void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
{
//	HAL_GPIO_TogglePin(GREEN_GPIO_Port, GREEN_Pin);
//	CS_ON;
//	if (LSM6DSL_Answer!=WHO_AM_I_Answer)
//			Got_Error();
}

void LSM6DSL_FIFO_CTRL(void)
{
	uint8_t CR3[2]={0x12, 0x40};  // it is recommended to set the BDU bit in CTRL3_C (12h) to 1
	uint8_t F_CR1[2]={0x06, 0x1E}; // FIFO threshold 30 words(1word = 2bytes)
	uint8_t F_CR3_4_5[4]={0x08, 0x01, 0x80, 0x0A}; // gyro not in fifo, acc no decimation, fifo only before threshold, FIFO odr=104 continuous mode
	uint8_t INT1_CR[2]={0x0D, 0x08}; // int 1 enable for threshold
	uint8_t CR4_C[2]={0x13, 0x04}; // I2C disable

	CS_OFF;
	HAL_SPI_Transmit(&hspi1, CR3, sizeof(CR3), 0x1000);
	CS_ON;

	CS_OFF;
	HAL_SPI_Transmit(&hspi1, F_CR1, sizeof(F_CR1), 0x1000);
	CS_ON;

	CS_OFF;
	HAL_SPI_Transmit(&hspi1, F_CR3_4_5, sizeof(F_CR3_4_5), 0x1000);
	CS_ON;

	CS_OFF;
	HAL_SPI_Transmit(&hspi1, INT1_CR, sizeof(INT1_CR), 0x1000);
	CS_ON;

	CS_OFF;
	HAL_SPI_Transmit(&hspi1, CR4_C, sizeof(CR4_C), 0x1000);
	CS_ON;
}
//  HAL_DMA_RegisterCallback(&hdma_spi1_tx, HAL_DMA_XFER_CPLT_CB_ID, DMA1_Channel2_Cplt_CallBack);
//  HAL_DMA_RegisterCallback(&hdma_spi1_rx, HAL_DMA_XFER_CPLT_CB_ID, DMA1_Channel2_Cplt_CallBack);
//void DMA1_Channel2_Cplt_CallBack(DMA_HandleTypeDef * hdma)
//{
//	if (hdma==&hdma_spi1_tx) {
//		HAL_GPIO_TogglePin(YELLOW_GPIO_Port, YELLOW_Pin);
//		HAL_SPI_Receive_DMA(&hspi1, &LSM6DSL_Answer, 1);
//	}
//	if (hdma==&hdma_spi1_rx) {
//		HAL_GPIO_TogglePin(GREEN_GPIO_Port, GREEN_Pin);
//		CS_ON;
//	}
//}
//void (*pDMA1_CPLT) (DMA_HandleTypeDef * hdma)=DMA1_Channel2_Cplt_CallBack;

