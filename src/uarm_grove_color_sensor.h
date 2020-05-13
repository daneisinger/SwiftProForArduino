//
// Created by cap on 5/13/20.
//

#ifndef _UARM_GROVE_COLOR_SENSOR_H
#define _UARM_GROVE_COLOR_SENSOR_H

#include "uarm_grove_common.h"
#include "TCS34725.h"

GroveModule_t* newGroveColorSensor();

bool groveColorSensorInit(GroveModule_t* self, uint8_t portNum);
void groveColorSensorReport(GroveModule_t* self);
bool groveColorSensorControl(GroveModule_t* self, GroveData_t data);
void groveColorSensorTick(GroveModule_t* self);

#endif //_UARM_GROVE_COLOR_SENSOR_H
