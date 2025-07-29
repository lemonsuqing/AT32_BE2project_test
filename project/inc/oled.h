#ifndef __OLED_H
#define __OLED_H

#include "at32f402_405_wk_config.h"
#include "i2c_application.h"
#include "wk_system.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

// OLED�豸I2C��ַ������Ӳ������ѡ��
#define OLED_I2C_ADDR         0x3C
// ��Ļ�ߴ����
#define OLED_WIDTH            128
#define OLED_HEIGHT           64
#define OLED_PAGE_COUNT       8   // 64�� / 8��ÿҳ = 8ҳ

// ����ģʽ����
#define OLED_CMD_MODE         0x00  // ����ģʽ
#define OLED_DATA_MODE        0x40  // ����ģʽ

// ��������״̬
typedef enum {
    OLED_OK = 0,
    OLED_ERR_I2C,
    OLED_ERR_PARAM,
    OLED_ERR_MEM
} oled_status_type;

// ȫ��I2C����������ⲿ��ʼ����
extern i2c_handle_type hi2c;

// ��ʼ������
oled_status_type OLED_Init(uint32_t timeout);

// ��Ļ��������
oled_status_type OLED_Clear(uint32_t timeout);
oled_status_type OLED_DisplayOn(uint32_t timeout);
oled_status_type OLED_DisplayOff(uint32_t timeout);
oled_status_type OLED_SetContrast(uint8_t contrast, uint32_t timeout);

// ��ͼ����
oled_status_type OLED_SetPos(uint8_t x, uint8_t page, uint32_t timeout);
oled_status_type OLED_DrawPixel(uint8_t x, uint8_t y, uint32_t timeout);
oled_status_type OLED_DrawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint32_t timeout);

// �ַ���ʾ����
oled_status_type OLED_ShowChar(uint8_t x, uint8_t page, char c, uint32_t timeout);
oled_status_type OLED_ShowString(uint8_t x, uint8_t page, const char *str, uint32_t timeout);
oled_status_type OLED_ShowNumber(uint8_t x, uint8_t page, uint32_t num, uint8_t len, uint32_t timeout);

#endif /* __OLED_H */
