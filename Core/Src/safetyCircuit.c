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

static bool contactorState = FALSE;
static bool contactorFault = FALSE;

bool errorSet = FALSE;

void contactorsFunction(void)
{
	static uint8_t restartClicks = 6;
	static uint32_t ticksTemp;

	if(!contactorFault && !errorSet)
	{
		if(getSoftwareStop())
		{
			HAL_GPIO_WritePin(K2_EN_GPIO_Port, K2_EN_Pin, FALSE);
			contactorState = FALSE;
			setK2information(FALSE);
			restartClicks = 6;
		}
		else
		{
			if(restartClicks)
			{
				if(HAL_GetTick() - ticksTemp >= CONTACTOR_CLICK_TIME)
				{
					if(getAckK2() == contactorState)
					{
						contactorState = !contactorState;
						HAL_GPIO_WritePin(K2_EN_GPIO_Port, K2_EN_Pin, contactorState);
						ticksTemp = HAL_GetTick();
						restartClicks--;
					}
					else
					{
						contactorFault = TRUE;
					}
				}
			}
			else
			{
				contactorState = checkSafetyOk();
				HAL_GPIO_WritePin(K2_EN_GPIO_Port, K2_EN_Pin, checkSafetyOk());
			}

			setK2information(checkSafetyOk());
		}
	}
	else
	{
		setK2information(FALSE);
		HAL_GPIO_WritePin(K2_EN_GPIO_Port, K2_EN_Pin, FALSE);
	}
}

