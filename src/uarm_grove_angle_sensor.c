//
// Created by cap on 5/1/20.
//

#include "uarm_grove_angle_sensor.h"

bool groveAngleSensorInit(GroveModule_t* self, uint8_t portNum)
{
    DB_PRINT_STR("Initialize module port\r\n");
    self->_portNum = portNum;
    self->_clkPin = getGrovePortClkPin(portNum);
    return true;
}

void groveAngleSensorReport(GroveModule_t* self)
{
    uint16_t value = getAnalogPinValue(self->_clkPin);
    uart_printf("@%d P%d N%d V%d\r\n", REPORT_TYPE_GROVE2, self->_portNum, GROVE_ANGLE_SENSOR, value);
}

bool groveAngleSensorControl(GroveModule_t* self, GroveData_t data)
{
    // Nothing to do
}

void groveAngleSensorTick(GroveModule_t* self)
{
    // Nothing to update
}

GroveModule_t* newGroveAngleSensor()
{
    GroveModule_t* self = newGroveModule(GROVE_ANGLE_SENSOR);
    self->init = groveAngleSensorInit;
    self->report = groveAngleSensorReport;
    self->control = groveAngleSensorControl;
    self->tick = groveAngleSensorTick;
    return self;
}
