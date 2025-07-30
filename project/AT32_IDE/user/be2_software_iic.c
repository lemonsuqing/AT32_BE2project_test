#include "be2_software_iic.h"

#define BE2_I2C_ADDR       0x32

static void delay_us(uint16_t us) {
    wk_delay_us(us);
}

static void i2c_start(void) {
    BE2_SDA_H();
    BE2_SCL_H();
    delay_us(4);
    BE2_SDA_L();
    delay_us(4);
    BE2_SCL_L();
}

static void i2c_stop(void) {
    BE2_SCL_L();
    BE2_SDA_L();
    delay_us(4);
    BE2_SCL_H();
    delay_us(4);
    BE2_SDA_H();
    delay_us(4);
}

static void i2c_send_bit(uint8_t bit) {
    if (bit)
        BE2_SDA_H();
    else
        BE2_SDA_L();

    delay_us(2);
    BE2_SCL_H();
    delay_us(4);
    BE2_SCL_L();
    delay_us(2);
}

static uint8_t i2c_recv_bit(void) {
    uint8_t bit;
    BE2_SDA_H(); // release line
    delay_us(2);
    BE2_SCL_H();
    delay_us(2);
    bit = BE2_SDA_READ();
    BE2_SCL_L();
    delay_us(2);
    return bit;
}

static void i2c_send_byte(uint8_t byte) {
    for (int i = 0; i < 8; i++) {
        i2c_send_bit(byte & 0x80);
        byte <<= 1;
    }
}

static uint8_t i2c_wait_ack(void) {
    uint8_t ack;
    BE2_SDA_H(); // release SDA
    delay_us(1);
    BE2_SCL_H();
    delay_us(2);
    ack = BE2_SDA_READ();
    BE2_SCL_L();
    return (ack == 0); // 0 = ACK
}

static uint8_t i2c_recv_byte(uint8_t ack) {
    uint8_t byte = 0;
    for (int i = 0; i < 8; i++) {
        byte <<= 1;
        byte |= i2c_recv_bit();
    }
    i2c_send_bit(!ack); // 0=ACK, 1=NACK
    return byte;
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
}

uint8_t BE2_I2C_CheckDevice(void) {
    i2c_start();
    i2c_send_byte((BE2_I2C_ADDR << 1) | 0);  // Write address
    uint8_t ack = i2c_wait_ack();
    i2c_stop();
    return ack;
}

uint8_t BE2_I2C_ReadReg(uint8_t reg) {
    uint8_t data;
    i2c_start();
    i2c_send_byte((BE2_I2C_ADDR << 1) | 0);  // Write
    i2c_wait_ack();
    i2c_send_byte(reg);
    i2c_wait_ack();
    i2c_start();
    i2c_send_byte((BE2_I2C_ADDR << 1) | 1);  // Read
    i2c_wait_ack();
    data = i2c_recv_byte(1);  // NACK
    i2c_stop();
    return data;
}

void BE2_Test_WhoAmI(void) {
    Serial_Printf("Testing LPMS-BE2 I2C...\r\n");

    if (BE2_I2C_CheckDevice()) {
        Serial_Printf("Device responded OK!\r\n");
        uint8_t whoami = BE2_I2C_ReadReg(0x00);
        Serial_Printf("WHOAMI = 0x%02X\r\n", whoami);
    } else {
        Serial_Printf("Device not found or no ACK.\r\n");
    }
}
