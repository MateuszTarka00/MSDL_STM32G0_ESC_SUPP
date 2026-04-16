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
#include "mainCpuCommunication.h"
#include "softwareTimer_ms.h"
#include "systemStartup.h"
#include "teachSpeed.h"

SoftwareTimerHandler checkSpeedTimer;
static SoftwareTimerHandler stepsErrorTimer;
static SoftwareTimerHandler changeSpeedDelay;

bool speedDelay = FALSE;
volatile uint32_t engineRotationTemporary = 0;
bool fastSpeedSet = FALSE;
bool slowSpeedSet = FALSE;

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

void speedDelayTimerCallback(void)
{
	speedDelay = FALSE;
}

void initSpeedTimer(void)
{
	deInitSoftwareTimer(&checkSpeedTimer);
	initSoftwareTimer(&checkSpeedTimer, SPEED_CHECK_MS, saveMeasuredRotationsValueTimerCallback, TRUE, &rotationsPerMinuteReal);
	initSoftwareTimer(&changeSpeedDelay, SPEED_CHANGE_DELAY_MS, speedDelayTimerCallback, TRUE, 0);
	startSoftwareTimer(&checkSpeedTimer);
}

void incrementRotationsNumber(uint16_t GPIO_Pin)
{
	engineRotationTemporary++;
}

static void stepsErrorTimerCallback(void *Param)
{
	if(programState == INSPECTION_MODE &&  getStandControl())
	{
		setStandOk(FALSE);
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
		if(getFastSpeedState())
		{
			rotationsPerMinute->engine.fastTime = engineRotationTemporary;
		}
		else if(getSlowSpeedState())
		{
			rotationsPerMinute->engine.slowTime = engineRotationTemporary;
		}
	}

	engineRotationTemporary = 0;

	// Enable
	__HAL_GPIO_EXTI_ENABLE_IT(ROTATION_S2_Pin);
	__HAL_GPIO_EXTI_ENABLE_IT(MIS_ST1_Pin);
}

bool checkSetFrequency(void)
{
	bool rotationsState = TRUE;

	if(!speedDelay)
	{
		if(getFastSpeedState())
		{
			if(checkErrorRange(rotationsPerMinuteReal.engine.fastTime,  rotationsPerMinuteGiven.engine.fastTime) || !getRotationControl())
			{

			}
			else
			{
				rotationsState = FALSE;
			}


		}
		else if(getSlowSpeedState())
		{
			if(checkErrorRange(rotationsPerMinuteReal.engine.slowTime,  rotationsPerMinuteGiven.engine.slowTime) || !getRotationControl())
			{

			}
			else
			{
				rotationsState = FALSE;
			}

		}
	}

	return rotationsState;
}

bool checkErrorRange(uint32_t real, uint32_t given)
{
	uint32_t errorRange = given/FREQUENCY_ERROR_RANGE;

	if(errorRange == 0)
	{
		errorRange = 1;
	}

	if((real <= (given + errorRange)) && (real >= (given - errorRange)))
	{
		return TRUE;
	}
	return FALSE;
}

void rotationsLoadParameters(void)
{
	rotationsPerMinuteGiven.engine.fastTime   = flash_parametersToSave.flash_RotationsPerMinuteFast.engine;
	rotationsPerMinuteGiven.step.fastTime     = flash_parametersToSave.flash_RotationsPerMinuteFast.step;

	rotationsPerMinuteGiven.engine.slowTime   = flash_parametersToSave.flash_RotationsPerMinuteSlow.engine;
	rotationsPerMinuteGiven.step.slowTime     = flash_parametersToSave.flash_RotationsPerMinuteSlow.step;
}

void rotationsSaveParameters(void)
{
	flash_parametersToSave.flash_RotationsPerMinuteFast.engine = rotationsPerMinuteGiven.engine.fastTime;
	flash_parametersToSave.flash_RotationsPerMinuteFast.step = rotationsPerMinuteGiven.step.fastTime;

	flash_parametersToSave.flash_RotationsPerMinuteSlow.engine = rotationsPerMinuteGiven.engine.slowTime;
	flash_parametersToSave.flash_RotationsPerMinuteSlow.step = rotationsPerMinuteGiven.step.slowTime;

	flash_parametersSave();
}

void stepsNormalExtiCallback(uint16_t GPIO_Pin)
{
	static bool steps1 = FALSE;
	static bool steps2 = FALSE;

	if(checkTargetFrequencyReached())
	{
		if(GPIO_Pin == MIS_ST2_Pin)
		{
			if(steps1)
			{
				steps1 = FALSE;
				stopSoftwareTimer(&stepsErrorTimer);
			}
			else
			{
				steps2 = TRUE;
				startSoftwareTimer(&stepsErrorTimer);
			}
		}
		else if(GPIO_Pin == MIS_ST1_Pin)
		{
			if(steps2)
			{
				steps2 = FALSE;
				stopSoftwareTimer(&stepsErrorTimer);
			}
			else
			{
				steps1 = TRUE;
				startSoftwareTimer(&stepsErrorTimer);
			}
		}
	}
}

void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin)
{
	switch(GPIO_Pin)
	{
	case ROTATION_S2_Pin:
		incrementRotationsNumber(GPIO_Pin);;
		break;

	case MIS_ST1_Pin:
		if(programState == TEACHING_STATE)
		{
			stepsTeachExtiCallback(GPIO_Pin);
		}
		else
		{
			stepsNormalExtiCallback(GPIO_Pin);
		}
		break;
	}
}

void setStandControlTimer(void)
{
	static bool fastSpeed = FALSE;
	static bool slowSpeed = FALSE;

	if(getFastSpeedState() && fastSpeed)
	{
		deInitSoftwareTimer(&stepsErrorTimer);
		initSoftwareTimer(&stepsErrorTimer, rotationsPerMinuteGiven.engine.fastTime, stepsErrorTimerCallback, FALSE, 0);
		speedDelay = TRUE;
		startSoftwareTimer(&changeSpeedDelay);
	}
	else if(getSlowSpeedState())
	{
		deInitSoftwareTimer(&stepsErrorTimer);
		initSoftwareTimer(&stepsErrorTimer, rotationsPerMinuteGiven.engine.slowTime, stepsErrorTimerCallback, FALSE, 0);
		speedDelay = TRUE;
		startSoftwareTimer(&changeSpeedDelay);
	}
}


