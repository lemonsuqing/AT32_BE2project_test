#include "be2_spi.h"
#include "wk_spi.h"
#include "Serial.h"
#include "wk_system.h"
#include "at32f402_405_wk_config.h"
// SPI2_CS --> PC3




uint8_t spi2_rw_byte(uint8_t byte) {
    Serial_Printf("[SPI] Send: 0x%02X\r\n", byte);

    int timeout = 10000;
    while (spi_i2s_flag_get(SPI2, SPI_I2S_TDBE_FLAG) == RESET && timeout--) {
        // optionally delay or just spin
    }
    if (timeout <= 0) {
        Serial_Printf("[SPI] ⛔ TDBE timeout!\r\n");
        return 0xFF;
    }

    spi_i2s_data_transmit(SPI2, byte);

    timeout = 10000;
    while (spi_i2s_flag_get(SPI2, SPI_I2S_RDBF_FLAG) == RESET && timeout--) {
        // optionally delay or just spin
    }
    if (timeout <= 0) {
        Serial_Printf("[SPI] ⛔ RDBF timeout!\r\n");
        return 0xFF;
    }

    uint8_t recv = spi_i2s_data_receive(SPI2);
    Serial_Printf("[SPI] Recv: 0x%02X\r\n", recv);
    return recv;
}
void be2_spi_init_cs_gpio(void) {
    gpio_init_type gpio_init_struct;
    crm_periph_clock_enable(CRM_GPIOC_PERIPH_CLOCK, TRUE);

    gpio_default_para_init(&gpio_init_struct);
    gpio_init_struct.gpio_pins = LPMS_CS_PIN;
    gpio_init_struct.gpio_mode = GPIO_MODE_OUTPUT;
    gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
    gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
    gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
    gpio_init(LPMS_CS_PORT, &gpio_init_struct);

    LPMS_CS_HIGH();  // 默认拉高
}

// 计算 LRC 校验（用于 LPBUS 协议）
static uint16_t be2_calculate_lrc(uint8_t *data, uint8_t len) {
    uint16_t sum = 0;
    for (uint8_t i = 0; i < len; ++i) {
        sum += data[i];
    }
    return sum;
}

// 切换到命令模式（GOTO_COMMAND_MODE, CMD = 0x0006）
bool be2_enter_command_mode(void) {
    uint8_t tx[11];
    tx[0] = 0x3A;
    tx[1] = 0x01; tx[2] = 0x00;  // Sensor ID
    tx[3] = 0x06; tx[4] = 0x00;  // CMD: GOTO_COMMAND_MODE
    tx[5] = 0x00; tx[6] = 0x00;  // Length
    uint16_t lrc = be2_calculate_lrc(&tx[1], 6);
    tx[7] = lrc & 0xFF;
    tx[8] = (lrc >> 8) & 0xFF;
    tx[9] = 0x0D; tx[10] = 0x0A;

    LPMS_CS_LOW();
    wk_delay_us(5);

    bool ok = true;
    for (int i = 0; i < 11; i++) {
        uint8_t resp = spi2_rw_byte(tx[i]);
        if (resp == 0x00 && i == 0) {
            ok = false;  // 发第一个字节时如果返回 0x00，说明设备无响应
        }
    }

    wk_delay_us(5);
    LPMS_CS_HIGH();
    wk_delay_ms(100);

    return ok;
}

// 读取 WHO_AM_I（地址 0x74）对应 LPBUS 指令: READ_REGISTER(0x000B)，Length=2, Param=0x74 0x00
uint8_t be2_read_whoami_lpb(void) {
    uint8_t tx[13];
    tx[0] = 0x3A;
    tx[1] = 0x01; tx[2] = 0x00;        // Sensor ID
    tx[3] = 0x0B; tx[4] = 0x00;        // CMD: READ_REGISTER
    tx[5] = 0x02; tx[6] = 0x00;        // Length = 2
    tx[7] = 0x74; tx[8] = 0x00;        // Param: addr=0x74, padding

    uint16_t lrc = be2_calculate_lrc(&tx[1], 8);
    tx[9]  = lrc & 0xFF;
    tx[10] = (lrc >> 8) & 0xFF;
    tx[11] = 0x0D;
    tx[12] = 0x0A;

    uint8_t rx[32] = {0};

    LPMS_CS_LOW();
    wk_delay_us(5);

    // 1. 发命令，同时接收响应
    for (int i = 0; i < 13; ++i) {
        rx[i] = spi2_rw_byte(tx[i]);
    }

    // 2. 再发 dummy byte 继续接收
    for (int i = 13; i < 25; ++i) {
        rx[i] = spi2_rw_byte(0xFF);
        // 可选：你可以检测是否收到帧尾 0x0D 0x0A，提前 break
    }

    wk_delay_us(5);
    LPMS_CS_HIGH();

    Serial_Printf("↪ RX total:\r\n");
    for (int i = 0; i < 25; i++) {
        Serial_Printf("  rx[%02d] = 0x%02X\r\n", i, rx[i]);
    }

    // 响应结构：
    // [0] = 0x3A
    // [1-2] = ID
    // [3-4] = CMD
    // [5-6] = len
    // [7] = WHO_AM_I 值
    // ...

    if (rx[0] == 0x3A && rx[3] == 0x0B) {
        return rx[7]; // WHO_AM_I 应该在这里
    }

    return 0x00;
}
