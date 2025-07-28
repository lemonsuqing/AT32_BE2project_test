#ifndef __BE2_SPI_H__
#define __BE2_SPI_H__

#include <stdint.h>
#include <stdbool.h>
#include "at32f402_405_wk_config.h"

// �Ĵ�����ַ���� (Ӳ���ֲḽ¼)
#define BE2_REG_STATUS             0x00
#define BE2_REG_FUN_CONFIG         0x01
#define BE2_REG_SYS_CONFIG         0x02
#define BE2_REG_DATA_CTRL          0x03
#define BE2_REG_DATA_ENABLE        0x04
#define BE2_REG_CTRL_0_A           0x05
#define BE2_REG_CTRL_1_G           0x06
#define BE2_REG_FILTER_CONFIG      0x07
#define BE2_REG_OFFSET_SETTING     0x08

#define BE2_REG_TIMESTAMP          0x20
#define BE2_REG_ACC_X              0x24
#define BE2_REG_ACC_Y              0x28
#define BE2_REG_ACC_Z              0x2C
#define BE2_REG_GYR_X              0x30
#define BE2_REG_GYR_Y              0x34
#define BE2_REG_GYR_Z              0x38
#define BE2_REG_QUAT_W             0x48
#define BE2_REG_QUAT_X             0x4C
#define BE2_REG_QUAT_Y             0x50
#define BE2_REG_QUAT_Z             0x54
#define BE2_REG_EULER_X            0x58
#define BE2_REG_EULER_Y            0x5C
#define BE2_REG_EULER_Z            0x60
#define BE2_REG_WHO_AM_I           0x74
#define BE2_REG_FIRMWARE_VERSION_0   0x75
#define BE2_REG_FIRMWARE_VERSION_1   0x76

// LP-BUSЭ�鶨��
#define LPBUS_HEADER 0x3A  // ֡ͷ��ʶ

// ��������
#define CMD_ENTER_COMMAND_MODE 0x01  // ��������ģʽ
#define CMD_READ_EULER         0x02  // ��ȡŷ����

// �Ĵ������壨��������Ҫ�ģ�
#define BE2_REG_SYS_CONFIG 0x02  // ϵͳ���üĴ���

// ���������ݽṹ
typedef struct {
    float timestamp;         // ʱ��� (��)
    float acc[3];            // ���ٶ� (g)
    float gyro[3];           // ������ (dps)
    float quaternion[4];     // ��Ԫ�� (w, x, y, z)
    float euler[3];          // ŷ���� (��)
} BE2_Data;

// ��������
void be2_spi_init(void);
void be2_read_data(BE2_Data *data);
uint8_t be2_read_register(uint8_t reg);
void be2_write_register(uint8_t reg, uint8_t value);
uint32_t be2_read_32bit(uint8_t reg);
float be2_read_float(uint8_t reg);
bool be2_enter_command_mode(void);
bool be2_read_euler(BE2_Data *data);
uint8_t be2_transfer_byte(uint8_t data);
uint8_t calculate_crc(uint8_t *data, uint8_t len);


#endif
