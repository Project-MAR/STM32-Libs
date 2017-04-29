/*
 * SHT1x.h
 *
 *  Created on: Apr 29, 2017
 *      Author: Project-MAR
 */

#ifndef SHT1X_H_
#define SHT1X_H_

#include "stm32l0xx_hal.h"

/* SHT11 Temperature and Humidity Sensor */
#define SHT11_Port			GPIOB
#define SHT11_SCL_Pin       GPIO_PIN_8
#define SHT11_SDA_Pin       GPIO_PIN_9

#define SCL_H		HAL_GPIO_WritePin(SHT11_Port, SHT11_SCL_Pin, GPIO_PIN_SET)
#define SCL_L		HAL_GPIO_WritePin(SHT11_Port, SHT11_SCL_Pin, GPIO_PIN_RESET)

#define SDA_H		HAL_GPIO_WritePin(SHT11_Port, SHT11_SDA_Pin, GPIO_PIN_SET)
#define SDA_L		HAL_GPIO_WritePin(SHT11_Port, SHT11_SDA_Pin, GPIO_PIN_RESET)

typedef enum
{
	Humidity = 0,
	Temperature
} ReadType;


void SHT1x_SCL_init(void);
void SHT1x_StartSequence(void);
void SHT1x_SDA_initInput(void);
void SHT1x_SDA_initOutput(void);
void SHT1x_reset(void);
void SHT1x_TX(uint8_t bit);
uint8_t SHT1x_RX(void);
uint32_t SHT1x_Read(ReadType Type);



#endif /* SHT1X_H_ */
