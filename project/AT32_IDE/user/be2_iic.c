#include "be2_iic.h"
#include "Serial.h"

// �ȴ���־ֱ������������ʱ������0�ɹ���1��ʱ
static uint8_t wait_flag_timeout(i2c_type *i2c, uint32_t flag, flag_status status, uint32_t timeout_us) {
    while(i2c_flag_get(i2c, flag) != status) {
        if(timeout_us-- == 0) return 1;
        wk_delay_us(1);
    }
    return 0;
}

// I2C���߿��еȴ�
static uint8_t wait_bus_idle(i2c_type *i2c) {
    return wait_flag_timeout(i2c, I2C_BUSYF_FLAG, RESET, I2C_TIMEOUT_US);
}

// ��ʼ��I2C��������Ĺ��̻����Լ����ã�
// ����ֻ��ӡ��־ʾ�����û���Ҫ���г�ʼ��I2C2�ӿ�Ӳ��
void BE2_I2C_Init(void) {
    Serial_Printf("BE2 I2C Init (using I2C2)\r\n");
    // ��������Ե��� i2c_init(...) �ȳ�ʼ������
}

// д1���Ĵ���
uint8_t BE2_I2C_WriteReg(uint8_t dev_addr, uint8_t reg_addr, uint8_t data) {
    if(wait_bus_idle(BE2_I2C)) return 1;

    i2c_start_generate(BE2_I2C);
    if(wait_flag_timeout(BE2_I2C, I2C_TDIS_FLAG, SET, I2C_TIMEOUT_US)) goto err_stop;

    i2c_transfer_addr_set(BE2_I2C, dev_addr);
    i2c_transfer_dir_set(BE2_I2C, I2C_DIR_TRANSMIT);
    if(wait_flag_timeout(BE2_I2C, I2C_ADDRF_FLAG, SET, I2C_TIMEOUT_US)) goto err_stop;
    i2c_flag_clear(BE2_I2C, I2C_ADDRF_FLAG);

    i2c_data_send(BE2_I2C, reg_addr);
    if(wait_flag_timeout(BE2_I2C, I2C_TDBE_FLAG, SET, I2C_TIMEOUT_US)) goto err_stop;

    i2c_data_send(BE2_I2C, data);
    if(wait_flag_timeout(BE2_I2C, I2C_TDBE_FLAG, SET, I2C_TIMEOUT_US)) goto err_stop;

    i2c_stop_generate(BE2_I2C);
    if(wait_flag_timeout(BE2_I2C, I2C_STOPF_FLAG, SET, I2C_TIMEOUT_US)) return 1;
    i2c_flag_clear(BE2_I2C, I2C_STOPF_FLAG);

    return 0;

err_stop:
    i2c_stop_generate(BE2_I2C);
    return 1;
}

// д����Ĵ���
uint8_t BE2_I2C_WriteRegs(uint8_t dev_addr, uint8_t reg_addr, uint8_t *buf, uint16_t len) {
    if(wait_bus_idle(BE2_I2C)) return 1;

    i2c_start_generate(BE2_I2C);
    if(wait_flag_timeout(BE2_I2C, I2C_TDIS_FLAG, SET, I2C_TIMEOUT_US)) goto err_stop;

    i2c_transfer_addr_set(BE2_I2C, dev_addr);
    i2c_transfer_dir_set(BE2_I2C, I2C_DIR_TRANSMIT);
    if(wait_flag_timeout(BE2_I2C, I2C_ADDRF_FLAG, SET, I2C_TIMEOUT_US)) goto err_stop;
    i2c_flag_clear(BE2_I2C, I2C_ADDRF_FLAG);

    i2c_data_send(BE2_I2C, reg_addr);
    if(wait_flag_timeout(BE2_I2C, I2C_TDBE_FLAG, SET, I2C_TIMEOUT_US)) goto err_stop;

    for(uint16_t i = 0; i < len; i++) {
        i2c_data_send(BE2_I2C, buf[i]);
        if(wait_flag_timeout(BE2_I2C, I2C_TDBE_FLAG, SET, I2C_TIMEOUT_US)) goto err_stop;
    }

    i2c_stop_generate(BE2_I2C);
    if(wait_flag_timeout(BE2_I2C, I2C_STOPF_FLAG, SET, I2C_TIMEOUT_US)) return 1;
    i2c_flag_clear(BE2_I2C, I2C_STOPF_FLAG);

    return 0;

err_stop:
    i2c_stop_generate(BE2_I2C);
    return 1;
}

// ��1���Ĵ���
uint8_t BE2_I2C_ReadReg(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data) {
    if(wait_bus_idle(BE2_I2C)) return 1;

    // ��д�Ĵ�����ַ
    i2c_start_generate(BE2_I2C);
    if(wait_flag_timeout(BE2_I2C, I2C_TDIS_FLAG, SET, I2C_TIMEOUT_US)) goto err_stop;

    i2c_transfer_addr_set(BE2_I2C, dev_addr);
    i2c_transfer_dir_set(BE2_I2C, I2C_DIR_TRANSMIT);
    if(wait_flag_timeout(BE2_I2C, I2C_ADDRF_FLAG, SET, I2C_TIMEOUT_US)) goto err_stop;
    i2c_flag_clear(BE2_I2C, I2C_ADDRF_FLAG);

    i2c_data_send(BE2_I2C, reg_addr);
    if(wait_flag_timeout(BE2_I2C, I2C_TDBE_FLAG, SET, I2C_TIMEOUT_US)) goto err_stop;

    // �����ź�׼����
    i2c_start_generate(BE2_I2C);
    if(wait_flag_timeout(BE2_I2C, I2C_TDIS_FLAG, SET, I2C_TIMEOUT_US)) goto err_stop;

    i2c_transfer_addr_set(BE2_I2C, dev_addr);
    i2c_transfer_dir_set(BE2_I2C, I2C_DIR_RECEIVE);

    // ��ֹACK����Ϊֻ��һ���ֽ�
    i2c_ack_enable(BE2_I2C, FALSE);

    if(wait_flag_timeout(BE2_I2C, I2C_ADDRF_FLAG, SET, I2C_TIMEOUT_US)) goto err_stop;
    i2c_flag_clear(BE2_I2C, I2C_ADDRF_FLAG);

    // �ȴ����ջ�������
    if(wait_flag_timeout(BE2_I2C, I2C_RDBF_FLAG, SET, I2C_TIMEOUT_US)) goto err_stop;

    *data = i2c_data_receive(BE2_I2C);

    i2c_stop_generate(BE2_I2C);
    if(wait_flag_timeout(BE2_I2C, I2C_STOPF_FLAG, SET, I2C_TIMEOUT_US)) return 1;
    i2c_flag_clear(BE2_I2C, I2C_STOPF_FLAG);

    i2c_ack_enable(BE2_I2C, TRUE);

    return 0;

err_stop:
    i2c_stop_generate(BE2_I2C);
    i2c_ack_enable(BE2_I2C, TRUE);
    return 1;
}

// ������Ĵ���
uint8_t BE2_I2C_ReadRegs(uint8_t dev_addr, uint8_t reg_addr, uint8_t *buf, uint16_t len) {
    if(wait_bus_idle(BE2_I2C)) return 1;

    // ��д�Ĵ�����ַ
    i2c_start_generate(BE2_I2C);
    if(wait_flag_timeout(BE2_I2C, I2C_TDIS_FLAG, SET, I2C_TIMEOUT_US)) goto err_stop;

    i2c_transfer_addr_set(BE2_I2C, dev_addr);
    i2c_transfer_dir_set(BE2_I2C, I2C_DIR_TRANSMIT);
    if(wait_flag_timeout(BE2_I2C, I2C_ADDRF_FLAG, SET, I2C_TIMEOUT_US)) goto err_stop;
    i2c_flag_clear(BE2_I2C, I2C_ADDRF_FLAG);

    i2c_data_send(BE2_I2C, reg_addr);
    if(wait_flag_timeout(BE2_I2C, I2C_TDBE_FLAG, SET, I2C_TIMEOUT_US)) goto err_stop;

    // ����׼��������
    i2c_start_generate(BE2_I2C);
    if(wait_flag_timeout(BE2_I2C, I2C_TDIS_FLAG, SET, I2C_TIMEOUT_US)) goto err_stop;

    i2c_transfer_addr_set(BE2_I2C, dev_addr);
    i2c_transfer_dir_set(BE2_I2C, I2C_DIR_RECEIVE);

    if(wait_flag_timeout(BE2_I2C, I2C_ADDRF_FLAG, SET, I2C_TIMEOUT_US)) goto err_stop;
    i2c_flag_clear(BE2_I2C, I2C_ADDRF_FLAG);

    for(uint16_t i = 0; i < len; i++) {
        // ���һ���ֽڽ���ACK
        if(i == len - 1) i2c_ack_enable(BE2_I2C, FALSE);
        else i2c_ack_enable(BE2_I2C, TRUE);

        if(wait_flag_timeout(BE2_I2C, I2C_RDBF_FLAG, SET, I2C_TIMEOUT_US)) goto err_stop;

        buf[i] = i2c_data_receive(BE2_I2C);
    }

    i2c_stop_generate(BE2_I2C);
    if(wait_flag_timeout(BE2_I2C, I2C_STOPF_FLAG, SET, I2C_TIMEOUT_US)) return 1;
    i2c_flag_clear(BE2_I2C, I2C_STOPF_FLAG);

    i2c_ack_enable(BE2_I2C, TRUE);

    return 0;

err_stop:
    i2c_stop_generate(BE2_I2C);
    i2c_ack_enable(BE2_I2C, TRUE);
    return 1;
}

void I2C_Scan(i2c_type *i2c) {
    uint8_t addr;
    uint32_t timeout;

    Serial_Printf("Start I2C Bus Scan...\r\n");
    for(addr = 3; addr < 0x78; addr++) {
        timeout = 100000;

        // ������ʼ�ź�
        i2c_start_generate(i2c);

        // �����豸��ַ��дģʽ��
        i2c_transfer_addr_set(i2c, addr << 1);
        i2c_transfer_dir_set(i2c, I2C_DIR_TRANSMIT);

        // �ȴ���ַӦ���־
        while(!i2c_flag_get(i2c, I2C_ADDRF_FLAG) && timeout--) {
            if(i2c_flag_get(i2c, I2C_ACKFAIL_FLAG))
                break;
        }

        if(timeout && !i2c_flag_get(i2c, I2C_ACKFAIL_FLAG)) {
            Serial_Printf("Device found at 0x%02X\r\n", addr);
        }

        // ����ֹͣ�źţ������־λ
        i2c_stop_generate(i2c);
        i2c_flag_clear(i2c, I2C_ADDRF_FLAG | I2C_ACKFAIL_FLAG);

        // ��ʱ����ֹ���߳�ͻ
        wk_delay_ms(1);
    }
    Serial_Printf("I2C Bus Scan Finished.\r\n");
}


