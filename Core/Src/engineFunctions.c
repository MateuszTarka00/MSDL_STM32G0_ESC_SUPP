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
#include "safetyCircuit.h"

SoftwareTimerHandler checkSpeedTimer;
static SoftwareTimerHandler stepsErrorTimer;
static SoftwareTimerHandler speedErrorDelay;

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

static bool checkSetFrequencyTimerCallback(void)
{
	if(checkTargetFrequencyReached())
	{
		if(getFastSpeedState())
		{
			if(checkErrorRange(rotationsPerMinuteReal.engine.fastTime,  rotationsPerMinuteGiven.engine.fastTime))
			{

			}
			else
			{
				errorSet = TRUE;
				setRotationOk(FALSE);
			}


		}
		else if(getSlowSpeedState())
		{
			if(checkErrorRange(rotationsPerMinuteReal.engine.slowTime,  rotationsPerMinuteGiven.engine.slowTime))
			{

			}
			else
			{
				errorSet = TRUE;
				setRotationOk(FALSE);
			}

		}
	}
}

void initSpeedTimer(void)
{
	deInitSoftwareTimer(&checkSpeedTimer);
	initSoftwareTimer(&checkSpeedTimer, SPEED_CHECK_MS, saveMeasuredRotationsValueTimerCallback, TRUE, &rotationsPerMinuteReal);

	deInitSoftwareTimer(&speedErrorDelay);
	initSoftwareTimer(&speedErrorDelay, SPEED_DELAY_MS, checkSetFrequencyTimerCallback, FALSE, 0);

	startSoftwareTimer(&checkSpeedTimer);
}

void incrementRotationsNumber(uint16_t GPIO_Pin)
{
	engineRotationTemporary++;
}

static void stepsErrorTimerCallback(void *Param)
{
	if(programState != INSPECTION_MODE && getStandControl())
	{
		setStandOk(FALSE);
		errorSet = TRUE;
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

	if(checkTargetFrequencyReached())
	{
		if(getFastSpeedState())
		{
			if(checkErrorRange(rotationsPerMinuteReal.engine.fastTime,  rotationsPerMinuteGiven.engine.fastTime))
			{
				stopSoftwareTimer(&speedErrorDelay);
			}
			else
			{
				startSoftwareTimer(&speedErrorDelay);
			}


		}
		else if(getSlowSpeedState())
		{
			if(checkErrorRange(rotationsPerMinuteReal.engine.slowTime,  rotationsPerMinuteGiven.engine.slowTime))
			{
				stopSoftwareTimer(&speedErrorDelay);
			}
			else
			{
				startSoftwareTimer(&speedErrorDelay);
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
	rotationsPerMinuteGiven.step.fastTime     = flash_parametersToSave.flash_RotationsPerMinuteFast.step + (flash_parametersToSave.flash_RotationsPerMinuteFast.step/10);

	rotationsPerMinuteGiven.engine.slowTime   = flash_parametersToSave.flash_RotationsPerMinuteSlow.engine;
	rotationsPerMinuteGiven.step.slowTime     = flash_parametersToSave.flash_RotationsPerMinuteSlow.step + (flash_parametersToSave.flash_RotationsPerMinuteSlow.step/10);
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

	if(checkTargetFrequencyReached() && getTeachFast())
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
		incrementRotationsNumber(GPIO_Pin);
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

	if(checkTargetFrequencyReached())
	{
		if(getFastSpeedState() && !fastSpeed)
		{
			fastSpeed = TRUE;
			slowSpeed = FALSE;
			deInitSoftwareTimer(&stepsErrorTimer);
			initSoftwareTimer(&stepsErrorTimer, rotationsPerMinuteGiven.engine.fastTime, stepsErrorTimerCallback, FALSE, 0);
		}
		else if(getSlowSpeedState() && !slowSpeed)
		{
			fastSpeed = FALSE;
			slowSpeed = TRUE;
			deInitSoftwareTimer(&stepsErrorTimer);
			initSoftwareTimer(&stepsErrorTimer, rotationsPerMinuteGiven.engine.slowTime, stepsErrorTimerCallback, FALSE, 0);
		}
	}
	else
	{
		fastSpeed = FALSE;
		slowSpeed = FALSE;
		deInitSoftwareTimer(&stepsErrorTimer);
	}
}


