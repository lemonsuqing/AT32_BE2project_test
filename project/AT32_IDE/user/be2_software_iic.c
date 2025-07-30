#include "be2_software_iic.h"
#include "wk_system.h"  // 用于延时函数

/**
 * @brief  初始化BE2（复用OLED的I2C引脚配置，开漏输出+上拉，符合20230707硬件手册表2-3电平要求）
 * @retval 0: 成功
 */
uint8_t BE2_I2C_Init(void)
{
    OLED_I2C_Init();  // 复用OLED的I2C引脚初始化（已验证兼容）
    wk_delay_ms(100); // 等待传感器上电稳定（参考20220802用户手册电源要求）
    return 0;
}

/**
 * @brief  读取BE2寄存器数据（遵循I2C通信规范，参考20230707硬件手册3.3节）
 * @param  reg_addr: 寄存器地址
 * @param  data: 接收缓冲区
 * @param  len: 读取长度
 * @retval 0: 成功；1: 设备无应答；2: 寄存器地址无应答；3: 读操作无应答
 */
uint8_t BE2_ReadReg(uint8_t reg_addr, uint8_t *data, uint8_t len)
{
    uint8_t i;

    // 1. 发送起始信号 + 写地址
    OLED_I2C_Start();
    OLED_I2C_SendByte(BE2_I2C_ADDR_WRITE);
    wk_delay_us(2);
    // 检查设备应答（SDA拉低为应答，参考20230707硬件手册I2C时序）
    if (OLED_W_SDA_READ() != 0)
    {
        OLED_I2C_Stop();
        return 1;
    }

    // 2. 发送寄存器地址
    OLED_I2C_SendByte(reg_addr);
    wk_delay_us(2);
    if (OLED_W_SDA_READ() != 0)
    {
        OLED_I2C_Stop();
        return 2;
    }

    // 3. 发送重复起始信号 + 读地址
    OLED_I2C_Start();
    OLED_I2C_SendByte(BE2_I2C_ADDR_READ);
    wk_delay_us(2);
    if (OLED_W_SDA_READ() != 0)
    {
        OLED_I2C_Stop();
        return 3;
    }

    // 4. 读取数据（最后1字节发送非应答）
    for (i = 0; i < len; i++)
    {
        // 读取1字节（高位在前，参考20230707硬件手册I2C数据传输格式）
        data[i] = 0;
        for (uint8_t bit = 0; bit < 8; bit++)
        {
            data[i] <<= 1;
            OLED_W_SCL(1);  // 时钟拉高，读取SDA
            wk_delay_us(2);
            if (OLED_W_SDA_READ())
                data[i] |= 0x01;
            OLED_W_SCL(0);  // 时钟拉低
            wk_delay_us(2);
        }
        // 发送应答（最后1字节非应答）
        OLED_W_SDA(i == len - 1 ? 1 : 0);
        wk_delay_us(2);
        OLED_W_SCL(1);
        wk_delay_us(2);
        OLED_W_SCL(0);
        OLED_W_SDA(1);  // 释放SDA
    }

    // 5. 发送停止信号
    OLED_I2C_Stop();
    return 0;
}

/**
 * @brief  读取加速度计数据（单位：g，静止时Z轴约为-1g，参考20220802用户手册2.2.1）
 */
uint8_t BE2_ReadAccelerometer(float *ax, float *ay, float *az)
{
    uint8_t buf[4];  // 32位浮点型（小端格式，参考20230707硬件手册数据格式）

    if (BE2_ReadReg(BE2_REG_ACC_X, buf, 4) != 0) return 1;
    *ax = *(float *)buf;

    if (BE2_ReadReg(BE2_REG_ACC_Y, buf, 4) != 0) return 2;
    *ay = *(float *)buf;

    if (BE2_ReadReg(BE2_REG_ACC_Z, buf, 4) != 0) return 3;
    *az = *(float *)buf;

    return 0;
}

/**
 * @brief  读取陀螺仪数据（单位：dps，静止时接近0，参考20220802用户手册2.2.2）
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
 * @brief  读取欧拉角数据（单位：度，参考20220802用户手册2.3.2，注意pitch±90°奇点问题）
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
