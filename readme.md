# simple51 soft i2c drive

## 简介
跨平台的IO模拟I2C解决方案,本文采用面向对象设计方案,旨在可以在任何平台上使用该接口.  
您只需要做一件事,每一个i2c引脚写三个io初始化的程序,就是这么简单:)  
demo中采用平头哥内核的W801作为演示.

## 使用方法
只有两个文件,其中`s51_i2c.h`内有一个结构体,  
```
typedef struct
{
    uint16_t ackWaitClk;                 // 应答超时时钟
    void (*pin_mode)(uint8_t, uint8_t);  // 切换引脚输入输出方式
    void (*pin_write)(uint8_t, uint8_t); // 引脚输出
    uint8_t (*pin_read)(uint8_t);        // 引脚读取

} s51_i2c_struct;
```
ackWaitClk为应答超时时钟,及为需要等待ACK的时钟长度,超时返回失败.  

`void (*pin_mode)(uint8_t, uint8_t);`  
该函数为引脚切换输入输出模式,第一个参数为引脚名称,第二个参数为引脚

`void (*pin_write)(uint8_t, uint8_t);`  
该函数为引脚输出高低电平

`uint8_t (*pin_read)(uint8_t);`  
该函数为引脚读取高低电平

在用户程序中,只需要注册这三个函数和设置ack 应答等待时间即可完成i2c的初始化.详细方法见demo.

