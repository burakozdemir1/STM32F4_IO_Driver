/*
 * gpio_driver.c
 *
 *  Created on: Feb 6, 2026
 *      Author: Burak Ozdemir
 */

#include "gpio_driver.h"

void IO_init(IO_Info_t	*ioInfo)
{
	/*input init*/
	ioInfo->inputsInfo.userButton.GPIOx 		= GPIOC;
	ioInfo->inputsInfo.userButton.GPIO_Pin		= GPIO_PIN_13;
	ioInfo->inputsInfo.userButton.numOfInput	= 0; // pin no
	ioInfo->inputsInfo.userButton.currentState	= GPIO_PIN_SET;
	ioInfo->inputsInfo.userButton.lastState		= GPIO_PIN_SET;
	ioInfo->inputsInfo.userButton.inputStatus	= Input_Status_Low;
	ioInfo->inputsInfo.userButton.currentTime	= 0;
	ioInfo->inputsInfo.userButton.debounceTime	= DEBOUNCE_TIME;

	/*output init*/
	ioInfo->outputsInfo.userLed.GPIOx			= GPIOA;
	ioInfo->outputsInfo.userLed.GPIO_Pin		= GPIO_PIN_5;
	ioInfo->outputsInfo.userLed.pinState		= GPIO_PIN_RESET;


}

void IO_statusControl(IO_Info_t	*ioInfo)
{
	IO_ouptputControl(&ioInfo->outputsInfo.userLed);
	IO_inputControlDebounce(&ioInfo->inputsInfo.userButton);

}

/*
 * BUTTON OPERATION NOTE:
 * Hardware: Nucleo board button is configured as ACTIVE-LOW
 * - Button RELEASED: GPIO reads HIGH (3.3V, logic 1) due to internal pull-up
 * - Button PRESSED:  GPIO reads LOW (0V, logic 0) as button shorts to GND
 *
 * Software Implementation:
 * We invert the logic for intuitive programming:
 * - Physical LOW (0V)  → Logical HIGH (1) = Button pressed
 * - Physical HIGH (3.3V) → Logical LOW (0)  = Button released
 *
 * Debounce handling ensures stable state detection after 50ms of stable signal.
 */
void IO_inputControlDebounce(Input_State_t *inputState)
{
    GPIO_PinState rawReading = HAL_GPIO_ReadPin(inputState->GPIOx, inputState->GPIO_Pin);

    inputState->currentState = (rawReading == GPIO_PIN_RESET) ? GPIO_PIN_SET : GPIO_PIN_RESET;

    if(inputState->currentState != inputState->lastState)
    {
        inputState->lastState = inputState->currentState;
        inputState->currentTime = HAL_GetTick();
    }


    if((HAL_GetTick() - inputState->currentTime) >= inputState->debounceTime)
    {
        if(inputState->currentState == GPIO_PIN_SET)
        {
            if(inputState->inputStatus == Input_Status_Low)
                inputState->inputStatus = Input_Status_High;

        }
        else
        {
            if(inputState->inputStatus == Input_Status_High)
                inputState->inputStatus = Input_Status_Low;

        }
    }

}
void IO_ouptputControl(Output_State_t *outputState)
{
	HAL_GPIO_WritePin(outputState->GPIOx, outputState->GPIO_Pin, outputState->pinState);
}










