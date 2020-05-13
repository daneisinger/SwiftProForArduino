//
// Created by cap on 4/28/20.
//

#include "uarm_grove_manage.h"

uint16_t PortMask[GROVE_TYPE_COUNT] = {
        0,
        0x38,	//GROVE_CHAINABLE_LED
        0x38,	//GROVE_BUTTON
        0x2006,	//GROVE_SLIDING_POTENTIOMETER
        0x338,	//GROVE_VIBRATION_MOTOR
        0x2006,	//GROVE_LIGHT_SENSOR
        0x2006,	//GROVE_ANGLE_SENSOR
        0x2006,	//GROVE_AIR_QUALITY_SENSOR
        0x2006,	//GROVE_SOUND_SENSOR
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
        0x2006,	//GROVE_EMG_DETECTOR
        1		//GROVE_OLED12864
};

GroveModule_t* pModule[GROVE_PORT_NUM_MAX] = {NULL};
GroveReport_t groveReport[GROVE_PORT_NUM_MAX] = {0};

bool initGroveModule(uint8_t portNum, GroveType_t type)
{
    DB_PRINT_STR("Init Grove port %d with Grove type %d\r\n", portNum, type);

    // Validate port, type and port-type combo
    if (portNum > GROVE_PORT_NUM_MAX - 1)
    {
        DB_PRINT_STR("Port num should be less than %d\r\n", GROVE_PORT_NUM_MAX);
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
        case GROVE_ANGLE_SENSOR:
            pModule[portNum] = newGroveAngleSensor();
            break;
        case GROVE_TEMPERATURE_HUMIDITY_SENSOR:
            pModule[portNum] = newGroveTHSensor();
            break;
        case GROVE_COLOR_SENSOR:
            pModule[portNum] = newGroveColorSensor();
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

    if (portNum > GROVE_PORT_NUM_MAX - 1)
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

    if (portNum > GROVE_PORT_NUM_MAX - 1)
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

bool setGroveModuleReportInterval(uint8_t portNum, long interval)
{
    if (portNum > GROVE_PORT_NUM_MAX - 1)
    {
        DB_PRINT_STR("Port num should be less than %d\r\n", GROVE_PORT_NUM_MAX);
        return false;
    }

    if (pModule[portNum] == NULL)
    {
        DB_PRINT_STR("Port %d not initialized\r\n", portNum);
        return false;
    }

    if (interval <= 0)
    {
        groveReport[portNum].interval = 0;
        return true;
    }

    if (getGroveMillis() == 0) {
        time3_set(0.001, incGroveMillis);
        time3_start();
    }

    groveReport[portNum].interval = interval;
    groveReport[portNum].timestamp = getGroveMillis();
    return true;
}

void GroveReportRun()
{
    for (int i = 0; i < GROVE_PORT_NUM_MAX; i++)
    {
        long curTimeStamp = getGroveMillis();

        // exist
        if (groveReport[i].interval != 0)
        {
            if((unsigned long)(curTimeStamp - groveReport[i].timestamp) >= groveReport[i].interval)
            {
                groveReport[i].timestamp = curTimeStamp;

                if (pModule[i] != NULL)
                {
                    pModule[i]->report(pModule[i]);
                }
            }
        }
    }
}

void GroveTick()
{
    for (int i = 0; i < GROVE_PORT_NUM_MAX; i++)
    {
        if (pModule[i] != NULL)
            pModule[i]->tick(pModule[i]);
    }
}

