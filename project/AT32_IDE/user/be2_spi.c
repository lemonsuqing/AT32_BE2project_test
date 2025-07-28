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

// 进入命令模式
bool lpms_enter_command_mode(void) {
    uint8_t cmd[] = { 0x3A, 0x01, 0x00, 0x06, 0xF1 };
    uint8_t rx[sizeof(cmd)];
    spi_write_buf(cmd, rx, sizeof(cmd));
    return true;
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
    uint8_t rx[32];
    uint8_t tx[32] = {0x00};

    // 接收 32 字节数据包
    spi_write_buf(tx, rx, 32);

    // 找到包头 0x3A
    int start = -1;
    for (int i = 0; i < 30; ++i) {
        if (rx[i] == 0x3A && rx[i + 31] == 0xF1) {
            start = i;
            break;
        }
    }

    if (start < 0) return false;

    // 数据解析（假设：roll/pitch/yaw 每个 float 占4字节）
    memcpy(&data->roll,  &rx[start + 7],  4);
    memcpy(&data->pitch, &rx[start + 11], 4);
    memcpy(&data->yaw,   &rx[start + 15], 4);

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
