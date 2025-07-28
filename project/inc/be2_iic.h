#ifndef __BE2_IIC_H
#define __BE2_IIC_H

#include "at32f402_405_wk_config.h"
#include <stdint.h>
#include "at32f402_405_i2c.h"  // ������APIͷ�ļ�

// I2C���豸��ַ��ADD0=0ʱ����Ӧ�ֲ���I2C���豸��ַ���ã�
#define BE2_I2C_ADDR           0x32   // �̶�ΪADD0=0ģʽ�ĵ�ַ{insert\_element\_0\_}

// �Ĵ�����ַ���壨֮ͬǰ�汾��������Ӳ���ֲ�һ�£�
#define REG_STATUS             0x00    // ״̬�Ĵ���
#define REG_FUN_CONFIG         0x01    // �������üĴ���
#define REG_SYS_CONFIG         0x02    // ϵͳ���üĴ���
#define REG_DATA_CTRL          0x03    // ���ݿ��ƼĴ���
#define REG_DATA_ENABLE        0x04    // ����ʹ�ܼĴ���
#define REG_CTRL_0_A           0x05    // ���ٶȼƿ��ƼĴ���
#define REG_CTRL_1_G           0x06    // �����ǿ��ƼĴ���
#define REG_FILTER_CONFIG      0x07    // �˲����üĴ���

// ���ݼĴ�����ַ��֮ͬǰ�汾��
#define REG_TIMESTAMP_0        0x20    // ʱ���
#define REG_ACC_X_0            0x24    // ���ٶ�X��
#define REG_ACC_Y_0            0x28    // ���ٶ�Y��
#define REG_ACC_Z_0            0x2C    // ���ٶ�Z��
#define REG_GYR_X_0            0x30    // ������X��
#define REG_GYR_Y_0            0x34    // ������Y��
#define REG_GYR_Z_0            0x38    // ������Z��
#define REG_QUAT_W_0           0x48    // ��Ԫ��W
#define REG_QUAT_X_0           0x4C    // ��Ԫ��X
#define REG_QUAT_Y_0           0x50    // ��Ԫ��Y
#define REG_QUAT_Z_0           0x54    // ��Ԫ��Z
#define REG_EULER_X_0          0x58    // ŷ����X(roll)
#define REG_EULER_Y_0          0x5C    // ŷ����Y(pitch)
#define REG_EULER_Z_0          0x60    // ŷ����Z(yaw)
#define REG_LIN_ACC_X_0        0x64    // ���Լ��ٶ�X
#define REG_LIN_ACC_Y_0        0x68    // ���Լ��ٶ�Y
#define REG_LIN_ACC_Z_0        0x6C    // ���Լ��ٶ�Z
#define REG_TEMP_0             0x70    // �¶�
#define REG_WHO_AM_I           0x74    // �豸ID

// ���������ݽṹ
typedef struct {
    uint32_t timestamp;               // ʱ���
    float acc_x, acc_y, acc_z;        // ���ٶ�(g)
    float gyr_x, gyr_y, gyr_z;        // ���ٶ�(dps)
    float quat_w, quat_x, quat_y, quat_z;  // ��Ԫ��
    float euler_roll, euler_pitch, euler_yaw;  // ŷ����(��)
    float lin_acc_x, lin_acc_y, lin_acc_z;  // ���Լ��ٶ�(g)
    float temp;                       // �¶�(��)
} BE2_Data;

// ��������
void BE2_I2C_Init(void);
uint8_t BE2_I2C_WriteReg(uint8_t reg, uint8_t data);
uint8_t BE2_I2C_ReadReg(uint8_t reg, uint8_t *data);
uint8_t BE2_I2C_ReadMultiReg(uint8_t reg, uint8_t *buf, uint16_t len);
uint8_t BE2_Init(void);
uint8_t BE2_ReadAllData(BE2_Data *data);
void BE2_PrintData(BE2_Data *data);

#endif
