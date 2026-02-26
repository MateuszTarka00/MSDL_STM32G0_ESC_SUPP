/*
 * systemStartup.c
 *
 *  Created on: 17 lut 2026
 *      Author: mateo
 */


#include "systemStartup.h"
#include "engineFunctions.h"
#include "flash.h"
#include "teachSpeed.h"
#include "confInputs.h"
#include "sensors.h"

ProgramStates programState = NORMAL_MODE;

void startupFunction(void)
{
	if(flash_loadParameters())
	{
		rotationsLoadParameters();
	}

	if(getTeachInput())
	{
		initTeachProcedure();
		programState = TEACHING_STATE;
	}
	else
	{
		if(checkInspectionMode())
		{
			programState = INSPECTION_MODE;
		}
		else
		{
			programState = NORMAL_MODE;
		}
		initSpeedTimer();
	}
}
