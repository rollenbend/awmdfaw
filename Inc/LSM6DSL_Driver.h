/*
 * LSM6DSL_Driver.h
 *
 *  Created on: 14 дек. 2018 г.
 *      Author: user
 */

#ifndef LSM6DSL_DRIVER_H_
#define LSM6DSL_DRIVER_H_

#include "stm32f3xx.h"
#include "main.h"

extern DMA_HandleTypeDef hdma_spi1_rx;
extern DMA_HandleTypeDef hdma_spi1_tx;
extern SPI_HandleTypeDef hspi1;

#define ReadCommand        (uint8_t)0x80
#define WriteCommand       (uint8_t)0x7F
#define WHO_AM_I_Addr      (uint8_t)0x0F
#define WHO_AM_I_Answer    (uint8_t)0x6A
#define CTRL1_XL		   (uint8_t)0x10
#define CTRL2_G			   (uint8_t)0x11

void LSM6DSL_Read_data(void);
uint32_t Is_this_LSM6DSL(void);
void DMA1_Channel2_IRQHandler(void);
void DMA1_Channel3_IRQHandler(void);
void LSM6DSL_Init(void);
void Got_Error(void);



#endif /* LSM6DSL_DRIVER_H_ */
