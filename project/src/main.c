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

  /* add user code begin 2 */

  lpms_be2_init(); // 初始化CS引脚

  Serial_Printf("=== LPMS-BE2 SPI DEMO START ===\r\n");
  wk_delay_ms(1000); // 等待传感器上电稳定

  if (!lpms_enter_command_mode()) {
	  Serial_Printf("❌ 进入命令模式失败\r\n");
	  while (1);
  }

  lpms_set_output_format_euler(); // 设置输出欧拉角
  lpms_set_output_frequency(50);  // 设置输出频率为 50Hz
  lpms_start_streaming();         // 开始数据流
  lpms_data_t data;

  /* add user code end 2 */

  while(1)
  {
    /* add user code begin 3 */
	  if (lpms_read_euler_angles(&data)) {
		  Serial_Printf("Euler: Roll=%.2f Pitch=%.2f Yaw=%.2f\r\n",
						data.roll, data.pitch, data.yaw);
	  }
	  wk_delay_ms(20); // 保持和频率同步（50Hz ≈ 20ms）
//	  if (whoami == 0x32) {  // 仅在通讯正常时读取
//	        be2_read_sensors();
//	      }
//	      wk_delay_ms(100);  // 10Hz读取频率
//	  LPMS_CS_LOW();
//	  for (int i = 0; i < 10; i++) {
//	      spi2_rw_byte(0x5A + i); // 发送递增数据便于区分
//	  }
//	  LPMS_CS_HIGH();
//
//	  Serial_Printf("SPI Send=0xA5\r\n");
//	  wk_delay_ms(500);  // 等一会，重复发送
    /* add user code end 3 */
  }
}

  /* add user code begin 4 */

  /* add user code end 4 */
