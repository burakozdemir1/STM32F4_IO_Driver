/*
 * adc_driver.h
 *
 *  Created on: Feb 7, 2026
 *      Author: Burak Ozdemir
 */

#ifndef INC_ADC_DRIVER_H_
#define INC_ADC_DRIVER_H_

#include "main.h"

#define	NUM_OF_ADC_CHANNEL		4
#define	VDDA					(uint32_t)3300 //mv(3.3V)

#define	DIGITAL_SCALE_6BITS		0x3F  //	63
#define	DIGITAL_SCALE_8BITS		0xFF  //	255
#define	DIGITAL_SCALE_10BITS	0x3FF //	1023
#define	DIGITAL_SCALE_12BITS	0xFFF //	2^12 = 4095

#define	DIGITAL_SCALE			DIGITAL_SCALE_12BITS

#define	ADC_AVARAGE_COUNT		64

typedef enum
{
	ADC_Channeel_0 = 0,
	ADC_Channeel_1,
	ADC_Channeel_Temp,
	ADC_Channeel_Vref
}ADC_Channel_Name;

typedef enum
{
	DMA_Transfer_Not_Completed = 0,
	DMA_Transfer_Completed,
	DMA_Transfer_Not_Yet_Start
}Dma_Transfer_Status_t;

typedef enum
{
	ADC_No_Error			=0,
	ADC_Init_Start_Error	=1
}ADC_Error_t;

typedef struct
{
	ADC_HandleTypeDef		*hadc;
	uint16_t 				adcConvertedData[NUM_OF_ADC_CHANNEL];
	uint16_t				adcAvarageData[NUM_OF_ADC_CHANNEL];
	float 					adcVoltageData[NUM_OF_ADC_CHANNEL];

	uint32_t				adcRawSum[NUM_OF_ADC_CHANNEL];
	uint8_t					adcSampleCount;

	uint8_t 				potPercentage;
	float 					temperature;
	float					realVDDA;

	Dma_Transfer_Status_t	dmaTransferStatus;
	ADC_Error_t				adcErrorStatus;
}ADC_Info_t;

void ADC_Initialization(ADC_Info_t *adcInfo,ADC_HandleTypeDef *hadc);
void ADC_DMA_Conversion(ADC_Info_t *adcInfo);

uint8_t MAP_Voltage_To_Percentage(float voltage, float inMin, float inMax, uint8_t outMin, uint8_t outMax);

#endif /* INC_ADC_DRIVER_H_ */
