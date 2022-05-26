/**
 * @file s51_i2c.c
 * @author bnsycmc(bnsycmc@qq.com)
 * @brief s51 soft i2c data transfer overview
 * @version 0.1
 * @date 2022-05-25
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "s51_i2c.h"
#include "stdio.h"
/**
 * @brief s51 soft i2c statr
 *
 * @param _i2c
 */
static void s51_i2c_Start(s51_i2c_struct *_i2c)
{
    _i2c->pin_mode(S51_I2C_PIN_NAME_SCL, S51_I2C_PIN_MODE_WRITE);
    _i2c->pin_mode(S51_I2C_PIN_NAME_SDA, S51_I2C_PIN_MODE_WRITE);

    _i2c->pin_write(S51_I2C_PIN_NAME_SCL, S51_I2C_PIN_STATE_HIGH);
    _i2c->pin_write(S51_I2C_PIN_NAME_SDA, S51_I2C_PIN_STATE_HIGH);

    _i2c->pin_write(S51_I2C_PIN_NAME_SDA, S51_I2C_PIN_STATE_LOW);
    _i2c->pin_write(S51_I2C_PIN_NAME_SCL, S51_I2C_PIN_STATE_LOW);
}

/**
 * @brief s51 soft i2c stop
 *
 * @param _i2c
 */
static void s51_i2c_Stop(s51_i2c_struct *_i2c)
{
    _i2c->pin_mode(S51_I2C_PIN_NAME_SCL, S51_I2C_PIN_MODE_WRITE);
    _i2c->pin_mode(S51_I2C_PIN_NAME_SDA, S51_I2C_PIN_MODE_WRITE);

    _i2c->pin_write(S51_I2C_PIN_NAME_SDA, S51_I2C_PIN_STATE_LOW);
    _i2c->pin_write(S51_I2C_PIN_NAME_SCL, S51_I2C_PIN_STATE_HIGH);
    _i2c->pin_write(S51_I2C_PIN_NAME_SDA, S51_I2C_PIN_STATE_HIGH);
}

/**
 * @brief s51 soft i2c wair ack signal
 *
 * @param _i2c
 * @return uint8_t 0:failed 1:success
 */
static uint8_t s51_i2c_WaitAck(s51_i2c_struct *_i2c)
{
    uint16_t timeCount;
    _i2c->pin_mode(S51_I2C_PIN_NAME_SDA, S51_I2C_PIN_MODE_READ);

    for (timeCount = 0; timeCount < _i2c->ackWaitClk; timeCount++)
    {
        if (_i2c->pin_read(S51_I2C_PIN_NAME_SDA) == S51_I2C_PIN_STATE_LOW)
        {
            _i2c->pin_write(S51_I2C_PIN_NAME_SCL, S51_I2C_PIN_STATE_HIGH);
            _i2c->pin_write(S51_I2C_PIN_NAME_SCL, S51_I2C_PIN_STATE_LOW);
            return S51_I2C_PIN_STATE_LOW;
        }
    }
    _i2c->pin_write(S51_I2C_PIN_NAME_SCL, S51_I2C_PIN_STATE_HIGH);
    _i2c->pin_write(S51_I2C_PIN_NAME_SCL, S51_I2C_PIN_STATE_LOW);

    return S51_I2C_PIN_STATE_HIGH;
}
static void s51_i2c_Ack(s51_i2c_struct *_i2c)
{
    _i2c->pin_mode(S51_I2C_PIN_NAME_SDA, S51_I2C_PIN_MODE_WRITE);
    _i2c->pin_write(S51_I2C_PIN_NAME_SCL, S51_I2C_PIN_STATE_LOW);
    _i2c->pin_write(S51_I2C_PIN_NAME_SDA, S51_I2C_PIN_STATE_LOW);
    _i2c->pin_write(S51_I2C_PIN_NAME_SCL, S51_I2C_PIN_STATE_HIGH);
    _i2c->pin_write(S51_I2C_PIN_NAME_SCL, S51_I2C_PIN_STATE_LOW);
}

static void s51_i2c_NoAck(s51_i2c_struct *_i2c)
{
    _i2c->pin_mode(S51_I2C_PIN_NAME_SDA, S51_I2C_PIN_MODE_WRITE);
    _i2c->pin_write(S51_I2C_PIN_NAME_SCL, S51_I2C_PIN_STATE_LOW);
    _i2c->pin_write(S51_I2C_PIN_NAME_SDA, S51_I2C_PIN_STATE_HIGH);
    _i2c->pin_write(S51_I2C_PIN_NAME_SCL, S51_I2C_PIN_STATE_HIGH);
    _i2c->pin_write(S51_I2C_PIN_NAME_SCL, S51_I2C_PIN_STATE_LOW);
}
static void s51_i2c_WriteByte(s51_i2c_struct *_i2c, uint8_t _byte)
{
    uint8_t i;
    uint8_t dataCache;
    _i2c->pin_mode(S51_I2C_PIN_NAME_SDA, S51_I2C_PIN_MODE_WRITE);
    dataCache = _byte;
    for (i = 0; i < 8; i++)
    {

        if (dataCache & 0x80)
        {
            _i2c->pin_write(S51_I2C_PIN_NAME_SDA, S51_I2C_PIN_STATE_HIGH);
        }
        else
        {
            _i2c->pin_write(S51_I2C_PIN_NAME_SDA, S51_I2C_PIN_STATE_LOW);
        }
        _i2c->pin_write(S51_I2C_PIN_NAME_SCL, S51_I2C_PIN_STATE_HIGH);
        _i2c->pin_write(S51_I2C_PIN_NAME_SCL, S51_I2C_PIN_STATE_LOW);

        dataCache = dataCache << 1;
    }
    _i2c->pin_write(S51_I2C_PIN_NAME_SDA, S51_I2C_PIN_STATE_HIGH); // the last bit set sda high and wait ack
}

static uint8_t s51_i2c_ReadByte(s51_i2c_struct *_i2c)
{
    uint8_t i;
    uint8_t dataCache;
    _i2c->pin_mode(S51_I2C_PIN_NAME_SDA, S51_I2C_PIN_MODE_READ);
    dataCache = 0;
    for (i = 0; i < 8; i++)
    {
        _i2c->pin_write(S51_I2C_PIN_NAME_SCL, S51_I2C_PIN_STATE_LOW);
        _i2c->pin_write(S51_I2C_PIN_NAME_SCL, S51_I2C_PIN_STATE_HIGH);
        dataCache = dataCache << 1;
        if (_i2c->pin_read(S51_I2C_PIN_NAME_SDA) == S51_I2C_PIN_STATE_HIGH)
        {
            dataCache = dataCache | 0x01;
        }
    }
    _i2c->pin_write(S51_I2C_PIN_NAME_SCL, S51_I2C_PIN_STATE_LOW);

    return dataCache;
}

/**
 * @brief transmit i2c data
 *
 * @param _i2c handle
 * @param address drive address
 * @param _data transmit buff
 * @param len transmit data len
 * @return uint8_t 0:falid 1:success
 */
uint8_t s51_i2c_TransmitDataAddress(s51_i2c_struct *_i2c, uint8_t address, uint8_t *_data, uint16_t len)
{
    uint16_t i;
    s51_i2c_Start(_i2c);
    s51_i2c_WriteByte(_i2c, address);
    if (s51_i2c_WaitAck(_i2c) == S51_I2C_PIN_STATE_HIGH)
    {
        return 0; // faild
    }

    for (i = 0; i < len; i++)
    {
        s51_i2c_WriteByte(_i2c, *_data++);
        if (s51_i2c_WaitAck(_i2c) == S51_I2C_PIN_STATE_HIGH)
        {
            return 0; // faild
        }
    }

    s51_i2c_Stop(_i2c);
    return 1; // success
}

/**
 * @brief receive i2c data
 *
 * @param _i2c handle
 * @param address drive address
 * @param _data read buff
 * @param len read len
 * @return uint8_t 0:faild,1:success
 */
uint8_t s51_i2c_ReceiveDataAddress(s51_i2c_struct *_i2c, uint8_t address, uint8_t *_data, uint16_t len)
{
    uint16_t i;
    s51_i2c_Start(_i2c);
    s51_i2c_WriteByte(_i2c, address);
    if (s51_i2c_WaitAck(_i2c) == S51_I2C_PIN_STATE_HIGH)
    {
        return 0; // faild
    }

    for (i = 0; i < len; i++)
    {
        *_data++ = s51_i2c_ReadByte(_i2c);
        if (i >= len)
        {
            s51_i2c_NoAck(_i2c);
        }
        else
        {
            s51_i2c_Ack(_i2c);
        }
    }

    s51_i2c_Stop(_i2c);
    return 1; // success
}

/**
 * @brief s51 soft i2c send data then read data without stop
 *
 * @param _i2c  handle
 * @param address  drive address
 * @param _Txdata send data buff
 * @param Txlen  send data len
 * @param _Rxdata read data buff
 * @param Rxlen  read data len
 * @return uint8_t 0:faild,1:success
 */
uint8_t s51_i2c_TransmitReceiveDataAddress(s51_i2c_struct *_i2c, uint8_t address, uint8_t *_Txdata, uint16_t Txlen, uint8_t *_Rxdata, uint16_t Rxlen)
{
    uint16_t i;
    s51_i2c_Start(_i2c);
    s51_i2c_WriteByte(_i2c, address);
    if (s51_i2c_WaitAck(_i2c) == S51_I2C_PIN_STATE_HIGH)
    {
        return 0; // faild
    }

    for (i = 0; i < Txlen; i++)
    {
        s51_i2c_WriteByte(_i2c, *_Txdata++);
        if (s51_i2c_WaitAck(_i2c) == S51_I2C_PIN_STATE_HIGH)
        {
            return 0; // faild
        }
    }

    s51_i2c_Start(_i2c);
    s51_i2c_WriteByte(_i2c, address + 1);
    if (s51_i2c_WaitAck(_i2c) == S51_I2C_PIN_STATE_HIGH)
    {
        return 0; // faild
    }
    for (i = 0; i < Rxlen; i++)
    {
        *_Rxdata++ = s51_i2c_ReadByte(_i2c);
        if (i >= Rxlen - 1)
        {
            s51_i2c_NoAck(_i2c);
        }
        else
        {
            s51_i2c_Ack(_i2c);
        }
    }

    s51_i2c_Stop(_i2c);

    return 1; // success
}