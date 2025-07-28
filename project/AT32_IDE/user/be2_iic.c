#include "be2_iic.h"
#include "Serial.h"

// 等待标志直到满足条件或超时，返回0成功，1超时
static uint8_t wait_flag_timeout(i2c_type *i2c, uint32_t flag, flag_status status, uint32_t timeout_us) {
    while(i2c_flag_get(i2c, flag) != status) {
        if(timeout_us-- == 0) return 1;
        wk_delay_us(1);
    }
    return 0;
}

// I2C总线空闲等待
static uint8_t wait_bus_idle(i2c_type *i2c) {
    return wait_flag_timeout(i2c, I2C_BUSYF_FLAG, RESET, I2C_TIMEOUT_US);
}

// 初始化I2C（根据你的工程环境自己配置）
// 这里只打印日志示例，用户需要自行初始化I2C2接口硬件
void BE2_I2C_Init(void) {
    Serial_Printf("BE2 I2C Init (using I2C2)\r\n");
    // 你这里可以调用 i2c_init(...) 等初始化函数
}

// 写1个寄存器
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

// 写多个寄存器
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

// 读1个寄存器
uint8_t BE2_I2C_ReadReg(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data) {
    if(wait_bus_idle(BE2_I2C)) return 1;

    // 先写寄存器地址
    i2c_start_generate(BE2_I2C);
    if(wait_flag_timeout(BE2_I2C, I2C_TDIS_FLAG, SET, I2C_TIMEOUT_US)) goto err_stop;

    i2c_transfer_addr_set(BE2_I2C, dev_addr);
    i2c_transfer_dir_set(BE2_I2C, I2C_DIR_TRANSMIT);
    if(wait_flag_timeout(BE2_I2C, I2C_ADDRF_FLAG, SET, I2C_TIMEOUT_US)) goto err_stop;
    i2c_flag_clear(BE2_I2C, I2C_ADDRF_FLAG);

    i2c_data_send(BE2_I2C, reg_addr);
    if(wait_flag_timeout(BE2_I2C, I2C_TDBE_FLAG, SET, I2C_TIMEOUT_US)) goto err_stop;

    // 重启信号准备读
    i2c_start_generate(BE2_I2C);
    if(wait_flag_timeout(BE2_I2C, I2C_TDIS_FLAG, SET, I2C_TIMEOUT_US)) goto err_stop;

    i2c_transfer_addr_set(BE2_I2C, dev_addr);
    i2c_transfer_dir_set(BE2_I2C, I2C_DIR_RECEIVE);

    // 禁止ACK，因为只读一个字节
    i2c_ack_enable(BE2_I2C, FALSE);

    if(wait_flag_timeout(BE2_I2C, I2C_ADDRF_FLAG, SET, I2C_TIMEOUT_US)) goto err_stop;
    i2c_flag_clear(BE2_I2C, I2C_ADDRF_FLAG);

    // 等待接收缓冲区满
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

// 读多个寄存器
uint8_t BE2_I2C_ReadRegs(uint8_t dev_addr, uint8_t reg_addr, uint8_t *buf, uint16_t len) {
    if(wait_bus_idle(BE2_I2C)) return 1;

    // 先写寄存器地址
    i2c_start_generate(BE2_I2C);
    if(wait_flag_timeout(BE2_I2C, I2C_TDIS_FLAG, SET, I2C_TIMEOUT_US)) goto err_stop;

    i2c_transfer_addr_set(BE2_I2C, dev_addr);
    i2c_transfer_dir_set(BE2_I2C, I2C_DIR_TRANSMIT);
    if(wait_flag_timeout(BE2_I2C, I2C_ADDRF_FLAG, SET, I2C_TIMEOUT_US)) goto err_stop;
    i2c_flag_clear(BE2_I2C, I2C_ADDRF_FLAG);

    i2c_data_send(BE2_I2C, reg_addr);
    if(wait_flag_timeout(BE2_I2C, I2C_TDBE_FLAG, SET, I2C_TIMEOUT_US)) goto err_stop;

    // 重启准备读数据
    i2c_start_generate(BE2_I2C);
    if(wait_flag_timeout(BE2_I2C, I2C_TDIS_FLAG, SET, I2C_TIMEOUT_US)) goto err_stop;

    i2c_transfer_addr_set(BE2_I2C, dev_addr);
    i2c_transfer_dir_set(BE2_I2C, I2C_DIR_RECEIVE);

    if(wait_flag_timeout(BE2_I2C, I2C_ADDRF_FLAG, SET, I2C_TIMEOUT_US)) goto err_stop;
    i2c_flag_clear(BE2_I2C, I2C_ADDRF_FLAG);

    for(uint16_t i = 0; i < len; i++) {
        // 最后一个字节禁用ACK
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

        // 生成起始信号
        i2c_start_generate(i2c);

        // 设置设备地址（写模式）
        i2c_transfer_addr_set(i2c, addr << 1);
        i2c_transfer_dir_set(i2c, I2C_DIR_TRANSMIT);

        // 等待地址应答标志
        while(!i2c_flag_get(i2c, I2C_ADDRF_FLAG) && timeout--) {
            if(i2c_flag_get(i2c, I2C_ACKFAIL_FLAG))
                break;
        }

        if(timeout && !i2c_flag_get(i2c, I2C_ACKFAIL_FLAG)) {
            Serial_Printf("Device found at 0x%02X\r\n", addr);
        }

        // 产生停止信号，清除标志位
        i2c_stop_generate(i2c);
        i2c_flag_clear(i2c, I2C_ADDRF_FLAG | I2C_ACKFAIL_FLAG);

        // 延时，防止总线冲突
        wk_delay_ms(1);
    }
    Serial_Printf("I2C Bus Scan Finished.\r\n");
}


