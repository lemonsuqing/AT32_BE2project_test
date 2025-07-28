#include "be2_spi.h"
#include "wk_spi.h"
#include "Serial.h"
#include "wk_system.h"
#include <string.h>
#include <math.h>

// 初始化传感器
void be2_spi_init(void) {
    // 重置传感器
	be2_write_register(BE2_REG_SYS_CONFIG, 0x80);
	wk_delay_ms(100);

	// 验证设备ID (应为0x32)
	uint8_t who_am_i = be2_read_register(BE2_REG_WHO_AM_I);
	if(who_am_i != 0x32) {
		// 打印错误信息而不是死循环
		Serial_Printf("Error: Incorrect device ID. Expected 0x32, got 0x%02X\r\n", who_am_i);
		// 继续初始化，但数据可能不可靠
	} else {
		Serial_Printf("Device ID verified: 0x%02X\r\n", who_am_i);
	}

//    // 验证设备ID (应为0x32)
//    if(be2_read_register(BE2_REG_WHO_AM_I) != 0x32) {
//        // 设备ID错误
//        while(1);
//    }

    // 配置数据输出 (使能时间戳、加速度、陀螺仪、四元数、欧拉角)
    be2_write_register(BE2_REG_DATA_ENABLE, 0x7F); // 0b01111111

    // 设置加速度范围 ±4g (默认)
    be2_write_register(BE2_REG_CTRL_0_A, 0x04);

    // 设置陀螺仪范围 ±2000dps (默认)
    be2_write_register(BE2_REG_CTRL_1_G, 0x10);

    // 设置输出频率 100Hz
    be2_write_register(BE2_REG_DATA_CTRL, 0x03);

    // 设置卡尔曼滤波 (默认)
    be2_write_register(BE2_REG_FILTER_CONFIG, 0x01);

    // 启动传感器
    be2_write_register(BE2_REG_SYS_CONFIG, 0x00);
}

// 读取单个寄存器
uint8_t be2_read_register(uint8_t reg) {
	uint8_t value;

	spi2_cs_enable();

	// 发送读命令 (RW=1) 和寄存器地址
	uint8_t cmd = (1 << 7) | reg;
	printf("SPI Read CMD: 0x%02X\r\n", cmd);
	uint8_t received = spi2_read_write_byte(cmd);

	// 接收数据 (发送dummy字节)
	value = spi2_read_write_byte(0xFF);
	printf("SPI Read Reg 0x%02X, Value: 0x%02X\r\n", reg, value);

	spi2_cs_disable();

	return value;
}

// 写入单个寄存器
void be2_write_register(uint8_t reg, uint8_t value) {
    spi2_cs_enable();

    // 发送写命令 (RW=0) 和寄存器地址
    spi2_read_write_byte(reg & 0x7F); // RW=0
    // 发送数据
    spi2_read_write_byte(value);

    spi2_cs_disable();
}

// 读取32位数据
uint32_t be2_read_32bit(uint8_t reg) {
    uint32_t value = 0;

    spi2_cs_enable();

    // 发送读命令和起始地址
    spi2_read_write_byte((1 << 7) | reg);

    // 读取4个字节 (小端格式)
    value |= spi2_read_write_byte(0xFF);
    value |= spi2_read_write_byte(0xFF) << 8;
    value |= spi2_read_write_byte(0xFF) << 16;
    value |= spi2_read_write_byte(0xFF) << 24;

    spi2_cs_disable();

    return value;
}

// 读取浮点数
float be2_read_float(uint8_t reg) {
    uint32_t raw = be2_read_32bit(reg);
    return *((float*)&raw);
}

// 读取完整传感器数据
void be2_read_data(BE2_Data *data) {
    // 时间戳 (单位: 0.002秒)
    uint32_t timestamp_raw = be2_read_32bit(BE2_REG_TIMESTAMP);
    data->timestamp = timestamp_raw * 0.002f;

    // 加速度
    data->acc[0] = be2_read_float(BE2_REG_ACC_X);
    data->acc[1] = be2_read_float(BE2_REG_ACC_Y);
    data->acc[2] = be2_read_float(BE2_REG_ACC_Z);

    // 陀螺仪
    data->gyro[0] = be2_read_float(BE2_REG_GYR_X);
    data->gyro[1] = be2_read_float(BE2_REG_GYR_Y);
    data->gyro[2] = be2_read_float(BE2_REG_GYR_Z);

    // 四元数
    data->quaternion[0] = be2_read_float(BE2_REG_QUAT_W); // w
    data->quaternion[1] = be2_read_float(BE2_REG_QUAT_X); // x
    data->quaternion[2] = be2_read_float(BE2_REG_QUAT_Y); // y
    data->quaternion[3] = be2_read_float(BE2_REG_QUAT_Z); // z

    // 欧拉角
    data->euler[0] = be2_read_float(BE2_REG_EULER_X); // roll
    data->euler[1] = be2_read_float(BE2_REG_EULER_Y); // pitch
    data->euler[2] = be2_read_float(BE2_REG_EULER_Z); // yaw
}
