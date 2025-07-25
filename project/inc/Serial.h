/*
 * Serial.h
 *
 *  Created on: 2025 May 27
 *      Author: HYS
 */

#ifndef _SERIAL_H_
#define _SERIAL_H_

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>

void Serial_SendByte(uint8_t Byte);
void Serial_SendArray(uint8_t *Array, uint16_t Length);
void Serial_SendString(char *String);
uint32_t Serial_Pow(uint32_t X, uint32_t Y);
void Serial_SendNumber(uint32_t Number, uint8_t Length);
int fputc(int ch, FILE *f);
void Serial_Printf(char *format, ...);
uint8_t Serial_GetRxFlag(void);
uint8_t Serial_GetRxData(void);
void Serial_Printf_DMA(const char *fmt, ...);


#endif /*_SERIAL_H_ */
