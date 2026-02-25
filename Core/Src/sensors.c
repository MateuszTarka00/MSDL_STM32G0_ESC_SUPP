/*
 * sensors.c
 *
 *  Created on: 25 lut 2026
 *      Author: mateo
 */
#include "sensors.h"

bool checkTargetFrequencyReached(void)
{
	return HAL_GPIO_ReadPin(ROTATION_CONTROL_GPIO_Port, ROTATION_CONTROL_Pin);
}
