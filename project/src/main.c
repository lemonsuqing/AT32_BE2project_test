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
#include "wk_spi.h"
#include "wk_usart.h"
#include "wk_system.h"

/* private includes ----------------------------------------------------------*/
/* add user code begin private includes */
#include "Serial.h"
#include "be2_spi.h"
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
  wk_usart1_init();Serial_Printf("System initialized. Starting LPMS-BE2 test...\r\n");

  /* init spi2 function. */
  wk_spi2_init();Serial_Printf("SPI2 initialized.\r\n");

  /* add user code begin 2 */
  wk_delay_ms(100);

    // 初始化传感器（使用LP-BUS协议）
    be2_spi_init();

    Serial_Printf("LPMS-BE2 initialization complete.\r\n");

  /* add user code end 2 */

  while(1)
  {
    /* add user code begin 3 */
	  be2_read_data(&sensor_data); // 改用LP-BUS命令读取

	  // 打印欧拉角数据
	  Serial_Printf("Roll: %.2f, Pitch: %.2f, Yaw: %.2f\r\n",
					sensor_data.euler[0],
					sensor_data.euler[1],
					sensor_data.euler[2]);

//	 be2_read_data(&sensor_data);
//
//	 // 打印欧拉角数据
//	 Serial_Printf("Roll: %.2f, Pitch: %.2f, Yaw: %.2f\r\n",
//			sensor_data.euler[0],
//			sensor_data.euler[1],
//			sensor_data.euler[2]);

	 // 根据传感器输出频率延时 (100Hz)
	 wk_delay_ms(200);
    /* add user code end 3 */
  }
}

  /* add user code begin 4 */

  /* add user code end 4 */
