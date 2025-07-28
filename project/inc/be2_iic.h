#ifndef __BE2_IIC_H
#define __BE2_IIC_H

#include "at32f402_405_wk_config.h"
#include <stdint.h>
#include "at32f402_405_i2c.h"  // 包含库API头文件

// I2C从设备地址（ADD0=0时，对应手册中I2C从设备地址设置）
#define BE2_I2C_ADDR           0x32   // 固定为ADD0=0模式的地址{insert\_element\_0\_}

// 寄存器地址定义（同之前版本，保持与硬件手册一致）
#define REG_STATUS             0x00    // 状态寄存器
#define REG_FUN_CONFIG         0x01    // 功能配置寄存器
#define REG_SYS_CONFIG         0x02    // 系统配置寄存器
#define REG_DATA_CTRL          0x03    // 数据控制寄存器
#define REG_DATA_ENABLE        0x04    // 数据使能寄存器
#define REG_CTRL_0_A           0x05    // 加速度计控制寄存器
#define REG_CTRL_1_G           0x06    // 陀螺仪控制寄存器
#define REG_FILTER_CONFIG      0x07    // 滤波配置寄存器

// 数据寄存器地址（同之前版本）
#define REG_TIMESTAMP_0        0x20    // 时间戳
#define REG_ACC_X_0            0x24    // 加速度X轴
#define REG_ACC_Y_0            0x28    // 加速度Y轴
#define REG_ACC_Z_0            0x2C    // 加速度Z轴
#define REG_GYR_X_0            0x30    // 陀螺仪X轴
#define REG_GYR_Y_0            0x34    // 陀螺仪Y轴
#define REG_GYR_Z_0            0x38    // 陀螺仪Z轴
#define REG_QUAT_W_0           0x48    // 四元数W
#define REG_QUAT_X_0           0x4C    // 四元数X
#define REG_QUAT_Y_0           0x50    // 四元数Y
#define REG_QUAT_Z_0           0x54    // 四元数Z
#define REG_EULER_X_0          0x58    // 欧拉角X(roll)
#define REG_EULER_Y_0          0x5C    // 欧拉角Y(pitch)
#define REG_EULER_Z_0          0x60    // 欧拉角Z(yaw)
#define REG_LIN_ACC_X_0        0x64    // 线性加速度X
#define REG_LIN_ACC_Y_0        0x68    // 线性加速度Y
#define REG_LIN_ACC_Z_0        0x6C    // 线性加速度Z
#define REG_TEMP_0             0x70    // 温度
#define REG_WHO_AM_I           0x74    // 设备ID

// 传感器数据结构
typedef struct {
    uint32_t timestamp;               // 时间戳
    float acc_x, acc_y, acc_z;        // 加速度(g)
    float gyr_x, gyr_y, gyr_z;        // 角速度(dps)
    float quat_w, quat_x, quat_y, quat_z;  // 四元数
    float euler_roll, euler_pitch, euler_yaw;  // 欧拉角(度)
    float lin_acc_x, lin_acc_y, lin_acc_z;  // 线性加速度(g)
    float temp;                       // 温度(℃)
} BE2_Data;

// 函数声明
void BE2_I2C_Init(void);
uint8_t BE2_I2C_WriteReg(uint8_t reg, uint8_t data);
uint8_t BE2_I2C_ReadReg(uint8_t reg, uint8_t *data);
uint8_t BE2_I2C_ReadMultiReg(uint8_t reg, uint8_t *buf, uint16_t len);
uint8_t BE2_Init(void);
uint8_t BE2_ReadAllData(BE2_Data *data);
void BE2_PrintData(BE2_Data *data);

#endif
