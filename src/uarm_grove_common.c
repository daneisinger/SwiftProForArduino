//
// Created by cap on 4/21/20.
//

#include "uarm_grove_common.h"

#define GROVE_CLK_PIN 	0
#define GROVE_DAT_PIN	1

uint8_t port_pin_map[GROVE_PORT_NUM_MAX][2] = {
        {GROVE_PORT0_CLK, GROVE_PORT0_DAT},		//port0 I2C
        {GROVE_PORT1_CLK, GROVE_PORT1_DAT},		//port1 A
        {GROVE_PORT2_CLK, GROVE_PORT2_DAT},		//port2 A
        {GROVE_PORT3_CLK, GROVE_PORT3_DAT},		//port3 D

        {GROVE_PORT4_CLK, GROVE_PORT4_DAT},		//port4 D/PWM
        {GROVE_PORT5_CLK, GROVE_PORT5_DAT},		//port5 D
        {0xff, 0xff},	                        //not used
        {0xff, 0xff},	                        //not used

        {GROVE_PORT8_CLK, 0xff},	            //port8 D
        {GROVE_PORT9_CLK, 0xff},	            //port9 D
        {0xff, 0xff},	                        //not used
        {0xff, 0xff},	                        //not used

        {0xff, 0xff},	                        //not used
        {GROVE_PORT13_CLK, 0xff},	            //port13 A
};

long groveMillis = 0;


GroveModule_t* newGroveModule(GroveType_t moduleType)
{
    DB_PRINT_STR("New Grove module of type: %d\r\n", moduleType);
    GroveModule_t *m = malloc(sizeof(*m));
    m->_moduleType = moduleType;
    return m;
}

void delGroveModule(GroveModule_t* module)
{
    DB_PRINT_STR("Free memory for module\r\n");
    free((void *)module);
}

uint8_t getGrovePortClkPin(uint8_t portNum)
{
    return port_pin_map[portNum][GROVE_CLK_PIN];
}

uint8_t getGrovePortDatPin(uint8_t portNum)
{
    return port_pin_map[portNum][GROVE_DAT_PIN];
}

void incGroveMillis()
{
    groveMillis++;
}

long getGroveMillis()
{
    return groveMillis;
}