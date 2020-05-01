//
// Created by cap on 4/28/20.
//

#ifndef _UARM_GROVE_MANAGE_H
#define _UARM_GROVE_MANAGE_H

#include "uarm_grove_common.h"
#include "uarm_grove_electromagnet.h"

bool initGroveModule(uint8_t portNum, GroveType_t type);
void deinitGroveModule(uint8_t portNum);
bool controlGroveModule(uint8_t portNum, GroveData_t data);

#endif //_UARM_GROVE_MANAGE_H
