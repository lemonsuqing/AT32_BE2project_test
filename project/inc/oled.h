#ifndef __OLED_H
#define __OLED_H

#include "at32f402_405_wk_config.h"
#include "i2c_application.h"
#include "wk_system.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

// OLED设备I2C地址（根据硬件接线选择）
#define OLED_I2C_ADDR         0x3C
// 屏幕尺寸参数
#define OLED_WIDTH            128
#define OLED_HEIGHT           64
#define OLED_PAGE_COUNT       8   // 64行 / 8行每页 = 8页

// 控制模式定义
#define OLED_CMD_MODE         0x00  // 命令模式
#define OLED_DATA_MODE        0x40  // 数据模式

// 函数返回状态
typedef enum {
    OLED_OK = 0,
    OLED_ERR_I2C,
    OLED_ERR_PARAM,
    OLED_ERR_MEM
} oled_status_type;

// 全局I2C句柄（需在外部初始化）
extern i2c_handle_type hi2c;

// 初始化函数
oled_status_type OLED_Init(uint32_t timeout);

// 屏幕操作函数
oled_status_type OLED_Clear(uint32_t timeout);
oled_status_type OLED_DisplayOn(uint32_t timeout);
oled_status_type OLED_DisplayOff(uint32_t timeout);
oled_status_type OLED_SetContrast(uint8_t contrast, uint32_t timeout);

// 绘图函数
oled_status_type OLED_SetPos(uint8_t x, uint8_t page, uint32_t timeout);
oled_status_type OLED_DrawPixel(uint8_t x, uint8_t y, uint32_t timeout);
oled_status_type OLED_DrawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint32_t timeout);

// 字符显示函数
oled_status_type OLED_ShowChar(uint8_t x, uint8_t page, char c, uint32_t timeout);
oled_status_type OLED_ShowString(uint8_t x, uint8_t page, const char *str, uint32_t timeout);
oled_status_type OLED_ShowNumber(uint8_t x, uint8_t page, uint32_t num, uint8_t len, uint32_t timeout);

#endif /* __OLED_H */
