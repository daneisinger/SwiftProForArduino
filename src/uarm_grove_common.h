//
// Created by cap on 4/21/20.
//

#ifndef _UARM_GROVE_COMMON_H
#define _UARM_GROVE_COMMON_H

#include "uarm_common.h"

#define GROVE_PORT_NUM_MAX	14

#define GROVE_PORT0_CLK 0
#define GROVE_PORT0_DAT 0

#define GROVE_PORT1_CLK 14
#define GROVE_PORT1_DAT 41

#define GROVE_PORT2_CLK 15
#define GROVE_PORT2_DAT 47

#define GROVE_PORT3_CLK 39
#define GROVE_PORT3_DAT 29

#define GROVE_PORT4_CLK 10
#define GROVE_PORT4_DAT 51

#define GROVE_PORT5_CLK 43
#define GROVE_PORT5_DAT 45

#define GROVE_PORT8_CLK 8
#define GROVE_PORT8_DAT 0xff

#define GROVE_PORT9_CLK 9
#define GROVE_PORT9_DAT 0xff

#define GROVE_PORT13_CLK 13
#define GROVE_PORT13_DAT 0xff

#define NOT_GROVE_PORT(port) (port<0 || (port>5 && port<8) || (port>9 && port<13) || port>13)


typedef enum GroveType
{
    GROVE_TYPE_NONE 					= 0,

    GROVE_CHAINABLE_LED 				= 1,
    GROVE_BUTTON 						= 2,
    GROVE_SLIDING_POTENTIOMETER 		= 3,
    GROVE_VIBRATION_MOTOR 				= 4,
    GROVE_LIGHT_SENSOR 					= 5,
    GROVE_ANGLE_SENSOR 					= 6,
    GROVE_AIR_QUALITY_SENSOR			= 7,
    GROVE_SOUND_SENSOR					= 8,
    GROVE_6AXIS_SENSOR					= 9,
    GROVE_COLOR_SENSOR 					= 10,
    GROVE_GESTURE_SERSOR 				= 11,
    GROVE_ULTRASONIC					= 12,
    GROVE_FAN 							= 13,
    GROVE_ELECTROMAGNET 				= 14,
    GROVE_TEMPERATURE_HUMIDITY_SENSOR 	= 15,
    GROVE_PIR_MOTION_SENSOR 			= 16,
    GROVE_RGBLCD 						= 17,
    GROVE_LINE_FINDER 					= 18,
    GROVE_IR_DISTANCE	 				= 19,
    GROVE_EMG_DETECTOR					= 20,
    GROVE_OLED12864						= 21,
    //GROVE_SERVO							= 22,

    GROVE_TYPE_COUNT
} GroveType_t;


typedef enum GroveDataType {
    GROVE_BYTE,
    GROVE_INT,
    GROVE_FLOAT,
    GROVE_LCD_VALUE,
    GROVE_LCD_RGB,
    GROVE_LCD_STATE,
    GROVE_LED_RGB
} GroveDataType_t;


typedef struct GroveData {
    GroveDataType_t dataType;
    uint8_t c, r, g, b;
    uint16_t i;
    float f;
    char *s;
} GroveData_t;

typedef struct GroveReport
{
    long interval;
    long timestamp;
} GroveReport_t;

#define REPORT_TYPE_GROVE2    11


struct GroveModule {
    bool (*init)(struct GroveModule* self, uint8_t portNum);
    void (*report)(struct GroveModule* self);
    bool (*control)(struct GroveModule* self, GroveData_t data);
    void (*tick)(struct GroveModule* self);

    GroveType_t _moduleType;
    uint8_t _portNum, _clkPin, _datPin;
};
typedef struct GroveModule GroveModule_t;

uint8_t getGrovePortClkPin(uint8_t portNum);
uint8_t getGrovePortDatPin(uint8_t portNum);

GroveModule_t* newGroveModule(GroveType_t moduleType);
void delGroveModule(GroveModule_t* module);

void incGroveMillis();
long getGroveMillis();



#endif //_UARM_GROVE_COMMON_H
