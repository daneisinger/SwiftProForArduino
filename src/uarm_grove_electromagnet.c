//
// Created by cap on 4/28/20.
//

#include "uarm_grove_electromagnet.h"

bool groveElectromagnetInit(GroveModule_t* self, uint8_t portNum)
{
    DB_PRINT_STR("Initialize module port\r\n");
    self->_portNum = portNum;

    return true;
}


void groveElectromagnetReport(GroveModule_t* self)
{
    // Nothing to report
}


bool groveElectromagnetControl(GroveModule_t* self, GroveData_t data)
{
    if (data.dataType == GROVE_INT) {
        if (data.i) {
            DB_PRINT_STR("Turning electromagnet on...\r\n");
            switch (self->_portNum) {
                CASE_PORT_WRITE_CLK(3, 1);
                CASE_PORT_WRITE_CLK(4, 1);
                CASE_PORT_WRITE_CLK(5, 1);
                CASE_PORT_WRITE_CLK(8, 1);
                CASE_PORT_WRITE_CLK(9, 1);
                default:
                    DB_PRINT_STR("Unupported pin for module.");
                    return false;
            }
        } else {
            DB_PRINT_STR("Turning electromagnet off...\r\n");
            switch (self->_portNum) {
                CASE_PORT_WRITE_CLK(3, 0);
                CASE_PORT_WRITE_CLK(4, 0);
                CASE_PORT_WRITE_CLK(5, 0);
                CASE_PORT_WRITE_CLK(8, 0);
                CASE_PORT_WRITE_CLK(9, 0);
                default:
                    DB_PRINT_STR("Unupported pin for module.");
                    return false;
            }
        }
        return true;
    }

    return false;
}


void groveElectromagnetTick(GroveModule_t* self)
{
    // Nothing to update
}


GroveModule_t* newGroveElectromagnet()
{
    GroveModule_t* self = newGroveModule(GROVE_ELECTROMAGNET);
    self->init = groveElectromagnetInit;
    self->report = groveElectromagnetReport;
    self->control = groveElectromagnetControl;
    self->tick = groveElectromagnetTick;
    return self;
}

