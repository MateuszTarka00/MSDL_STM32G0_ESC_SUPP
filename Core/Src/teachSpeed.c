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

SoftwareTimerHandler teachTimer;
TeachStateMachine teachState = PREPARATION;

void initTeachProcedure(void)
{
	deInitSoftwareTimer(&teachTimer);
	initSoftwareTimer(&teachTimer, TEACH_TIME_MS, saveMeasuredRotationsValueTimerCallback, FALSE, &rotationsPerMinuteGiven);
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
				startSoftwareTimer(&teachTimer);
				teachState = SLOW_SPEED_TIME;
			}
			break;

		case SLOW_SPEED_TIME:
			if(teachTimer.start == FALSE)
			{
				teachState = SLOW_SPEED_CONFIRMATION;
				setSpeedReady(FALSE);
			}
			break;

		case SLOW_SPEED_CONFIRMATION:
			if(getTeachFast())
			{
				teachState = WAIT_FOR_SLOW_SPEED;
			}
			else if(getTeachSlow())
			{
				teachState = WAIT_FOR_FAST_SPEED;
			}
			break;

		case WAIT_FOR_FAST_SPEED:
			if(checkTargetFrequencyReached())
			{
				setSpeedReady(TRUE);
				startSoftwareTimer(&teachTimer);
				teachState = FAST_SPEED_TIME;
			}
			break;

		case FAST_SPEED_TIME:
			if(teachTimer.start == FALSE)
			{
				teachState = FAST_SPEED_CONFIRMATION;
				setSpeedReady(FALSE);
			}
			break;

		case FAST_SPEED_CONFIRMATION:
			if(getTeachFast())
			{
				teachState = WAIT_FOR_SLOW_SPEED;
			}
			else if(getEndTeaching())
			{
				teachState = WAIT_FOR_SLOW_SPEED;
			}
			break;

		case END:
			programState = NORMAL_MODE;
			deInitSoftwareTimer(&teachTimer);
			initSpeedTimer();
			rotationsSaveParameters();
			break;
	}
}



