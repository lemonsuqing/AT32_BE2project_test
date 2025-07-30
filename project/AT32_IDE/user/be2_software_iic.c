#include "be2_software_iic.h"

/**
 * @brief  初始化BE2（复用OLED的I2C引脚）
 * @retval 0: 成功
 */
uint8_t BE2_I2C_Init(void)
{
    OLED_I2C_Init();  // 复用OLED的I2C引脚配置（开漏输出+上拉）
    wk_delay_ms(100); // 等待传感器上电稳定
    return 0;
}

/**
 * @brief  向BE2寄存器写入数据
 * @param  reg_addr: 寄存器地址
 * @param  data: 要写入的数据
 * @retval 0: 成功；1: 设备无应答；2: 寄存器地址无应答
 */
uint8_t BE2_WriteReg(uint8_t reg_addr, uint8_t data)
{
    // 发送起始信号+写地址
    OLED_I2C_Start();
    OLED_I2C_SendByte(BE2_I2C_ADDR_WRITE);
    wk_delay_us(2);
    if (OLED_W_SDA_READ() != 0) { OLED_I2C_Stop(); return 1; }

    // 发送寄存器地址
    OLED_I2C_SendByte(reg_addr);
    wk_delay_us(2);
    if (OLED_W_SDA_READ() != 0) { OLED_I2C_Stop(); return 2; }

    // 发送数据
    OLED_I2C_SendByte(data);
    wk_delay_us(2);

    // 发送停止信号
    OLED_I2C_Stop();
    return 0;
}

/**
 * @brief  从BE2寄存器读取数据
 * @param  reg_addr: 寄存器地址
 * @param  data: 接收缓冲区
 * @param  len: 读取长度
 * @retval 0: 成功；1: 设备无应答；2: 寄存器地址无应答；3: 读操作无应答
 */
uint8_t BE2_ReadReg(uint8_t reg_addr, uint8_t *data, uint8_t len)
{
    uint8_t i;

    // 发送起始信号+写地址
    OLED_I2C_Start();
    OLED_I2C_SendByte(BE2_I2C_ADDR_WRITE);
    wk_delay_us(2);
    if (OLED_W_SDA_READ() != 0) { OLED_I2C_Stop(); return 1; }

    // 发送寄存器地址
    OLED_I2C_SendByte(reg_addr);
    wk_delay_us(2);
    if (OLED_W_SDA_READ() != 0) { OLED_I2C_Stop(); return 2; }

    // 重复起始信号+读地址
    OLED_I2C_Start();
    OLED_I2C_SendByte(BE2_I2C_ADDR_READ);
    wk_delay_us(2);
    if (OLED_W_SDA_READ() != 0) { OLED_I2C_Stop(); return 3; }

    // 读取数据（最后1字节发送非应答）
    for (i = 0; i < len; i++)
    {
        data[i] = 0;
        // 读取8位数据（高位在前）
        for (uint8_t bit = 0; bit < 8; bit++)
        {
            data[i] <<= 1;
            OLED_W_SCL(1);
            wk_delay_us(2);
            if (OLED_W_SDA_READ()) data[i] |= 0x01;
            OLED_W_SCL(0);
            wk_delay_us(2);
        }
        // 发送应答（最后1字节非应答）
        OLED_W_SDA(i == len - 1 ? 1 : 0);
        wk_delay_us(2);
        OLED_W_SCL(1);
        wk_delay_us(2);
        OLED_W_SCL(0);
        OLED_W_SDA(1); // 释放SDA
    }

    OLED_I2C_Stop();
    return 0;
}

/**
 * @brief  使能传感器数据输出（配置DATA_ENABLE和CTRL0_A寄存器）
 * @retval 0: 成功；其他: 错误码
 */
uint8_t BE2_EnableSensors(void)
{
    // 使能加速度计、陀螺仪（DATA_ENABLE寄存器：Bit5=1, Bit6=1）
    if (BE2_WriteReg(BE2_REG_DATA_ENABLE, 0x60) != 0) return 1;
    // 配置加速度计输出原始数据（CTRL0_A：Bit4=0）
    if (BE2_WriteReg(BE2_REG_CTRL0_A, 0x00) != 0) return 2;
    wk_delay_ms(50);
    return 0;
}

/**
 * @brief  读取加速度计数据（单位：g）
 * @retval 0: 成功；其他: 错误码
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
 * @brief  读取陀螺仪数据（单位：dps）
 * @retval 0: 成功；其他: 错误码
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
 * @brief  读取欧拉角数据（单位：度）
 * @retval 0: 成功；其他: 错误码
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
