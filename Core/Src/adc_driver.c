/*
 * adc_driver.c
 *
 *  Created on: Feb 7, 2026
 *      Author: Burak Ozdemir
 */

#include "adc_driver.h"

Dma_Transfer_Status_t globalDmaTransferStatus = DMA_Transfer_Not_Yet_Start;

void ADC_Initialization(ADC_Info_t *adcInfo,ADC_HandleTypeDef *hadc)
{
	adcInfo->hadc = hadc;
	adcInfo->adcErrorStatus = ADC_No_Error;

	for(int i=0; i<NUM_OF_ADC_CHANNEL ;i++)
	{
		adcInfo->adcConvertedData[i] 	= 0;
		adcInfo->adcVoltageData[i] 		= 0;
		adcInfo->adcRawSum[i]			= 0;
	}

	adcInfo->adcSampleCount					= 0;

	adcInfo->dmaTransferStatus = globalDmaTransferStatus;

	if(HAL_ADC_Start_DMA(adcInfo->hadc, (uint32_t *)adcInfo->adcConvertedData, NUM_OF_ADC_CHANNEL) != HAL_OK)
		adcInfo->adcErrorStatus = ADC_Init_Start_Error;

}


// FIXME : When all data is collected 64 times and then averaged, it becomes more stable and less affected by noise.
void ADC_DMA_Conversion(ADC_Info_t *adcInfo)
{
	adcInfo->dmaTransferStatus = globalDmaTransferStatus;

	if(adcInfo->dmaTransferStatus == DMA_Transfer_Completed)
	{
		globalDmaTransferStatus 	= DMA_Transfer_Not_Completed;
		for(int i = 0; i<NUM_OF_ADC_CHANNEL ; i++)
		{
			adcInfo->adcRawSum[i] += adcInfo->adcConvertedData[i];
		}
		adcInfo->adcSampleCount += 1;

		if(adcInfo->adcSampleCount >= ADC_AVARAGE_COUNT)
		{
			adcInfo->adcSampleCount = 0;

			for(int i = 0 ;i<NUM_OF_ADC_CHANNEL ; i++)
			{
				adcInfo->adcAvarageData[i] = adcInfo->adcRawSum[i] / ADC_AVARAGE_COUNT;

//				adcInfo->adcVoltageData[i] = ( __LL_ADC_CALC_DATA_TO_VOLTAGE(VDDA,
//																			 avarageADCrawData,
//																			 DIGITAL_SCALE) / 1000.0); //  / 1000 -> for mV to V
				adcInfo->adcRawSum[i] = 0;
			}

			adcInfo->realVDDA 		= 		__LL_ADC_CALC_VREFANALOG_VOLTAGE(adcInfo->adcAvarageData[ADC_Channeel_Vref],DIGITAL_SCALE);
			adcInfo->temperature	= 		__LL_ADC_CALC_TEMPERATURE(adcInfo->realVDDA,adcInfo->adcAvarageData[ADC_Channeel_Temp],DIGITAL_SCALE);

			for(int i = 0 ;i<NUM_OF_ADC_CHANNEL ; i++)
			{
				adcInfo->adcVoltageData[i] = (__LL_ADC_CALC_DATA_TO_VOLTAGE(adcInfo->realVDDA,adcInfo->adcAvarageData[i],DIGITAL_SCALE) / 1000.0);
			}


			adcInfo->potPercentage = MAP_Voltage_To_Percentage(adcInfo->adcVoltageData[ADC_Channeel_0],
															   	0.0,
																3.3,
																0,
																100);
		}
	}

}

uint8_t  MAP_Voltage_To_Percentage(float voltage, float inMin, float inMax, uint8_t outMin, uint8_t outMax)
{
	uint8_t percentage = 0;

	percentage = (voltage - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;

	return percentage;
}


//when the ADC conversion is complete, interrupt function
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
	globalDmaTransferStatus = DMA_Transfer_Completed;
}

//generates an interrupt when the ADC conversion is in progress
void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef *hadc)
{
	globalDmaTransferStatus = DMA_Transfer_Not_Completed;
}
