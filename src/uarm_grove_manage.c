//
// Created by cap on 4/28/20.
//

#include "uarm_grove_manage.h"

#define PORT_NUM_MAX	14

uint16_t PortMask[GROVE_TYPE_COUNT] = {
        0,
        0x38,	//GROVE_CHAINABLE_LED
        0x38,	//GROVE_BUTTON
        6,		//GROVE_SLIDING_POTENTIOMETER
        0x338,	//GROVE_VIBRATION_MOTOR
        0x6,	//GROVE_LIGHT_SENSOR
        0x6,	//GROVE_ANGLE_SENSOR
        0x6,	//GROVE_AIR_QUALITY_SENSOR
        0x6,	//GROVE_SOUND_SENSOR
        1,		//GROVE_6AXIS_SENSOR
        1,		//GROVE_COLOR_SENSOR
        1,		//GROVE_GESTURE_SERSOR
        0x310,	//GROVE_ULTRASONIC
        0x310,	//GROVE_FAN
        0x338,	//GROVE_ELECTROMAGNET
        1,		//GROVE_TEMPERATURE_HUMIDITY_SENSOR
        0x338,	//GROVE_PIR_MOTION_SENSOR
        1,		//GROVE_RGBLCD
        0x338,	//GROVE_LINE_FINDER
        0x338,	//GROVE_IR_DISTANCE
        0x6,	//GROVE_EMG_DETECTOR
        1		//GROVE_OLED12864
};

GroveModule_t* pModule[PORT_NUM_MAX] = {NULL};


bool initGroveModule(uint8_t portNum, GroveType_t type)
{
    DB_PRINT_STR("Init Grove port %d with Grove type %d\r\n", portNum, type);

    // Validate port, type and port-type combo
    if (portNum > PORT_NUM_MAX - 1)
    {
        DB_PRINT_STR("Port num should be less than %d\r\n", PORT_NUM_MAX);
        return false;
    }

    if (type > GROVE_TYPE_COUNT - 1)
    {
        DB_PRINT_STR("Module num should be less than %d\r\n", GROVE_TYPE_COUNT);
        return false;
    }

    if (!(PortMask[type] & (1 << portNum)))
    {
        DB_PRINT_STR("Invalid port for module %d: ", type);
        return false;
    }

    // Free up memory for module if previously used
    if (pModule[portNum] != NULL)
    {
        delGroveModule(pModule[portNum]);
        pModule[portNum] = NULL;
    }

    // Initialize modules by type
    switch (type)
    {
        case GROVE_ELECTROMAGNET:
            pModule[portNum] = newGroveElectromagnet();
            break;
        default:
            DB_PRINT_STR("Module %d not implemented.\r\n", type);
            break;
    }

    // Set pins for port based on pin map
    if (pModule[portNum] != NULL)
    {
        if (pModule[portNum]->init(pModule[portNum], portNum))
        {
            return true;
        }
        else
        {
            DB_PRINT_STR("Init failed\r\n");
        }
    }
    else
    {
        DB_PRINT_STR("Memory allocation failed\r\n");
    }

    return false;
}


void deinitGroveModule(uint8_t portNum)
{
    DB_PRINT_STR("De-init Grove port %d\r\n", portNum);

    if (portNum > PORT_NUM_MAX - 1)
    {
        return;
    }

    if (pModule[portNum] != NULL)
    {
        delGroveModule(pModule[portNum]);
        pModule[portNum] = NULL;
    }

}


bool controlGroveModule(uint8_t portNum, GroveData_t data)
{
    DB_PRINT_STR("Control Grove port %d\r\n", portNum);

    if (portNum > PORT_NUM_MAX - 1)
    {
        return false;
    }

    if (pModule[portNum] != NULL)
    {
        return pModule[portNum]->control(pModule[portNum], data);
    }

    DB_PRINT_STR("Grove port not initialized\r\n");
    return false;
}