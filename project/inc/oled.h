#ifndef __OLED_H
#define __OLED_H

#include "at32f402_405.h"
#include "i2c_application.h"
#include <stdint.h>

// OLED���ò���
#define OLED_WIDTH       128     // ��Ļ���
#define OLED_HEIGHT      64      // ��Ļ�߶�
#define OLED_PAGE_COUNT  8       // ҳ����(64/8)
#define OLED_I2C_ADDR    0x3C    // OLED I2C��ַ(7λ��ַ)
#define OLED_TIMEOUT     1000    // I2C������ʱʱ��(ms)

// ����/��������
#define OLED_CMD         0x00    // д����
#define OLED_DATA        0x40    // д����

// ��������
void OLED_Init(i2c_handle_type *hi2c);                    // ��ʼ��OLED
void OLED_Clear(void);                                   // ����
void OLED_DisplayOn(void);                               // ������ʾ
void OLED_DisplayOff(void);                              // �ر���ʾ
void OLED_SetPos(uint8_t x, uint8_t y);                  // ���ù��λ��(x:0-127, y:0-7)
void OLED_ShowChar(uint8_t x, uint8_t y, char chr);      // ��ʾ�����ַ�(6x8)
void OLED_ShowString(uint8_t x, uint8_t y, char *str);   // ��ʾ�ַ���
void OLED_ShowNumber(uint8_t x, uint8_t y, uint32_t num, uint8_t len);  // ��ʾ����

#endif
