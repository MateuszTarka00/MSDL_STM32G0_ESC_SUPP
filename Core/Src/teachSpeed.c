/*
 * teachSpeed.c
 *
 *  Created on: 25 lut 2026
 *      Author: mateo
 */


#include "teachSpeed.h"
#include "engineFunctions.h"
#include "sensors.h"
#include "mainCpuCommunication.h"
#include "systemStartup.h"
#include "flash.h"

SoftwareTimerHandler teachTimer;
TeachStateMachine teachState = PREPARATION;

static uint32_t gapsBetweenSteps = 0;
static uint32_t stepsCounted = 0;

void initTeachProcedure(void)
{
	deInitSoftwareTimer(&teachTimer);
	initSoftwareTimer(&teachTimer, TEACH_TIME_MS, saveMeasuredRotationsValueTimerCallback, FALSE, &rotationsPerMinuteGiven);
}

void calculateTeachedSpeeds(bool slowFast)
{
	if(slowFast)
	{
		rotationsPerMinuteGiven.engine.fastTime = rotationsPerMinuteGiven.engine.fastTime/10;

		if(stepsCounted)
		{
			rotationsPerMinuteGiven.step.fastTime = gapsBetweenSteps/stepsCounted;
		}
	}
	else
	{
		rotationsPerMinuteGiven.engine.slowTime = rotationsPerMinuteGiven.engine.slowTime/10;

		if(stepsCounted)
		{
			rotationsPerMinuteGiven.step.slowTime = gapsBetweenSteps/stepsCounted;
		}
	}

	gapsBetweenSteps = 0;
	stepsCounted = 0;

}

void teachStateMachineHandler(void)
{
	switch(teachState)
	{
		case PREPARATION:
			if(getTeachSlow())
			{
				teachState = WAIT_FOR_SLOW_SPEED;
			}
			break;

		case WAIT_FOR_SLOW_SPEED:
			if(checkTargetFrequencyReached())
			{
				setSpeedReady(TRUE);
				engineRotationTemporary = 0;
				startSoftwareTimer(&teachTimer);
				teachState = SLOW_SPEED_TIME;
			}
			break;

		case SLOW_SPEED_TIME:
			if(teachTimer.start == FALSE)
			{
				teachState = SLOW_SPEED_CONFIRMATION;
				calculateTeachedSpeeds(FALSE);
				setSpeedReady(FALSE);
			}
			break;

		case SLOW_SPEED_CONFIRMATION:
			if(getTeachFast())
			{
				teachState = WAIT_FOR_FAST_SPEED;
			}
			else if(getTeachSlow())
			{
				teachState = WAIT_FOR_SLOW_SPEED;
			}
			break;

		case WAIT_FOR_FAST_SPEED:
			if(checkTargetFrequencyReached())
			{
				setSpeedReady(TRUE);
				engineRotationTemporary = 0;
				startSoftwareTimer(&teachTimer);
				teachState = FAST_SPEED_TIME;
			}
			break;

		case FAST_SPEED_TIME:
			if(teachTimer.start == FALSE)
			{
				teachState = FAST_SPEED_CONFIRMATION;
				calculateTeachedSpeeds(TRUE);
				setSpeedReady(FALSE);
			}
			break;

		case FAST_SPEED_CONFIRMATION:
			if(getTeachFast())
			{
				teachState = WAIT_FOR_FAST_SPEED;
			}
			else if(getEndTeaching())
			{
				teachState = END;
			}
			break;

		case END:
			deInitSoftwareTimer(&teachTimer);
			teachState = WAIT_FOR_RESET;
			rotationsSaveParameters();
//			flash_parametersSave();
			break;

		case WAIT_FOR_RESET:
			break;
	}
}

void stepsTeachExtiCallback(uint16_t GPIO_Pin)
{
	static bool steps1 = FALSE;
	static bool steps2 = FALSE;
	static uint32_t gapBetweenStepsTemp = 0;

	if(teachState == SLOW_SPEED_TIME || teachState == FAST_SPEED_TIME)
	{

		if(GPIO_Pin == MIS_ST2_Pin)
		{
			if(steps1)
			{
				steps1 = FALSE;
				gapBetweenStepsTemp = HAL_GetTick() - gapBetweenStepsTemp;
				gapsBetweenSteps += gapBetweenStepsTemp;
				gapBetweenStepsTemp = 0;
			}
			else
			{
				gapBetweenStepsTemp = HAL_GetTick();
				stepsCounted++;
				steps2 = TRUE;
			}
		}
		else if(GPIO_Pin == MIS_ST1_Pin)
		{
			if(steps2)
			{
				steps2 = FALSE;
				gapBetweenStepsTemp = HAL_GetTick() - gapBetweenStepsTemp;
				gapsBetweenSteps += gapBetweenStepsTemp;
				gapBetweenStepsTemp = 0;
			}
			else
			{
				gapBetweenStepsTemp = HAL_GetTick();
				stepsCounted++;
				steps1 = TRUE;
			}
		}
	}
}



