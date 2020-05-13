/**************************************************************************/
/*!
    Copyright (c) 2012, Adafruit Industries
    All rights reserved.

    @file     TCS34725.c
    @author   KTOWN (Adafruit Industries)
    @license  BSD (see TCS34725.h)

    Driver for the TCS34725 digital color sensors.

    Adafruit invests time and resources providing this open source code,
    please support Adafruit and open-source hardware by purchasing
    products from Adafruit!

    @section  HISTORY

    v1.0 - First release
    v1.0c - Ported to C
*/
/**************************************************************************/
#include "TCS34725.h"

/*========================================================================*/
/*                          PRIVATE FUNCTIONS                             */
/*========================================================================*/

bool _tcs34725Initialised;
tcs34725Gain_t _tcs34725Gain;
tcs34725IntegrationTime_t _tcs34725IntegrationTime;

/**************************************************************************/
/*!
    @brief  Writes a register and an 8 bit value over I2C
*/
/**************************************************************************/
static void TCS34725_write8 (uint8_t reg, uint8_t value)
{
    i2c_start((TCS34725_ADDRESS << 1) + I2C_WRITE);
    i2c_write(TCS34725_COMMAND_BIT | reg);
    i2c_write(value);
    i2c_stop();
}

/**************************************************************************/
/*!
    @brief  Reads an 8 bit value over I2C
*/
/**************************************************************************/
static uint8_t TCS34725_read8(uint8_t reg)
{
    uint8_t data;

    i2c_start((TCS34725_ADDRESS << 1) + I2C_WRITE);
    i2c_write(TCS34725_COMMAND_BIT | reg);

    i2c_rep_start((TCS34725_ADDRESS << 1) + I2C_READ);
    data = i2c_readNak();

    i2c_stop();
    return data;
}

/**************************************************************************/
/*!
    @brief  Reads a 16 bit values over I2C
*/
/**************************************************************************/
static uint16_t TCS34725_read16(uint8_t reg)
{
    uint16_t data = 0;

    i2c_start((TCS34725_ADDRESS << 1) + I2C_WRITE);
    i2c_write(TCS34725_COMMAND_BIT | reg);

    /*
    uint16_t x; uint16_t t;
    Wire.requestFrom(TCS34725_ADDRESS, 2);
    t = Wire.read();
    x = Wire.read();
    x <<= 8;
    x |= t;
    return x;
    */

    i2c_rep_start((TCS34725_ADDRESS << 1) + I2C_READ);

    // Low byte is read first
    data = i2c_readAck();
    data = (i2c_readNak() << 8) | data;

    i2c_stop();
    return data;
}

/**************************************************************************/
/*!
    Enables the device
*/
/**************************************************************************/
static void TCS34725_enable(void)
{
    TCS34725_write8(TCS34725_ENABLE, TCS34725_ENABLE_PON);
    delay_ms(3);
    TCS34725_write8(TCS34725_ENABLE, TCS34725_ENABLE_PON | TCS34725_ENABLE_AEN);
}

/**************************************************************************/
/*!
    Disables the device (putting it in lower power sleep mode)
*/
/**************************************************************************/
static void TCS34725_disable(void)
{
    /* Turn the device off to save power */
    uint8_t reg = 0;
    reg = TCS34725_read8(TCS34725_ENABLE);
    TCS34725_write8(TCS34725_ENABLE, reg & ~(TCS34725_ENABLE_PON | TCS34725_ENABLE_AEN));
}

/*========================================================================*/
/*                           PUBLIC FUNCTIONS                             */
/*========================================================================*/

void TCS34725_init(tcs34725IntegrationTime_t it, tcs34725Gain_t gain)
{
    _tcs34725Initialised = false;
    _tcs34725IntegrationTime = it;
    _tcs34725Gain = gain;
}

/**************************************************************************/
/*!
    Initializes I2C and configures the sensor (call this function before
    doing anything else)
*/
/**************************************************************************/
bool TCS34725_begin(void)
{
    i2c_init();

    /* Make sure we're actually connected */
    uint8_t x = TCS34725_read8(TCS34725_ID);
    DB_PRINT_STR("TCS34725_ID register value: %hu\r\n", x);

    if ((x != TCS34725_ID_VALUE) && (x != TCS34725_STATUS_AINT))
    {
        return false;
    }
    _tcs34725Initialised = true;

    /* Set default integration time and gain */
    TCS34725_setIntegrationTime(_tcs34725IntegrationTime);
    TCS34725_setGain(_tcs34725Gain);

    /* Note: by default, the device is in power down mode on bootup */
    TCS34725_enable();

    return true;
}

/**************************************************************************/
/*!
    Sets the integration time for the TC34725
*/
/**************************************************************************/
void TCS34725_setIntegrationTime(tcs34725IntegrationTime_t it)
{
    if (!_tcs34725Initialised) TCS34725_begin();

    /* Update the timing register */
    TCS34725_write8(TCS34725_ATIME, it);

    /* Update value placeholders */
    _tcs34725IntegrationTime = it;
}

/**************************************************************************/
/*!
    Adjusts the gain on the TCS34725 (adjusts the sensitivity to light)
*/
/**************************************************************************/
void TCS34725_setGain(tcs34725Gain_t gain)
{
    if (!_tcs34725Initialised) begin();

    /* Update the timing register */
    TCS34725_write8(TCS34725_CONTROL, gain);

    /* Update value placeholders */
    _tcs34725Gain = gain;
}

/**************************************************************************/
/*!
    @brief  Reads the raw red, green, blue and clear channel values
*/
/**************************************************************************/
void TCS34725_getRawData_noDelay (uint16_t *r, uint16_t *g, uint16_t *b, uint16_t *c) {
    if (!_tcs34725Initialised) TCS34725_begin();

    *c = TCS34725_read16(TCS34725_CDATAL);
    *r = TCS34725_read16(TCS34725_RDATAL);
    *g = TCS34725_read16(TCS34725_GDATAL);
    *b = TCS34725_read16(TCS34725_BDATAL);
}

void TCS34725_getRawData (uint16_t *r, uint16_t *g, uint16_t *b, uint16_t *c)
{
    if (!_tcs34725Initialised) TCS34725_begin();

    *c = TCS34725_read16(TCS34725_CDATAL);
    *r = TCS34725_read16(TCS34725_RDATAL);
    *g = TCS34725_read16(TCS34725_GDATAL);
    *b = TCS34725_read16(TCS34725_BDATAL);

    /* Set a delay for the integration time */
    switch (_tcs34725IntegrationTime)
    {
        case TCS34725_INTEGRATIONTIME_2_4MS:
            delay_ms(3);
            break;
        case TCS34725_INTEGRATIONTIME_24MS:
            delay_ms(24);
            break;
        case TCS34725_INTEGRATIONTIME_50MS:
            delay_ms(50);
            break;
        case TCS34725_INTEGRATIONTIME_101MS:
            delay_ms(101);
            break;
        case TCS34725_INTEGRATIONTIME_154MS:
            delay_ms(154);
            break;
        case TCS34725_INTEGRATIONTIME_700MS:
            delay_ms(700);
            break;
        }
}

/**************************************************************************/
/*!
    @brief  Converts the raw R/G/B values to color temperature in degrees
            Kelvin
*/
/**************************************************************************/
uint16_t TCS34725_calculateColorTemperature(uint16_t r, uint16_t g, uint16_t b)
{
    float X, Y, Z;      /* RGB to XYZ correlation      */
    float xc, yc;       /* Chromaticity co-ordinates   */
    float n;            /* McCamy's formula            */
    float cct;

    /* 1. Map RGB values to their XYZ counterparts.    */
    /* Based on 6500K fluorescent, 3000K fluorescent   */
    /* and 60W incandescent values for a wide range.   */
    /* Note: Y = Illuminance or lux                    */
    X = (-0.14282F * r) + (1.54924F * g) + (-0.95641F * b);
    Y = (-0.32466F * r) + (1.57837F * g) + (-0.73191F * b);
    Z = (-0.68202F * r) + (0.77073F * g) + ( 0.56332F * b);

    /* 2. Calculate the chromaticity co-ordinates      */
    xc = (X) / (X + Y + Z);
    yc = (Y) / (X + Y + Z);

    /* 3. Use McCamy's formula to determine the CCT    */
    n = (xc - 0.3320F) / (0.1858F - yc);

    /* Calculate the final CCT */
    cct = (449.0F * powf(n, 3)) + (3525.0F * powf(n, 2)) + (6823.3F * n) + 5520.33F;

    /* Return the results in degrees Kelvin */
    return (uint16_t)cct;
}

/**************************************************************************/
/*!
    @brief  Converts the raw R/G/B values to lux
*/
/**************************************************************************/
uint16_t TCS34725_calculateLux(uint16_t r, uint16_t g, uint16_t b)
{
    float illuminance;

    /* This only uses RGB ... how can we integrate clear or calculate lux */
    /* based exclusively on clear since this might be more reliable?      */
    illuminance = (-0.32466F * r) + (1.57837F * g) + (-0.73191F * b);

    return (uint16_t)illuminance;
}


void TCS34725_setInterrupt(bool i) {
    uint8_t r = TCS34725_read8(TCS34725_ENABLE);
    if (i) {
        r |= TCS34725_ENABLE_AIEN;
    } else {
        r &= ~TCS34725_ENABLE_AIEN;
    }
    TCS34725_write8(TCS34725_ENABLE, r);
}

void TCS34725_clearInterrupt(void) {
    i2c_start((TCS34725_ADDRESS << 1) + I2C_WRITE);
    i2c_write(TCS34725_COMMAND_BIT | TCS34725_CLEAR_INTERRUPT);
    i2c_stop();
}


void TCS34725_setIntLimits(uint16_t low, uint16_t high) {
    TCS34725_write8(TCS34725_AILTL, low & 0xFF);
    TCS34725_write8(TCS34725_AILTH, low >> 8);
    TCS34725_write8(TCS34725_AIHTL, high & 0xFF);
    TCS34725_write8(TCS34725_AIHTH, high >> 8);
}

void TCS34725_setPersistence(uint8_t pers)
{
    TCS34725_write8(TCS34725_PERS, TCS34725_PERS_NONE);
}
