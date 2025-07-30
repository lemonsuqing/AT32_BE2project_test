#include "be2_software_iic.h"

#define BE2_I2C_ADDR       0x32

static void delay_us(uint16_t us) {
    wk_delay_us(us);
}

static void i2c_start(void) {
    Serial_Printf("===== ������ʼ�ź� =====\r\n");
    BE2_SDA_H();
    BE2_SCL_H();
    delay_us(4);
    Serial_Printf("SDA�Ӹ߱�� (START)\r\n");
    BE2_SDA_L();
    delay_us(4);
    BE2_SCL_L();
    delay_us(2);
}

static void i2c_stop(void) {
    Serial_Printf( "===== ����ֹͣ�ź� =====\r\n");
    BE2_SCL_L();
    BE2_SDA_L();
    delay_us(4);
    BE2_SCL_H();
    delay_us(4);
    Serial_Printf("SDA�ӵͱ�� (STOP)\r\n");
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
    Serial_Printf("���ͱ���: %d (SCL�ߵ�ƽ����)\r\n", bit);
    delay_us(4);
    BE2_SCL_L();
    delay_us(2);
}

static uint8_t i2c_recv_bit(void) {
    uint8_t bit;
    BE2_SDA_H();  // �ͷ����ߣ��ɴӻ�����
    delay_us(2);
    BE2_SCL_H();
    delay_us(2);
    bit = BE2_SDA_READ();
    Serial_Printf("���ձ���: %d (SCL�ߵ�ƽ��ȡ)\r\n", bit);
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
    BE2_SDA_H();  // �ͷ�SDA���ȴ��ӻ�����
    delay_us(10);
    BE2_SCL_H();
    delay_us(10);
    ack = BE2_SDA_READ();  // 0=ACK��1=NACK
    BE2_SCL_L();
    delay_us(2);

    if (ack == 0) {
        Serial_Printf("�յ�ACK (�ӻ���Ӧ)\r\n");
    } else {
        Serial_Printf("�յ�NACK (�ӻ�����Ӧ)\r\n");
    }
    return (ack == 0);  // �����Ƿ��յ�ACK
}

static uint8_t i2c_recv_byte(uint8_t ack) {
    uint8_t byte = 0;
    Serial_Printf("��ʼ�����ֽ� (������: ");
    for (int i = 0; i < 8; i++) {
        byte <<= 1;
        byte |= i2c_recv_bit();
    }
    Serial_Printf(") -> 0x%02X\r\n", byte);

    // ����Ӧ���ź�
    i2c_send_bit(!ack);  // 0=ACK��1=NACK
    if (ack) {
        Serial_Printf("����NACK (��������)\r\n");
    } else {
        Serial_Printf("����ACK (��������)\r\n");
    }
    return byte;
}

void I2C_Bus_Reset(void) {
    Serial_Printf("===== ����I2C���� =====\r\n");
    BE2_SDA_H();
    for (int i = 0; i < 9; i++) {  // ����9��ʱ������
        BE2_SCL_L();
        delay_us(10);
        BE2_SCL_H();
        Serial_Printf("��λʱ������ %d\r\n", i+1);
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
    Serial_Printf("���ų�ʼ����� (SCL=H, SDA=H)\r\n");

    I2C_Bus_Reset();
}

uint8_t BE2_I2C_CheckDevice(void) {
    uint8_t ack = 0;
    Serial_Printf("\r\n===== ��ʼ����豸 =====");
    Serial_Printf("������ַ: 0x%02X (7λ)\r\n", BE2_I2C_ADDR);

    i2c_start();
    uint8_t write_addr = (BE2_I2C_ADDR << 1) | 0;  // д��ַ�����λ0��
    i2c_send_byte(write_addr);
    ack = i2c_wait_ack();  // �ȴ��豸Ӧ��

    i2c_stop();
    Serial_Printf("�豸�����: %s\r\n", ack ? "����" : "������");
    return ack;
}

uint8_t BE2_I2C_ReadReg(uint8_t reg) {
    uint8_t data = 0;
    Serial_Printf( "\r\n===== ��ȡ�Ĵ��� 0x%02X =====", reg);

    // ����1: ������ʼ�ź� + д��ַ
    i2c_start();
    uint8_t write_addr = (BE2_I2C_ADDR << 1) | 0;
    Serial_Printf( "����д��ַ: 0x%02X\r\n", write_addr);
    i2c_send_byte(write_addr);
    if (!i2c_wait_ack()) {
        Serial_Printf( "����: ����д��ַ��δ�յ�ACK\r\n");
        i2c_stop();
        return 0xFF;  // �����־
    }

    // ����2: ���ͼĴ�����ַ
    Serial_Printf( "���ͼĴ�����ַ: 0x%02X\r\n", reg);
    i2c_send_byte(reg);
    if (!i2c_wait_ack()) {
        Serial_Printf( "����: ���ͼĴ�����ַ��δ�յ�ACK\r\n");
        i2c_stop();
        return 0xFF;
    }

    // ����3: �����ظ���ʼ�ź� + ����ַ
    i2c_start();  // �ظ���ʼ
    uint8_t read_addr = (BE2_I2C_ADDR << 1) | 1;
    Serial_Printf( "���Ͷ���ַ: 0x%02X\r\n", read_addr);
    i2c_send_byte(read_addr);
    if (!i2c_wait_ack()) {
        Serial_Printf( "����: ���Ͷ���ַ��δ�յ�ACK\r\n");
        i2c_stop();
        return 0xFF;
    }

    // ����4: �������ݣ����һ���ֽڷ���NACK��
    data = i2c_recv_byte(1);  // 1=NACK
    Serial_Printf( "�ӼĴ���0x%02X��ȡ������: 0x%02X\r\n", reg, data);

    // ����5: ����ֹͣ�ź�
    i2c_stop();
    return data;
}

void BE2_Test_WhoAmI(void) {
    Serial_Printf("\r\n===== ��ʼWHOAMI���� =====");
    BE2_I2C_Init();  // ��ʼ��I2C

    // ����豸�Ƿ����
    if (BE2_I2C_CheckDevice()) {
        Serial_Printf( "�豸ͨ����������ʼ��ȡWHOAMI...\r\n");
        // ��ȡWHOAMI�Ĵ�����ͨ��Ϊ0x00��������ֲ�ȷ�ϣ�
        uint8_t whoami = BE2_I2C_ReadReg(0x00);

        // ��֤WHOAMIֵ���ο��ֲᣬ������ȷֵΪ0x1A��
        if (whoami == 0x1A) {
            Serial_Printf( "WHOAMI��֤�ɹ�: 0x%02X (����Ԥ��)\r\n", whoami);
        } else {
            Serial_Printf( "WHOAMI��֤ʧ��: 0x%02X (Ԥ��0x1A)\r\n", whoami);
        }
    } else {
        Serial_Printf( "�豸����Ӧ������ʧ��\r\n");
    }
    Serial_Printf("===== WHOAMI���Խ��� =====\r\n");
}
