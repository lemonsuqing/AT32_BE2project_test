#include "be2_software_iic.h"
#include "wk_system.h"  // ������ʱ����

/**
 * @brief  ��ʼ��BE2������OLED��I2C�������ã���©���+����������20230707Ӳ���ֲ��2-3��ƽҪ��
 * @retval 0: �ɹ�
 */
uint8_t BE2_I2C_Init(void)
{
    OLED_I2C_Init();  // ����OLED��I2C���ų�ʼ��������֤���ݣ�
    wk_delay_ms(100); // �ȴ��������ϵ��ȶ����ο�20220802�û��ֲ��ԴҪ��
    return 0;
}

/**
 * @brief  ��ȡBE2�Ĵ������ݣ���ѭI2Cͨ�Ź淶���ο�20230707Ӳ���ֲ�3.3�ڣ�
 * @param  reg_addr: �Ĵ�����ַ
 * @param  data: ���ջ�����
 * @param  len: ��ȡ����
 * @retval 0: �ɹ���1: �豸��Ӧ��2: �Ĵ�����ַ��Ӧ��3: ��������Ӧ��
 */
uint8_t BE2_ReadReg(uint8_t reg_addr, uint8_t *data, uint8_t len)
{
    uint8_t i;

    // 1. ������ʼ�ź� + д��ַ
    OLED_I2C_Start();
    OLED_I2C_SendByte(BE2_I2C_ADDR_WRITE);
    wk_delay_us(2);
    // ����豸Ӧ��SDA����ΪӦ�𣬲ο�20230707Ӳ���ֲ�I2Cʱ��
    if (OLED_W_SDA_READ() != 0)
    {
        OLED_I2C_Stop();
        return 1;
    }

    // 2. ���ͼĴ�����ַ
    OLED_I2C_SendByte(reg_addr);
    wk_delay_us(2);
    if (OLED_W_SDA_READ() != 0)
    {
        OLED_I2C_Stop();
        return 2;
    }

    // 3. �����ظ���ʼ�ź� + ����ַ
    OLED_I2C_Start();
    OLED_I2C_SendByte(BE2_I2C_ADDR_READ);
    wk_delay_us(2);
    if (OLED_W_SDA_READ() != 0)
    {
        OLED_I2C_Stop();
        return 3;
    }

    // 4. ��ȡ���ݣ����1�ֽڷ��ͷ�Ӧ��
    for (i = 0; i < len; i++)
    {
        // ��ȡ1�ֽڣ���λ��ǰ���ο�20230707Ӳ���ֲ�I2C���ݴ����ʽ��
        data[i] = 0;
        for (uint8_t bit = 0; bit < 8; bit++)
        {
            data[i] <<= 1;
            OLED_W_SCL(1);  // ʱ�����ߣ���ȡSDA
            wk_delay_us(2);
            if (OLED_W_SDA_READ())
                data[i] |= 0x01;
            OLED_W_SCL(0);  // ʱ������
            wk_delay_us(2);
        }
        // ����Ӧ�����1�ֽڷ�Ӧ��
        OLED_W_SDA(i == len - 1 ? 1 : 0);
        wk_delay_us(2);
        OLED_W_SCL(1);
        wk_delay_us(2);
        OLED_W_SCL(0);
        OLED_W_SDA(1);  // �ͷ�SDA
    }

    // 5. ����ֹͣ�ź�
    OLED_I2C_Stop();
    return 0;
}

/**
 * @brief  ��ȡ���ٶȼ����ݣ���λ��g����ֹʱZ��ԼΪ-1g���ο�20220802�û��ֲ�2.2.1��
 */
uint8_t BE2_ReadAccelerometer(float *ax, float *ay, float *az)
{
    uint8_t buf[4];  // 32λ�����ͣ�С�˸�ʽ���ο�20230707Ӳ���ֲ����ݸ�ʽ��

    if (BE2_ReadReg(BE2_REG_ACC_X, buf, 4) != 0) return 1;
    *ax = *(float *)buf;

    if (BE2_ReadReg(BE2_REG_ACC_Y, buf, 4) != 0) return 2;
    *ay = *(float *)buf;

    if (BE2_ReadReg(BE2_REG_ACC_Z, buf, 4) != 0) return 3;
    *az = *(float *)buf;

    return 0;
}

/**
 * @brief  ��ȡ���������ݣ���λ��dps����ֹʱ�ӽ�0���ο�20220802�û��ֲ�2.2.2��
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
 * @brief  ��ȡŷ�������ݣ���λ���ȣ��ο�20220802�û��ֲ�2.3.2��ע��pitch��90��������⣩
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
