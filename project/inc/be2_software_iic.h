#ifndef BE2_SOFTWARE_IIC_H
#define BE2_SOFTWARE_IIC_H

#include "at32f402_405.h"
#include "wk_system.h"
#include "wk_gpio.h"
#include "oled.h"
#include <stdint.h>

// BE2 I2C地址（MODE0=0接GND时）
#define BE2_I2C_ADDR_WRITE   0x64  // 0x32 << 1（写操作）
#define BE2_I2C_ADDR_READ    0x65  // 0x32 << 1 | 1（读操作）

// 寄存器地址
#define BE2_REG_DATA_ENABLE  0x04  // 数据使能寄存器
#define BE2_REG_CTRL0_A      0x05  // 加速度计控制寄存器
#define BE2_REG_ACC_X        0x24  // 加速度计X轴（4字节，小端浮点）
#define BE2_REG_ACC_Y        0x28  // 加速度计Y轴
#define BE2_REG_ACC_Z        0x2C  // 加速度计Z轴
#define BE2_REG_GYR_X        0x30  // 陀螺仪X轴
#define BE2_REG_GYR_Y        0x34  // 陀螺仪Y轴
#define BE2_REG_GYR_Z        0x38  // 陀螺仪Z轴
#define BE2_REG_EULER_X      0x58  // 欧拉角X（横滚角）
#define BE2_REG_EULER_Y      0x5C  // 欧拉角Y（俯仰角）
#define BE2_REG_EULER_Z      0x60  // 欧拉角Z（偏航角）

// 函数声明
uint8_t BE2_I2C_Init(void);                     // 初始化BE2
uint8_t BE2_WriteReg(uint8_t reg_addr, uint8_t data);  // 写入寄存器
uint8_t BE2_ReadReg(uint8_t reg_addr, uint8_t *data, uint8_t len);  // 读取寄存器
uint8_t BE2_EnableSensors(void);                // 使能传感器数据输出
uint8_t BE2_ReadAccelerometer(float *ax, float *ay, float *az);     // 读取加速度计
uint8_t BE2_ReadGyroscope(float *gx, float *gy, float *gz);         // 读取陀螺仪
uint8_t BE2_ReadEulerAngle(float *roll, float *pitch, float *yaw);  // 读取欧拉角
uint8_t BE2_EnterStreamingMode(void);

#endif
