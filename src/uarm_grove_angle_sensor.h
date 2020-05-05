//
// Created by cap on 5/1/20.
//

#ifndef _UARM_GROVE_ANGLESENSOR_H
#define _UARM_GROVE_ANGLESENSOR_H

#include "uarm_grove_common.h"
#include "uarm_peripheral.h"

GroveModule_t* newGroveAngleSensor();

bool groveAngleSensorInit(GroveModule_t* self, uint8_t portNum);
void groveAngleSensorReport(GroveModule_t* self);
bool groveAngleSensorControl(GroveModule_t* self, GroveData_t data);
void groveAngleSensorTick(GroveModule_t* self);

#endif //_UARM_GROVE_ANGLESENSOR_H
