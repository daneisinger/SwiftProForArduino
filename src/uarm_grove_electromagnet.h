//
// Created by cap on 4/28/20.
//

#ifndef _UARM_GROVE_ELECTROMAGNET_H
#define _UARM_GROVE_ELECTROMAGNET_H

#include "uarm_grove_common.h"

GroveModule_t* newGroveElectromagnet();

bool groveElectromagnetInit(GroveModule_t* self, uint8_t portNum);
void groveElectromagnetReport(GroveModule_t* self);
bool groveElectromagnetControl(GroveModule_t* self, GroveData_t data);
void groveElectromagnetTick(GroveModule_t* self);

#endif //_UARM_GROVE_ELECTROMAGNET_H
