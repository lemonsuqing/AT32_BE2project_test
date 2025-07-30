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

//uint8_t BE2_TestAddress(uint8_t be2_addr)
//{
//    uint8_t ack = 1; // 默认为无响应
//    uint8_t i;
//
//    // 1. 发送I2C起始信号（复用OLED的起始函数，符合I2C时序）
//    OLED_I2C_Start();
//
//    // 2. 发送BE2的I2C写地址（7位地址+写位0）
//    for (i = 0; i < 8; i++)
//    {
//        OLED_W_SDA(be2_addr & (0x80 >> i)); // 按位发送地址
//        wk_delay_us(2);                       // 延时符合手册中I2C电平有效时间（≥1μs）
//        OLED_W_SCL(1);                      // 时钟拉高，从机采样
//        wk_delay_us(2);
//
//        // 第8位（最低位）发送后，检查从机应答（ACK）
//        if (i == 7)
//        {
//            OLED_W_SDA(1); // 释放SDA，准备接收应答
//            wk_delay_us(2);
//            if (OLED_W_SDA_READ() == 0) // 若SDA被拉低，说明有应答
//            {
//                ack = 0;
//            }
//        }
//
//        OLED_W_SCL(0); // 时钟拉低，准备下一位
//        wk_delay_us(2);
//    }
//
//    // 3. 发送I2C停止信号
//    OLED_I2C_Stop();
//
//    return ack;
//}

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


//  float ax = 0.0f, ay = 0.0f, az = 0.0f;       // 加速度计数据（单位：g）
//  float gx = 0.0f, gy = 0.0f, gz = 0.0f;       // 陀螺仪数据（单位：dps）
//  float roll = 0.0f, pitch = 0.0f, yaw = 0.0f; // 欧拉角数据（单位：度）
//  uint8_t ret;                                 // 存储读取函数的返回值

  BE2_I2C_TestMain();
  BE2_I2C_Init();                // 初始化I2C引脚（复用OLED配置）
  uint8_t enable_ret = BE2_EnableSensors();  // 使能传感器数据输出
  if (enable_ret != 0)
  {
	Serial_Printf("传感器使能失败！错误码: %d\r\n", enable_ret);
	while(1);  // 初始化失败时挂起
  }
  if (BE2_EnterStreamingMode() != 0)
  {
      Serial_Printf("切换到流模式失败！\r\n");
      while(1);
  }
  Serial_Printf("切换到流模式成功~\r\n");
  float ax, ay, az;       // 加速度计数据
  float gx, gy, gz;       // 陀螺仪数据
  float roll, pitch, yaw; // 欧拉角数据
  uint8_t read_ret;       // 读取结果
  /* add user code end 2 */

  while(1)
  {
    /* add user code begin 3 */
	  // 读取加速度计
//	  read_ret = BE2_ReadAccelerometer(&ax, &ay, &az);
//	  if (read_ret == 0)
//	  {
//		  Serial_Printf("加速度: X=%.2f g, Y=%.2f g, Z=%.2f g\r\n", ax, ay, az);
//	  }
//	  else
//	  {
//		  Serial_Printf("加速度读取失败！错误码: %d\r\n", read_ret);
//	  }
//
//	  // 读取陀螺仪
//	  read_ret = BE2_ReadGyroscope(&gx, &gy, &gz);
//	  if (read_ret == 0)
//	  {
//		  Serial_Printf("陀螺仪: X=%.2f dps, Y=%.2f dps, Z=%.2f dps\r\n", gx, gy, gz);
//	  }
//	  else
//	  {
//		  Serial_Printf("陀螺仪读取失败！错误码: %d\r\n", read_ret);
//	  }
//
//	  // 读取欧拉角
//	  read_ret = BE2_ReadEulerAngle(&roll, &pitch, &yaw);
//	  if (read_ret == 0)
//	  {
//		  Serial_Printf("欧拉角: 横滚=%.2f °, 俯仰=%.2f °, 偏航=%.2f °\r\n", roll, pitch, yaw);
//	  }
//	  else
//	  {
//		  Serial_Printf("欧拉角读取失败！错误码: %d\r\n", read_ret);
//	  }


	wk_delay_ms(500);  // 每500ms读取一次（参考文档默认输出频率）
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
