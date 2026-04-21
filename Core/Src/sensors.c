/*
 * sensors.c
 *
 *  Created on: 25 lut 2026
 *      Author: mateo
 */
#include "sensors.h"

bool checkTargetFrequencyReached(void)
{
	return HAL_GPIO_ReadPin(IN_FREQ_GPIO_Port, IN_FREQ_Pin);
}

bool checkInspectionMode(void)
{
	return HAL_GPIO_ReadPin(INSPECTION_MODE_GPIO_Port, INSPECTION_MODE_Pin);
}

bool checkSafetyOk(void)
{
	return HAL_GPIO_ReadPin(SAFETY_END_GPIO_Port, SAFETY_END_Pin);
}

bool getAckK2(void)
{
	return HAL_GPIO_ReadPin(ACK_K2_GPIO_Port, ACK_K2_Pin);
}
