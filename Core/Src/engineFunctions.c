/*
 * engineFunctions.c
 *
 *  Created on: 29 sty 2026
 *      Author: mateo
 */


#include "engineFunctions.h"
#include "confInputs.h"
#include "sensors.h"
#include "flash.h"

volatile uint32_t engineRotationTemporary = 0;
volatile uint32_t stepRotationTemporary = 0;

RotationsPerMinute rotationsPerMinuteReal =
{
		{0, 0},
		{0, 0}
};

RotationsPerMinute rotationsPerMinuteGiven =
{
		{0, 0},
		{0, 0}
};

void incrementRotationsNumber(uint16_t GPIO_Pin)
{
	switch(GPIO_Pin)
	{
	case ROTATION_S2_Pin:
		engineRotationTemporary++;
		break;

	case MIS_ST1_Pin:
		stepRotationTemporary++;
		break;
	}
}

void saveMeasuredRotationsValueTimerCallback(RotationsPerMinute *rotationsPerMinute)
{
	// Disable
	__HAL_GPIO_EXTI_DISABLE_IT(ROTATION_S2_Pin);
	__HAL_GPIO_EXTI_DISABLE_IT(MIS_ST1_Pin);

	// Clear pending
	EXTI->RPR1 = ROTATION_S2_Pin | MIS_ST1_Pin;
	EXTI->FPR1 = ROTATION_S2_Pin | MIS_ST1_Pin;

	if(checkTargetFrequencyReached())
	{
		if(highSpeedSet)
		{
			rotationsPerMinute->engine.fastTime = engineRotationTemporary;
			rotationsPerMinute->step.fastTime = stepRotationTemporary;
		}
		else
		{
			rotationsPerMinute->engine.slowTime = engineRotationTemporary;
			rotationsPerMinute->step.slowTime = stepRotationTemporary;
		}
	}

	engineRotationTemporary = 0;
	handrailRotationTemporary = 0;
	stepRotationTemporary = 0;

	// Enable
	__HAL_GPIO_EXTI_ENABLE_IT(ROTATION_S2_Pin);
	__HAL_GPIO_EXTI_ENABLE_IT(MIS_ST1_Pin);
}

bool checkSetFrequency(void)
{
	if(highSpeedSet)
	{
		if(	(checkErrorRange(rotationsPerMinuteReal.engine.fastTime,  rotationsPerMinuteGiven.engine.fastTime) || getRotationControl()) &&
			(checkErrorRange(rotationsPerMinuteReal.step.fastTime, rotationsPerMinuteGiven.step.fastTime) || getStandControl()))
		{
			return TRUE;
		}
	}
	else
	{
		if(	checkErrorRange(rotationsPerMinuteReal.engine.slowTime,  rotationsPerMinuteGiven.engine.slowTime) &&
			checkErrorRange(rotationsPerMinuteReal.step.slowTime, rotationsPerMinuteGiven.step.slowTime))
		{
			return TRUE;
		}
	}

	return FALSE;
}

bool checkErrorRange(uint32_t real, uint32_t given)
{
	if((real <= (given + FREQUENCY_ERROR_RANGE)) || (real >= (given - FREQUENCY_ERROR_RANGE)))
	{
		return TRUE;
	}
	return FALSE;
}

void rotationsLoadParameters(void)
{
	flash_parametersToSave.flash_RotationsPerMinuteFast.engine = rotationsPerMinuteReal.engine.fastTime;
	flash_parametersToSave.flash_RotationsPerMinuteFast.step = rotationsPerMinuteReal.step.fastTime;

	flash_parametersToSave.flash_RotationsPerMinuteSlow.engine = rotationsPerMinuteReal.engine.slowTime;
	flash_parametersToSave.flash_RotationsPerMinuteSlow.step = rotationsPerMinuteReal.step.slowTime;
}

void rotationsSaveParameters(void)
{
	rotationsPerMinuteReal.engine.fastTime   = flash_parametersToSave.flash_RotationsPerMinuteFast.engine;
	rotationsPerMinuteReal.step.fastTime     = flash_parametersToSave.flash_RotationsPerMinuteFast.step;

	rotationsPerMinuteReal.engine.slowTime   = flash_parametersToSave.flash_RotationsPerMinuteSlow.engine;
	rotationsPerMinuteReal.step.slowTime     = flash_parametersToSave.flash_RotationsPerMinuteSlow.step;

	flash_parametersSave();
}

void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin)
{
	incrementRotationsNumber(GPIO_Pin);
}


