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
///* BMPͼƬ����
//   ͼƬ��ʽΪ��λ���飬�±�ֱ��ӦͼƬ�Ŀ�͸ߣ�
//   BMP_xx[H/8][L];
//*/
//extern const uint8_t BMP_Picture[64/8][64];
//
//
///* ����������״̬ */
//typedef enum
//{
//    SET_PIXEL = 0x01,
//    RESET_PIXEL = 0x00,
//} PixelStatus;
//
///* I2C���ò���������ʵ��Ӳ���޸ģ� */
//#define OLED_I2C_HANDLE      hi2c2
//#define OLED_I2C_ADDRESS     0x3C  // OLED��I2C��ַ��7λ��ַ��������1λ��
//
///* ���ܺ������� */
//// д���ݣ�I2C�ײ㣩
//void wk_i2c_write_byte(uint8_t addr, uint8_t data);
//// д����
//void WriteCmd(uint8_t IIC_Command);
//// д����
//void WriteDat(uint8_t IIC_Data);
//// ��ʼ��OLED
//void OLED_Init(void);
//// ����OLED�����ѣ�
//void OLED_ON(void);
//// �ر�OLED�����ߣ�
//void OLED_OFF(void);
//// ˢ�»��������ݵ�GDDRAM
//void OLED_RefreshRAM(void);
//// ������ݻ�����
//void OLED_ClearRAM(void);
//// ȫ�����
//void OLED_FullyFill(uint8_t fill_Data);
//// ����
//void OLED_FullyClear(void);
//// �����������ص�����
//void OLED_SetPixel(int16_t x, int16_t y, uint8_t set_pixel);
//// ����������ص�����
//PixelStatus OLED_GetPixel(int16_t x, int16_t y);
//
//// ��ʾӢ���ַ���
//void OLED_ShowStr(int16_t x, int16_t y, uint8_t ch[], uint8_t TextSize);
//// ��ʾ�����ַ���
//void OLED_ShowCN(int16_t x, int16_t y, uint8_t* ch);
//// ��ʾ��Ӣ�Ļ������
//void OLED_ShowMixedCH(int16_t x, int16_t y, uint8_t* ch);
//// ��ʾBMPͼƬ
//void OLED_DrawBMP(int16_t x0, int16_t y0, int16_t L, int16_t H, const uint8_t BMP[]);
//
//// �������
//void OLED_AreaFill(int16_t x0, int16_t y0, int16_t L, int16_t H, uint8_t fill_data);
//// �������
//void OLED_AreaClear(int16_t x0, int16_t y0, int16_t L, int16_t H);
//// ȫ������ȡ��
//void OLED_FullyToggle(void);
//// ��������ȡ��
//void OLED_AreaToggle(int16_t x0, int16_t y0, int16_t L, int16_t H);
//// ȫ����ֱƫ��
//void OLED_VerticalShift(void);
//// ȫ��ˮƽ����
//void OLED_HorizontalShift(uint8_t direction);
//// ȫ����ֱˮƽ����
//void OLED_VerticalAndHorizontalShift(uint8_t direction);
//// ��ʾģʽ�л�������/���ԣ�
//void OLED_DisplayMode(uint8_t mode);
//// ���ȵ���
//void OLED_IntensityControl(uint8_t intensity);

#endif
