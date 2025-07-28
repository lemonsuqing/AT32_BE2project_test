#ifndef __BE2_IIC_H
#define __BE2_IIC_H

#include "at32f402_405_wk_config.h"
#include "at32f402_405_i2c.h"
#include <stdint.h>
#include "wk_system.h"

// BE2 传感器I2C地址（ADD0=0）
#define BE2_I2C_ADDR       0x32

// I2C总线和超时时间（单位us）
#define BE2_I2C            I2C2
#define I2C_TIMEOUT_US     100000

// 函数接口
void BE2_I2C_Init(void);

uint8_t BE2_I2C_WriteReg(uint8_t dev_addr, uint8_t reg_addr, uint8_t data);
uint8_t BE2_I2C_WriteRegs(uint8_t dev_addr, uint8_t reg_addr, uint8_t *buf, uint16_t len);

uint8_t BE2_I2C_ReadReg(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data);
uint8_t BE2_I2C_ReadRegs(uint8_t dev_addr, uint8_t reg_addr, uint8_t *buf, uint16_t len);

void I2C_Scan(i2c_type *i2c);
uint8_t I2C_VerifyDevice(i2c_type *i2c, uint8_t device_addr, uint8_t reg);

#endif
