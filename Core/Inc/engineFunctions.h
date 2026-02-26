/*
 * engineFunctions.h
 *
 *  Created on: 29 sty 2026
 *      Author: mateo
 */

#ifndef INC_ENGINEFUNCTIONS_H_
#define INC_ENGINEFUNCTIONS_H_

#include "main.h"
#include "softwareTimer_ms.h"

#define FREQUENCY_ERROR_RANGE 	10
#define SPEED_CHECK_MS			1000

extern volatile uint32_t engineRotationTemporary;
extern volatile uint32_t stepRotationTemporary;

typedef struct
{
	uint32_t slowTime;
	uint32_t fastTime;
}SpeedTimes;

typedef struct
{
	SpeedTimes engine;
	SpeedTimes step;
}RotationsPerMinute;

extern RotationsPerMinute rotationsPerMinuteReal;
extern RotationsPerMinute rotationsPerMinuteGiven;

void initSpeedTimer(void);
void saveMeasuredRotationsValueTimerCallback(RotationsPerMinute *rotationsPerMinute);
void incrementRotationsNumber(uint16_t GPIO_Pin);
void enableFastSpeed(void);
void enableSlowSpeed(void);
bool checkSetFrequency(void);
bool checkErrorRange(uint32_t real, uint32_t given);
void rotationsLoadParameters(void);
void rotationsSaveParameters(void);

#endif /* INC_ENGINEFUNCTIONS_H_ */
