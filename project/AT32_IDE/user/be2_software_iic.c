#include "be2_software_iic.h"

// 引脚操作宏
#define SCL_HIGH    gpio_bits_set(BE2_IIC_SCL_PORT, BE2_IIC_SCL_PIN)
#define SCL_LOW     gpio_bits_reset(BE2_IIC_SCL_PORT, BE2_IIC_SCL_PIN)
#define SDA_HIGH    gpio_bits_set(BE2_IIC_SDA_PORT, BE2_IIC_SDA_PIN)
#define SDA_LOW     gpio_bits_reset(BE2_IIC_SDA_PORT, BE2_IIC_SDA_PIN)
#define SDA_READ    gpio_input_data_bit_read(BE2_IIC_SDA_PORT, BE2_IIC_SDA_PIN)

// 延时函数（确保I2C时序满足400kbit/s要求）
static void IIC_DelayUs(uint16_t us)
{
    wk_delay_us(us);  // 需实现微秒级延时
}

// 初始化I2C引脚
void BE2_IIC_Init(void)
{
    gpio_init_type gpio_init_struct;

    // 使能GPIO时钟（根据实际端口修改）
    crm_periph_clock_enable(CRM_GPIOB_PERIPH_CLOCK, TRUE);

    // 配置SCL和SDA为推挽输出
    gpio_default_para_init(&gpio_init_struct);
    gpio_init_struct.gpio_mode = GPIO_MODE_OUTPUT;
    gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
    gpio_init_struct.gpio_pull = GPIO_PULL_UP;
    gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
    gpio_init_struct.gpio_pins = BE2_IIC_SCL_PIN | BE2_IIC_SDA_PIN;
    gpio_init(BE2_IIC_SCL_PORT, &gpio_init_struct);

    // 初始化为高电平
    SCL_HIGH;
    SDA_HIGH;
    IIC_DelayUs(10);
}

// 发送起始信号
static void IIC_Start(void)
{
    SDA_HIGH;
    SCL_HIGH;
    IIC_DelayUs(5);
    SDA_LOW;
    IIC_DelayUs(5);
    SCL_LOW;  // 拉低时钟，准备发送数据
}

// 发送停止信号
static void IIC_Stop(void)
{
    SDA_LOW;
    SCL_HIGH;
    IIC_DelayUs(5);
    SDA_HIGH;
    IIC_DelayUs(5);
}

// 发送应答信号（0：应答，1：非应答）
static void IIC_SendAck(uint8_t ack)
{
    if (ack) SDA_HIGH;
    else SDA_LOW;
    IIC_DelayUs(2);
    SCL_HIGH;
    IIC_DelayUs(5);
    SCL_LOW;
    SDA_HIGH;  // 释放SDA
    IIC_DelayUs(1);
}

// 等待应答信号
static uint8_t IIC_WaitAck(void)
{
    uint8_t timeout = 0;
    SDA_HIGH;  // 释放SDA
    IIC_DelayUs(2);
    SCL_HIGH;
    IIC_DelayUs(2);

    while (SDA_READ)
    {
        timeout++;
        if (timeout > 200)
        {
            IIC_Stop();
            return 1;  // 应答超时
        }
        IIC_DelayUs(1);
    }
    SCL_LOW;
    IIC_DelayUs(1);
    return 0;  // 应答成功
}

// 发送一个字节
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

// 读取一个字节（ack：0-发送应答，1-发送非应答）
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

// 读取寄存器数据
uint8_t BE2_IIC_ReadReg(uint8_t reg_addr, uint8_t *data, uint16_t len)
{
    IIC_Start();
    IIC_SendByte(BE2_IIC_ADDR);  // 发送写地址
    if (IIC_WaitAck())
    {
        IIC_Stop();
        return 1;  // 设备无应答
    }

    IIC_SendByte(reg_addr);  // 发送寄存器地址
    if (IIC_WaitAck())
    {
        IIC_Stop();
        return 2;  // 寄存器地址无应答
    }

    IIC_Start();
    IIC_SendByte(BE2_IIC_ADDR | 0x01);  // 发送读地址
    if (IIC_WaitAck())
    {
        IIC_Stop();
        return 3;  // 读操作无应答
    }

    // 读取数据
    for (uint16_t i = 0; i < len; i++)
    {
        // 最后一个字节发送非应答
        data[i] = IIC_ReadByte((i == len - 1) ? 1 : 0);
    }

    IIC_Stop();
    return 0;  // 成功
}

// 读取加速度计数据（单位：g）
uint8_t BE2_ReadAccelerometer(float *ax, float *ay, float *az)
{
    uint8_t buf[12];  // X、Y、Z各4字节
    uint8_t ret = BE2_IIC_ReadReg(BE2_REG_ACC_X, buf, 12);
    if (ret != 0) return ret;

    // 转换为float（小端格式）
    *ax = *(float *)&buf[0];
    *ay = *(float *)&buf[4];
    *az = *(float *)&buf[8];
    return 0;
}

// 读取陀螺仪数据（单位：dps）
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

// 读取欧拉角数据（单位：度）
uint8_t BE2_ReadEulerAngle(float *roll, float *pitch, float *yaw)
{
    uint8_t buf[12];
    uint8_t ret = BE2_IIC_ReadReg(BE2_REG_EULER_X, buf, 12);
    if (ret != 0) return ret;

    *roll = *(float *)&buf[0];   // 横滚角
    *pitch = *(float *)&buf[4];  // 俯仰角
    *yaw = *(float *)&buf[8];    // 偏航角
    return 0;
}
