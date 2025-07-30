#ifndef BE2_SOFTWARE_IIC_H
#define BE2_SOFTWARE_IIC_H

#include "at32f402_405.h"
#include "wk_system.h"
#include "wk_gpio.h"
#include "Serial.h"
#include "oled.h"
#include <stdint.h>

/* 引脚定义 */
#define BE2_SCL_PORT     GPIOB
#define BE2_SDA_PORT     GPIOB
#define BE2_SCL_PIN      GPIO_PINS_8
#define BE2_SDA_PIN      GPIO_PINS_9

/* 操作宏 */
#define BE2_W_SCL(x)     gpio_bits_write(BE2_SCL_PORT, BE2_SCL_PIN, (x) ? SET : RESET)
#define BE2_W_SDA(x)     gpio_bits_write(BE2_SDA_PORT, BE2_SDA_PIN, (x) ? SET : RESET)
#define BE2_R_SDA()      gpio_input_data_bit_read(BE2_SDA_PORT, BE2_SDA_PIN)

/* 函数声明 */
void BE2_I2C_Init(void);
void BE2_I2C_WriteByte(uint8_t reg, uint8_t data);
uint8_t BE2_I2C_ReadByte(uint8_t reg);
void BE2_I2C_ReadBytes(uint8_t reg, uint8_t *buf, uint8_t len);
void be2_test(void);
#endif
