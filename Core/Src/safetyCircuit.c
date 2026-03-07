/*
 * safetyCircuit.c
 *
 *  Created on: 7 mar 2026
 *      Author: mateo
 */


#include "safetyCircuit.h"
#include "sensors.h"
#include "mainCpuCommunication.h"

#define CONTACTOR_CLICK_TIME 200

void contactorsFunction(void)
{
	static uint8_t restartClicks = 6;
	static uint32_t ticksTemp;

	if(getSoftwareStop())
	{
		HAL_GPIO_WritePin(K2_EN_GPIO_Port, K2_EN_Pin, FALSE);
		restartClicks = 6;
	}
	else
	{
		if(restartClicks)
		{
			if(HAL_GetTick() - ticksTemp >= CONTACTOR_CLICK_TIME)
			{
				HAL_GPIO_TogglePin(K2_EN_GPIO_Port, K2_EN_Pin);
				ticksTemp = HAL_GetTick();
				restartClicks--;
			}
		}
		else
		{
			HAL_GPIO_WritePin(K2_EN_GPIO_Port, K2_EN_Pin, checkSafetyOk());
		}
	}
}

