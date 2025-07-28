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

  Serial_Printf("Initializing BE2 sensor...\r\n");
  be2_spi_init();

  be2_write_register(BE2_REG_SYS_CONFIG, 0x80); // 复位
  wk_delay_ms(200); // 延长至200ms，确保稳定

  // 复位后读取SYS_CONFIG
  uint8_t sys_config = be2_read_register(BE2_REG_SYS_CONFIG);
  Serial_Printf("SYS_CONFIG after reset: 0x%02X (expected 0x00)\r\n", sys_config);

  uint8_t status = be2_read_register(BE2_REG_STATUS);
  Serial_Printf("Sensor status: 0x%02X\r\n", status);

    // 尝试读取固件版本
    uint8_t fw_ver0 = be2_read_register(BE2_REG_FIRMWARE_VERSION_0);
    uint8_t fw_ver1 = be2_read_register(BE2_REG_FIRMWARE_VERSION_1);
    Serial_Printf("Firmware version: %d.%d.%d\r\n",
           (fw_ver1 >> 4) & 0x0F,  // 主版本
           fw_ver1 & 0x0F,         // 次版本
           fw_ver0 & 0x0F);        // 修订版本

    Serial_Printf("LPMS-BE2 initialization complete.\r\n");


  /* add user code end 2 */

  while(1)
  {
    /* add user code begin 3 */
	  float roll = be2_read_float(BE2_REG_EULER_X);
	  float pitch = be2_read_float(BE2_REG_EULER_Y);
	  float yaw = be2_read_float(BE2_REG_EULER_Z);

	  Serial_Printf("Roll: %.2f, Pitch: %.2f, Yaw: %.2f\r\n\n", roll, pitch, yaw);

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
