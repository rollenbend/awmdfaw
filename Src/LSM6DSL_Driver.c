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
	uint8_t accgyrodata[12]={0};
	uint8_t Addrreadaccgyro = 0x22|0x80; // начинаем читать с OUTX_L_G (22h), бит чтения = 1(0x80)

	HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, DISABLE);
	HAL_SPI_Transmit(&hspi1, &Addrreadaccgyro, 1, 0x100);
	HAL_SPI_Receive(&hspi1, accgyrodata, 12, 0x100);
	HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, ENABLE);

	HSOW2_CAN_Transmit(accgyrodata);

	if (++LED_Turn_Count%20==0) HAL_GPIO_TogglePin(BLUE_IMU_GPIO_Port, BLUE_IMU_Pin);
}

void LSM6DSL_Init(void)
{
	uint8_t CR3[2]={0x12, 0x01};  // SW_RESET=1 сброс настроек датчика, чтобы предыдущие действия не мешались
	uint8_t CR1_2[3]={0x10, 0x60, 0x60}; // ставим выдачу в 416Гц для аксела и гиро. чувств 2g и 250dps

	HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, DISABLE);
	HAL_SPI_Transmit(&hspi1, CR3, sizeof(CR3), 0x1000);
	HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, ENABLE);
	HAL_Delay(1); // ждем пока сбросятся настройки

	HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, DISABLE);
	HAL_SPI_Transmit(&hspi1, CR1_2, sizeof(CR1_2), 0x1000);
	HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, ENABLE);
}
uint32_t Is_this_LSM6DSL(void)
{
	uint8_t LSM6DSL_Answer=0;
	uint8_t WHO_AM_I_Addr_read=0x8F;
	HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, DISABLE);
	//HAL_SPI_TransmitReceive(&hspi1, &WHO_AM_I_Addr_read, &LSM6DSL_Answer, 2, 0x200);
//	HAL_SPI_TransmitReceive_DMA(&hspi1, &WHO_AM_I_Addr_read, &LSM6DSL_Answer, 1);
//	HAL_SPI_Transmit_DMA(&hspi1, &WHO_AM_I_Addr_read, 1);
//	HAL_Delay(1);
//	HAL_SPI_TransmitReceive_DMA(&hspi1,  &LSM6DSL_Answer,  &LSM6DSL_Answer, 1);
//	HAL_SPI_Receive_DMA(&hspi1, &LSM6DSL_Answer, 1);
	HAL_SPI_Transmit(&hspi1, &WHO_AM_I_Addr_read, 1, 0x100);
	HAL_SPI_Receive(&hspi1, &LSM6DSL_Answer, 1, 0x100);
	HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, ENABLE);
	if (LSM6DSL_Answer==WHO_AM_I_Answer) return 1;
	return 0;
}

void Got_Error(void)
{
	for (int i=0; i<15; i++){
		HAL_GPIO_TogglePin(WARNING_GPIO_Port, WARNING_Pin);
		HAL_Delay(200);
	}
}


void DMA1_Channel2_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel2_IRQn 0 */

  /* USER CODE END DMA1_Channel2_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_spi1_rx);
  /* USER CODE BEGIN DMA1_Channel2_IRQn 1 */

  /* USER CODE END DMA1_Channel2_IRQn 1 */
}

/**
* @brief This function handles DMA1 channel3 global interrupt.
*/
void DMA1_Channel3_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel3_IRQn 0 */

  /* USER CODE END DMA1_Channel3_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_spi1_tx);
  /* USER CODE BEGIN DMA1_Channel3_IRQn 1 */

  /* USER CODE END DMA1_Channel3_IRQn 1 */
}
