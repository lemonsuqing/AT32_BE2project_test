#ifndef BE2_SOFTWARE_IIC_H
#define BE2_SOFTWARE_IIC_H

#include "at32f402_405.h"
#include "i2c_application.h"
#include "wk_system.h"
#include "wk_gpio.h"
#include "oled.h"
#include <stddef.h>
#include <stdint.h>

// BE2 I2C地址（参考20230707硬件手册表3-1：MODE0=0时为0x32）
#define BE2_I2C_ADDR_WRITE   0x64  // 0x32 << 1（写操作）
#define BE2_I2C_ADDR_READ    0x65  // 0x32 << 1 | 1（读操作）

// 寄存器地址（参考20230707硬件手册附录寄存器映射）
#define BE2_REG_ACC_X        0x24  // 加速度计X轴（4字节，小端浮点）
#define BE2_REG_ACC_Y        0x28  // 加速度计Y轴
#define BE2_REG_ACC_Z        0x2C  // 加速度计Z轴
#define BE2_REG_GYR_X        0x30  // 陀螺仪X轴（4字节，小端浮点）
#define BE2_REG_GYR_Y        0x34  // 陀螺仪Y轴
#define BE2_REG_GYR_Z        0x38  // 陀螺仪Z轴
#define BE2_REG_EULER_X      0x58  // 欧拉角X（横滚角，4字节浮点）
#define BE2_REG_EULER_Y      0x5C  // 欧拉角Y（俯仰角）
#define BE2_REG_EULER_Z      0x60  // 欧拉角Z（偏航角）

// 函数声明
uint8_t BE2_I2C_Init(void);  // 初始化BE2（复用OLED的I2C引脚配置）
uint8_t BE2_ReadReg(uint8_t reg_addr, uint8_t *data, uint8_t len);  // 读取寄存器
uint8_t BE2_ReadAccelerometer(float *ax, float *ay, float *az);     // 读取加速度计（单位：g，参考20220802用户手册2.2.1）
uint8_t BE2_ReadGyroscope(float *gx, float *gy, float *gz);         // 读取陀螺仪（单位：dps，参考20220802用户手册2.2.2）
uint8_t BE2_ReadEulerAngle(float *roll, float *pitch, float *yaw);  // 读取欧拉角（单位：度，参考20220802用户手册2.3.2）


#endif
