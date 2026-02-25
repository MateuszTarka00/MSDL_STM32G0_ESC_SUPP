/*
 * mainCpuCommunication.h
 *
 *  Created on: 25 lut 2026
 *      Author: mateo
 */

#ifndef INC_MAINCPUCOMMUNICATION_H_
#define INC_MAINCPUCOMMUNICATION_H_

#include "main.h"

void setSpeedReady(bool onOff);
void setSafetyOk(bool onOff);
void setStandOk(bool onOff);
void setRotationOk(bool onOff);
bool getTeachFast(void);
bool getTeachSlow(void);
bool getFactoryReset(void);
bool getTeachInput(void);

#endif /* INC_MAINCPUCOMMUNICATION_H_ */
