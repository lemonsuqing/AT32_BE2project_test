#include "be2_iic.h"
#include "wk_i2c.h"
#include "Serial.h"
#include "at32f402_405_i2c.h"  // ������APIͷ�ļ�
#include <string.h>
#include <math.h>

// I2C��ʱʱ�䣨ms��
#define I2C_TIMEOUT_MS         100

/**
 * @brief  ��ʼ��BE2������I2C����
 * @param  ��
 * @retval ��
 */
void BE2_I2C_Init(void) {
    Serial_Printf("BE2 I2C Mode (ADD0=0) Init\r\n");
}

/**
 * @brief  ��BE2�Ĵ���д��һ���ֽ�
 * @param  reg: �Ĵ�����ַ
 * @param  data: Ҫд�������
 * @retval 0:�ɹ� 1:ʧ��
 */
uint8_t BE2_I2C_WriteReg(uint8_t reg, uint8_t data) {
    uint32_t timeout = I2C_TIMEOUT_MS * 1000;

    // �ȴ����߿���
    while(i2c_flag_get(I2C2, I2C_BUSYF_FLAG) && timeout--);
    if(timeout == 0) return 1;

    // ������ʼ�ź�
    i2c_start_generate(I2C2);
    timeout = I2C_TIMEOUT_MS * 1000;
    while(!i2c_flag_get(I2C2, I2C_TDIS_FLAG) && timeout--);  // �ȴ���ʼ�źŷ������
    if(timeout == 0) return 1;

    // ���ôӻ���ַ�ʹ��䷽��д��
    i2c_transfer_addr_set(I2C2, BE2_I2C_ADDR >> 1);  // 7λ��ַ������1λ
    i2c_transfer_dir_set(I2C2, I2C_DIR_TRANSMIT);

    // ���ͼĴ�����ַ
    i2c_data_send(I2C2, reg);
    timeout = I2C_TIMEOUT_MS * 1000;
    while(!i2c_flag_get(I2C2, I2C_TDBE_FLAG) && timeout--);  // �ȴ����ͻ�������
    if(timeout == 0) return 1;

    // ��������
    i2c_data_send(I2C2, data);
    timeout = I2C_TIMEOUT_MS * 1000;
    while(!i2c_flag_get(I2C2, I2C_TDBE_FLAG) && timeout--);
    if(timeout == 0) return 1;

    // ����ֹͣ�ź�
    i2c_stop_generate(I2C2);
    timeout = I2C_TIMEOUT_MS * 1000;
    while(!i2c_flag_get(I2C2, I2C_STOPF_FLAG) && timeout--);
    if(timeout == 0) return 1;
    i2c_flag_clear(I2C2, I2C_STOPF_FLAG);  // ���ֹͣ��־

    return 0;
}

/**
 * @brief  ��BE2�Ĵ�����ȡһ���ֽ�
 * @param  reg: �Ĵ�����ַ
 * @param  data: ��ȡ��������
 * @retval 0:�ɹ� 1:ʧ��
 */
uint8_t BE2_I2C_ReadReg(uint8_t reg, uint8_t *data) {
    uint32_t timeout = I2C_TIMEOUT_MS * 1000;

    // ��д��Ҫ��ȡ�ļĴ�����ַ
    if(BE2_I2C_WriteReg(reg, 0xFF) != 0)
        return 1;

    // �ȴ����߿���
    while(i2c_flag_get(I2C2, I2C_BUSYF_FLAG) && timeout--);
    if(timeout == 0) return 1;

    // ������ʼ�źţ���ģʽ��
    i2c_start_generate(I2C2);
    timeout = I2C_TIMEOUT_MS * 1000;
    while(!i2c_flag_get(I2C2, I2C_TDIS_FLAG) && timeout--);
    if(timeout == 0) return 1;

    // ���ôӻ���ַ�ʹ��䷽�򣨶���
    i2c_transfer_addr_set(I2C2, BE2_I2C_ADDR >> 1);  // 7λ��ַ
    i2c_transfer_dir_set(I2C2, I2C_DIR_RECEIVE);

    // ����ACK�����ֽڶ�ȡ��
    i2c_ack_enable(I2C2, FALSE);

    // �ȴ����ջ������ǿ�
    timeout = I2C_TIMEOUT_MS * 1000;
    while(!i2c_flag_get(I2C2, I2C_RDBF_FLAG) && timeout--);
    if(timeout == 0) return 1;
    *data = i2c_data_receive(I2C2);

    // ����ֹͣ�ź�
    i2c_stop_generate(I2C2);
    timeout = I2C_TIMEOUT_MS * 1000;
    while(!i2c_flag_get(I2C2, I2C_STOPF_FLAG) && timeout--);
    if(timeout == 0) return 1;
    i2c_flag_clear(I2C2, I2C_STOPF_FLAG);

    // �ָ�ACKʹ��
    i2c_ack_enable(I2C2, TRUE);

    return 0;
}

/**
 * @brief  ��BE2������ȡ����Ĵ���
 * @param  reg: ��ʼ�Ĵ�����ַ
 * @param  buf: ���ջ�����
 * @param  len: Ҫ��ȡ�ĳ���
 * @retval 0:�ɹ� 1:ʧ��
 */
uint8_t BE2_I2C_ReadMultiReg(uint8_t reg, uint8_t *buf, uint16_t len) {
    uint32_t timeout = I2C_TIMEOUT_MS * 1000;
    uint16_t i;

    // ��д����ʼ�Ĵ�����ַ
    if(BE2_I2C_WriteReg(reg, 0xFF) != 0)
        return 1;

    // �ȴ����߿���
    while(i2c_flag_get(I2C2, I2C_BUSYF_FLAG) && timeout--);
    if(timeout == 0) return 1;

    // ������ʼ�źţ���ģʽ��
    i2c_start_generate(I2C2);
    timeout = I2C_TIMEOUT_MS * 1000;
    while(!i2c_flag_get(I2C2, I2C_TDIS_FLAG) && timeout--);
    if(timeout == 0) return 1;

    // ���ôӻ���ַ�ʹ��䷽�򣨶���
    i2c_transfer_addr_set(I2C2, BE2_I2C_ADDR >> 1);
    i2c_transfer_dir_set(I2C2, I2C_DIR_RECEIVE);

    // ��ȡ����
    for(i = 0; i < len; i++) {
        // ���һ���ֽ�ǰ����ACK
        if(i == len - 1)
            i2c_ack_enable(I2C2, FALSE);

        // �ȴ����ջ������ǿ�
        timeout = I2C_TIMEOUT_MS * 1000;
        while(!i2c_flag_get(I2C2, I2C_RDBF_FLAG) && timeout--);
        if(timeout == 0) return 1;
        buf[i] = i2c_data_receive(I2C2);
    }

    // ����ֹͣ�ź�
    i2c_stop_generate(I2C2);
    timeout = I2C_TIMEOUT_MS * 1000;
    while(!i2c_flag_get(I2C2, I2C_STOPF_FLAG) && timeout--);
    if(timeout == 0) return 1;
    i2c_flag_clear(I2C2, I2C_STOPF_FLAG);

    // �ָ�ACKʹ��
    i2c_ack_enable(I2C2, TRUE);

    return 0;
}

/**
 * @brief  ��ʼ��BE2������
 * @param  ��
 * @retval 0:�ɹ� 1:ʧ��
 */
uint8_t BE2_Init(void) {
    uint8_t whoami;
    uint8_t ret;

    // ��ȡ�豸ID��WHO_AM_IĬ��0x32��
    ret = BE2_I2C_ReadReg(REG_WHO_AM_I, &whoami);
    if(ret != 0 || whoami != 0x32) {
        Serial_Printf("BE2 Init Failed! WHO_AM_I=0x%02X\r\n", whoami);
        return 1;
    }

    // ʹ�ܼĴ����޸ģ�FUN_CONFIG Bit7=1��
    ret |= BE2_I2C_WriteReg(REG_FUN_CONFIG, 0x80);

    // �����������ʹ��
    ret |= BE2_I2C_WriteReg(REG_DATA_ENABLE, 0x37);  // ʹ�ܼ��ٶȡ������ǵ�

    // �������Ƶ��Ϊ100Hz
    ret |= BE2_I2C_WriteReg(REG_DATA_CTRL, 0x03);

    if(ret == 0)
        Serial_Printf("BE2 Init Success (I2C ADD0=0)\r\n");
    else
        Serial_Printf("BE2 Config Error\r\n");

    return ret;
}

/**
 * @brief  �ֽ�ת��������С�˸�ʽ��
 * @param  bytes: 4�ֽ�����
 * @retval ת����ĸ�����
 */
float BytesToFloat(uint8_t *bytes) {
    union {
        uint8_t b[4];
        float f;
    } data;
    data.b[0] = bytes[0];
    data.b[1] = bytes[1];
    data.b[2] = bytes[2];
    data.b[3] = bytes[3];
    return data.f;
}

/**
 * @brief  ��ȡBE2���д���������
 * @param  data: �洢���ݵĽṹ��ָ��
 * @retval 0:�ɹ� 1:ʧ��
 */
uint8_t BE2_ReadAllData(BE2_Data *data) {
    uint8_t buf[4];
    uint8_t ret = 0;

    // ��ȡʱ���
    ret |= BE2_I2C_ReadMultiReg(REG_TIMESTAMP_0, buf, 4);
    data->timestamp = (buf[3] << 24) | (buf[2] << 16) | (buf[1] << 8) | buf[0];

    // ��ȡ���ٶ�
    ret |= BE2_I2C_ReadMultiReg(REG_ACC_X_0, buf, 4);
    data->acc_x = BytesToFloat(buf);
    ret |= BE2_I2C_ReadMultiReg(REG_ACC_Y_0, buf, 4);
    data->acc_y = BytesToFloat(buf);
    ret |= BE2_I2C_ReadMultiReg(REG_ACC_Z_0, buf, 4);
    data->acc_z = BytesToFloat(buf);

    // ��ȡ������
    ret |= BE2_I2C_ReadMultiReg(REG_GYR_X_0, buf, 4);
    data->gyr_x = BytesToFloat(buf);
    ret |= BE2_I2C_ReadMultiReg(REG_GYR_Y_0, buf, 4);
    data->gyr_y = BytesToFloat(buf);
    ret |= BE2_I2C_ReadMultiReg(REG_GYR_Z_0, buf, 4);
    data->gyr_z = BytesToFloat(buf);

    // ��ȡ��Ԫ��
    ret |= BE2_I2C_ReadMultiReg(REG_QUAT_W_0, buf, 4);
    data->quat_w = BytesToFloat(buf);
    ret |= BE2_I2C_ReadMultiReg(REG_QUAT_X_0, buf, 4);
    data->quat_x = BytesToFloat(buf);
    ret |= BE2_I2C_ReadMultiReg(REG_QUAT_Y_0, buf, 4);
    data->quat_y = BytesToFloat(buf);
    ret |= BE2_I2C_ReadMultiReg(REG_QUAT_Z_0, buf, 4);
    data->quat_z = BytesToFloat(buf);

    // ��ȡŷ����
    ret |= BE2_I2C_ReadMultiReg(REG_EULER_X_0, buf, 4);
    data->euler_roll = BytesToFloat(buf);
    ret |= BE2_I2C_ReadMultiReg(REG_EULER_Y_0, buf, 4);
    data->euler_pitch = BytesToFloat(buf);
    ret |= BE2_I2C_ReadMultiReg(REG_EULER_Z_0, buf, 4);
    data->euler_yaw = BytesToFloat(buf);

    // ��ȡ���Լ��ٶ�
    ret |= BE2_I2C_ReadMultiReg(REG_LIN_ACC_X_0, buf, 4);
    data->lin_acc_x = BytesToFloat(buf);
    ret |= BE2_I2C_ReadMultiReg(REG_LIN_ACC_Y_0, buf, 4);
    data->lin_acc_y = BytesToFloat(buf);
    ret |= BE2_I2C_ReadMultiReg(REG_LIN_ACC_Z_0, buf, 4);
    data->lin_acc_z = BytesToFloat(buf);

    // ��ȡ�¶�
    ret |= BE2_I2C_ReadMultiReg(REG_TEMP_0, buf, 4);
    data->temp = BytesToFloat(buf);

    return ret;
}

/**
 * @brief  ��ӡ���������ݵ�����
 * @param  data: Ҫ��ӡ������
 * @retval ��
 */
void BE2_PrintData(BE2_Data *data) {
    Serial_Printf("Timestamp: %u\r\n", data->timestamp);
    Serial_Printf("Acc: %.3f, %.3f, %.3f g\r\n",
                 data->acc_x, data->acc_y, data->acc_z);
    Serial_Printf("Gyr: %.3f, %.3f, %.3f dps\r\n",
                 data->gyr_x, data->gyr_y, data->gyr_z);
    Serial_Printf("Euler: Roll=%.2f, Pitch=%.2f, Yaw=%.2f ��\r\n",
                 data->euler_roll, data->euler_pitch, data->euler_yaw);
    Serial_Printf("Quat: %.4f, %.4f, %.4f, %.4f\r\n",
                 data->quat_w, data->quat_x, data->quat_y, data->quat_z);
    Serial_Printf("LinAcc: %.3f, %.3f, %.3f g\r\n",
                 data->lin_acc_x, data->lin_acc_y, data->lin_acc_z);
    Serial_Printf("Temp: %.2f ��C\r\n", data->temp);
    Serial_Printf("----------------------------------------\r\n");
}
