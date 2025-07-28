#ifndef __BE2_SPI_H__
#define __BE2_SPI_H__

#include <stdint.h>
#include <stdbool.h>
#include "at32f402_405_wk_config.h"

// 传感器数据结构体定义（对应20220802lpmsBE2用户手册中表2-2的32位浮点型数据格式）
typedef struct {
    uint32_t timestamp;           // 时间戳（500Hz更新率）
    float acc_raw[3];             // 原始加速度计数据(g)
    float acc_calibrated[3];      // 校准后加速度计数据(g)
    float gyro_raw[3];            // 原始陀螺仪数据(dps)
    float gyro_bias_calibrated[3];// 静止偏差校准后陀螺仪数据(dps)
    float gyro_alignment[3];      // 坐标轴校准后陀螺仪数据(dps)
    float quaternion[4];          // 四元数(w,x,y,z，归一化单位)
    float euler[3];               // 欧拉角(roll,pitch,yaw，度)
} BE2_SensorData;

// SPI片选引脚定义（参考20230707lpmsBE2硬件手册中2.2节功能引脚）
#define LPMS_CS_PORT   GPIOC
#define LPMS_CS_PIN    GPIO_PINS_3
#define LPMS_CS_LOW()  gpio_bits_reset(LPMS_CS_PORT, LPMS_CS_PIN)
#define LPMS_CS_HIGH() gpio_bits_set(LPMS_CS_PORT, LPMS_CS_PIN)

// 函数声明
void be2_spi_init_cs_gpio(void);                  // 初始化片选引脚
bool be2_enter_command_mode(void);                // 进入命令模式（20220802用户手册3.5节）
uint8_t be2_read_whoami_lpb(void);                // 读取WHO_AM_I寄存器（20230707硬件手册6.1节）
bool be2_read_sensor_data(BE2_SensorData *data);  // 读取传感器数据（20220802用户手册3.3节）
bool be2_sensor_init(void);                       // 传感器初始化
uint8_t be2_spi_read_reg(uint8_t addr);

#endif
