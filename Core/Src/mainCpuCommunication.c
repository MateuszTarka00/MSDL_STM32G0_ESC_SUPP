/*
 * mainCpuCommunication.c
 *
 *  Created on: 25 lut 2026
 *      Author: mateo
 */


#include "mainCpuCommunication.h"

void setSpeedReady(bool onOff)
{
	return HAL_GPIO_WritePin(SPEED_READY_GPIO_Port, SPEED_READY_Pin, onOff);
}

void setSafetyOk(bool onOff)
{
	return HAL_GPIO_WritePin(SAFETY_OK_GPIO_Port, SAFETY_OK_Pin, onOff);
}

void setStandOk(bool onOff)
{
	return HAL_GPIO_WritePin(STAND_OK_GPIO_Port, STAND_OK_Pin, onOff);
}

void setRotationOk(bool onOff)
{
	return HAL_GPIO_WritePin(ROTATION_OK_GPIO_Port, ROTATION_OK_Pin, onOff);
}

bool getTeachFast(void)
{
	return HAL_GPIO_ReadPin(TEACH_FAST_GPIO_Port, TEACH_FAST_Pin);
}

bool getTeachSlow(void)
{
	return HAL_GPIO_ReadPin(TEACH_SLOW_GPIO_Port, TEACH_SLOW_Pin);
}

bool getFactoryReset(void)
{
	return HAL_GPIO_ReadPin(FACTORY_RESET_GPIO_Port, FACTORY_RESET_Pin);
}

bool getTeachInput(void)
{
	return HAL_GPIO_ReadPin(TEACH_INPUT_GPIO_Port, TEACH_INPUT_Pin);
}

bool getFastSpeedState(void)
{
	return HAL_GPIO_ReadPin(FAST_SPEED_GPIO_Port, FAST_SPEED_Pin);
}

bool getSlowSpeedState(void)
{
	return HAL_GPIO_ReadPin(SLOW_SPEED_GPIO_Port, SLOW_SPEED_Pin);
}

bool getEndTeaching(void)
{
	return HAL_GPIO_ReadPin(END_TEACHING_GPIO_Port, END_TEACHING_Pin);
}
