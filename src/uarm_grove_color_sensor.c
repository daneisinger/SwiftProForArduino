//
// Created by cap on 5/13/20.
//

#include "uarm_grove_color_sensor.h"

GroveModule_t* newGroveColorSensor()
{
    GroveModule_t* self = newGroveModule(GROVE_COLOR_SENSOR);
    self->init = groveColorSensorInit;
    self->report = groveColorSensorReport;
    self->control = groveColorSensorControl;
    self->tick = groveColorSensorTick;
    return self;
}

bool groveColorSensorInit(GroveModule_t* self, uint8_t portNum)
{
    DB_PRINT_STR("Initialize module port (I2C)\r\n");
    self->_portNum = portNum;

    TCS34725_init(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_16X);

    if (TCS34725_begin()) {
        TCS34725_setPersistence(TCS34725_PERS_NONE);
        TCS34725_setInterrupt(true);
    } else {
        DB_PRINT_STR("No TCS34725 found ... check your connections\r\n");
        DB_PRINT_STR("use P0(I2C)\r\n");
        return false;
    }

    return true;
}

void groveColorSensorReport(GroveModule_t* self)
{
    uint16_t red, green, blue, c;

    TCS34725_getRawData_noDelay(&red, &green, &blue, &c);
    TCS34725_clearInterrupt();

    red /= 255;
    green /= 255;
    blue /= 255;

    red = constrain(red,0,255);
    green = constrain(green,0,255);
    blue = constrain(blue,0,255);

    uart_printf("@%d P%d N%d R%d G%d B%d\r\n", REPORT_TYPE_GROVE2, self->_portNum, GROVE_COLOR_SENSOR, red, green, blue);
}

bool groveColorSensorControl(GroveModule_t* self, GroveData_t data)
{
    // nothing to do
}

void groveColorSensorTick(GroveModule_t* self)
{
    // nothing to do
}
