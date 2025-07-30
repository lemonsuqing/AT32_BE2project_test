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
  BE2_IIC_Init();  // 配置 SCL/SDA 引脚为推挽输出、上拉，初始化时序

  IIC_Start();
  IIC_SendByte(BE2_IIC_ADDR);  // 发送写地址（如 0x32<<1=0x64）
  uint8_t ack = IIC_WaitAck();
  if (ack == 0) {
	  printf("设备地址应答正常！\n");
  } else {
	  printf("设备无应答！检查地址、接线、供电\n");
  }
  IIC_Stop();

  while(1);

//  float ax = 0.0f, ay = 0.0f, az = 0.0f;       // 加速度计数据（单位：g）
//  float gx = 0.0f, gy = 0.0f, gz = 0.0f;       // 陀螺仪数据（单位：dps）
//  float roll = 0.0f, pitch = 0.0f, yaw = 0.0f; // 欧拉角数据（单位：度）
//  uint8_t ret;                                 // 存储读取函数的返回值

  /* add user code end 2 */

  while(1)
  {
    /* add user code begin 3 */
//	  ret = BE2_ReadAccelerometer(&ax, &ay, &az);
//	  if (ret == 0){
//		  Serial_Printf("[Accelerometer] ax=%.2f g, ay=%.2f g, az=%.2f g\r\n", ax, ay, az);
//	  }else{
//		  // 错误码含义：1=设备无应答，2=寄存器地址无应答，3=读操作无应答
//		  Serial_Printf("[Error] Accelerometer read failed! Code: %d\r\n", ret);
//	  }
//
//
//	  // --------------------- 读取陀螺仪数据 ---------------------
//	  ret = BE2_ReadGyroscope(&gx, &gy, &gz);
//	  if (ret == 0){
//		  Serial_Printf("[Gyroscope]    gx=%.2f dps, gy=%.2f dps, gz=%.2f dps\r\n", gx, gy, gz);
//	  }else{
//		  Serial_Printf("[Error] Gyroscope read failed! Code: %d\r\n", ret);
//	  }
//
//
//	  // --------------------- 读取欧拉角数据 ---------------------
//	  ret = BE2_ReadEulerAngle(&roll, &pitch, &yaw);
//	  if (ret == 0){
//		  Serial_Printf("[Euler Angle]  roll=%.2f °, pitch=%.2f °, yaw=%.2f °\r\n", roll, pitch, yaw);
//	  }else{
//		  Serial_Printf("[Error] Euler Angle read failed! Code: %d\r\n", ret);
//	  }
	wk_delay_ms(200);  // 控制读取频率（与手册中默认100Hz输出兼容）
    /* add user code end 3 */
  }
}

  /* add user code begin 4 */

  /* add user code end 4 */
