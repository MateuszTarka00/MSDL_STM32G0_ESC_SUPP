/*
 * mainCpuCommunication.c
 *
 *  Created on: 25 lut 2026
 *      Author: mateo
 */


#include "mainCpuCommunication.h"

#define HEARTBIT_MS	500

void setSpeedReady(bool onOff)
{
	HAL_GPIO_WritePin(SPEED_READY_GPIO_Port, SPEED_READY_Pin, onOff);
}

void setSafetyOk(bool onOff)
{
	HAL_GPIO_WritePin(SAFETY_OK_GPIO_Port, SAFETY_OK_Pin, onOff);
}

void setStandOk(bool onOff)
{
	HAL_GPIO_WritePin(STAND_OK_GPIO_Port, STAND_OK_Pin, onOff);
}

void setRotationOk(bool onOff)
{
	HAL_GPIO_WritePin(ROTATION_OK_GPIO_Port, ROTATION_OK_Pin, onOff);
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

bool getSoftwareStop(void)
{
	return HAL_GPIO_ReadPin(SOFTWARE_STOP_GPIO_Port, SOFTWARE_STOP_Pin);
}

void setHeartBit(void)
{
	static bool heartBit = FALSE;
	static uint32_t ticksTemp;


	if(HAL_GetTick() - ticksTemp >= HEARTBIT_MS)
	{
		if(heartBit)
		{
			heartBit = FALSE;
		}
		else
		{
			heartBit = TRUE;
		}

		ticksTemp = HAL_GetTick();
		setSpeedReady(heartBit);
	}
}

void setK2information(bool onOff)
{
	HAL_GPIO_WritePin(K2_INF_GPIO_Port, K2_INF_Pin, onOff);
}
