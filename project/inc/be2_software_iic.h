#ifndef BE2_SOFTWARE_IIC_H
#define BE2_SOFTWARE_IIC_H

#include "at32f402_405.h"
#include "wk_system.h"
#include "wk_gpio.h"
#include "Serial.h"
#include "oled.h"
#include <stdint.h>
#include <stdio.h>

/* 引脚定义 */
#define BE2_SCL_PORT     GPIOB
#define BE2_SDA_PORT     GPIOB
#define BE2_SCL_PIN      GPIO_PINS_8
#define BE2_SDA_PIN      GPIO_PINS_9

/* 宏操作 */
#define BE2_SCL_H()      gpio_bits_write(BE2_SCL_PORT, BE2_SCL_PIN, SET)
#define BE2_SCL_L()      gpio_bits_write(BE2_SCL_PORT, BE2_SCL_PIN, RESET)
#define BE2_SDA_H()      gpio_bits_write(BE2_SDA_PORT, BE2_SDA_PIN, SET)
#define BE2_SDA_L()      gpio_bits_write(BE2_SDA_PORT, BE2_SDA_PIN, RESET)
#define BE2_SDA_READ()   gpio_input_data_bit_read(BE2_SDA_PORT, BE2_SDA_PIN)

/* 函数接口 */
void BE2_I2C_Init(void);
uint8_t BE2_I2C_CheckDevice(void);
uint8_t BE2_I2C_ReadReg(uint8_t reg);
void BE2_Test_WhoAmI(void);

#endif
