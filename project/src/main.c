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

//  be2_spi_init_cs_gpio();
//
//  wk_delay_ms(1000);
//
//  Serial_Printf("=== LPMS-BE2 SPI LPBUS Test ===\r\n");
//
//  be2_spi_init_cs_gpio();
//  wk_spi2_init();
//  wk_delay_ms(500);
//
//  // Step 1: Enter command mode
//  Serial_Printf("Step 1: Enter Command Mode...\r\n");
//  if (be2_enter_command_mode()) {
//      Serial_Printf("✅ Entered command mode.\r\n");
//  } else {
//      Serial_Printf("❌ Failed to enter command mode.\r\n");
//  }
//
//  // Step 2: Read WHO_AM_I (0x74)
//  Serial_Printf("Step 2: Read WHO_AM_I (0x74)...\r\n");
//  uint8_t who = be2_read_whoami_lpb();
//  Serial_Printf("WHO_AM_I = 0x%02X %s\r\n", who, (who == 0x32) ? "✅ OK" : "❌ ERROR");
  Serial_Printf("=== Init LPMS-BE2 ===\r\n");
  if (!be2_sensor_init()) {
	  Serial_Printf("❌ xxx Init ERROR xxx\r\n");
	  while(1); // 初始化失败
  }
  BE2_SensorData data;

  /* add user code end 2 */

  while(1)
  {
    /* add user code begin 3 */
	  if (be2_read_sensor_data(&data)) {
		  Serial_Printf("=== 传感器数据 ===\r\n");
		  Serial_Printf("时间戳: %u\r\n", data.timestamp);
		  Serial_Printf("加速度: %.3f, %.3f, %.3f g\r\n",
					  data.acc_calibrated[0],
					  data.acc_calibrated[1],
					  data.acc_calibrated[2]);
		  Serial_Printf("角速度: %.3f, %.3f, %.3f dps\r\n",
					  data.gyro_alignment[0],
					  data.gyro_alignment[1],
					  data.gyro_alignment[2]);
		  Serial_Printf("欧拉角: roll=%.2f, pitch=%.2f, yaw=%.2f 度\r\n",
					  data.euler[0], data.euler[1], data.euler[2]);
	  }
	  wk_delay_ms(100); // 10Hz读取频率
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
