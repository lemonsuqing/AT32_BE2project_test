#ifndef BE2_SOFTWARE_IIC_H
#define BE2_SOFTWARE_IIC_H

#include "at32f402_405.h"
#include "i2c_application.h"
#include "wk_system.h"
#include "wk_gpio.h"
#include <stddef.h>
#include <stdint.h>

// ���Ŷ��壨�ɸ���ʵ��Ӳ���޸ģ�
#define BE2_IIC_SCL_PORT    GPIOB
#define BE2_IIC_SDA_PORT    GPIOB
#define BE2_IIC_SCL_PIN     GPIO_PINS_8
#define BE2_IIC_SDA_PIN     GPIO_PINS_9

// I2C��ַ������MODE0�������ã�0x32��0x33��
#define BE2_IIC_ADDR        0x33 << 1  // ����1λ������дλ

// �Ĵ�����ַ���ο�BE2Ӳ���ֲḽ¼��
#define BE2_REG_ACC_X       0x24    // ���ٶȼ�X�����ݣ�4�ֽڣ�
#define BE2_REG_GYR_X       0x30    // ������X�����ݣ�4�ֽڣ�
#define BE2_REG_QUAT_W      0x48    // ��Ԫ��W��4�ֽڣ�
#define BE2_REG_EULER_X     0x58    // ŷ����X��4�ֽڣ�

// ��������
void BE2_IIC_Init(void);
uint8_t BE2_IIC_ReadReg(uint8_t reg_addr, uint8_t *data, uint16_t len);
uint8_t BE2_ReadAccelerometer(float *ax, float *ay, float *az);
uint8_t BE2_ReadGyroscope(float *gx, float *gy, float *gz);
uint8_t BE2_ReadEulerAngle(float *roll, float *pitch, float *yaw);

void IIC_Start(void);
void IIC_Stop(void);
void IIC_SendAck(uint8_t ack);
uint8_t IIC_WaitAck(void);
void IIC_SendByte(uint8_t data);
uint8_t IIC_ReadByte(uint8_t ack);

#endif
