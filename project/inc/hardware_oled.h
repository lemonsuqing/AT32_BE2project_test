#ifndef HARDWARE_OLED_H__
#define HARDWARE_OLED_H__

#include "at32f402_405.h"
#include "i2c_application.h"
#include "wk_system.h"
#include "wk_gpio.h"
#include "wk_i2c.h"
#include "Serial.h"
#include <stddef.h>
#include <stdint.h>
//
///* BMP图片声明
//   图片格式为二位数组，下标分别对应图片的宽和高：
//   BMP_xx[H/8][L];
//*/
//extern const uint8_t BMP_Picture[64/8][64];
//
//
///* 设置坐标点的状态 */
//typedef enum
//{
//    SET_PIXEL = 0x01,
//    RESET_PIXEL = 0x00,
//} PixelStatus;
//
///* I2C配置参数（根据实际硬件修改） */
//#define OLED_I2C_HANDLE      hi2c2
//#define OLED_I2C_ADDRESS     0x3C  // OLED的I2C地址（7位地址，已左移1位）
//
///* 功能函数声明 */
//// 写数据（I2C底层）
//void wk_i2c_write_byte(uint8_t addr, uint8_t data);
//// 写命令
//void WriteCmd(uint8_t IIC_Command);
//// 写数据
//void WriteDat(uint8_t IIC_Data);
//// 初始化OLED
//void OLED_Init(void);
//// 开启OLED（唤醒）
//void OLED_ON(void);
//// 关闭OLED（休眠）
//void OLED_OFF(void);
//// 刷新缓冲区数据到GDDRAM
//void OLED_RefreshRAM(void);
//// 清除数据缓冲区
//void OLED_ClearRAM(void);
//// 全屏填充
//void OLED_FullyFill(uint8_t fill_Data);
//// 清屏
//void OLED_FullyClear(void);
//// 设置坐标像素点数据
//void OLED_SetPixel(int16_t x, int16_t y, uint8_t set_pixel);
//// 获得坐标像素点数据
//PixelStatus OLED_GetPixel(int16_t x, int16_t y);
//
//// 显示英文字符串
//void OLED_ShowStr(int16_t x, int16_t y, uint8_t ch[], uint8_t TextSize);
//// 显示中文字符串
//void OLED_ShowCN(int16_t x, int16_t y, uint8_t* ch);
//// 显示中英文混合文字
//void OLED_ShowMixedCH(int16_t x, int16_t y, uint8_t* ch);
//// 显示BMP图片
//void OLED_DrawBMP(int16_t x0, int16_t y0, int16_t L, int16_t H, const uint8_t BMP[]);
//
//// 区域填充
//void OLED_AreaFill(int16_t x0, int16_t y0, int16_t L, int16_t H, uint8_t fill_data);
//// 区域清除
//void OLED_AreaClear(int16_t x0, int16_t y0, int16_t L, int16_t H);
//// 全屏内容取反
//void OLED_FullyToggle(void);
//// 区域内容取反
//void OLED_AreaToggle(int16_t x0, int16_t y0, int16_t L, int16_t H);
//// 全屏垂直偏移
//void OLED_VerticalShift(void);
//// 全屏水平滚动
//void OLED_HorizontalShift(uint8_t direction);
//// 全屏垂直水平滚动
//void OLED_VerticalAndHorizontalShift(uint8_t direction);
//// 显示模式切换（正常/反显）
//void OLED_DisplayMode(uint8_t mode);
//// 亮度调节
//void OLED_IntensityControl(uint8_t intensity);

#endif
