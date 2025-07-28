#include "be2_spi.h"
#include "wk_spi.h"
#include "Serial.h"
#include "wk_system.h"
#include <string.h>
#include <math.h>

// 底层SPI传输函数
uint8_t be2_transfer_byte(uint8_t data) {
    spi2_cs_enable();
    uint8_t ret = spi2_read_write_byte(data);
    spi2_cs_disable();
    return ret;
}

// CRC校验计算（简单异或）
uint8_t calculate_crc(uint8_t *data, uint8_t len) {
    uint8_t crc = 0;
    for(uint8_t i = 0; i < len; i++) {
        crc ^= data[i];
    }
    return crc;
}

// 写入寄存器（仅保留复位所需功能）
void be2_write_register(uint8_t reg, uint8_t value) {
	Serial_Printf("Writing reg 0x%02X = 0x%02X...\r\n", reg, value);  // 标记步骤
    spi2_cs_enable();
    spi2_read_write_byte(reg & 0x7F);  // 写命令（最高位为0）
    spi2_read_write_byte(value);
    spi2_cs_disable();
    wk_delay_us(50);  // 等待写入完成
}

// 读取寄存器（仅作兼容保留）
uint8_t be2_read_register(uint8_t reg_addr)
{
    uint8_t tx_buf[10] = {
        0x3A,       // Header
        0x01,       // Read command
        0x00,       // Reserved
        0x06,       // Payload length
        reg_addr,   // Register address (e.g., 0x74)
        0x00, 0x00, 0x00, 0x00, 0x00 // Padding
    };

    uint8_t rx_buf[10] = {0};

    spi2_cs_enable();
    for (int i = 0; i < 10; i++)
    {
        rx_buf[i] = spi2_read_write_byte(tx_buf[i]);
    }
    spi2_cs_disable();

    wk_delay_ms(10); // 等待响应帧到来

    // 读取响应帧：通常为 16 字节或以上
    uint8_t resp[16] = {0};
    spi2_cs_enable();
    for (int i = 0; i < 16; i++)
    {
        resp[i] = spi2_read_write_byte(0xFF);
    }
    spi2_cs_disable();

    // 响应数据在 resp[4] 之后
    return resp[4];  // WHO_AM_I 应该在这里返回 0x62 等
}

// 进入命令模式
bool be2_enter_command_mode(void) {
    // 命令帧结构：3A 01 00 06 00 00 00 F1
	Serial_Printf("Trying to enter command mode...\r\n");  // 标记步骤
	    uint8_t cmd_frame[] = {
	        LPBUS_HEADER, CMD_ENTER_COMMAND_MODE, 0x00, 0x06, 0x00, 0x00, 0x00, 0xF1
	    };
	    uint8_t response[8] = {0};

	    // 发送命令帧前输出标记
	    Serial_Printf("Sending command mode frame...\r\n");
	    spi2_cs_enable();
	    for(uint8_t i=0; i<8; i++) {
	        spi2_read_write_byte(cmd_frame[i]);
	    }
	    spi2_cs_disable();
	    Serial_Printf("Command frame sent, waiting for response...\r\n");  // 标记步骤
	    wk_delay_ms(10);  // 短延时，避免卡死

	    // 读取响应前输出标记
	    Serial_Printf("Reading command mode response...\r\n");
	    spi2_cs_enable();
	    for(uint8_t i=0; i<8; i++) {
	        response[i] = spi2_read_write_byte(0xFF);
	    }
	    spi2_cs_disable();

	    // 输出响应结果（无论是否有效）
	    Serial_Printf("Command mode response: ");
	    for(uint8_t i=0; i<8; i++) {
	        Serial_Printf("0x%02X ", response[i]);
	    }
	    Serial_Printf("\r\n");

	    // 验证响应
	    if(response[0] != LPBUS_HEADER) {
	        Serial_Printf("Command mode failed: invalid header\r\n");
	        return false;
	    }
	    if(response[7] != calculate_crc(response, 7)) {
	        Serial_Printf("Command mode failed: CRC mismatch\r\n");
	        return false;
	    }
	    return true;
}

// 读取欧拉角数据
bool be2_read_euler(BE2_Data *data) {
    // 读欧拉角命令帧
    uint8_t cmd_frame[] = {
        LPBUS_HEADER,
        CMD_READ_EULER,
        0x00,         // 设备地址
        0x03,         // 数据长度
        0x05, 0x00, 0x00,  // 数据域（指定欧拉角）
        0x00          // CRC占位
    };
    // 计算CRC
    cmd_frame[8] = calculate_crc(cmd_frame, 8);

    // 发送命令
    spi2_cs_enable();
    for(uint8_t i = 0; i < 9; i++) {
        spi2_read_write_byte(cmd_frame[i]);
    }
    spi2_cs_disable();
    wk_delay_ms(10);  // 等待数据准备

    // 读取响应（16字节）
    uint8_t response[16] = {0};
    spi2_cs_enable();
    for(uint8_t i = 0; i < 16; i++) {
        response[i] = spi2_read_write_byte(0xFF);
    }
    spi2_cs_disable();

    // 验证响应
    if(response[0] != LPBUS_HEADER) {
        Serial_Printf("Euler read failed: invalid header\r\n");
        return false;
    }

    if(response[15] != calculate_crc(response, 15)) {
        Serial_Printf("Euler read failed: CRC mismatch\r\n");
        return false;
    }

    // 解析欧拉角（小端格式）
    data->euler[0] = *(float*)&response[4];  // Roll (4-7字节)
    data->euler[1] = *(float*)&response[8];  // Pitch (8-11字节)
    data->euler[2] = *(float*)&response[12]; // Yaw (12-15字节)

    return true;
}

// 初始化传感器
void be2_spi_init(void) {
    Serial_Printf("Initializing BE2 sensor...\r\n");

    // 复位传感器
    be2_write_register(BE2_REG_SYS_CONFIG, 0x80);
    wk_delay_ms(200);  // 延长复位等待时间

    // 进入命令模式（关键步骤）
    if(be2_enter_command_mode()) {
        Serial_Printf("Successfully entered command mode\r\n");
    } else {
        Serial_Printf("Failed to enter command mode - check connections\r\n");
    }
}

// 读取32位数据（兼容保留）
uint32_t be2_read_32bit(uint8_t reg) {
    Serial_Printf("Warning: Direct 32bit read not supported\r\n");
    return 0;
}

// 读取浮点数（兼容保留）
float be2_read_float(uint8_t reg) {
    Serial_Printf("Warning: Direct float read not supported\r\n");
    return 0.0f;
}

// 读取完整传感器数据
void be2_read_data(BE2_Data *data) {
    // 仅读取欧拉角（可扩展其他数据）
    if(!be2_read_euler(data)) {
        // 读取失败时清零
        data->euler[0] = 0.0f;
        data->euler[1] = 0.0f;
        data->euler[2] = 0.0f;
    }
}
