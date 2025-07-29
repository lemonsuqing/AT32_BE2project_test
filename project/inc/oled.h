#ifndef __OLED_H
#define __OLED_H

#include "at32f402_405_wk_config.h"
#include "wk_i2c.h"
#include "wk_spi.h"
#include "wk_usart.h"
#include "wk_gpio.h"
#include "Serial.h"
#include <stdint.h>
#include <string.h>

// OLED配置参数
#define OLED_WIDTH       128     // 屏幕宽度
#define OLED_HEIGHT      64      // 屏幕高度
#define OLED_PAGE_COUNT  8       // 页数量(64/8)
#define OLED_I2C_ADDR    0x3C    // OLED I2C地址(7位，根据模块实际修改，常见0x3C或0x3D)
#define OLED_I2C         I2C2    // 使用的I2C外设

// 命令/数据区分
#define OLED_CMD         0x00    // 写命令(Co=0, D/C#=0)
#define OLED_DATA        0x40    // 写数据(Co=0, D/C#=1)

// 函数声明
void OLED_Init(void);                    // 初始化OLED
void OLED_Clear(void);                   // 清屏
void OLED_DisplayOn(void);               // 开启显示
void OLED_DisplayOff(void);              // 关闭显示
void OLED_SetPos(uint8_t x, uint8_t y);  // 设置光标位置(x:0-127, y:0-7)
void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t c);  // 显示单个字符(6x8)
void OLED_ShowString(uint8_t x, uint8_t y, uint8_t *str);  // 显示字符串
void OLED_ShowNumber(uint8_t x, uint8_t y, uint32_t num, uint8_t len);  // 显示数字

#endif
