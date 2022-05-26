#ifndef __S51_I2C_H__
#define __S51_I2C_H__

#include "stdint.h"

#define S51_I2C_PIN_NAME_SDA 0
#define S51_I2C_PIN_NAME_SCL 1

#define S51_I2C_PIN_MODE_READ 0
#define S51_I2C_PIN_MODE_WRITE 1

#define S51_I2C_PIN_STATE_LOW 0
#define S51_I2C_PIN_STATE_HIGH 1

typedef struct
{
    uint16_t ackWaitClk;                 // 应答超时时钟
    void (*pin_mode)(uint8_t, uint8_t);  // 切换引脚输入输出方式
    void (*pin_write)(uint8_t, uint8_t); // 引脚输出
    uint8_t (*pin_read)(uint8_t);        // 引脚读取

} s51_i2c_struct;

uint8_t s51_i2c_TransmitDataAddress(s51_i2c_struct *_i2c, uint8_t address, uint8_t *_data, uint16_t len);
uint8_t s51_i2c_ReceiveDataAddress(s51_i2c_struct *_i2c, uint8_t address, uint8_t *_data, uint16_t len);
uint8_t s51_i2c_TransmitReceiveDataAddress(s51_i2c_struct *_i2c, uint8_t address, uint8_t *_Txdata, uint16_t Txlen, uint8_t *_Rxdata, uint16_t Rxlen);
#endif