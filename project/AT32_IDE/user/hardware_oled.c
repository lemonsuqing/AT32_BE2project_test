//#include "hardware_oled.h"
//#include "i2c_application.h"  // ����wk��I2CӦ�ò�ͷ�ļ�
//#include "OLED_Font.h"
//
//// �ⲿ����I2C���������i2c�����ļ��г�ʼ����
//extern i2c_handle_type hi2c2;
//
///* OLED�Դ滺���� [ҳ][��] */
//static uint8_t OLED_RAM[8][128];
//
///* ��Ļ�ߴ���� */
//#define displayWidth   128
//#define displayHeight  64
//
//static void delay_ms(uint32_t us){
//	wk_delay_ms(us);
//}
//
///**
// * @brief  ͨ��I2Cд��һ���ֽڣ���ַ+���ݣ�
// * @param  addr: �Ĵ�����ַ��0x00Ϊ���0x40Ϊ���ݣ�
// * @param  data: Ҫд�������
// * @retval ��
// */
//void wk_i2c_write_byte(uint8_t addr, uint8_t data)
//{
//    uint8_t tx_buf[2] = {addr, data};
//    i2c_status_type status;
//
//    // ʹ��wk��I2C���ͺ���
//    status = i2c_master_transmit(&hi2c2, OLED_I2C_ADDRESS, tx_buf, 2, 1000);
//    Serial_Printf("I2C addr=%02X, data=%02X, status=%d\n", OLED_I2C_ADDRESS, data, status);
//    if (status != I2C_OK)
//    {
//        // ����Ӵ�������I2C���ã�
//    	Serial_Printf("I2C send failed!");
//        i2c_reset_ctrl2_register(&hi2c2);
//    }
//}
//
///**
// * @brief  д����
// * @param  IIC_Command: �����ֽ�
// * @retval ��
// */
//void WriteCmd(uint8_t IIC_Command)
//{
//    wk_i2c_write_byte(0x00, IIC_Command);
//}
//
///**
// * @brief  д����
// * @param  IIC_Data: �����ֽ�
// * @retval ��
// */
//void WriteDat(uint8_t IIC_Data)
//{
//    wk_i2c_write_byte(0x40, IIC_Data);
//}
//
///**
// * @brief  ��ʼ��OLED
// * @param  ��
// * @retval ��
// */
//void OLED_Init(void)
//{
//    // ��ʼ��ǰ��ʱ��ȷ��Ӳ������
//    delay_ms(500);
//
//    // OLED��ʼ���������У���ԭ����һ�£�
//    WriteCmd(0xAE);  // �ر���ʾ
//    WriteCmd(0x20);  // �����ڴ�Ѱַģʽ
//    WriteCmd(0x10);  // ҳѰַģʽ
//
//    WriteCmd(0xb0);  // ����ҳ��ַ����ʼҳ0��
//    WriteCmd(0x00);  // �е͵�ַ
//    WriteCmd(0x10);  // �иߵ�ַ
//
//    WriteCmd(0xc8);  // COMɨ�跽��ת
//    WriteCmd(0x40);  // ��ʼ�е�ַ
//    WriteCmd(0x81);  // �Աȶȿ���
//    WriteCmd(0xff);  // ���Աȶ�
//
//    WriteCmd(0xa1);  // ����ӳ�䣨0->127��
//    WriteCmd(0xa6);  // ������ʾ���Ƿ��ԣ�
//    WriteCmd(0xa8);  // ���ö�·���ñ�
//    WriteCmd(0x3F);  // 64�и���
//
//    WriteCmd(0xa4);  // ��ʾRAM����
//    WriteCmd(0xd3);  // ��ʾƫ��
//    WriteCmd(0x00);  // ��ƫ��
//
//    WriteCmd(0xd5);  // ��ʾʱ�ӷ�Ƶ
//    WriteCmd(0xf0);  // ��Ƶϵ��
//    WriteCmd(0xd9);  // Ԥ�������
//    WriteCmd(0x22);  // Ԥ�������
//
//    WriteCmd(0xda);  // COM��������
//    WriteCmd(0x12);  // Ӳ������
//    WriteCmd(0xdb);  // VCOMH����
//    WriteCmd(0x20);  // 0.77xVcc
//
//    WriteCmd(0x8d);  // ��ɱ�����
//    WriteCmd(0x14);  // ������ɱ�
//    WriteCmd(0xaf);  // ������ʾ
//
//    OLED_FullyClear();  // ������ʼ��
//}
//
///**
// * @brief  ����OLED��������ɱã�
// * @param  ��
// * @retval ��
// */
//void OLED_ON(void)
//{
//    WriteCmd(0X8D);  // ��ɱ�����
//    WriteCmd(0X14);  // ������ɱ�
//    WriteCmd(0XAF);  // ������ʾ
//}
//
///**
// * @brief  ����OLED���رյ�ɱã�
// * @param  ��
// * @retval ��
// */
//void OLED_OFF(void)
//{
//    WriteCmd(0X8D);  // ��ɱ�����
//    WriteCmd(0X10);  // �رյ�ɱ�
//    WriteCmd(0XAE);  // �ر���ʾ
//}
//
///**
// * @brief  ������������ˢ�µ�OLED��Ļ
// * @param  ��
// * @retval ��
// */
//void OLED_RefreshRAM(void)
//{
//    uint8_t page, col;
//
//    for (page = 0; page < 8; page++)
//    {
//        // ����ҳ��ַ
//        WriteCmd(0xb0 + page);
//        // �����е�ַ����4λ+��4λ��
//        WriteCmd(0x00);
//        WriteCmd(0x10);
//
//        // д�뵱ǰҳ������������
//        for (col = 0; col < 128; col++)
//        {
//            WriteDat(OLED_RAM[page][col]);
//        }
//    }
//}
//
///**
// * @brief  ����Դ滺��������ˢ����Ļ��
// * @param  ��
// * @retval ��
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
// * @brief  ȫ�����ָ������
// * @param  fill_Data: ������ݣ�0x00Ϊ�ڣ�0xFFΪ�ף�
// * @retval ��
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
// * @brief  ȫ����������0x00��
// * @param  ��
// * @retval ��
// */
//void OLED_FullyClear(void)
//{
//    OLED_FullyFill(0x00);
//}
//
///**
// * @brief  ����ָ���������ص�״̬
// * @param  x: �����꣨0~127��
// * @param  y: �����꣨0~63��
// * @param  set_pixel: ״̬��SET_PIXEL������RESET_PIXELϨ��
// * @retval ��
// */
//void OLED_SetPixel(int16_t x, int16_t y, uint8_t set_pixel)
//{
//    if (x >= 0 && x < displayWidth && y >= 0 && y < displayHeight)
//    {
//        uint8_t page = y / 8;    // ����ҳ��ַ
//        uint8_t bit  = y % 8;    // ����ҳ��λƫ��
//
//        if (set_pixel == SET_PIXEL)
//        {
//            OLED_RAM[page][x] |= (1 << bit);  // ��λ
//        }
//        else
//        {
//            OLED_RAM[page][x] &= ~(1 << bit); // ����
//        }
//    }
//}
//
///**
// * @brief  ��ȡָ���������ص�״̬
// * @param  x: �����꣨0~127��
// * @param  y: �����꣨0~63��
// * @retval ����״̬��SET_PIXEL/RESET_PIXEL��
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
// * @brief  ��ʾASCII�ַ���
// * @param  x,y: ��ʼ����
// * @param  ch[]: �ַ���
// * @param  TextSize: �����С��1:6x8��2:8x16��
// * @retval ��
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
//            case 1:  // 6x8����
//            {
//                while (ch[j] != '\0')
//                {
//                    c = ch[j] - 32;  // ����ƫ�ƣ��ո�Ϊ32��
//                    if (c < 0) break;  // ��Ч�ַ�
//
//                    // ���д���
//                    if (x >= 125 || (127 - x < 6))
//                    {
//                        x = 0;
//                        y += 8;
//                        if (63 - y < 8) break;
//                    }
//
//                    // ����6x8�ַ�
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
//            case 2:  // 8x16����
//            {
//                while (ch[j] != '\0')
//                {
//                    c = ch[j] - 32;
//                    if (c < 0) break;
//
//                    // ���д���
//                    if (x >= 127 || (127 - x < 8))
//                    {
//                        x = 0;
//                        y += 16;
//                        if (63 - y < 16) break;
//                    }
//
//                    // ����8x16�ַ�
//                    for (uint8_t m = 0; m < 2; m++)  // ��������ҳ
//                    {
//                        for (uint8_t n = 0; n < 8; n++)  // ��
//                        {
//                            for (uint8_t i = 0; i < 8; i++)  // ��
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
// * @brief  ��ʾ16x16����
// * @param  x,y: ��ʼ����
// * @param  ch[]: ����GB2312����
// * @retval ��
// */
//void OLED_ShowCN(int16_t x, int16_t y, uint8_t* ch)
//{
//    if (x >= 0 && x < displayWidth && y >= 0 && y < displayHeight)
//    {
//        int32_t len = 0;
//        uint8_t offset = sizeof(F16x16_CN[0].index);  // �����������ȣ�2�ֽڣ�
//
//        while (ch[len] != '\0')
//        {
//            // ���д���
//            if (x >= 127 || (127 - x < 16))
//            {
//                x = 0;
//                y += 16;
//                if (63 - y < 16) break;
//            }
//
//            // ���Һ��ֵ���
//            for (uint8_t i = 0; i < sizeof(F16x16_CN)/sizeof(GB2312_CN); i++)
//            {
//                if (F16x16_CN[i].index[0] == ch[len] && F16x16_CN[i].index[1] == ch[len+1])
//                {
//                    // ����16x16����
//                    for (uint8_t m = 0; m < 2; m++)  // ������ҳ
//                    {
//                        for (uint8_t n = 0; n < 16; n++)  // ��
//                        {
//                            for (uint8_t j = 0; j < 8; j++)  // ��
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
// * @brief  ��ʾ��Ӣ�Ļ���ַ���
// * @param  x,y: ��ʼ����
// * @param  ch[]: ����ַ���
// * @retval ��
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
//            if (ch[len] >= 0xA1)  // ���֣�GB2312��0xA1��ʼ��
//            {
//                for (uint8_t i = 0; i < sizeof(F16x16_CN)/sizeof(GB2312_CN); i++)
//                {
//                    if (F16x16_CN[i].index[0] == ch[len] && F16x16_CN[i].index[1] == ch[len+1])
//                    {
//                        // ���д���
//                        if (x >= 127 || (127 - x < 16))
//                        {
//                            x = 0;
//                            y += 16;
//                            if (63 - y < 16) break;
//                        }
//
//                        // ���ƺ���
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
//            else if (ch[len] <= 127)  // ASCII�ַ�
//            {
//                c = ch[len] - 32;
//                if (c < 0) break;
//
//                // ���д���
//                if (x >= 127 || (127 - x < 8))
//                {
//                    x = 0;
//                    y += 16;
//                    if (63 - y < 16) break;
//                }
//
//                // ����8x16 ASCII
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
// * @brief  ����BMPͼƬ
// * @param  x0,y0: ��ʼ����
// * @param  L,H: ͼƬ���
// * @param  BMP[]: ͼƬ�������ݣ�����ȡģ��
// * @retval ��
// */
//void OLED_DrawBMP(int16_t x0, int16_t y0, int16_t L, int16_t H, const uint8_t BMP[])
//{
//    if (x0 >= 0 && x0 + L <= displayWidth && y0 >= 0 && y0 + H <= displayHeight)
//    {
//        const uint8_t* p = BMP;
//
//        // ��8��һ�鴦��һҳ��
//        for (int16_t y = y0; y < y0 + H; y += 8)
//        {
//            for (int16_t x = x0; x < x0 + L; x++)
//            {
//                // ���Ƶ�ǰ���8������
//                for (int16_t i = 0; i < 8; i++)
//                {
//                    if (y + i < y0 + H)  // ���ⳬ��ͼƬ�߶�
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
// * @brief  �������
// * @param  x0,y0: ��ʼ����
// * @param  L,H: ������
// * @param  fill_data: ������ݣ�0x00~0xFF��
// * @retval ��
// */
//void OLED_AreaFill(int16_t x0, int16_t y0, int16_t L, int16_t H, uint8_t fill_data)
//{
//    if (x0 >= 0 && x0 + L <= displayWidth && y0 >= 0 && y0 + H <= displayHeight)
//    {
//        for (int16_t y = y0; y < y0 + H; y++)
//        {
//            for (int16_t x = x0; x < x0 + L; x++)
//            {
//                // ��λ�������ֵ
//                uint8_t pixel = (fill_data >> (y % 8)) & 0x01;
//                OLED_SetPixel(x, y, pixel);
//            }
//        }
//        OLED_RefreshRAM();
//    }
//}
//
///**
// * @brief  �������
// * @param  x0,y0: ��ʼ����
// * @param  L,H: ������
// * @retval ��
// */
//void OLED_AreaClear(int16_t x0, int16_t y0, int16_t L, int16_t H)
//{
//    OLED_AreaFill(x0, y0, L, H, 0x00);  // ���0x00�����
//}
//
///**
// * @brief  ȫ������ȡ��
// * @param  ��
// * @retval ��
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
// * @brief  ��������ȡ��
// * @param  x0,y0: ��ʼ����
// * @param  L,H: ������
// * @retval ��
// */
//void OLED_AreaToggle(int16_t x0, int16_t y0, int16_t L, int16_t H)
//{
//    if (x0 >= 0 && x0 + L <= displayWidth && y0 >= 0 && y0 + H <= displayHeight)
//    {
//        for (int16_t y = y0; y < y0 + H; y++)
//        {
//            for (int16_t x = x0; x < x0 + L; x++)
//            {
//                // ȡ����ǰ����
//                OLED_SetPixel(x, y, !OLED_GetPixel(x, y));
//            }
//        }
//        OLED_RefreshRAM();
//    }
//}
//
///**
// * @brief  ȫ����ֱƫ�ƶ���
// * @param  ��
// * @retval ��
// */
//void OLED_VerticalShift(void)
//{
//    for (uint8_t i = 0; i < displayHeight; i++)
//    {
//        WriteCmd(0xd3);  // ������ʾƫ��
//        WriteCmd(i);     // ƫ����
//        delay_ms(40);    // �������
//    }
//}
//
///**
// * @brief  ȫ��ˮƽ����
// * @param  direction: ����LEFT:0x27��RIGHT:0x26��
// * @retval ��
// */
//void OLED_HorizontalShift(uint8_t direction)
//{
//    WriteCmd(direction);       // ��������
//    WriteCmd(0x00);            // �����ֽ�
//    WriteCmd(0x00);            // ��ʼҳ
//    WriteCmd(0x05);            // �����ٶ�
//    WriteCmd(0x07);            // ����ҳ
//    WriteCmd(0x00);            // �����ֽ�
//    WriteCmd(0xff);            // �����ֽ�
//    WriteCmd(0x2f);            // ��������
//}
//
///**
// * @brief  ȫ����ֱˮƽ����
// * @param  direction: ����UP:0x29��DOWN:0x2A��
// * @retval ��
// */
//void OLED_VerticalAndHorizontalShift(uint8_t direction)
//{
//    WriteCmd(direction);       // ��������
//    WriteCmd(0x00);            // �����ֽ�
//    WriteCmd(0x00);            // ��ʼҳ
//    WriteCmd(0x05);            // �����ٶ�
//    WriteCmd(0x07);            // ����ҳ
//    WriteCmd(0x01);            // ��ֱƫ����
//    WriteCmd(0x2f);            // ��������
//}
//
///**
// * @brief  ��ʾģʽ�л�������/���ԣ�
// * @param  mode: ģʽ��ON:0xA7���ԣ�OFF:0xA6������
// * @retval ��
// */
//void OLED_DisplayMode(uint8_t mode)
//{
//    WriteCmd(mode);
//}
//
///**
// * @brief  ������Ļ����
// * @param  intensity: ����ֵ��0~255��
// * @retval ��
// */
//void OLED_IntensityControl(uint8_t intensity)
//{
//    WriteCmd(0x81);    // �Աȶȿ���
//    WriteCmd(intensity);  // ����ֵ
//}
