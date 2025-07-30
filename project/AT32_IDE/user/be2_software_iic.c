#include "be2_software_iic.h"

#define BE2_I2C_ADDR       0x32

static void delay_us(uint16_t us) {
    wk_delay_us(us);
}

static void i2c_start(void) {
    Serial_Printf("===== 发送起始信号 =====\r\n");
    BE2_SDA_H();
    BE2_SCL_H();
    delay_us(4);
    Serial_Printf("SDA从高变低 (START)\r\n");
    BE2_SDA_L();
    delay_us(4);
    BE2_SCL_L();
    delay_us(2);
}

static void i2c_stop(void) {
    Serial_Printf( "===== 发送停止信号 =====\r\n");
    BE2_SCL_L();
    BE2_SDA_L();
    delay_us(4);
    BE2_SCL_H();
    delay_us(4);
    Serial_Printf("SDA从低变高 (STOP)\r\n");
    BE2_SDA_H();
    delay_us(4);
}

static void i2c_send_bit(uint8_t bit) {
    if (bit) {
        BE2_SDA_H();
    } else {
        BE2_SDA_L();
    }
    delay_us(2);
    BE2_SCL_H();
    Serial_Printf("发送比特: %d (SCL高电平采样)\r\n", bit);
    delay_us(4);
    BE2_SCL_L();
    delay_us(2);
}

static uint8_t i2c_recv_bit(void) {
    uint8_t bit;
    BE2_SDA_H();  // 释放总线，由从机控制
    delay_us(2);
    BE2_SCL_H();
    delay_us(2);
    bit = BE2_SDA_READ();
    Serial_Printf("接收比特: %d (SCL高电平读取)\r\n", bit);
    BE2_SCL_L();
    delay_us(2);
    return bit;
}

static void i2c_send_byte(uint8_t byte) {
    Serial_Printf("[I2C] Sending byte 0x%02X: ", byte);
    for (int i = 0; i < 8; i++) {
        uint8_t bit = (byte & 0x80) ? 1 : 0;
        Serial_Printf("%d", bit);
        i2c_send_bit(bit);
        byte <<= 1;
    }
    Serial_Printf("\r\n");
}

static uint8_t i2c_wait_ack(void) {
    uint8_t ack;
    BE2_SDA_H();  // 释放SDA，等待从机拉低
    delay_us(10);
    BE2_SCL_H();
    delay_us(10);
    ack = BE2_SDA_READ();  // 0=ACK，1=NACK
    BE2_SCL_L();
    delay_us(2);

    if (ack == 0) {
        Serial_Printf("收到ACK (从机响应)\r\n");
    } else {
        Serial_Printf("收到NACK (从机无响应)\r\n");
    }
    return (ack == 0);  // 返回是否收到ACK
}

static uint8_t i2c_recv_byte(uint8_t ack) {
    uint8_t byte = 0;
    Serial_Printf("开始接收字节 (二进制: ");
    for (int i = 0; i < 8; i++) {
        byte <<= 1;
        byte |= i2c_recv_bit();
    }
    Serial_Printf(") -> 0x%02X\r\n", byte);

    // 发送应答信号
    i2c_send_bit(!ack);  // 0=ACK，1=NACK
    if (ack) {
        Serial_Printf("发送NACK (结束接收)\r\n");
    } else {
        Serial_Printf("发送ACK (继续接收)\r\n");
    }
    return byte;
}

void I2C_Bus_Reset(void) {
    Serial_Printf("===== 重置I2C总线 =====\r\n");
    BE2_SDA_H();
    for (int i = 0; i < 9; i++) {  // 发送9个时钟脉冲
        BE2_SCL_L();
        delay_us(10);
        BE2_SCL_H();
        Serial_Printf("复位时钟脉冲 %d\r\n", i+1);
        delay_us(10);
    }
    i2c_stop();
}

void BE2_I2C_Init(void) {
    gpio_init_type gpio_init_struct;
    crm_periph_clock_enable(CRM_GPIOB_PERIPH_CLOCK, TRUE);
    gpio_default_para_init(&gpio_init_struct);
    gpio_init_struct.gpio_pins = BE2_SCL_PIN | BE2_SDA_PIN;
    gpio_init_struct.gpio_mode = GPIO_MODE_OUTPUT;
    gpio_init_struct.gpio_out_type = GPIO_OUTPUT_OPEN_DRAIN;
    gpio_init_struct.gpio_pull = GPIO_PULL_UP;
    gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
    gpio_init(BE2_SCL_PORT, &gpio_init_struct);

    BE2_SCL_H();
    BE2_SDA_H();
    delay_us(10);
    Serial_Printf("引脚初始化完成 (SCL=H, SDA=H)\r\n");

    I2C_Bus_Reset();
}

uint8_t BE2_I2C_CheckDevice(void) {
    uint8_t ack = 0;
    Serial_Printf("\r\n===== 开始检测设备 =====");
    Serial_Printf("基础地址: 0x%02X (7位)\r\n", BE2_I2C_ADDR);

    i2c_start();
    uint8_t write_addr = (BE2_I2C_ADDR << 1) | 0;  // 写地址（最低位0）
    i2c_send_byte(write_addr);
    ack = i2c_wait_ack();  // 等待设备应答

    i2c_stop();
    Serial_Printf("设备检测结果: %s\r\n", ack ? "存在" : "不存在");
    return ack;
}

uint8_t BE2_I2C_ReadReg(uint8_t reg) {
    uint8_t data = 0;
    Serial_Printf( "\r\n===== 读取寄存器 0x%02X =====", reg);

    // 步骤1: 发送起始信号 + 写地址
    i2c_start();
    uint8_t write_addr = (BE2_I2C_ADDR << 1) | 0;
    Serial_Printf( "发送写地址: 0x%02X\r\n", write_addr);
    i2c_send_byte(write_addr);
    if (!i2c_wait_ack()) {
        Serial_Printf( "错误: 发送写地址后未收到ACK\r\n");
        i2c_stop();
        return 0xFF;  // 错误标志
    }

    // 步骤2: 发送寄存器地址
    Serial_Printf( "发送寄存器地址: 0x%02X\r\n", reg);
    i2c_send_byte(reg);
    if (!i2c_wait_ack()) {
        Serial_Printf( "错误: 发送寄存器地址后未收到ACK\r\n");
        i2c_stop();
        return 0xFF;
    }

    // 步骤3: 发送重复起始信号 + 读地址
    i2c_start();  // 重复起始
    uint8_t read_addr = (BE2_I2C_ADDR << 1) | 1;
    Serial_Printf( "发送读地址: 0x%02X\r\n", read_addr);
    i2c_send_byte(read_addr);
    if (!i2c_wait_ack()) {
        Serial_Printf( "错误: 发送读地址后未收到ACK\r\n");
        i2c_stop();
        return 0xFF;
    }

    // 步骤4: 接收数据（最后一个字节发送NACK）
    data = i2c_recv_byte(1);  // 1=NACK
    Serial_Printf( "从寄存器0x%02X读取到数据: 0x%02X\r\n", reg, data);

    // 步骤5: 发送停止信号
    i2c_stop();
    return data;
}

void BE2_Test_WhoAmI(void) {
    Serial_Printf("\r\n===== 开始WHOAMI测试 =====");
    BE2_I2C_Init();  // 初始化I2C

    // 检查设备是否存在
    if (BE2_I2C_CheckDevice()) {
        Serial_Printf( "设备通信正常，开始读取WHOAMI...\r\n");
        // 读取WHOAMI寄存器（通常为0x00，需根据手册确认）
        uint8_t whoami = BE2_I2C_ReadReg(0x00);

        // 验证WHOAMI值（参考手册，假设正确值为0x1A）
        if (whoami == 0x1A) {
            Serial_Printf( "WHOAMI验证成功: 0x%02X (符合预期)\r\n", whoami);
        } else {
            Serial_Printf( "WHOAMI验证失败: 0x%02X (预期0x1A)\r\n", whoami);
        }
    } else {
        Serial_Printf( "设备无响应，测试失败\r\n");
    }
    Serial_Printf("===== WHOAMI测试结束 =====\r\n");
}
