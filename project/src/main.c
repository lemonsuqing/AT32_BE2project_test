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
#include "be2_iic.h"
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

/* add user code end function prototypes */

/* private user code ---------------------------------------------------------*/
/* add user code begin 0 */
BE2_Data sensor_data;
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

  BE2_I2C_Init();    // 初始化BE2的I2C通信
  wk_delay_ms(100);  // 等待传感器上电稳定

  for (uint8_t addr = 3; addr < 0x78; addr++)
  {
      // 设定地址和写模式
      i2c_transfer_addr_set(I2C1, addr);
      i2c_transfer_dir_set(I2C1, I2C_DIR_TRANSMIT);

      // 生成起始信号
      i2c_start_generate(I2C1);

      // 等待ADDR发送完成
      while(i2c_flag_get(I2C1, I2C_ADDRF_FLAG) == RESET)
      {
          // 可以设置超时避免死循环
      }

      // 检测是否收到ACK（无ACK即失败）
      if (!i2c_flag_get(I2C1, I2C_ACKFAIL_FLAG))
      {
          // 有设备响应
          printf("Found device at address 0x%02X\r\n", addr);
      }

      // 发送停止信号，清理状态
      i2c_stop_generate(I2C1);

      // 清除标志位
      i2c_flag_clear(I2C1, I2C_ADDRF_FLAG | I2C_ACKFAIL_FLAG);
  }
//  if(BE2_Init() != 0)
//  {
//	Serial_Printf("Sensor init failed!\r\n");
//	while(1);  // 初始化失败时阻塞
//  }

  /* add user code end 2 */

  while(1)
  {
    /* add user code begin 3 */
//  if(BE2_ReadAllData(&sensor_data) == 0)  // 读取传感器数据
//  {
//	BE2_PrintData(&sensor_data);  // 通过串口打印数据
//  }
//  else
//  {
//	Serial_Printf("Data read failed!\r\n");
//  }
  wk_delay_ms(200);  // 控制读取频率（与手册中默认100Hz输出兼容）
    /* add user code end 3 */
  }
}

  /* add user code begin 4 */

  /* add user code end 4 */
