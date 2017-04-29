/*
 * SHT1x.c
 *
 *  Created on: Apr 29, 2017
 *      Author: Project-MAR
 */

#include "SHT1x.h"

uint32_t SHT1x_Read(ReadType Type)
{
	uint32_t result = 0;
	uint8_t readOut = 0;
	float rawValue;
	uint8_t i;

	SHT1x_reset();
	SHT1x_SDA_initOutput();

	if(Type == Humidity){
		SHT1x_TX(0);
		SHT1x_TX(0);
		SHT1x_TX(0);

		SHT1x_TX(0);
		SHT1x_TX(0);
		SHT1x_TX(1);
		SHT1x_TX(0);
		SHT1x_TX(1);
	}
	else if(Type == Temperature){
		SHT1x_TX(0);
		SHT1x_TX(0);
		SHT1x_TX(0);

		SHT1x_TX(0);
		SHT1x_TX(0);
		SHT1x_TX(0);
		SHT1x_TX(1);
		SHT1x_TX(1);
	}

	SHT1x_SDA_initInput();
	readOut = SHT1x_RX();    // ack: Should be 0
	HAL_Delay(1);

	/* after ack, SDA will be HIGH until data ready */
	while(HAL_GPIO_ReadPin(SHT11_Port, SHT11_SDA_Pin) == GPIO_PIN_SET);
	HAL_Delay(1);

	for(i = 1; i <=8; i++){
		result <<= 1;
		readOut = SHT1x_RX();
		result |= readOut;
		//asm("NOP");
	}

	/* Every 8 bit, MCU must ack by send 0 to SHT1x */
	SHT1x_SDA_initOutput();
	SHT1x_TX(0);
	HAL_Delay(1);

	SHT1x_SDA_initInput();
	for(i = 1; i <=8; i++){
		result <<= 1;
		readOut = SHT1x_RX();
		result |= readOut;
		//asm("NOP");
	}

	if(Type == Humidity){
		rawValue = (0.0405 * result) - (result * result * 0.0000028) - 4;
		result = (uint32_t)(rawValue * 100);
		//asm("NOP");
	}
	else if(Type == Temperature){
		rawValue = (0.01 * result) - 39.636;
		result = (uint32_t)(rawValue * 100);
		//asm("NOP");
	}

	return result;
}

void SHT1x_TX(uint8_t bit)
{
	/* Load data */
	SCL_L;
	HAL_GPIO_WritePin(SHT11_Port, SHT11_SDA_Pin, bit);
	HAL_Delay(1);

	/* Send */
	SCL_H;
	HAL_Delay(1);
	SCL_L;
	HAL_Delay(1);
}

uint8_t SHT1x_RX(void)
{
	uint readOut;

	SCL_L;
	HAL_Delay(1);
	SCL_H;
	readOut = HAL_GPIO_ReadPin(SHT11_Port, SHT11_SDA_Pin);
	SCL_L;
	HAL_Delay(1);

	return readOut;
}

void SHT1x_StartSequence(void)
{
	SHT1x_SDA_initOutput();

	SDA_H;
	SCL_L;
	HAL_Delay(3);

	SCL_H;
	HAL_Delay(3);

	SDA_L;
	HAL_Delay(3);

	SCL_L;
	HAL_Delay(3);
	SCL_H;
	HAL_Delay(3);

	SDA_H;
	HAL_Delay(3);
	SCL_L;
	HAL_Delay(3);
}

void SHT1x_SCL_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	__HAL_RCC_GPIOB_CLK_ENABLE();

	GPIO_InitStruct.Pin = SHT11_SCL_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(SHT11_Port, &GPIO_InitStruct);
	HAL_GPIO_WritePin(SHT11_Port, SHT11_SCL_Pin, GPIO_PIN_RESET);
}

void SHT1x_SDA_initInput(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	__HAL_RCC_GPIOB_CLK_ENABLE();

	GPIO_InitStruct.Pin = SHT11_SDA_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(SHT11_Port, &GPIO_InitStruct);
}

void SHT1x_SDA_initOutput(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	__HAL_RCC_GPIOB_CLK_ENABLE();

	GPIO_InitStruct.Pin = SHT11_SDA_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(SHT11_Port, &GPIO_InitStruct);
	HAL_GPIO_WritePin(SHT11_Port, SHT11_SDA_Pin, GPIO_PIN_RESET);
}

void SHT1x_reset(void)
{
	uint8_t i;

	SDA_H;
	HAL_Delay(1);
	for(i = 1; i <= 12; i++)
		SHT1x_TX(1);

	HAL_Delay(5);
	SHT1x_StartSequence();
}

