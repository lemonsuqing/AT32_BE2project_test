//#include "hardware_oled.h"
//#include "i2c_application.h"  // 包含wk库I2C应用层头文件
//#include "OLED_Font.h"
//
//// 外部声明I2C句柄（需在i2c配置文件中初始化）
//extern i2c_handle_type hi2c2;
//
///* OLED显存缓冲区 [页][列] */
//static uint8_t OLED_RAM[8][128];
//
///* 屏幕尺寸参数 */
//#define displayWidth   128
//#define displayHeight  64
//
//static void delay_ms(uint32_t us){
//	wk_delay_ms(us);
//}
//
///**
// * @brief  通过I2C写入一个字节（地址+数据）
// * @param  addr: 寄存器地址（0x00为命令，0x40为数据）
// * @param  data: 要写入的数据
// * @retval 无
// */
//void wk_i2c_write_byte(uint8_t addr, uint8_t data)
//{
//    uint8_t tx_buf[2] = {addr, data};
//    i2c_status_type status;
//
//    // 使用wk库I2C发送函数
//    status = i2c_master_transmit(&hi2c2, OLED_I2C_ADDRESS, tx_buf, 2, 1000);
//    Serial_Printf("I2C addr=%02X, data=%02X, status=%d\n", OLED_I2C_ADDRESS, data, status);
//    if (status != I2C_OK)
//    {
//        // 可添加错误处理（如I2C重置）
//    	Serial_Printf("I2C send failed!");
//        i2c_reset_ctrl2_register(&hi2c2);
//    }
//}
//
///**
// * @brief  写命令
// * @param  IIC_Command: 命令字节
// * @retval 无
// */
//void WriteCmd(uint8_t IIC_Command)
//{
//    wk_i2c_write_byte(0x00, IIC_Command);
//}
//
///**
// * @brief  写数据
// * @param  IIC_Data: 数据字节
// * @retval 无
// */
//void WriteDat(uint8_t IIC_Data)
//{
//    wk_i2c_write_byte(0x40, IIC_Data);
//}
//
///**
// * @brief  初始化OLED
// * @param  无
// * @retval 无
// */
//void OLED_Init(void)
//{
//    // 初始化前延时，确保硬件就绪
//    delay_ms(500);
//
//    // OLED初始化命令序列（与原功能一致）
//    WriteCmd(0xAE);  // 关闭显示
//    WriteCmd(0x20);  // 设置内存寻址模式
//    WriteCmd(0x10);  // 页寻址模式
//
//    WriteCmd(0xb0);  // 设置页地址（初始页0）
//    WriteCmd(0x00);  // 列低地址
//    WriteCmd(0x10);  // 列高地址
//
//    WriteCmd(0xc8);  // COM扫描方向反转
//    WriteCmd(0x40);  // 起始行地址
//    WriteCmd(0x81);  // 对比度控制
//    WriteCmd(0xff);  // 最大对比度
//
//    WriteCmd(0xa1);  // 段重映射（0->127）
//    WriteCmd(0xa6);  // 正常显示（非反显）
//    WriteCmd(0xa8);  // 设置多路复用比
//    WriteCmd(0x3F);  // 64行复用
//
//    WriteCmd(0xa4);  // 显示RAM内容
//    WriteCmd(0xd3);  // 显示偏移
//    WriteCmd(0x00);  // 无偏移
//
//    WriteCmd(0xd5);  // 显示时钟分频
//    WriteCmd(0xf0);  // 分频系数
//    WriteCmd(0xd9);  // 预充电周期
//    WriteCmd(0x22);  // 预充电设置
//
//    WriteCmd(0xda);  // COM引脚配置
//    WriteCmd(0x12);  // 硬件配置
//    WriteCmd(0xdb);  // VCOMH设置
//    WriteCmd(0x20);  // 0.77xVcc
//
//    WriteCmd(0x8d);  // 电荷泵设置
//    WriteCmd(0x14);  // 开启电荷泵
//    WriteCmd(0xaf);  // 开启显示
//
//    OLED_FullyClear();  // 清屏初始化
//}
//
///**
// * @brief  唤醒OLED（开启电荷泵）
// * @param  无
// * @retval 无
// */
//void OLED_ON(void)
//{
//    WriteCmd(0X8D);  // 电荷泵设置
//    WriteCmd(0X14);  // 开启电荷泵
//    WriteCmd(0XAF);  // 开启显示
//}
//
///**
// * @brief  休眠OLED（关闭电荷泵）
// * @param  无
// * @retval 无
// */
//void OLED_OFF(void)
//{
//    WriteCmd(0X8D);  // 电荷泵设置
//    WriteCmd(0X10);  // 关闭电荷泵
//    WriteCmd(0XAE);  // 关闭显示
//}
//
///**
// * @brief  将缓冲区数据刷新到OLED屏幕
// * @param  无
// * @retval 无
// */
//void OLED_RefreshRAM(void)
//{
//    uint8_t page, col;
//
//    for (page = 0; page < 8; page++)
//    {
//        // 设置页地址
//        WriteCmd(0xb0 + page);
//        // 设置列地址（低4位+高4位）
//        WriteCmd(0x00);
//        WriteCmd(0x10);
//
//        // 写入当前页的所有列数据
//        for (col = 0; col < 128; col++)
//        {
//            WriteDat(OLED_RAM[page][col]);
//        }
//    }
//}
//
///**
// * @brief  清除显存缓冲区（不刷新屏幕）
// * @param  无
// * @retval 无
// */
//void OLED_ClearRAM(void)
//{
//    uint16_t m, n;
//    for (m = 0; m < 8; m++)
//    {
//        for (n = 0; n < 128; n++)
//        {
//            OLED_RAM[m][n] = 0x00;
//        }
//    }
//}
//
///**
// * @brief  全屏填充指定数据
// * @param  fill_Data: 填充数据（0x00为黑，0xFF为白）
// * @retval 无
// */
//void OLED_FullyFill(uint8_t fill_Data)
//{
//    uint16_t m, n;
//    for (m = 0; m < 8; m++)
//    {
//        for (n = 0; n < 128; n++)
//        {
//            OLED_RAM[m][n] = fill_Data;
//        }
//    }
//    OLED_RefreshRAM();
//}
//
///**
// * @brief  全屏清除（填充0x00）
// * @param  无
// * @retval 无
// */
//void OLED_FullyClear(void)
//{
//    OLED_FullyFill(0x00);
//}
//
///**
// * @brief  设置指定坐标像素点状态
// * @param  x: 列坐标（0~127）
// * @param  y: 行坐标（0~63）
// * @param  set_pixel: 状态（SET_PIXEL点亮，RESET_PIXEL熄灭）
// * @retval 无
// */
//void OLED_SetPixel(int16_t x, int16_t y, uint8_t set_pixel)
//{
//    if (x >= 0 && x < displayWidth && y >= 0 && y < displayHeight)
//    {
//        uint8_t page = y / 8;    // 计算页地址
//        uint8_t bit  = y % 8;    // 计算页内位偏移
//
//        if (set_pixel == SET_PIXEL)
//        {
//            OLED_RAM[page][x] |= (1 << bit);  // 置位
//        }
//        else
//        {
//            OLED_RAM[page][x] &= ~(1 << bit); // 清零
//        }
//    }
//}
//
///**
// * @brief  获取指定坐标像素点状态
// * @param  x: 列坐标（0~127）
// * @param  y: 行坐标（0~63）
// * @retval 像素状态（SET_PIXEL/RESET_PIXEL）
// */
//PixelStatus OLED_GetPixel(int16_t x, int16_t y)
//{
//    if (x >= 0 && x < displayWidth && y >= 0 && y < displayHeight)
//    {
//        uint8_t page = y / 8;
//        uint8_t bit  = y % 8;
//
//        if (OLED_RAM[page][x] & (1 << bit))
//        {
//            return SET_PIXEL;
//        }
//    }
//    return RESET_PIXEL;
//}
//
///**
// * @brief  显示ASCII字符串
// * @param  x,y: 起始坐标
// * @param  ch[]: 字符串
// * @param  TextSize: 字体大小（1:6x8；2:8x16）
// * @retval 无
// */
//void OLED_ShowStr(int16_t x, int16_t y, uint8_t ch[], uint8_t TextSize)
//{
//    if (x >= 0 && x < displayWidth && y >= 0 && y < displayHeight)
//    {
//        int32_t c = 0;
//        uint8_t j = 0;
//
//        switch (TextSize)
//        {
//            case 1:  // 6x8字体
//            {
//                while (ch[j] != '\0')
//                {
//                    c = ch[j] - 32;  // 计算偏移（空格为32）
//                    if (c < 0) break;  // 无效字符
//
//                    // 换行处理
//                    if (x >= 125 || (127 - x < 6))
//                    {
//                        x = 0;
//                        y += 8;
//                        if (63 - y < 8) break;
//                    }
//
//                    // 绘制6x8字符
//                    for (uint8_t m = 0; m < 6; m++)
//                    {
//                        for (uint8_t n = 0; n < 8; n++)
//                        {
//                            OLED_SetPixel(x + m, y + n, (F6x8[c][m] >> n) & 0x01);
//                        }
//                    }
//                    x += 6;
//                    j++;
//                }
//            } break;
//
//            case 2:  // 8x16字体
//            {
//                while (ch[j] != '\0')
//                {
//                    c = ch[j] - 32;
//                    if (c < 0) break;
//
//                    // 换行处理
//                    if (x >= 127 || (127 - x < 8))
//                    {
//                        x = 0;
//                        y += 16;
//                        if (63 - y < 16) break;
//                    }
//
//                    // 绘制8x16字符
//                    for (uint8_t m = 0; m < 2; m++)  // 分上下两页
//                    {
//                        for (uint8_t n = 0; n < 8; n++)  // 列
//                        {
//                            for (uint8_t i = 0; i < 8; i++)  // 行
//                            {
//                            	OLED_SetPixel(x + n, y + i + m*8, (F8X16[c][n + m*8] >> i) & 0x01);
//                            }
//                        }
//                    }
//                    x += 8;
//                    j++;
//                }
//            } break;
//        }
//    }
//    OLED_RefreshRAM();
//}
//
///**
// * @brief  显示16x16汉字
// * @param  x,y: 起始坐标
// * @param  ch[]: 汉字GB2312编码
// * @retval 无
// */
//void OLED_ShowCN(int16_t x, int16_t y, uint8_t* ch)
//{
//    if (x >= 0 && x < displayWidth && y >= 0 && y < displayHeight)
//    {
//        int32_t len = 0;
//        uint8_t offset = sizeof(F16x16_CN[0].index);  // 汉字索引长度（2字节）
//
//        while (ch[len] != '\0')
//        {
//            // 换行处理
//            if (x >= 127 || (127 - x < 16))
//            {
//                x = 0;
//                y += 16;
//                if (63 - y < 16) break;
//            }
//
//            // 查找汉字点阵
//            for (uint8_t i = 0; i < sizeof(F16x16_CN)/sizeof(GB2312_CN); i++)
//            {
//                if (F16x16_CN[i].index[0] == ch[len] && F16x16_CN[i].index[1] == ch[len+1])
//                {
//                    // 绘制16x16汉字
//                    for (uint8_t m = 0; m < 2; m++)  // 上下两页
//                    {
//                        for (uint8_t n = 0; n < 16; n++)  // 列
//                        {
//                            for (uint8_t j = 0; j < 8; j++)  // 行
//                            {
//                                OLED_SetPixel(x + n, y + j + m*8, (F16x16_CN[i].encoder[n + m*16] >> j) & 0x01);
//                            }
//                        }
//                    }
//                    x += 16;
//                    len += offset;
//                    break;
//                }
//            }
//        }
//    }
//    OLED_RefreshRAM();
//}
//
///**
// * @brief  显示中英文混合字符串
// * @param  x,y: 起始坐标
// * @param  ch[]: 混合字符串
// * @retval 无
// */
//void OLED_ShowMixedCH(int16_t x, int16_t y, uint8_t* ch)
//{
//    if (x >= 0 && x < displayWidth && y >= 0 && y < displayHeight)
//    {
//        int32_t len = 0, c;
//        uint8_t offset = sizeof(F16x16_CN[0].index);
//
//        while (ch[len] != '\0')
//        {
//            if (ch[len] >= 0xA1)  // 汉字（GB2312从0xA1开始）
//            {
//                for (uint8_t i = 0; i < sizeof(F16x16_CN)/sizeof(GB2312_CN); i++)
//                {
//                    if (F16x16_CN[i].index[0] == ch[len] && F16x16_CN[i].index[1] == ch[len+1])
//                    {
//                        // 换行处理
//                        if (x >= 127 || (127 - x < 16))
//                        {
//                            x = 0;
//                            y += 16;
//                            if (63 - y < 16) break;
//                        }
//
//                        // 绘制汉字
//                        for (uint8_t m = 0; m < 2; m++)
//                        {
//                            for (uint8_t n = 0; n < 16; n++)
//                            {
//                                for (uint8_t j = 0; j < 8; j++)
//                                {
//                                    OLED_SetPixel(x + n, y + j + m*8, (F16x16_CN[i].encoder[n + m*16] >> j) & 0x01);
//                                }
//                            }
//                        }
//                        x += 16;
//                        len += offset;
//                        break;
//                    }
//                }
//            }
//            else if (ch[len] <= 127)  // ASCII字符
//            {
//                c = ch[len] - 32;
//                if (c < 0) break;
//
//                // 换行处理
//                if (x >= 127 || (127 - x < 8))
//                {
//                    x = 0;
//                    y += 16;
//                    if (63 - y < 16) break;
//                }
//
//                // 绘制8x16 ASCII
//                for (uint8_t m = 0; m < 2; m++)
//                {
//                    for (uint8_t n = 0; n < 8; n++)
//                    {
//                        for (uint8_t i = 0; i < 8; i++)
//                        {
//                            OLED_SetPixel(x + n, y + i + m*8, (F8X16[c][n + m*8] >> i) & 0x01);
//                        }
//                    }
//                }
//                x += 8;
//                len++;
//            }
//        }
//    }
//    OLED_RefreshRAM();
//}
//
///**
// * @brief  绘制BMP图片
// * @param  x0,y0: 起始坐标
// * @param  L,H: 图片宽高
// * @param  BMP[]: 图片点阵数据（纵向取模）
// * @retval 无
// */
//void OLED_DrawBMP(int16_t x0, int16_t y0, int16_t L, int16_t H, const uint8_t BMP[])
//{
//    if (x0 >= 0 && x0 + L <= displayWidth && y0 >= 0 && y0 + H <= displayHeight)
//    {
//        const uint8_t* p = BMP;
//
//        // 按8行一组处理（一页）
//        for (int16_t y = y0; y < y0 + H; y += 8)
//        {
//            for (int16_t x = x0; x < x0 + L; x++)
//            {
//                // 绘制当前点的8个像素
//                for (int16_t i = 0; i < 8; i++)
//                {
//                    if (y + i < y0 + H)  // 避免超出图片高度
//                    {
//                        OLED_SetPixel(x, y + i, (*p >> i) & 0x01);
//                    }
//                }
//                p++;
//            }
//        }
//    }
//    OLED_RefreshRAM();
//}
//
///**
// * @brief  区域填充
// * @param  x0,y0: 起始坐标
// * @param  L,H: 区域宽高
// * @param  fill_data: 填充数据（0x00~0xFF）
// * @retval 无
// */
//void OLED_AreaFill(int16_t x0, int16_t y0, int16_t L, int16_t H, uint8_t fill_data)
//{
//    if (x0 >= 0 && x0 + L <= displayWidth && y0 >= 0 && y0 + H <= displayHeight)
//    {
//        for (int16_t y = y0; y < y0 + H; y++)
//        {
//            for (int16_t x = x0; x < x0 + L; x++)
//            {
//                // 按位设置填充值
//                uint8_t pixel = (fill_data >> (y % 8)) & 0x01;
//                OLED_SetPixel(x, y, pixel);
//            }
//        }
//        OLED_RefreshRAM();
//    }
//}
//
///**
// * @brief  区域清除
// * @param  x0,y0: 起始坐标
// * @param  L,H: 区域宽高
// * @retval 无
// */
//void OLED_AreaClear(int16_t x0, int16_t y0, int16_t L, int16_t H)
//{
//    OLED_AreaFill(x0, y0, L, H, 0x00);  // 填充0x00即清除
//}
//
///**
// * @brief  全屏内容取反
// * @param  无
// * @retval 无
// */
//void OLED_FullyToggle(void)
//{
//    for (uint16_t m = 0; m < 8; m++)
//    {
//        for (uint16_t n = 0; n < 128; n++)
//        {
//            OLED_RAM[m][n] = ~OLED_RAM[m][n];
//        }
//    }
//    OLED_RefreshRAM();
//}
//
///**
// * @brief  区域内容取反
// * @param  x0,y0: 起始坐标
// * @param  L,H: 区域宽高
// * @retval 无
// */
//void OLED_AreaToggle(int16_t x0, int16_t y0, int16_t L, int16_t H)
//{
//    if (x0 >= 0 && x0 + L <= displayWidth && y0 >= 0 && y0 + H <= displayHeight)
//    {
//        for (int16_t y = y0; y < y0 + H; y++)
//        {
//            for (int16_t x = x0; x < x0 + L; x++)
//            {
//                // 取反当前像素
//                OLED_SetPixel(x, y, !OLED_GetPixel(x, y));
//            }
//        }
//        OLED_RefreshRAM();
//    }
//}
//
///**
// * @brief  全屏垂直偏移动画
// * @param  无
// * @retval 无
// */
//void OLED_VerticalShift(void)
//{
//    for (uint8_t i = 0; i < displayHeight; i++)
//    {
//        WriteCmd(0xd3);  // 设置显示偏移
//        WriteCmd(i);     // 偏移量
//        delay_ms(40);    // 动画间隔
//    }
//}
//
///**
// * @brief  全屏水平滚动
// * @param  direction: 方向（LEFT:0x27；RIGHT:0x26）
// * @retval 无
// */
//void OLED_HorizontalShift(uint8_t direction)
//{
//    WriteCmd(direction);       // 滚动方向
//    WriteCmd(0x00);            // 虚拟字节
//    WriteCmd(0x00);            // 起始页
//    WriteCmd(0x05);            // 滚动速度
//    WriteCmd(0x07);            // 结束页
//    WriteCmd(0x00);            // 虚拟字节
//    WriteCmd(0xff);            // 虚拟字节
//    WriteCmd(0x2f);            // 开启滚动
//}
//
///**
// * @brief  全屏垂直水平滚动
// * @param  direction: 方向（UP:0x29；DOWN:0x2A）
// * @retval 无
// */
//void OLED_VerticalAndHorizontalShift(uint8_t direction)
//{
//    WriteCmd(direction);       // 滚动方向
//    WriteCmd(0x00);            // 虚拟字节
//    WriteCmd(0x00);            // 起始页
//    WriteCmd(0x05);            // 滚动速度
//    WriteCmd(0x07);            // 结束页
//    WriteCmd(0x01);            // 垂直偏移量
//    WriteCmd(0x2f);            // 开启滚动
//}
//
///**
// * @brief  显示模式切换（正常/反显）
// * @param  mode: 模式（ON:0xA7反显；OFF:0xA6正常）
// * @retval 无
// */
//void OLED_DisplayMode(uint8_t mode)
//{
//    WriteCmd(mode);
//}
//
///**
// * @brief  调节屏幕亮度
// * @param  intensity: 亮度值（0~255）
// * @retval 无
// */
//void OLED_IntensityControl(uint8_t intensity)
//{
//    WriteCmd(0x81);    // 对比度控制
//    WriteCmd(intensity);  // 亮度值
//}
