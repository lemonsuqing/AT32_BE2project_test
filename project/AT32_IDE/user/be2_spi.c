#include "be2_spi.h"
#include "wk_spi.h"
#include "Serial.h"
#include "wk_system.h"
#include <string.h>
#include <math.h>

#define LPMS_CS_LOW()   gpio_bits_reset(GPIOC, GPIO_PINS_3)
#define LPMS_CS_HIGH()  gpio_bits_set(GPIOC, GPIO_PINS_3)

static void spi_write_buf(uint8_t *tx, uint8_t *rx, uint16_t len) {
    LPMS_CS_LOW();
    wk_delay_ms(20);
    for (uint16_t i = 0; i < len; i++) {
        rx[i] = spi2_rw_byte(tx[i]);
    }
    wk_delay_ms(20);
    LPMS_CS_HIGH();
}

static void print_buffer(const char *prefix, uint8_t *buf, uint16_t len) {
    Serial_Printf("%s:", prefix);
    for (uint16_t i = 0; i < len; i++) {
        Serial_Printf(" %02X", buf[i]);
    }
    Serial_Printf("\r\n");
}

// SPI发送接收一个缓冲区数据（不加延时）
static void spi_write_buf_debug(uint8_t *tx, uint8_t *rx, uint16_t len) {
    LPMS_CS_LOW();
    for (uint16_t i = 0; i < len; i++) {
        rx[i] = spi2_rw_byte(tx[i]);
    }
    LPMS_CS_HIGH();
}

//static void spi_write_buf(uint8_t *tx, uint8_t *rx, uint16_t len) {
//    LPMS_CS_LOW();
//    // wk_delay_ms(20); // 建议去掉延时
//    for (uint16_t i = 0; i < len; i++) {
//        rx[i] = spi2_rw_byte(tx[i]);
//    }
//    // wk_delay_ms(20); // 建议去掉延时
//    LPMS_CS_HIGH();
//}

// 进入命令模式
bool lpms_enter_command_mode(void) {
	uint8_t cmd[] = { 0x3A, 0x01, 0x00, 0x06, 0xF1 };
	    uint8_t rx[32];
	    uint8_t tx[32] = {0};
	    uint8_t valid = 0;

	    spi_write_buf_debug(cmd, rx, sizeof(cmd));
	    print_buffer("[CMD Mode TX]", cmd, sizeof(cmd));
	    print_buffer("[CMD Mode RX]", rx, sizeof(cmd));

	    wk_delay_ms(50); // 增加延时

	    for (int i = 0; i < 10; i++) { // 多次读，扩大尝试次数
	        spi_write_buf_debug(tx, rx, 32);
	        print_buffer("[CMD Mode Read RX]", rx, 32);

	        if (rx[0] == 0x3A && rx[31] == 0xF1) {
	            valid = 1;
	            break;
	        }

	        wk_delay_ms(10);
	    }

	    return valid == 1;
}

// 设置输出为欧拉角
void lpms_set_output_format_euler(void) {
    uint8_t cmd[] = {
        0x3A, 0x02, 0x03, // header, command, payload_len
        0x00, 0x03, 0x01, // payload: set output euler
        0xF1
    };
    uint8_t rx[sizeof(cmd)];
    spi_write_buf(cmd, rx, sizeof(cmd));
}

// 设置输出频率
void lpms_set_output_frequency(uint8_t hz) {
    uint8_t cmd[] = {
        0x3A, 0x03, 0x02,
        0x01, hz,
        0xF1
    };
    uint8_t rx[sizeof(cmd)];
    spi_write_buf(cmd, rx, sizeof(cmd));
}

// 启动数据输出
void lpms_start_streaming(void) {
    uint8_t cmd[] = {
        0x3A, 0x04, 0x01,
        0x01, // start streaming
        0xF1
    };
    uint8_t rx[sizeof(cmd)];
    spi_write_buf(cmd, rx, sizeof(cmd));
}

// 读取欧拉角数据（从传感器自动发回的数据包中解析）
bool lpms_read_euler_angles(lpms_data_t *data) {
    uint8_t tx[32] = {0};
    uint8_t rx[32];

    spi_write_buf_debug(tx, rx, 32);
    print_buffer("[Read Euler TX]", tx, 32);
    print_buffer("[Read Euler RX]", rx, 32);

    if (rx[0] != 0x3A || rx[31] != 0xF1) {
        Serial_Printf("Invalid data packet header or footer\r\n");
        return false;
    }

    // 解析roll, pitch, yaw，假设为float格式
    memcpy(&data->roll,  &rx[7],  4);
    memcpy(&data->pitch, &rx[11], 4);
    memcpy(&data->yaw,   &rx[15], 4);

    return true;
}

void lpms_be2_init(void) {
    // 初始化CS引脚
    gpio_init_type gpio_init_struct;
//    crm_periph_clock_enable(CRM_GPIOC, TRUE);
    gpio_default_para_init(&gpio_init_struct);
    gpio_init_struct.gpio_pins = GPIO_PINS_3;
    gpio_init_struct.gpio_mode = GPIO_MODE_OUTPUT;
    gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
    gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
    gpio_init(GPIOC, &gpio_init_struct);

    LPMS_CS_HIGH();
}
