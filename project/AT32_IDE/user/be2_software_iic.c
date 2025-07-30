#include "be2_software_iic.h"

#define BE2_I2C_ADDR       0x32

static void delay_us(uint16_t us) {
    wk_delay_us(us);
}

static void i2c_start(void) {
    Serial_Printf("===== Sending start signal =====\r\n");
    BE2_SDA_H();
    BE2_SCL_H();
    delay_us(4);
    Serial_Printf("SDA from high to low (START)\r\n");
    BE2_SDA_L();
    delay_us(4);
    BE2_SCL_L();
    delay_us(2);
}

static void i2c_stop(void) {
    Serial_Printf("===== Sending stop signal =====\r\n");
    BE2_SCL_L();
    BE2_SDA_L();
    delay_us(4);
    BE2_SCL_H();
    delay_us(4);
    Serial_Printf("SDA from low to high (STOP)\r\n");
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
    Serial_Printf("Sending bit: %d (SCL high for sampling)\r\n", bit);
    delay_us(4);
    BE2_SCL_L();
    delay_us(2);
}

static uint8_t i2c_recv_bit(void) {
    uint8_t bit;
    BE2_SDA_H();  // release line
    delay_us(2);
    BE2_SCL_H();
    delay_us(2);
    bit = BE2_SDA_READ();
    Serial_Printf("Receiving bit: %d (SCL high for reading)\r\n", bit);
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
    BE2_SDA_H();  // release SDA, wait for slave to pull low
    delay_us(10);
    BE2_SCL_H();
    delay_us(10);
    ack = BE2_SDA_READ();  // 0=ACK, 1=NACK
    BE2_SCL_L();
    delay_us(2);

    if (ack == 0) {
        Serial_Printf("Received ACK (slave responded)\r\n");
    } else {
        Serial_Printf("Received NACK (slave not responding)\r\n");
    }
    return (ack == 0);  // return whether ACK is received
}

static uint8_t i2c_recv_byte(uint8_t ack) {
    uint8_t byte = 0;
    Serial_Printf("Start receiving byte (binary: ");
    for (int i = 0; i < 8; i++) {
        byte <<= 1;
        byte |= i2c_recv_bit();
    }
    Serial_Printf(") -> 0x%02X\r\n", byte);

    // Send ACK/NACK
    i2c_send_bit(!ack);  // 0=ACK, 1=NACK
    if (ack) {
        Serial_Printf("Sending NACK (end receiving)\r\n");
    } else {
        Serial_Printf("Sending ACK (continue receiving)\r\n");
    }
    return byte;
}

void I2C_Bus_Reset(void) {
    Serial_Printf("===== Resetting I2C bus =====\r\n");
    BE2_SDA_H();
    for (int i = 0; i < 9; i++) {  // Send 9 clock pulses
        BE2_SCL_L();
        delay_us(10);
        BE2_SCL_H();
        Serial_Printf("Reset clock pulse %d\r\n", i+1);
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
    Serial_Printf("Pin initialization completed (SCL=H, SDA=H)\r\n");

    I2C_Bus_Reset();
}

uint8_t BE2_I2C_CheckDevice(void) {
    uint8_t ack = 0;
    Serial_Printf("\r\n===== Starting device detection =====");
    Serial_Printf("Base address: 0x%02X (7-bit)\r\n", BE2_I2C_ADDR);

    i2c_start();
    uint8_t write_addr = (BE2_I2C_ADDR << 1) | 0;  // Write address (LSB 0)
    i2c_send_byte(write_addr);
    ack = i2c_wait_ack();  // Wait for device ACK

    i2c_stop();
    Serial_Printf("Device detection result: %s\r\n", ack ? "Exists" : "Not exists");
    return ack;
}

uint8_t BE2_I2C_ReadReg(uint8_t reg) {
    uint8_t data = 0;
    Serial_Printf("\r\n===== Reading register 0x%02X =====", reg);

    // Step 1: Send start signal + write address
    i2c_start();
    uint8_t write_addr = (BE2_I2C_ADDR << 1) | 0;
    Serial_Printf("Sending write address: 0x%02X\r\n", write_addr);
    i2c_send_byte(write_addr);
    if (!i2c_wait_ack()) {
        Serial_Printf("Error: No ACK after sending write address\r\n");
        i2c_stop();
        return 0xFF;  // Error flag
    }

    // Step 2: Send register address
    Serial_Printf("Sending register address: 0x%02X\r\n", reg);
    i2c_send_byte(reg);
    if (!i2c_wait_ack()) {
        Serial_Printf("Error: No ACK after sending register address\r\n");
        i2c_stop();
        return 0xFF;
    }

    // Step 3: Send repeated start + read address
    i2c_start();  // Repeated start
    uint8_t read_addr = (BE2_I2C_ADDR << 1) | 1;
    Serial_Printf("Sending read address: 0x%02X\r\n", read_addr);
    i2c_send_byte(read_addr);
    if (!i2c_wait_ack()) {
        Serial_Printf("Error: No ACK after sending read address\r\n");
        i2c_stop();
        return 0xFF;
    }

    // Step 4: Receive data (send NACK for last byte)
    data = i2c_recv_byte(1);  // 1=NACK
    Serial_Printf("Data read from register 0x%02X: 0x%02X\r\n", reg, data);

    // Step 5: Send stop signal
    i2c_stop();
    return data;
}

void BE2_Test_WhoAmI(void) {
    Serial_Printf("\r\n===== Starting WHOAMI test =====");
    BE2_I2C_Init();  // Initialize I2C

    // Check if device exists
    if (BE2_I2C_CheckDevice()) {
        Serial_Printf("Device communication normal, starting to read WHOAMI...\r\n");
        // Read WHOAMI register (usually 0x00, confirm with datasheet)
        uint8_t whoami = BE2_I2C_ReadReg(0x00);

        // Verify WHOAMI value (reference datasheet, assume expected value is 0x1A)
        if (whoami == 0x1A) {
            Serial_Printf("WHOAMI verification successful: 0x%02X (as expected)\r\n", whoami);
        } else {
            Serial_Printf("WHOAMI verification failed: 0x%02X (expected 0x1A)\r\n", whoami);
        }
    } else {
        Serial_Printf("Device not responding, test failed\r\n");
    }
    Serial_Printf("===== WHOAMI test completed =====\r\n");
}
