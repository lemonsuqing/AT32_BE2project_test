#ifndef BE2_SOFTWARE_IIC_H
#define BE2_SOFTWARE_IIC_H

#include "at32f402_405.h"
#include "wk_system.h"
#include "wk_gpio.h"
#include "oled.h"
#include <stdint.h>

// BE2 I2C��ַ��MODE0=0��GNDʱ��
#define BE2_I2C_ADDR_WRITE   0x64  // 0x32 << 1��д������
#define BE2_I2C_ADDR_READ    0x65  // 0x32 << 1 | 1����������

// �Ĵ�����ַ
#define BE2_REG_DATA_ENABLE  0x04  // ����ʹ�ܼĴ���
#define BE2_REG_CTRL0_A      0x05  // ���ٶȼƿ��ƼĴ���
#define BE2_REG_ACC_X        0x24  // ���ٶȼ�X�ᣨ4�ֽڣ�С�˸��㣩
#define BE2_REG_ACC_Y        0x28  // ���ٶȼ�Y��
#define BE2_REG_ACC_Z        0x2C  // ���ٶȼ�Z��
#define BE2_REG_GYR_X        0x30  // ������X��
#define BE2_REG_GYR_Y        0x34  // ������Y��
#define BE2_REG_GYR_Z        0x38  // ������Z��
#define BE2_REG_EULER_X      0x58  // ŷ����X������ǣ�
#define BE2_REG_EULER_Y      0x5C  // ŷ����Y�������ǣ�
#define BE2_REG_EULER_Z      0x60  // ŷ����Z��ƫ���ǣ�

// ��������
uint8_t BE2_I2C_Init(void);                     // ��ʼ��BE2
uint8_t BE2_WriteReg(uint8_t reg_addr, uint8_t data);  // д��Ĵ���
uint8_t BE2_ReadReg(uint8_t reg_addr, uint8_t *data, uint8_t len);  // ��ȡ�Ĵ���
uint8_t BE2_EnableSensors(void);                // ʹ�ܴ������������
uint8_t BE2_ReadAccelerometer(float *ax, float *ay, float *az);     // ��ȡ���ٶȼ�
uint8_t BE2_ReadGyroscope(float *gx, float *gy, float *gz);         // ��ȡ������
uint8_t BE2_ReadEulerAngle(float *roll, float *pitch, float *yaw);  // ��ȡŷ����
uint8_t BE2_EnterStreamingMode(void);

#endif
