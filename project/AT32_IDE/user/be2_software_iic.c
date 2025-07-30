#include "be2_software_iic.h"

#define BE2_ADDR  0x32  // 7位地址，已根据 MODE0=0 决定

static void i2c_delay_us(uint16_t us)
{
    wk_delay_us(us);
}

static void i2c_start(void)
{
    BE2_W_SDA(1);
    BE2_W_SCL(1);
    i2c_delay_us(2);
    BE2_W_SDA(0);
    i2c_delay_us(2);
    BE2_W_SCL(0);
}

static void i2c_stop(void)
{
    BE2_W_SDA(0);
    BE2_W_SCL(1);
    i2c_delay_us(2);
    BE2_W_SDA(1);
    i2c_delay_us(2);
}

static void i2c_send_bit(uint8_t bit)
{
    BE2_W_SDA(bit);
    i2c_delay_us(2);
    BE2_W_SCL(1);
    i2c_delay_us(2);
    BE2_W_SCL(0);
}

static uint8_t i2c_recv_bit(void)
{
    uint8_t bit;
    BE2_W_SDA(1);
    i2c_delay_us(1);
    BE2_W_SCL(1);
    i2c_delay_us(2);
    bit = BE2_R_SDA();
    BE2_W_SCL(0);
    return bit;
}

static void i2c_send_byte(uint8_t byte)
{
    for (uint8_t i = 0; i < 8; i++) {
        i2c_send_bit(byte & 0x80);
        byte <<= 1;
    }
    i2c_recv_bit(); // 忽略ACK
}

static uint8_t i2c_recv_byte(uint8_t ack)
{
    uint8_t byte = 0;
    for (uint8_t i = 0; i < 8; i++) {
        byte <<= 1;
        byte |= i2c_recv_bit();
    }
    i2c_send_bit(!ack); // 发送ACK=0 or NACK=1
    return byte;
}

void BE2_I2C_Init(void)
{
    gpio_init_type gpio_init_struct;
    crm_periph_clock_enable(CRM_GPIOB_PERIPH_CLOCK, TRUE);
    gpio_default_para_init(&gpio_init_struct);
    gpio_init_struct.gpio_pins = BE2_SCL_PIN | BE2_SDA_PIN;
    gpio_init_struct.gpio_mode = GPIO_MODE_OUTPUT;
    gpio_init_struct.gpio_out_type = GPIO_OUTPUT_OPEN_DRAIN;
    gpio_init_struct.gpio_pull = GPIO_PULL_UP;
    gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
    gpio_init(BE2_SCL_PORT, &gpio_init_struct);

    BE2_W_SCL(1);
    BE2_W_SDA(1);
}

void BE2_I2C_WriteByte(uint8_t reg, uint8_t data)
{
    i2c_start();
    i2c_send_byte(BE2_ADDR << 1 | 0); // Write
    i2c_send_byte(reg);
    i2c_send_byte(data);
    i2c_stop();
}

uint8_t BE2_I2C_ReadByte(uint8_t reg)
{
    uint8_t data;
    i2c_start();
    i2c_send_byte(BE2_ADDR << 1 | 0); // Write
    i2c_send_byte(reg);
    i2c_start();
    i2c_send_byte(BE2_ADDR << 1 | 1); // Read
    data = i2c_recv_byte(1); // 发送NACK
    i2c_stop();
    return data;
}

void BE2_I2C_ReadBytes(uint8_t reg, uint8_t *buf, uint8_t len)
{
    i2c_start();
    i2c_send_byte(BE2_ADDR << 1 | 0);
    i2c_send_byte(reg);
    i2c_start();
    i2c_send_byte(BE2_ADDR << 1 | 1);
    for (uint8_t i = 0; i < len; i++) {
        buf[i] = i2c_recv_byte(i < (len - 1) ? 0 : 1); // 最后一个字节发送NACK
    }
    i2c_stop();
}

void be2_test(void)
{
    uint8_t whoami = BE2_I2C_ReadByte(0x74);
    Serial_Printf("whoami = 0x%02X\r\n", whoami);
}
