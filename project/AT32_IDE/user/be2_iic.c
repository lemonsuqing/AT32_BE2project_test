#include "be2_iic.h"
#include "wk_i2c.h"
#include "Serial.h"
#include "at32f402_405_i2c.h"  // 包含库API头文件
#include <string.h>
#include <math.h>

// I2C超时时间（ms）
#define I2C_TIMEOUT_MS         100

/**
 * @brief  初始化BE2传感器I2C配置
 * @param  无
 * @retval 无
 */
void BE2_I2C_Init(void) {
    Serial_Printf("BE2 I2C Mode (ADD0=0) Init\r\n");
}

/**
 * @brief  向BE2寄存器写入一个字节
 * @param  reg: 寄存器地址
 * @param  data: 要写入的数据
 * @retval 0:成功 1:失败
 */
uint8_t BE2_I2C_WriteReg(uint8_t reg, uint8_t data) {
    uint32_t timeout = I2C_TIMEOUT_MS * 1000;

    // 等待总线空闲
    while(i2c_flag_get(I2C2, I2C_BUSYF_FLAG) && timeout--);
    if(timeout == 0) return 1;

    // 生成起始信号
    i2c_start_generate(I2C2);
    timeout = I2C_TIMEOUT_MS * 1000;
    while(!i2c_flag_get(I2C2, I2C_TDIS_FLAG) && timeout--);  // 等待起始信号发送完成
    if(timeout == 0) return 1;

    // 设置从机地址和传输方向（写）
    i2c_transfer_addr_set(I2C2, BE2_I2C_ADDR >> 1);  // 7位地址需右移1位
    i2c_transfer_dir_set(I2C2, I2C_DIR_TRANSMIT);

    // 发送寄存器地址
    i2c_data_send(I2C2, reg);
    timeout = I2C_TIMEOUT_MS * 1000;
    while(!i2c_flag_get(I2C2, I2C_TDBE_FLAG) && timeout--);  // 等待发送缓冲区空
    if(timeout == 0) return 1;

    // 发送数据
    i2c_data_send(I2C2, data);
    timeout = I2C_TIMEOUT_MS * 1000;
    while(!i2c_flag_get(I2C2, I2C_TDBE_FLAG) && timeout--);
    if(timeout == 0) return 1;

    // 生成停止信号
    i2c_stop_generate(I2C2);
    timeout = I2C_TIMEOUT_MS * 1000;
    while(!i2c_flag_get(I2C2, I2C_STOPF_FLAG) && timeout--);
    if(timeout == 0) return 1;
    i2c_flag_clear(I2C2, I2C_STOPF_FLAG);  // 清除停止标志

    return 0;
}

/**
 * @brief  从BE2寄存器读取一个字节
 * @param  reg: 寄存器地址
 * @param  data: 读取到的数据
 * @retval 0:成功 1:失败
 */
uint8_t BE2_I2C_ReadReg(uint8_t reg, uint8_t *data) {
    uint32_t timeout = I2C_TIMEOUT_MS * 1000;

    // 先写入要读取的寄存器地址
    if(BE2_I2C_WriteReg(reg, 0xFF) != 0)
        return 1;

    // 等待总线空闲
    while(i2c_flag_get(I2C2, I2C_BUSYF_FLAG) && timeout--);
    if(timeout == 0) return 1;

    // 生成起始信号（读模式）
    i2c_start_generate(I2C2);
    timeout = I2C_TIMEOUT_MS * 1000;
    while(!i2c_flag_get(I2C2, I2C_TDIS_FLAG) && timeout--);
    if(timeout == 0) return 1;

    // 设置从机地址和传输方向（读）
    i2c_transfer_addr_set(I2C2, BE2_I2C_ADDR >> 1);  // 7位地址
    i2c_transfer_dir_set(I2C2, I2C_DIR_RECEIVE);

    // 禁用ACK（单字节读取）
    i2c_ack_enable(I2C2, FALSE);

    // 等待接收缓冲区非空
    timeout = I2C_TIMEOUT_MS * 1000;
    while(!i2c_flag_get(I2C2, I2C_RDBF_FLAG) && timeout--);
    if(timeout == 0) return 1;
    *data = i2c_data_receive(I2C2);

    // 生成停止信号
    i2c_stop_generate(I2C2);
    timeout = I2C_TIMEOUT_MS * 1000;
    while(!i2c_flag_get(I2C2, I2C_STOPF_FLAG) && timeout--);
    if(timeout == 0) return 1;
    i2c_flag_clear(I2C2, I2C_STOPF_FLAG);

    // 恢复ACK使能
    i2c_ack_enable(I2C2, TRUE);

    return 0;
}

/**
 * @brief  从BE2连续读取多个寄存器
 * @param  reg: 起始寄存器地址
 * @param  buf: 接收缓冲区
 * @param  len: 要读取的长度
 * @retval 0:成功 1:失败
 */
uint8_t BE2_I2C_ReadMultiReg(uint8_t reg, uint8_t *buf, uint16_t len) {
    uint32_t timeout = I2C_TIMEOUT_MS * 1000;
    uint16_t i;

    // 先写入起始寄存器地址
    if(BE2_I2C_WriteReg(reg, 0xFF) != 0)
        return 1;

    // 等待总线空闲
    while(i2c_flag_get(I2C2, I2C_BUSYF_FLAG) && timeout--);
    if(timeout == 0) return 1;

    // 生成起始信号（读模式）
    i2c_start_generate(I2C2);
    timeout = I2C_TIMEOUT_MS * 1000;
    while(!i2c_flag_get(I2C2, I2C_TDIS_FLAG) && timeout--);
    if(timeout == 0) return 1;

    // 设置从机地址和传输方向（读）
    i2c_transfer_addr_set(I2C2, BE2_I2C_ADDR >> 1);
    i2c_transfer_dir_set(I2C2, I2C_DIR_RECEIVE);

    // 读取数据
    for(i = 0; i < len; i++) {
        // 最后一个字节前禁用ACK
        if(i == len - 1)
            i2c_ack_enable(I2C2, FALSE);

        // 等待接收缓冲区非空
        timeout = I2C_TIMEOUT_MS * 1000;
        while(!i2c_flag_get(I2C2, I2C_RDBF_FLAG) && timeout--);
        if(timeout == 0) return 1;
        buf[i] = i2c_data_receive(I2C2);
    }

    // 生成停止信号
    i2c_stop_generate(I2C2);
    timeout = I2C_TIMEOUT_MS * 1000;
    while(!i2c_flag_get(I2C2, I2C_STOPF_FLAG) && timeout--);
    if(timeout == 0) return 1;
    i2c_flag_clear(I2C2, I2C_STOPF_FLAG);

    // 恢复ACK使能
    i2c_ack_enable(I2C2, TRUE);

    return 0;
}

/**
 * @brief  初始化BE2传感器
 * @param  无
 * @retval 0:成功 1:失败
 */
uint8_t BE2_Init(void) {
    uint8_t whoami;
    uint8_t ret;

    // 读取设备ID（WHO_AM_I默认0x32）
    ret = BE2_I2C_ReadReg(REG_WHO_AM_I, &whoami);
    if(ret != 0 || whoami != 0x32) {
        Serial_Printf("BE2 Init Failed! WHO_AM_I=0x%02X\r\n", whoami);
        return 1;
    }

    // 使能寄存器修改（FUN_CONFIG Bit7=1）
    ret |= BE2_I2C_WriteReg(REG_FUN_CONFIG, 0x80);

    // 配置数据输出使能
    ret |= BE2_I2C_WriteReg(REG_DATA_ENABLE, 0x37);  // 使能加速度、陀螺仪等

    // 配置输出频率为100Hz
    ret |= BE2_I2C_WriteReg(REG_DATA_CTRL, 0x03);

    if(ret == 0)
        Serial_Printf("BE2 Init Success (I2C ADD0=0)\r\n");
    else
        Serial_Printf("BE2 Config Error\r\n");

    return ret;
}

/**
 * @brief  字节转浮点数（小端格式）
 * @param  bytes: 4字节数组
 * @retval 转换后的浮点数
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
 * @brief  读取BE2所有传感器数据
 * @param  data: 存储数据的结构体指针
 * @retval 0:成功 1:失败
 */
uint8_t BE2_ReadAllData(BE2_Data *data) {
    uint8_t buf[4];
    uint8_t ret = 0;

    // 读取时间戳
    ret |= BE2_I2C_ReadMultiReg(REG_TIMESTAMP_0, buf, 4);
    data->timestamp = (buf[3] << 24) | (buf[2] << 16) | (buf[1] << 8) | buf[0];

    // 读取加速度
    ret |= BE2_I2C_ReadMultiReg(REG_ACC_X_0, buf, 4);
    data->acc_x = BytesToFloat(buf);
    ret |= BE2_I2C_ReadMultiReg(REG_ACC_Y_0, buf, 4);
    data->acc_y = BytesToFloat(buf);
    ret |= BE2_I2C_ReadMultiReg(REG_ACC_Z_0, buf, 4);
    data->acc_z = BytesToFloat(buf);

    // 读取陀螺仪
    ret |= BE2_I2C_ReadMultiReg(REG_GYR_X_0, buf, 4);
    data->gyr_x = BytesToFloat(buf);
    ret |= BE2_I2C_ReadMultiReg(REG_GYR_Y_0, buf, 4);
    data->gyr_y = BytesToFloat(buf);
    ret |= BE2_I2C_ReadMultiReg(REG_GYR_Z_0, buf, 4);
    data->gyr_z = BytesToFloat(buf);

    // 读取四元数
    ret |= BE2_I2C_ReadMultiReg(REG_QUAT_W_0, buf, 4);
    data->quat_w = BytesToFloat(buf);
    ret |= BE2_I2C_ReadMultiReg(REG_QUAT_X_0, buf, 4);
    data->quat_x = BytesToFloat(buf);
    ret |= BE2_I2C_ReadMultiReg(REG_QUAT_Y_0, buf, 4);
    data->quat_y = BytesToFloat(buf);
    ret |= BE2_I2C_ReadMultiReg(REG_QUAT_Z_0, buf, 4);
    data->quat_z = BytesToFloat(buf);

    // 读取欧拉角
    ret |= BE2_I2C_ReadMultiReg(REG_EULER_X_0, buf, 4);
    data->euler_roll = BytesToFloat(buf);
    ret |= BE2_I2C_ReadMultiReg(REG_EULER_Y_0, buf, 4);
    data->euler_pitch = BytesToFloat(buf);
    ret |= BE2_I2C_ReadMultiReg(REG_EULER_Z_0, buf, 4);
    data->euler_yaw = BytesToFloat(buf);

    // 读取线性加速度
    ret |= BE2_I2C_ReadMultiReg(REG_LIN_ACC_X_0, buf, 4);
    data->lin_acc_x = BytesToFloat(buf);
    ret |= BE2_I2C_ReadMultiReg(REG_LIN_ACC_Y_0, buf, 4);
    data->lin_acc_y = BytesToFloat(buf);
    ret |= BE2_I2C_ReadMultiReg(REG_LIN_ACC_Z_0, buf, 4);
    data->lin_acc_z = BytesToFloat(buf);

    // 读取温度
    ret |= BE2_I2C_ReadMultiReg(REG_TEMP_0, buf, 4);
    data->temp = BytesToFloat(buf);

    return ret;
}

/**
 * @brief  打印传感器数据到串口
 * @param  data: 要打印的数据
 * @retval 无
 */
void BE2_PrintData(BE2_Data *data) {
    Serial_Printf("Timestamp: %u\r\n", data->timestamp);
    Serial_Printf("Acc: %.3f, %.3f, %.3f g\r\n",
                 data->acc_x, data->acc_y, data->acc_z);
    Serial_Printf("Gyr: %.3f, %.3f, %.3f dps\r\n",
                 data->gyr_x, data->gyr_y, data->gyr_z);
    Serial_Printf("Euler: Roll=%.2f, Pitch=%.2f, Yaw=%.2f °\r\n",
                 data->euler_roll, data->euler_pitch, data->euler_yaw);
    Serial_Printf("Quat: %.4f, %.4f, %.4f, %.4f\r\n",
                 data->quat_w, data->quat_x, data->quat_y, data->quat_z);
    Serial_Printf("LinAcc: %.3f, %.3f, %.3f g\r\n",
                 data->lin_acc_x, data->lin_acc_y, data->lin_acc_z);
    Serial_Printf("Temp: %.2f °C\r\n", data->temp);
    Serial_Printf("----------------------------------------\r\n");
}
