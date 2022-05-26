#include "../s51_i2c.h"
s51_i2c_struct s51_i2c1Handle;

/*
  ______   _______     __           ______    ______   ________  ________        ______   ______    ______
 /      \ |       \  _/  \         /      \  /      \ |        \|        \      |      \ /      \  /      \
|  $$$$$$\| $$$$$$$ |   $$        |  $$$$$$\|  $$$$$$\| $$$$$$$$ \$$$$$$$$       \$$$$$$|  $$$$$$\|  $$$$$$\
| $$___\$$| $$____   \$$$$        | $$___\$$| $$  | $$| $$__       | $$           | $$   \$$__| $$| $$   \$$
 \$$    \ | $$    \   | $$         \$$    \ | $$  | $$| $$  \      | $$           | $$   /      $$| $$
 _\$$$$$$\ \$$$$$$$\  | $$         _\$$$$$$\| $$  | $$| $$$$$      | $$           | $$  |  $$$$$$ | $$   __
|  \__| $$|  \__| $$ _| $$_       |  \__| $$| $$__/ $$| $$         | $$          _| $$_ | $$_____ | $$__/  \
 \$$    $$ \$$    $$|   $$ \       \$$    $$ \$$    $$| $$         | $$         |   $$ \| $$     \ \$$    $$
  \$$$$$$   \$$$$$$  \$$$$$$        \$$$$$$   \$$$$$$  \$$          \$$          \$$$$$$ \$$$$$$$$  \$$$$$$



 */

/**
 * @brief 设置引脚模式
 *
 * @param pinName S51_I2C_PIN_NAME_SDA,S51_I2C_PIN_NAME_SCL
 * @param pinMode S51_I2C_PIN_MODE_READ,S51_I2C_PIN_MODE_WRITE
 */
void bsp_i2c1_pinMode(uint8_t pinName, uint8_t pinMode)
{
    uint16_t i;
    if (pinName == S51_I2C_PIN_NAME_SDA)
    {
        if (pinMode == S51_I2C_PIN_MODE_WRITE)
        {
            tls_gpio_cfg(WM_IO_PB_00, WM_GPIO_DIR_OUTPUT, WM_GPIO_ATTR_PULLHIGH);
        }
        else
        {
            tls_gpio_cfg(WM_IO_PB_00, WM_GPIO_DIR_INPUT, WM_GPIO_ATTR_FLOATING);
        }
    }
    if (pinName == S51_I2C_PIN_NAME_SCL)
    {
        if (pinMode == S51_I2C_PIN_MODE_WRITE)
        {
            tls_gpio_cfg(WM_IO_PB_01, WM_GPIO_DIR_OUTPUT, WM_GPIO_ATTR_PULLHIGH);
        }
        else
        {
            tls_gpio_cfg(WM_IO_PB_01, WM_GPIO_DIR_INPUT, WM_GPIO_ATTR_FLOATING);
        }
    }
    // 如果需要添加延时,在此处添加延时函数
    // for (i = 0; i < 255; i++)
    // {
    // }
}

/**
 * @brief 设置引脚输出
 *
 * @param pinName S51_I2C_PIN_NAME_SDA,S51_I2C_PIN_NAME_SCL
 * @param pinState S51_I2C_PIN_STATE_LOW,S51_I2C_PIN_STATE_HIGH
 */
void bsp_i2c1_pinWrite(uint8_t pinName, uint8_t pinState)
{
    uint16_t i;
    if (pinName == S51_I2C_PIN_NAME_SDA)
    {
        if (pinState == S51_I2C_PIN_STATE_HIGH)
        {
            tls_gpio_write(WM_IO_PB_00, 1);
        }
        else
        {
            tls_gpio_write(WM_IO_PB_00, 0);
        }
    }
    if (pinName == S51_I2C_PIN_NAME_SCL)
    {
        if (pinState == S51_I2C_PIN_STATE_HIGH)
        {
            tls_gpio_write(WM_IO_PB_01, 1);
        }
        else
        {
            tls_gpio_write(WM_IO_PB_01, 0);
        }
    }
    // 如果需要添加延时,在此处添加延时函数
    for (i = 0; i < 100; i++)
    {
    }
}

/**
 * @brief 读取引脚输入
 *
 * @param pinName S51_I2C_PIN_NAME_SDA,S51_I2C_PIN_NAME_SCL
 * @return uint8_t S51_I2C_PIN_STATE_LOW,S51_I2C_PIN_STATE_HIGH
 */
uint8_t bsp_i2c1_pinRead(uint8_t pinName)
{
    if (pinName == S51_I2C_PIN_NAME_SDA)
    {
        return (tls_gpio_read(WM_IO_PB_00));
    }
    if (pinName == S51_I2C_PIN_NAME_SCL)
    {
        return (tls_gpio_read(WM_IO_PB_01));
    }
}

/**
 * @brief 注册所有的板载i2c
 *
 */
void bsp_i2c_AllInit(void)
{
    // 注册函数
    s51_i2c1Handle.pin_mode = bsp_i2c1_pinMode;
    s51_i2c1Handle.pin_write = bsp_i2c1_pinWrite;
    s51_i2c1Handle.pin_read = bsp_i2c1_pinRead;

    // wait ack 时钟超时
    s51_i2c1Handle.ackWaitClk = 3;
}

void test_i2cRead(void)
{
    uint8_t txbuff[10];
    txbuff[0] = 0;
    // txbuff[0] = 0x00; //起始地址为0
    // txbuff[1] = 0;    //秒
    // txbuff[2] = 0;    //分
    // txbuff[3] = 0;    //小时
    // txbuff[4] = 0;    //星期
    // txbuff[5] = 0x26; //日
    // txbuff[6] = 0x05; //月
    // txbuff[7] = 0x22; //年

    if (s51_i2c_TransmitReceiveDataAddress(&s51_i2c1Handle, 0xD0, txbuff, 1, (uint8_t *)&txbuff[1], 7))
    {
        printf("ds3231 read success:[%d-%d-%d %d:%d:%d]\n",
               txbuff[7], txbuff[6], txbuff[5], txbuff[3], txbuff[2], txbuff[1]);
    }
    else
    {
        printf("ds3231 read faild\n");
    }
}

void bsp_init(void)
{
    bsp_i2c_AllInit();
}