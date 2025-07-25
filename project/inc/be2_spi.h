#ifndef __BE2_SPI_H__
#define __BE2_SPI_H__

#include <stdint.h>


#define LPMS_CS_PORT   GPIOC
#define LPMS_CS_PIN    GPIO_PINS_3
#define LPMS_CS_LOW()  gpio_bits_reset(LPMS_CS_PORT, LPMS_CS_PIN)
#define LPMS_CS_HIGH() gpio_bits_set(LPMS_CS_PORT, LPMS_CS_PIN)

void be2_spi_init_cs_gpio(void);
void be2_enter_command_mode(void);
uint8_t be2_read_whoami_lpb(void);

#endif
