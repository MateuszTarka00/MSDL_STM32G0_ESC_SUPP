/*
 * teachSpeed.h
 *
 *  Created on: 25 lut 2026
 *      Author: mateo
 */

#ifndef INC_TEACHSPEED_H_
#define INC_TEACHSPEED_H_

#include "main.h"

#define TEACH_TIME_MS	10000

typedef enum
{
	PREPARATION,
	WAIT_FOR_SLOW_SPEED,
	SLOW_SPEED_TIME,
	SLOW_SPEED_CONFIRMATION,
	WAIT_FOR_FAST_SPEED,
	FAST_SPEED_TIME,
	FAST_SPEED_CONFIRMATION,
	END,
}TeachStateMachine;

void initTeachProcedure(void);
void teachStateMachineHandler(void);

#endif /* INC_TEACHSPEED_H_ */
