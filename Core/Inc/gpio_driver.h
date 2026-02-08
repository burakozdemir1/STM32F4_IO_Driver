/*
 * gpio_driver.h
 *
 *  Created on: Feb 6, 2026
 *      Author: Burak Ozdemir
 */

#ifndef INC_GPIO_DRIVER_H_
#define INC_GPIO_DRIVER_H_

#include "main.h"


#define DEBOUNCE_TIME	100 //100ms

typedef enum
{
	Input_Status_Low,
	Input_Status_High
}Input_Status_t;

typedef struct
{
	GPIO_TypeDef		*GPIOx;
	uint16_t			GPIO_Pin;
	uint16_t			numOfInput;

	GPIO_PinState		currentState;
	GPIO_PinState		lastState;

	Input_Status_t		inputStatus;

	uint32_t			currentTime;
	uint32_t			debounceTime;

}Input_State_t;

typedef struct
{
	Input_State_t		userButton; //kaç tane input kullanacaksan eklersin

}Inputs_Info_t;


typedef struct
{
	GPIO_TypeDef		*GPIOx;
	uint16_t			GPIO_Pin;

	GPIO_PinState		pinState;

}Output_State_t;


typedef struct
{
	Output_State_t		userLed;

}Outputs_Info_t;


typedef struct
{

	Inputs_Info_t		inputsInfo;
	Outputs_Info_t		outputsInfo;
}IO_Info_t;

void IO_init(IO_Info_t	*ioInfo);
void IO_statusControl(IO_Info_t	*ioInfo);
void IO_inputControlDebounce(Input_State_t *inputState);
void IO_ouptputControl(Output_State_t *outputState);


#endif /* INC_GPIO_DRIVER_H_ */
