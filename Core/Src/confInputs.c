/*
 * confInputs.c
 *
 *  Created on: 24 lut 2026
 *      Author: mateo
 */

#include "confInputs.h"

bool getStandControl(void)
{
	return HAL_GPIO_ReadPin(STAND_CONTROL_GPIO_Port, STAND_CONTROL_Pin);
}

bool getRotationControl(void)
{
	return HAL_GPIO_ReadPin(ROTATION_CONTROL_GPIO_Port, ROTATION_CONTROL_Pin);
}
