//
// Created by cap on 5/10/20.
//

#ifndef _UARM_GROVE_TH_H
#define _UARM_GROVE_TH_H

#include "uarm_grove_common.h"
#include "i2cmaster.h"

GroveModule_t* newGroveTHSensor();

bool groveTHSensorInit(GroveModule_t* self, uint8_t portNum);
void groveTHSensorReport(GroveModule_t* self);
bool groveTHSensorControl(GroveModule_t* self, GroveData_t data);
void groveTHSensorTick(GroveModule_t* self);

static float ReadTemperature(void);
static float ReadHumidity(void);
static uint8_t isAvailable();
static uint16_t ReadData(void);
static bool WriteReg(uint8_t u8Reg,uint8_t u8Data);
static uint8_t ReadReg(uint8_t u8Reg);

#endif //_UARM_GROVE_TH_H
