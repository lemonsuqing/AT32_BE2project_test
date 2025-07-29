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

// OLED���ò���
#define OLED_WIDTH       128     // ��Ļ���
#define OLED_HEIGHT      64      // ��Ļ�߶�
#define OLED_PAGE_COUNT  8       // ҳ����(64/8)
#define OLED_I2C_ADDR    0x3C    // OLED I2C��ַ(7λ������ģ��ʵ���޸ģ�����0x3C��0x3D)
#define OLED_I2C         I2C2    // ʹ�õ�I2C����

// ����/��������
#define OLED_CMD         0x00    // д����(Co=0, D/C#=0)
#define OLED_DATA        0x40    // д����(Co=0, D/C#=1)

// ��������
void OLED_Init(void);                    // ��ʼ��OLED
void OLED_Clear(void);                   // ����
void OLED_DisplayOn(void);               // ������ʾ
void OLED_DisplayOff(void);              // �ر���ʾ
void OLED_SetPos(uint8_t x, uint8_t y);  // ���ù��λ��(x:0-127, y:0-7)
void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t c);  // ��ʾ�����ַ�(6x8)
void OLED_ShowString(uint8_t x, uint8_t y, uint8_t *str);  // ��ʾ�ַ���
void OLED_ShowNumber(uint8_t x, uint8_t y, uint32_t num, uint8_t len);  // ��ʾ����

#endif
