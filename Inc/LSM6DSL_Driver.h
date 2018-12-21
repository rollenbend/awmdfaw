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

typedef struct {
	int16_t gyrodata;  // Z axis gyro
	int16_t accdata;   // for X axis acc
} AccGyroSampleTypeDef;

#define CS_OFF             HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, DISABLE)
#define CS_ON              HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, ENABLE)
#define ReadCommand        (uint8_t)0x80
#define WriteCommand       (uint8_t)0x7F
#define WHO_AM_I_Addr      (uint8_t)0x0F
#define WHO_AM_I_Answer    (uint8_t)0x6A
#define CTRL1_XL		   (uint8_t)0x10
#define CTRL2_G			   (uint8_t)0x11

#define GYRO_OUT_X_L       (uint8_t)0x22
#define GYRO_OUT_X_H       (uint8_t)0x23
#define GYRO_OUT_Y_L       (uint8_t)0x24
#define GYRO_OUT_Y_H       (uint8_t)0x25
#define GYRO_OUT_Z_L       (uint8_t)0x26
#define GYRO_OUT_Z_H       (uint8_t)0x27
#define ACC_OUT_X_L        (uint8_t)0x28
#define ACC_OUT_X_H        (uint8_t)0x29
#define ACC_OUT_Y_L        (uint8_t)0x2a
#define ACC_OUT_Y_H        (uint8_t)0x2b
#define ACC_OUT_Z_L        (uint8_t)0x2c
#define ACC_OUT_Z_H        (uint8_t)0x2d

#define ACC_ODR_OFF        ((uint8_t)0x00) /* output turn off */
#define ACC_ODR_12_5       ((uint8_t)0x10) /* out data rate 12,5Hz */
#define ACC_ODR_26         (uint8_t)0x20
#define ACC_ODR_52         (uint8_t)0x30
#define ACC_ODR_104        (uint8_t)0x40
#define ACC_ODR_208        (uint8_t)0x50
#define ACC_ODR_416        (uint8_t)0x60
#define ACC_ODR_833        (uint8_t)0x70
#define ACC_ODR_1k66       (uint8_t)0x80
#define ACC_ODR_3k33       (uint8_t)0x90
#define ACC_ODR_6k66       (uint8_t)0xa0 /* out data rate 6,66kHz */

#define ACC_SCALE_2G       (uint8_t)0x00 /* FULL SCALE OF ACCELEROMETER IS +-2G */
#define ACC_SCALE_4G       (uint8_t)0x08
#define ACC_SCALE_8G       (uint8_t)0x0C
#define ACC_SCALE_16G      (uint8_t)0x04 /* FULL SCALE OF ACCELEROMETER IS +-16G */

#define GYRO_ODR_OFF        (uint8_t)0x00 /* output turn off */
#define GYRO_ODR_12_5       (uint8_t)0x10 /* out data rate 12,5Hz */
#define GYRO_ODR_26         (uint8_t)0x20
#define GYRO_ODR_52         (uint8_t)0x30
#define GYRO_ODR_104        (uint8_t)0x40
#define GYRO_ODR_208        (uint8_t)0x50
#define GYRO_ODR_416        (uint8_t)0x60
#define GYRO_ODR_833        (uint8_t)0x70
#define GYRO_ODR_1k66      	(uint8_t)0x80
#define GYRO_ODR_3k33    	(uint8_t)0x90
#define GYRO_ODR_6k66      	(uint8_t)0xa0 // out data rate 6,66kHz

#define GYRO_SCALE_245dps   	   (uint8_t)0x00 // FULL SCALE OF GYROSCOPE IS 245dps
#define GYRO_SCALE_500dps   	   (uint8_t)0x04
#define GYRO_SCALE_1000dps  	   (uint8_t)0x08
#define GYRO_SCALE_2000dps 		   (uint8_t)0x0C // FULL SCALE OF GYROSCOPE IS 2000dps

#define LSM_Acc_Sensitivity_2g     (float)     1.0f            /*!< accelerometer sensitivity with 2 g full scale [LSB/mg] */
#define LSM_Acc_Sensitivity_4g     (float)     0.5f            /*!< accelerometer sensitivity with 4 g full scale [LSB/mg] */
#define LSM_Acc_Sensitivity_8g     (float)     0.25f           /*!< accelerometer sensitivity with 8 g full scale [LSB/mg] */
#define LSM_Acc_Sensitivity_16g    (float)     0.0834f         /*!< accelerometer sensitivity with 12 g full scale [LSB/mg] */
#define LSM_Acc_Divider            (float)     16.0f             /* Because 16-bit value to G-scale*/

void LSM6DSL_Read_data(void);
void Is_this_LSM6DSL(void);
void DMA1_Channel2_IRQHandler(void);
void DMA1_Channel3_IRQHandler(void);
void LSM6DSL_Init(void);
void Got_Error(void);
void LSM6DSL_Set_ODR(uint8_t Acc_ODR, uint8_t Gyro_ODR);
void LSM6DSL_Software_Reset(void);
void LSM6DSL_FIFO_CTRL(void);
void DMA1_Channel2_Cplt_CallBack(DMA_HandleTypeDef * _hdma);
#endif /* LSM6DSL_DRIVER_H_ */
