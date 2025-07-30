#ifndef BE2_SOFTWARE_IIC_H
#define BE2_SOFTWARE_IIC_H

#include "at32f402_405.h"
#include "i2c_application.h"
#include "wk_system.h"
#include "wk_gpio.h"
#include "oled.h"
#include <stddef.h>
#include <stdint.h>

// BE2 I2C��ַ���ο�20230707Ӳ���ֲ��3-1��MODE0=0ʱΪ0x32��
#define BE2_I2C_ADDR_WRITE   0x64  // 0x32 << 1��д������
#define BE2_I2C_ADDR_READ    0x65  // 0x32 << 1 | 1����������

// �Ĵ�����ַ���ο�20230707Ӳ���ֲḽ¼�Ĵ���ӳ�䣩
#define BE2_REG_ACC_X        0x24  // ���ٶȼ�X�ᣨ4�ֽڣ�С�˸��㣩
#define BE2_REG_ACC_Y        0x28  // ���ٶȼ�Y��
#define BE2_REG_ACC_Z        0x2C  // ���ٶȼ�Z��
#define BE2_REG_GYR_X        0x30  // ������X�ᣨ4�ֽڣ�С�˸��㣩
#define BE2_REG_GYR_Y        0x34  // ������Y��
#define BE2_REG_GYR_Z        0x38  // ������Z��
#define BE2_REG_EULER_X      0x58  // ŷ����X������ǣ�4�ֽڸ��㣩
#define BE2_REG_EULER_Y      0x5C  // ŷ����Y�������ǣ�
#define BE2_REG_EULER_Z      0x60  // ŷ����Z��ƫ���ǣ�

// ��������
uint8_t BE2_I2C_Init(void);  // ��ʼ��BE2������OLED��I2C�������ã�
uint8_t BE2_ReadReg(uint8_t reg_addr, uint8_t *data, uint8_t len);  // ��ȡ�Ĵ���
uint8_t BE2_ReadAccelerometer(float *ax, float *ay, float *az);     // ��ȡ���ٶȼƣ���λ��g���ο�20220802�û��ֲ�2.2.1��
uint8_t BE2_ReadGyroscope(float *gx, float *gy, float *gz);         // ��ȡ�����ǣ���λ��dps���ο�20220802�û��ֲ�2.2.2��
uint8_t BE2_ReadEulerAngle(float *roll, float *pitch, float *yaw);  // ��ȡŷ���ǣ���λ���ȣ��ο�20220802�û��ֲ�2.3.2��


#endif
