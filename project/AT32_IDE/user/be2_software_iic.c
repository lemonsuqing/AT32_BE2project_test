#include "be2_software_iic.h"

/**
 * @brief  ��ʼ��BE2������OLED��I2C���ţ�
 * @retval 0: �ɹ�
 */
uint8_t BE2_I2C_Init(void)
{
    OLED_I2C_Init();  // ����OLED��I2C�������ã���©���+������
    wk_delay_ms(100); // �ȴ��������ϵ��ȶ�
    return 0;
}

/**
 * @brief  ��BE2�Ĵ���д������
 * @param  reg_addr: �Ĵ�����ַ
 * @param  data: Ҫд�������
 * @retval 0: �ɹ���1: �豸��Ӧ��2: �Ĵ�����ַ��Ӧ��
 */
uint8_t BE2_WriteReg(uint8_t reg_addr, uint8_t data)
{
    // ������ʼ�ź�+д��ַ
    OLED_I2C_Start();
    OLED_I2C_SendByte(BE2_I2C_ADDR_WRITE);
    wk_delay_us(2);
    if (OLED_W_SDA_READ() != 0) { OLED_I2C_Stop(); return 1; }

    // ���ͼĴ�����ַ
    OLED_I2C_SendByte(reg_addr);
    wk_delay_us(2);
    if (OLED_W_SDA_READ() != 0) { OLED_I2C_Stop(); return 2; }

    // ��������
    OLED_I2C_SendByte(data);
    wk_delay_us(2);

    // ����ֹͣ�ź�
    OLED_I2C_Stop();
    return 0;
}

/**
 * @brief  ��BE2�Ĵ�����ȡ����
 * @param  reg_addr: �Ĵ�����ַ
 * @param  data: ���ջ�����
 * @param  len: ��ȡ����
 * @retval 0: �ɹ���1: �豸��Ӧ��2: �Ĵ�����ַ��Ӧ��3: ��������Ӧ��
 */
uint8_t BE2_ReadReg(uint8_t reg_addr, uint8_t *data, uint8_t len)
{
    uint8_t i;

    // ������ʼ�ź�+д��ַ
    OLED_I2C_Start();
    OLED_I2C_SendByte(BE2_I2C_ADDR_WRITE);
    wk_delay_us(2);
    if (OLED_W_SDA_READ() != 0) { OLED_I2C_Stop(); return 1; }

    // ���ͼĴ�����ַ
    OLED_I2C_SendByte(reg_addr);
    wk_delay_us(2);
    if (OLED_W_SDA_READ() != 0) { OLED_I2C_Stop(); return 2; }

    // �ظ���ʼ�ź�+����ַ
    OLED_I2C_Start();
    OLED_I2C_SendByte(BE2_I2C_ADDR_READ);
    wk_delay_us(2);
    if (OLED_W_SDA_READ() != 0) { OLED_I2C_Stop(); return 3; }

    // ��ȡ���ݣ����1�ֽڷ��ͷ�Ӧ��
    for (i = 0; i < len; i++)
    {
        data[i] = 0;
        // ��ȡ8λ���ݣ���λ��ǰ��
        for (uint8_t bit = 0; bit < 8; bit++)
        {
            data[i] <<= 1;
            OLED_W_SCL(1);
            wk_delay_us(2);
            if (OLED_W_SDA_READ()) data[i] |= 0x01;
            OLED_W_SCL(0);
            wk_delay_us(2);
        }
        // ����Ӧ�����1�ֽڷ�Ӧ��
        OLED_W_SDA(i == len - 1 ? 1 : 0);
        wk_delay_us(2);
        OLED_W_SCL(1);
        wk_delay_us(2);
        OLED_W_SCL(0);
        OLED_W_SDA(1); // �ͷ�SDA
    }

    OLED_I2C_Stop();
    return 0;
}

/**
 * @brief  ʹ�ܴ������������������DATA_ENABLE��CTRL0_A�Ĵ�����
 * @retval 0: �ɹ�������: ������
 */
uint8_t BE2_EnableSensors(void)
{
    // ʹ�ܼ��ٶȼơ������ǣ�DATA_ENABLE�Ĵ�����Bit5=1, Bit6=1��
    if (BE2_WriteReg(BE2_REG_DATA_ENABLE, 0x60) != 0) return 1;
    // ���ü��ٶȼ����ԭʼ���ݣ�CTRL0_A��Bit4=0��
    if (BE2_WriteReg(BE2_REG_CTRL0_A, 0x00) != 0) return 2;
    wk_delay_ms(50);
    return 0;
}

/**
 * @brief  ��ȡ���ٶȼ����ݣ���λ��g��
 * @retval 0: �ɹ�������: ������
 */
uint8_t BE2_ReadAccelerometer(float *ax, float *ay, float *az)
{
    uint8_t buf[4];
    if (BE2_ReadReg(BE2_REG_ACC_X, buf, 4) != 0) return 1;
    *ax = *(float *)buf;

    if (BE2_ReadReg(BE2_REG_ACC_Y, buf, 4) != 0) return 2;
    *ay = *(float *)buf;

    if (BE2_ReadReg(BE2_REG_ACC_Z, buf, 4) != 0) return 3;
    *az = *(float *)buf;

    return 0;
}

/**
 * @brief  ��ȡ���������ݣ���λ��dps��
 * @retval 0: �ɹ�������: ������
 */
uint8_t BE2_ReadGyroscope(float *gx, float *gy, float *gz)
{
    uint8_t buf[4];
    if (BE2_ReadReg(BE2_REG_GYR_X, buf, 4) != 0) return 1;
    *gx = *(float *)buf;

    if (BE2_ReadReg(BE2_REG_GYR_Y, buf, 4) != 0) return 2;
    *gy = *(float *)buf;

    if (BE2_ReadReg(BE2_REG_GYR_Z, buf, 4) != 0) return 3;
    *gz = *(float *)buf;

    return 0;
}

/**
 * @brief  ��ȡŷ�������ݣ���λ���ȣ�
 * @retval 0: �ɹ�������: ������
 */
uint8_t BE2_ReadEulerAngle(float *roll, float *pitch, float *yaw)
{
    uint8_t buf[4];
    if (BE2_ReadReg(BE2_REG_EULER_X, buf, 4) != 0) return 1;
    *roll = *(float *)buf;

    if (BE2_ReadReg(BE2_REG_EULER_Y, buf, 4) != 0) return 2;
    *pitch = *(float *)buf;

    if (BE2_ReadReg(BE2_REG_EULER_Z, buf, 4) != 0) return 3;
    *yaw = *(float *)buf;

    return 0;
}
