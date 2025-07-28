#include "be2_spi.h"
#include "wk_spi.h"
#include "Serial.h"
#include "wk_system.h"
#include <string.h>

// 提前声明静态函数（解决函数调用顺序问题）
static float parse_float(uint8_t *data);
static uint16_t calculate_lrc(uint8_t *data, uint16_t len);
static bool be2_send_command(uint8_t *tx_buf, uint16_t tx_len, uint8_t *rx_buf, uint16_t rx_max_len, uint16_t *rx_len);

// 解析32位浮点型数据（小端格式，参考20220802用户手册3.3节数据传输格式）
static float parse_float(uint8_t *data) {
    union {
        uint8_t bytes[4];  // 小端模式：低位字节在前
        float value;
    } u;
    u.bytes[0] = data[0];
    u.bytes[1] = data[1];
    u.bytes[2] = data[2];
    u.bytes[3] = data[3];
    return u.value;
}

// 初始化片选引脚（参考20230707硬件手册2.2节引脚配置）
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

    LPMS_CS_HIGH();  // 默认拉高片选信号（20230707硬件手册3.2节SPI从设备片选）
}

// 进入命令模式（参考20220802用户手册3.5节"将传感器进入Command Mode"例程）
bool be2_enter_command_mode(void) {
    uint8_t tx_buf[11] = {0};
    uint8_t rx_buf[32] = {0};
    uint16_t rx_len = 0;

    tx_buf[0] = 0x3A;                  // 包头（20220802用户手册表2-1）
    tx_buf[1] = 0x01; tx_buf[2] = 0x00;// Sensor ID（默认1，低位在前）
    tx_buf[3] = 0x06; tx_buf[4] = 0x00;// 指令号（GOTO_COMMAND_MODE=0x0006）
    tx_buf[5] = 0x00; tx_buf[6] = 0x00;// 数据长度（0字节）
    uint16_t lrc = calculate_lrc(&tx_buf[1], 6);  // 计算LRC校验和
    tx_buf[7] = lrc & 0xFF;            // LRC低位
    tx_buf[8] = (lrc >> 8) & 0xFF;     // LRC高位
    tx_buf[9] = 0x0D; tx_buf[10] = 0x0A;// 包尾（0x0D 0x0A）

    if (!be2_send_command(tx_buf, 11, rx_buf, 32, &rx_len)) {
        Serial_Printf("❌ 命令模式切换失败\r\n");
        return false;
    }

    // 验证响应（ACK指令号=0x0000，20220802用户手册3.5节）
    return (rx_buf[0] == 0x3A && rx_buf[3] == 0x00 && rx_buf[4] == 0x00);
}

// 读取WHO_AM_I寄存器（20230707硬件手册6.1节，寄存器地址0x74，默认值0x32）
uint8_t be2_read_whoami_lpb(void) {
    uint8_t tx_buf[13] = {0};
    uint8_t rx_buf[32] = {0};
    uint16_t rx_len = 0;

    tx_buf[0] = 0x3A;                  // 包头
    tx_buf[1] = 0x01; tx_buf[2] = 0x00;// Sensor ID
    tx_buf[3] = 0x0B; tx_buf[4] = 0x00;// 指令号（READ_REGISTER=0x000B）
    tx_buf[5] = 0x02; tx_buf[6] = 0x00;// 数据长度（2字节）
    tx_buf[7] = 0x74; tx_buf[8] = 0x00;// 寄存器地址（0x74，低位在前）
    uint16_t lrc = calculate_lrc(&tx_buf[1], 8);  // 计算LRC校验和
    tx_buf[9] = lrc & 0xFF;
    tx_buf[10] = (lrc >> 8) & 0xFF;
    tx_buf[11] = 0x0D; tx_buf[12] = 0x0A;// 包尾

    be2_send_command(tx_buf, 13, rx_buf, 32, &rx_len);

    // 解析响应数据（有效数据从第7字节开始，20220802用户手册表2-1）
    return (rx_buf[0] == 0x3A) ? rx_buf[7] : 0x00;
}

// 计算LRC校验和（20220802用户手册3.3节，LRC=Sensor ID+指令号+数据长度+数据的和）
static uint16_t calculate_lrc(uint8_t *data, uint16_t len) {
    uint16_t sum = 0;
    for (uint16_t i = 0; i < len; i++) {
        sum += data[i];
    }
    return sum;
}

// 发送LPBUS指令并接收响应（遵循20220802用户手册3.3节LPBUS协议）
static bool be2_send_command(uint8_t *tx_buf, uint16_t tx_len, uint8_t *rx_buf, uint16_t rx_max_len, uint16_t *rx_len) {
    // 调试：打印片选信号状态（参考20230707硬件手册3.2节SPI片选控制）
    Serial_Printf("[SPI] 片选拉低，开始通信...\r\n");
    LPMS_CS_LOW();  // 拉低片选，选中传感器
    wk_delay_us(5);

    // 调试：打印发送的指令包（参考20220802用户手册表2-1数据包结构）
    Serial_Printf("[发送] 指令包长度: %d 字节，内容: ", tx_len);
    for (uint16_t i = 0; i < tx_len; i++) {
        Serial_Printf("0x%02X ", tx_buf[i]);
    }
    Serial_Printf("\r\n");

    // 发送命令帧
    for (uint16_t i = 0; i < tx_len; i++) {
        uint8_t recv_during_send = spi2_rw_byte(tx_buf[i]);  // 发送时的回传字节
        // 调试：打印每字节发送及同时接收的内容（SPI全双工特性）
        Serial_Printf("[发送] 第%d字节: 0x%02X，接收回传: 0x%02X\r\n", i, tx_buf[i], recv_during_send);
    }

    // 接收响应帧
    *rx_len = 0;
    uint8_t frame_end = 0;
    Serial_Printf("[接收] 开始接收响应...\r\n");
    while (*rx_len < rx_max_len && !frame_end) {
        rx_buf[*rx_len] = spi2_rw_byte(0xFF);  // 发送dummy字节读取响应
        // 调试：打印每字节接收内容
        Serial_Printf("[接收] 第%d字节: 0x%02X\r\n", *rx_len, rx_buf[*rx_len]);

        // 检测包尾（0x0D 0x0A，20220802用户手册表2-1定义）
        if (*rx_len > 0 && rx_buf[*rx_len-1] == 0x0D && rx_buf[*rx_len] == 0x0A) {
            frame_end = 1;
            Serial_Printf("[接收] 检测到包尾，结束接收\r\n");
        }
        (*rx_len)++;
    }

    // 调试：打印完整接收数据
    Serial_Printf("[接收] 响应总长度: %d 字节，完整内容: ", *rx_len);
    for (uint16_t i = 0; i < *rx_len; i++) {
        Serial_Printf("0x%02X ", rx_buf[i]);
    }
    Serial_Printf("\r\n");

    wk_delay_us(5);
    LPMS_CS_HIGH();  // 拉高片选，结束通信
    Serial_Printf("[SPI] 片选拉高，通信结束\r\n");

    return frame_end;
}

// 读取传感器数据（20220802用户手册3.3节，GET_IMU_DATA指令=0x0009）
bool be2_read_sensor_data(BE2_SensorData *data) {
    uint8_t tx_buf[11] = {0};
    uint8_t rx_buf[128] = {0};
    uint16_t rx_len = 0;

    // 构建GET_IMU_DATA指令包
    tx_buf[0] = 0x3A;                  // 包头
    tx_buf[1] = 0x01; tx_buf[2] = 0x00;// Sensor ID
    tx_buf[3] = 0x09; tx_buf[4] = 0x00;// 指令号（GET_IMU_DATA=0x0009）
    tx_buf[5] = 0x00; tx_buf[6] = 0x00;// 数据长度（0字节）
    uint16_t lrc = calculate_lrc(&tx_buf[1], 6);
    tx_buf[7] = lrc & 0xFF;
    tx_buf[8] = (lrc >> 8) & 0xFF;
    tx_buf[9] = 0x0D; tx_buf[10] = 0x0A;// 包尾

    // 发送指令并接收响应
    if (!be2_send_command(tx_buf, 11, rx_buf, 128, &rx_len)) {
        Serial_Printf("❌ 未收到完整帧\r\n");
        return false;
    }

    // 验证包头
    if (rx_buf[0] != 0x3A) {
        Serial_Printf("❌ 包头错误\r\n");
        return false;
    }

    // 解析数据长度（20220802用户手册表2-1，字节5-6为数据长度，低位在前）
    uint16_t data_len = (rx_buf[6] << 8) | rx_buf[5];
    if (data_len < 64) {  // 最小数据长度检查（默认输出8种数据，每种4-16字节）
        Serial_Printf("❌ 数据长度异常: %d\r\n", data_len);
        return false;
    }

    // 验证LRC校验（20220802用户手册3.3节，LRC位于数据后2字节）
    uint16_t recv_lrc = (rx_buf[7 + data_len + 1] << 8) | rx_buf[7 + data_len];
    uint16_t calc_lrc = calculate_lrc(&rx_buf[1], 6 + data_len);
    if (recv_lrc != calc_lrc) {
        Serial_Printf("❌ LRC校验失败(接收:0x%04X,计算:0x%04X)\r\n", recv_lrc, calc_lrc);
        return false;
    }

    // 解析数据（默认32位浮点模式，参考20220802用户手册表2-2）
    uint8_t *payload = &rx_buf[7];  // 有效数据起始位置
    int idx = 0;

    // 时间戳（4字节，UInt32）
    data->timestamp = (payload[idx+3] << 24) | (payload[idx+2] << 16) |
                     (payload[idx+1] << 8) | payload[idx];
    idx += 4;

    // 原始加速度计数据（3x4字节，Vector3f）
    for (int i = 0; i < 3; i++) {
        data->acc_raw[i] = parse_float(&payload[idx]);
        idx += 4;
    }

    // 校准后加速度计数据（3x4字节，Vector3f）
    for (int i = 0; i < 3; i++) {
        data->acc_calibrated[i] = parse_float(&payload[idx]);
        idx += 4;
    }

    // 原始陀螺仪数据（3x4字节，Vector3f）
    for (int i = 0; i < 3; i++) {
        data->gyro_raw[i] = parse_float(&payload[idx]);
        idx += 4;
    }

    // 静止偏差校准后陀螺仪数据（3x4字节，Vector3f）
    for (int i = 0; i < 3; i++) {
        data->gyro_bias_calibrated[i] = parse_float(&payload[idx]);
        idx += 4;
    }

    // 坐标轴校准后陀螺仪数据（3x4字节，Vector3f）
    for (int i = 0; i < 3; i++) {
        data->gyro_alignment[i] = parse_float(&payload[idx]);
        idx += 4;
    }

    // 四元数（4x4字节，Vector4f）
    for (int i = 0; i < 4; i++) {
        data->quaternion[i] = parse_float(&payload[idx]);
        idx += 4;
    }

    // 欧拉角（3x4字节，Vector3f）
    for (int i = 0; i < 3; i++) {
        data->euler[i] = parse_float(&payload[idx]);
        idx += 4;
    }

    return true;
}

// 传感器初始化（整合模式切换、设备验证等流程）
bool be2_sensor_init(void) {
    // 初始化片选引脚
    be2_spi_init_cs_gpio();
    wk_delay_ms(100);

    // 初始化SPI（配置为20230707硬件手册3.2节要求：CPOL=0, CPHA=0, 8位数据）
    wk_spi2_init();
    wk_delay_ms(100);

    // 进入命令模式（20220802用户手册3.1节，配置参数需在命令模式）
    if (!be2_enter_command_mode()) {
        return false;
    }
    wk_delay_ms(100);

    // 验证WHO_AM_I寄存器（20230707硬件手册6.1节，默认值0x32）
    uint8_t whoami = be2_read_whoami_lpb();
    if (whoami != 0x32) {
        Serial_Printf("❌ WHO_AM_I验证失败:0x%02X\r\n", whoami);
        return false;
    }

    // 切换到流模式（20220802用户手册3.1节，流模式下持续输出数据）
    uint8_t tx_buf[11] = {0};
    uint8_t rx_buf[32] = {0};
    uint16_t rx_len = 0;

    tx_buf[0] = 0x3A;                  // 包头
    tx_buf[1] = 0x01; tx_buf[2] = 0x00;// Sensor ID
    tx_buf[3] = 0x07; tx_buf[4] = 0x00;// 指令号（GOTO_STREAMING_MODE=0x0007）
    tx_buf[5] = 0x00; tx_buf[6] = 0x00;// 数据长度
    uint16_t lrc = calculate_lrc(&tx_buf[1], 6);
    tx_buf[7] = lrc & 0xFF;
    tx_buf[8] = (lrc >> 8) & 0xFF;
    tx_buf[9] = 0x0D; tx_buf[10] = 0x0A;// 包尾

    be2_send_command(tx_buf, 11, rx_buf, 32, &rx_len);
    wk_delay_ms(100);

    Serial_Printf("✅ 传感器初始化完成\r\n");
    return true;
}

// 读单个寄存器（参考硬件手册图3.1“SPI单个字节读取”时序）
uint8_t be2_spi_read_reg(uint8_t addr) {
    uint8_t tx_buf[2] = {0};
    uint8_t rx_buf[2] = {0};

    // 第一个字节：RW=1（读）+ 地址（addr的低7位）
    tx_buf[0] = 0x80 | (addr & 0x7F);  // 0x80=10000000b（RW=1）
    tx_buf[1] = 0x00;  // 第二个字节为dummy（用于接收数据）

    LPMS_CS_LOW();
    wk_delay_us(10);

    // 发送2字节（16个时钟脉冲，符合硬件手册“单个字节读取需16个时钟”的要求）
    rx_buf[0] = spi2_rw_byte(tx_buf[0]);  // 发送地址+RW位
    rx_buf[1] = spi2_rw_byte(tx_buf[1]);  // 接收寄存器数据

    wk_delay_us(10);
    LPMS_CS_HIGH();

    return rx_buf[1];  // 第二个字节为有效数据（硬件手册图3.1说明）
}
