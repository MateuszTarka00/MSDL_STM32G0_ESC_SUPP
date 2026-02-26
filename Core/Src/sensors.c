/*
 * sensors.c
 *
 *  Created on: 25 lut 2026
 *      Author: mateo
 */
#include "sensors.h"

#define INSPECTION_MODE_Pin GPIO_PIN_1
#define INSPECTION_MODE_GPIO_Port GPIOB

bool checkTargetFrequencyReached(void)
{
	return HAL_GPIO_ReadPin(ROTATION_CONTROL_GPIO_Port, ROTATION_CONTROL_Pin);
}

bool checkInspectionMode(void)
{
	return HAL_GPIO_ReadPin(INSPECTION_MODE_GPIO_Port, INSPECTION_MODE_Pin);
}

bool checkSafetyOk(void)
{
	return HAL_GPIO_ReadPin(SAFETY_END_GPIO_Port, SAFETY_END_Pin);
}
