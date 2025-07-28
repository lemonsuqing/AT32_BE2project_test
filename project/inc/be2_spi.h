#ifndef __BE2_SPI_H__
#define __BE2_SPI_H__

#include <stdint.h>
#include <stdbool.h>
#include "at32f402_405_wk_config.h"

// ���������ݽṹ�嶨�壨��Ӧ20220802lpmsBE2�û��ֲ��б�2-2��32λ���������ݸ�ʽ��
typedef struct {
    uint32_t timestamp;           // ʱ�����500Hz�����ʣ�
    float acc_raw[3];             // ԭʼ���ٶȼ�����(g)
    float acc_calibrated[3];      // У׼����ٶȼ�����(g)
    float gyro_raw[3];            // ԭʼ����������(dps)
    float gyro_bias_calibrated[3];// ��ֹƫ��У׼������������(dps)
    float gyro_alignment[3];      // ������У׼������������(dps)
    float quaternion[4];          // ��Ԫ��(w,x,y,z����һ����λ)
    float euler[3];               // ŷ����(roll,pitch,yaw����)
} BE2_SensorData;

// SPIƬѡ���Ŷ��壨�ο�20230707lpmsBE2Ӳ���ֲ���2.2�ڹ������ţ�
#define LPMS_CS_PORT   GPIOC
#define LPMS_CS_PIN    GPIO_PINS_3
#define LPMS_CS_LOW()  gpio_bits_reset(LPMS_CS_PORT, LPMS_CS_PIN)
#define LPMS_CS_HIGH() gpio_bits_set(LPMS_CS_PORT, LPMS_CS_PIN)

// ��������
void be2_spi_init_cs_gpio(void);                  // ��ʼ��Ƭѡ����
bool be2_enter_command_mode(void);                // ��������ģʽ��20220802�û��ֲ�3.5�ڣ�
uint8_t be2_read_whoami_lpb(void);                // ��ȡWHO_AM_I�Ĵ�����20230707Ӳ���ֲ�6.1�ڣ�
bool be2_read_sensor_data(BE2_SensorData *data);  // ��ȡ���������ݣ�20220802�û��ֲ�3.3�ڣ�
bool be2_sensor_init(void);                       // ��������ʼ��
uint8_t be2_spi_read_reg(uint8_t addr);

#endif
