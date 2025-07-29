#ifndef __OLED_H
#define __OLED_H

#include "at32f402_405.h"
#include "i2c_application.h"
#include <stdint.h>

// OLED配置参数
#define OLED_WIDTH       128     // 屏幕宽度
#define OLED_HEIGHT      64      // 屏幕高度
#define OLED_PAGE_COUNT  8       // 页数量(64/8)
#define OLED_I2C_ADDR    0x3C    // OLED I2C地址(7位地址)
#define OLED_TIMEOUT     1000    // I2C操作超时时间(ms)

// 命令/数据区分
#define OLED_CMD         0x00    // 写命令
#define OLED_DATA        0x40    // 写数据

// 函数声明
void OLED_Init(i2c_handle_type *hi2c);                    // 初始化OLED
void OLED_Clear(void);                                   // 清屏
void OLED_DisplayOn(void);                               // 开启显示
void OLED_DisplayOff(void);                              // 关闭显示
void OLED_SetPos(uint8_t x, uint8_t y);                  // 设置光标位置(x:0-127, y:0-7)
void OLED_ShowChar(uint8_t x, uint8_t y, char chr);      // 显示单个字符(6x8)
void OLED_ShowString(uint8_t x, uint8_t y, char *str);   // 显示字符串
void OLED_ShowNumber(uint8_t x, uint8_t y, uint32_t num, uint8_t len);  // 显示数字

#endif
