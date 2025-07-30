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
//#include "oled.h"
#include "hardware_oled.h"
#include "i2c_application.h"
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
void I2C_Scan_WK(i2c_type *i2c) {
    uint8_t addr;
    uint32_t timeout;

    Serial_Printf("Starting I2C Scan (WK Library)...\r\n");
    for (addr = 0x08; addr <= 0x77; addr++) {
        timeout = 100000; // 超时计数，约100ms
        while (i2c_flag_get(i2c, I2C_BUSYF_FLAG) != RESET && timeout--) {
			wk_delay_us(1);
		}
		if (timeout == 0) {
			Serial_Printf("Bus busy, skip addr 0x%02X\r\n", addr);
			continue;
		}

        // 生成起始信号
        i2c_start_generate(i2c);

        // 设置设备地址（左移1位，包含写方向位）
        i2c_transfer_addr_set(i2c, addr << 1);
        i2c_transfer_dir_set(i2c, I2C_DIR_TRANSMIT);

        // 等待地址响应：检查地址匹配标志或超时/ACK失败
        while (!i2c_flag_get(i2c, I2C_ADDRF_FLAG) && timeout--) {
            // 若检测到ACK失败，提前退出等待
            if (i2c_flag_get(i2c, I2C_ACKFAIL_FLAG)) {
                break;
            }
            wk_delay_us(1); // 微秒级延时
        }

        // 若未超时且未检测到ACK失败，说明设备存在
        if (timeout > 0 && !i2c_flag_get(i2c, I2C_ACKFAIL_FLAG)) {
            Serial_Printf("Found device at 0x%02X\r\n", addr);
        }

        // 生成停止信号，结束本次通信
        i2c_stop_generate(i2c);
        // 清除标志位，为下次检测做准备
        i2c_flag_clear(i2c, I2C_ADDRF_FLAG | I2C_ACKFAIL_FLAG);

        // 短暂延时，避免总线冲突
        wk_delay_ms(1);
    }
    Serial_Printf("I2C Scan Completed\r\n");
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
//  OLED_FullyClear();
//
//  OLED_ShowStr(0, 0, (uint8_t *)"Hello", 1);
//  OLED_RefreshRAM();
  I2C_Scan_WK(I2C2);

  /* add user code end 2 */

  while(1)
  {
    /* add user code begin 3 */
//	OLED_ShowChar(1,1,'A');//显示一个字符
//	OLED_ShowString(1,3,"HelloWorld");//显示字符串
//	OLED_ShowNum(2,1,12345,5);//显示无符号十进制数
//	OLED_ShowSignedNum(2,7,-66,2);//显示有符号十进制数
//	OLED_ShowHexNum(3,1,0XAA66,4);//显示16进制数字
//	OLED_ShowBinNum(4,1,0XAA55,16);//显示二进制
//  wk_delay_ms(200);  // 控制读取频率（与手册中默认100Hz输出兼容）
    /* add user code end 3 */
  }
}

  /* add user code begin 4 */

  /* add user code end 4 */
