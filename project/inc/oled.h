#ifndef __OLED_H
#define __OLED_H

#include "at32f402_405.h"
#include "i2c_application.h"
#include "wk_system.h"
#include "wk_gpio.h"
#include <stddef.h>
#include <stdint.h>

void OLED_Init(void);
void OLED_Clear(void);
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char);
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String);
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length);
void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);

void OLED_I2C_Init(void);
void OLED_I2C_Start(void);
void OLED_I2C_Stop(void);
void OLED_I2C_SendByte(uint8_t Byte);
void OLED_WriteCommand(uint8_t Command);
void OLED_WriteData(uint8_t Data);
void OLED_SetCursor(uint8_t Y, uint8_t X);

/* 引脚定义 */
#define OLED_SCL_PORT        GPIOB
#define OLED_SDA_PORT        GPIOB
#define OLED_SCL_PIN         GPIO_PINS_8
#define OLED_SDA_PIN         GPIO_PINS_9

/* 引脚操作宏 */
#define OLED_W_SCL(x)        gpio_bits_write(OLED_SCL_PORT, OLED_SCL_PIN, (x) ? SET : RESET)
#define OLED_W_SDA(x)        gpio_bits_write(OLED_SDA_PORT, OLED_SDA_PIN, (x) ? SET : RESET)
#define OLED_W_SDA_READ()  	 gpio_input_data_bit_read(OLED_SDA_PORT, OLED_SDA_PIN)


#endif
