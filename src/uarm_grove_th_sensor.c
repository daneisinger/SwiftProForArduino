//
// Created by cap on 5/10/20.
//

#include "uarm_grove_th_sensor.h"

#define TH02_I2C_DEV_ID         0x40
#define TH02_I2C_DEV_ID_8BIT    (TH02_I2C_DEV_ID << 1)

#define REG_STATUS           0x00
#define REG_DATA_H           0x01
#define REG_DATA_L           0x02
#define REG_CONFIG           0x03
#define REG_ID               0x11

#define STATUS_RDY_MASK      0x01    //poll RDY,0 indicate the conversion is done

#define CMD_MEASURE_HUMI     0x01    //perform a humility measurement
#define CMD_MEASURE_TEMP     0x11    //perform a temperature measurement

#define TH02_WR_REG_MODE      0xC0
#define TH02_RD_REG_MODE      0x80


GroveModule_t* newGroveTHSensor()
{
    GroveModule_t* self = newGroveModule(GROVE_TEMPERATURE_HUMIDITY_SENSOR);
    self->init = groveTHSensorInit;
    self->report = groveTHSensorReport;
    self->control = groveTHSensorControl;
    self->tick = groveTHSensorTick;
    return self;
}

bool groveTHSensorInit(GroveModule_t* self, uint8_t portNum)
{
    DB_PRINT_STR("Initialize module port (I2C)\r\n");
    DB_PRINT_STR("F_CPU: %lu\r\n", F_CPU);
    self->_portNum = portNum;
    i2c_init();
    return true;
}

void groveTHSensorReport(GroveModule_t* self)
{
    uart_printf("@%d P%d N%d T%f H%f\r\n", REPORT_TYPE_GROVE2, self->_portNum, GROVE_TEMPERATURE_HUMIDITY_SENSOR, ReadTemperature(), ReadHumidity());
}

bool groveTHSensorControl(GroveModule_t* self, GroveData_t data)
{
    // nothing to do
}

void groveTHSensorTick(GroveModule_t* self)
{
    // nothing to do
}

static float ReadTemperature(void)
{
    /* Start a new temperature conversion */
    if (WriteReg(REG_CONFIG, CMD_MEASURE_TEMP)) {
        //delay(100);
        /* Wait until conversion is done */
        while (!isAvailable());
        uint16_t value = ReadData();

        value = value >> 2;

        /*
          Formula:
          Temperature(C) = (Value/32) - 50
        */

        return (value / 32.0) - 50.0;
    }
    return 0;
}

static float ReadHumidity(void)
{
    /* Start a new humidity conversion */
    if(WriteReg(REG_CONFIG, CMD_MEASURE_HUMI)) {
        /* Wait until conversion is done */
        //delay(100);
        while (!isAvailable());
        uint16_t value = ReadData();

        value = value >> 4;

        /*
          Formula:
          Humidity(%) = (Value/16) - 24
        */

        return (value / 16.0) - 24.0;
    }
    return 0;
}

static uint8_t isAvailable()
{
    uint8_t status =  ReadReg(REG_STATUS);
    if(status & STATUS_RDY_MASK)
    {
        return 0;    //ready
    }
    else
    {
        return 1;    //not ready yet
    }
}

static bool WriteReg(uint8_t u8Reg,uint8_t u8Data)
{
    DB_PRINT_STR("WriteReg\r\n");
    if(i2c_start(TH02_I2C_DEV_ID_8BIT) == 0)
    {
        i2c_write(u8Reg);
        i2c_write(u8Data);
        i2c_stop();
        return true;
    }
    else
    {
        DB_PRINT_STR("Unable to access device.\r\n");
    }
    return false;
}

static uint16_t ReadData(void)
{
    uint16_t data = 0;

    DB_PRINT_STR("ReadData\r\n");
    if(i2c_start(TH02_I2C_DEV_ID_8BIT) == 0)
    {
        i2c_write(REG_DATA_H);

        i2c_rep_start(TH02_I2C_DEV_ID_8BIT + I2C_READ);
        data = i2c_readAck();
        data = (data << 8) | i2c_readNak();

        i2c_stop();
    }
    else
    {
        DB_PRINT_STR("Unable to access device.\r\n");
    }
    return data;
}

static uint8_t ReadReg(uint8_t u8Reg)
{
    uint8_t data = 0;

    /* Send a register reading command */
    DB_PRINT_STR("ReadReg\r\n");
    if(i2c_start(TH02_I2C_DEV_ID_8BIT) == 0)
    {
        i2c_write(u8Reg);

        i2c_rep_start(TH02_I2C_DEV_ID_8BIT + I2C_READ);
        data = i2c_readNak();

        i2c_stop();
    }
    else
    {
        DB_PRINT_STR("Unable to access device.\r\n");
    }
    return data;
}
