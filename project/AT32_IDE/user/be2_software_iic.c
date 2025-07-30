#include "be2_software_iic.h"

// ���Ų�����
#define SCL_HIGH    gpio_bits_set(BE2_IIC_SCL_PORT, BE2_IIC_SCL_PIN)
#define SCL_LOW     gpio_bits_reset(BE2_IIC_SCL_PORT, BE2_IIC_SCL_PIN)
#define SDA_HIGH    gpio_bits_set(BE2_IIC_SDA_PORT, BE2_IIC_SDA_PIN)
#define SDA_LOW     gpio_bits_reset(BE2_IIC_SDA_PORT, BE2_IIC_SDA_PIN)
#define SDA_READ    gpio_input_data_bit_read(BE2_IIC_SDA_PORT, BE2_IIC_SDA_PIN)

// ��ʱ������ȷ��I2Cʱ������400kbit/sҪ��
static void IIC_DelayUs(uint16_t us)
{
    wk_delay_us(us);  // ��ʵ��΢�뼶��ʱ
}

// ��ʼ��I2C����
void BE2_IIC_Init(void)
{
    gpio_init_type gpio_init_struct;

    // ʹ��GPIOʱ�ӣ�����ʵ�ʶ˿��޸ģ�
    crm_periph_clock_enable(CRM_GPIOB_PERIPH_CLOCK, TRUE);

    // ����SCL��SDAΪ�������
    gpio_default_para_init(&gpio_init_struct);
    gpio_init_struct.gpio_mode = GPIO_MODE_OUTPUT;
    gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
    gpio_init_struct.gpio_pull = GPIO_PULL_UP;
    gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
    gpio_init_struct.gpio_pins = BE2_IIC_SCL_PIN | BE2_IIC_SDA_PIN;
    gpio_init(BE2_IIC_SCL_PORT, &gpio_init_struct);

    // ��ʼ��Ϊ�ߵ�ƽ
    SCL_HIGH;
    SDA_HIGH;
    IIC_DelayUs(10);
}

// ������ʼ�ź�
static void IIC_Start(void)
{
    SDA_HIGH;
    SCL_HIGH;
    IIC_DelayUs(5);
    SDA_LOW;
    IIC_DelayUs(5);
    SCL_LOW;  // ����ʱ�ӣ�׼����������
}

// ����ֹͣ�ź�
static void IIC_Stop(void)
{
    SDA_LOW;
    SCL_HIGH;
    IIC_DelayUs(5);
    SDA_HIGH;
    IIC_DelayUs(5);
}

// ����Ӧ���źţ�0��Ӧ��1����Ӧ��
static void IIC_SendAck(uint8_t ack)
{
    if (ack) SDA_HIGH;
    else SDA_LOW;
    IIC_DelayUs(2);
    SCL_HIGH;
    IIC_DelayUs(5);
    SCL_LOW;
    SDA_HIGH;  // �ͷ�SDA
    IIC_DelayUs(1);
}

// �ȴ�Ӧ���ź�
static uint8_t IIC_WaitAck(void)
{
    uint8_t timeout = 0;
    SDA_HIGH;  // �ͷ�SDA
    IIC_DelayUs(2);
    SCL_HIGH;
    IIC_DelayUs(2);

    while (SDA_READ)
    {
        timeout++;
        if (timeout > 200)
        {
            IIC_Stop();
            return 1;  // Ӧ��ʱ
        }
        IIC_DelayUs(1);
    }
    SCL_LOW;
    IIC_DelayUs(1);
    return 0;  // Ӧ��ɹ�
}

// ����һ���ֽ�
static void IIC_SendByte(uint8_t data)
{
    uint8_t i;
    for (i = 0; i < 8; i++)
    {
        if (data & 0x80) SDA_HIGH;
        else SDA_LOW;
        data <<= 1;
        IIC_DelayUs(2);
        SCL_HIGH;
        IIC_DelayUs(5);
        SCL_LOW;
        IIC_DelayUs(1);
    }
}

// ��ȡһ���ֽڣ�ack��0-����Ӧ��1-���ͷ�Ӧ��
static uint8_t IIC_ReadByte(uint8_t ack)
{
    uint8_t data = 0;
    uint8_t i;

    for (i = 0; i < 8; i++)
    {
        data <<= 1;
        SCL_HIGH;
        IIC_DelayUs(3);
        if (SDA_READ) data |= 0x01;
        SCL_LOW;
        IIC_DelayUs(1);
    }
    IIC_SendAck(ack);
    return data;
}

// ��ȡ�Ĵ�������
uint8_t BE2_IIC_ReadReg(uint8_t reg_addr, uint8_t *data, uint16_t len)
{
    IIC_Start();
    IIC_SendByte(BE2_IIC_ADDR);  // ����д��ַ
    if (IIC_WaitAck())
    {
        IIC_Stop();
        return 1;  // �豸��Ӧ��
    }

    IIC_SendByte(reg_addr);  // ���ͼĴ�����ַ
    if (IIC_WaitAck())
    {
        IIC_Stop();
        return 2;  // �Ĵ�����ַ��Ӧ��
    }

    IIC_Start();
    IIC_SendByte(BE2_IIC_ADDR | 0x01);  // ���Ͷ���ַ
    if (IIC_WaitAck())
    {
        IIC_Stop();
        return 3;  // ��������Ӧ��
    }

    // ��ȡ����
    for (uint16_t i = 0; i < len; i++)
    {
        // ���һ���ֽڷ��ͷ�Ӧ��
        data[i] = IIC_ReadByte((i == len - 1) ? 1 : 0);
    }

    IIC_Stop();
    return 0;  // �ɹ�
}

// ��ȡ���ٶȼ����ݣ���λ��g��
uint8_t BE2_ReadAccelerometer(float *ax, float *ay, float *az)
{
    uint8_t buf[12];  // X��Y��Z��4�ֽ�
    uint8_t ret = BE2_IIC_ReadReg(BE2_REG_ACC_X, buf, 12);
    if (ret != 0) return ret;

    // ת��Ϊfloat��С�˸�ʽ��
    *ax = *(float *)&buf[0];
    *ay = *(float *)&buf[4];
    *az = *(float *)&buf[8];
    return 0;
}

// ��ȡ���������ݣ���λ��dps��
uint8_t BE2_ReadGyroscope(float *gx, float *gy, float *gz)
{
    uint8_t buf[12];
    uint8_t ret = BE2_IIC_ReadReg(BE2_REG_GYR_X, buf, 12);
    if (ret != 0) return ret;

    *gx = *(float *)&buf[0];
    *gy = *(float *)&buf[4];
    *gz = *(float *)&buf[8];
    return 0;
}

// ��ȡŷ�������ݣ���λ���ȣ�
uint8_t BE2_ReadEulerAngle(float *roll, float *pitch, float *yaw)
{
    uint8_t buf[12];
    uint8_t ret = BE2_IIC_ReadReg(BE2_REG_EULER_X, buf, 12);
    if (ret != 0) return ret;

    *roll = *(float *)&buf[0];   // �����
    *pitch = *(float *)&buf[4];  // ������
    *yaw = *(float *)&buf[8];    // ƫ����
    return 0;
}
