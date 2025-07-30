/* add user code begin Header */
/**
  **************************************************************************
  * @file     main.c
  * @brief    main program
  **************************************************************************
  *                       Copyright notice & Disclaimer
  *
  * The software Board Support Package (BSP) that is made available to
  * download from Artery official website is the copyrighted work of Artery.
  * Artery authorizes customers to use, copy, and distribute the BSP
  * software and its related documentation for the purpose of design and
  * development in conjunction with Artery microcontrollers. Use of the
  * software is governed by this copyright notice and the following disclaimer.
  *
  * THIS SOFTWARE IS PROVIDED ON "AS IS" BASIS WITHOUT WARRANTIES,
  * GUARANTEES OR REPRESENTATIONS OF ANY KIND. ARTERY EXPRESSLY DISCLAIMS,
  * TO THE FULLEST EXTENT PERMITTED BY LAW, ALL EXPRESS, IMPLIED OR
  * STATUTORY OR OTHER WARRANTIES, GUARANTEES OR REPRESENTATIONS,
  * INCLUDING BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE, OR NON-INFRINGEMENT.
  *
  **************************************************************************
  */
/* add user code end Header */

/* Includes ------------------------------------------------------------------*/
#include "at32f402_405_wk_config.h"
#include "wk_i2c.h"
#include "wk_spi.h"
#include "wk_usart.h"
#include "wk_gpio.h"
#include "wk_system.h"

/* private includes ----------------------------------------------------------*/
/* add user code begin private includes */
#include "Serial.h"
//#include "be2_spi.h"
//#include "be2_iic.h"
#include "oled.h"
//#include "hardware_oled.h"
#include "i2c_application.h"
#include "be2_software_iic.h"
/* add user code end private includes */

/* private typedef -----------------------------------------------------------*/
/* add user code begin private typedef */

/* add user code end private typedef */

/* private define ------------------------------------------------------------*/
/* add user code begin private define */

/* add user code end private define */

/* private macro -------------------------------------------------------------*/
/* add user code begin private macro */

/* add user code end private macro */

/* private variables ---------------------------------------------------------*/
/* add user code begin private variables */

/* add user code end private variables */

/* private function prototypes --------------------------------------------*/
/* add user code begin function prototypes */
uint8_t BE2_TestAddress(uint8_t write_addr)
{
    uint8_t ack = 1; // 默认为无响应
    uint8_t i;

    OLED_W_SDA(1);
    OLED_W_SCL(1);
    wk_delay_us(5);

    // 发送起始信号（SCL高时，SDA从高变低）
    OLED_W_SDA(0);
    wk_delay_us(5);
    OLED_W_SCL(0);
    wk_delay_us(5);

    // 发送8位写地址（7位地址+0）
    for (i = 0; i < 8; i++)
    {
        OLED_W_SDA((write_addr >> (7 - i)) & 0x01); // 高位在前
        wk_delay_us(2);
        OLED_W_SCL(1); // 时钟拉高，从机采样
        wk_delay_us(5); // 满足最小高电平时间（≥4.7μs）

        // 第8位后检测ACK（SDA被从机拉低）
        if (i == 7)
        {
            OLED_W_SDA(1); // 释放SDA
            wk_delay_us(2);
            OLED_W_SCL(0);
            wk_delay_us(2);
            OLED_W_SCL(1);
            wk_delay_us(2);
            if (OLED_W_SDA_READ() == 0) // 正确检测到ACK
            {
                ack = 0;
            }
        }

        OLED_W_SCL(0);
        wk_delay_us(2);
    }

    // 发送停止信号（SCL高时，SDA从低变高）
    OLED_W_SDA(0);
    wk_delay_us(5);
    OLED_W_SCL(1);
    wk_delay_us(5);
    OLED_W_SDA(1);
    wk_delay_us(5);

    return ack;
}

/**
 * @brief  BE2地址测试主函数（需在main中调用）
 */
void BE2_I2C_TestMain(void)
{
    // 初始化I2C引脚（复用OLED的初始化，开漏输出+上拉，符合I2C标准）
    OLED_I2C_Init();
    wk_delay_ms(100); // 等待传感器上电稳定

    // 根据BE2的MODE0引脚状态选择地址（参考20230707硬件手册表3-1）
    // 若MODE0接GND：地址为0x32<<1=0x64；若接VCC：地址为0x33<<1=0x66
    uint8_t test_addr = 0x64; // 替换为实际地址

    // 执行地址测试
    uint8_t result = BE2_TestAddress(test_addr);

    // 打印测试结果
    if (result == 0)
    {
        Serial_Printf("BE2地址0x%02X响应正常！\n", test_addr);
    }
    else
    {
        Serial_Printf("BE2地址0x%02X无响应，请检查：\n", test_addr);
        Serial_Printf("1. MODE1是否接高电平（I2C模式）\n");
        Serial_Printf("2. MODE0电平与地址是否匹配\n");
        Serial_Printf("3. 接线（SCL/SDA是否接反、GND是否共地）\n");
    }
}
/* add user code end function prototypes */

/* private user code ---------------------------------------------------------*/
/* add user code begin 0 */
//BE2_Data sensor_data;
/* add user code end 0 */

/**
  * @brief main function.
  * @param  none
  * @retval none
  */
int main(void)
{
  /* add user code begin 1 */

  /* add user code end 1 */

  /* system clock config. */
  wk_system_clock_config();

  /* config periph clock. */
  wk_periph_clock_config();

  /* nvic config. */
  wk_nvic_config();

  /* timebase config. */
  wk_timebase_init();

  /* init usart1 function. */
  wk_usart1_init();

  /* init spi2 function. */
  wk_spi2_init();

  /* init i2c2 function. */
  wk_i2c2_init();

  /* init gpio function. */
  wk_gpio_config();

  /* add user code begin 2 */
  wk_delay_ms(100);
//  OLED_Init();
//
//  OLED_ShowChar(1,1,'A');//显示一个字符
//  OLED_ShowString(1,3,"HelloWorld");//显示字符串
//  OLED_ShowNum(2,1,12345,5);//显示无符号十进制数
//  OLED_ShowSignedNum(2,7,-66,2);//显示有符号十进制数
//  OLED_ShowHexNum(3,1,0XAA66,4);//显示16进制数字
//  OLED_ShowBinNum(4,1,0XAA55,16);//显示二进制
  BE2_I2C_Init();

//  gpio_bits_write(GPIOB, GPIO_PINS_8, RESET); // 拉低
//  wk_delay_ms(1000);
//  gpio_bits_write(GPIOB, GPIO_PINS_8, SET);   // 拉高

  BE2_Test_WhoAmI();
  scan_addr();
  /* add user code end 2 */

  while(1)
  {
    /* add user code begin 3 */

	wk_delay_ms(500);  // 每500ms读取一次（参考文档默认输出频率）
    /* add user code end 3 */
  }
}

  /* add user code begin 4 */

  /* add user code end 4 */
