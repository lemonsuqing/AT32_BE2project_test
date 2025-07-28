/* add user code begin Header */
/**
  **************************************************************************
  * @file     wk_spi.c
  * @brief    work bench config program
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
#include "wk_spi.h"

/* add user code begin 0 */

/* add user code end 0 */

/**
  * @brief  init spi2 function
  * @param  none
  * @retval none
  */
void wk_spi2_init(void)
{
  /* add user code begin spi2_init 0 */

  /* add user code end spi2_init 0 */

  gpio_init_type gpio_init_struct;
  spi_init_type spi_init_struct;

  gpio_default_para_init(&gpio_init_struct);
  spi_default_para_init(&spi_init_struct);

  /* add user code begin spi2_init 1 */

  /* add user code end spi2_init 1 */

  /* configure the SCK pin */
  gpio_pin_mux_config(GPIOB, GPIO_PINS_SOURCE1, GPIO_MUX_6);
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_pins = GPIO_PINS_1;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init(GPIOB, &gpio_init_struct);

  /* configure the MISO pin */
  gpio_pin_mux_config(GPIOC, GPIO_PINS_SOURCE2, GPIO_MUX_5);
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_pins = GPIO_PINS_2;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init(GPIOC, &gpio_init_struct);

  /* configure the MOSI pin */
  gpio_pin_mux_config(GPIOC, GPIO_PINS_SOURCE1, GPIO_MUX_6);
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_pins = GPIO_PINS_1;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init(GPIOC, &gpio_init_struct);

  /* configure param */
  spi_init_struct.transmission_mode = SPI_TRANSMIT_FULL_DUPLEX;
  spi_init_struct.master_slave_mode = SPI_MODE_MASTER;
  spi_init_struct.frame_bit_num = SPI_FRAME_8BIT;
  spi_init_struct.first_bit_transmission = SPI_FIRST_BIT_MSB;
  spi_init_struct.mclk_freq_division = SPI_MCLK_DIV_8;
  spi_init_struct.clock_polarity = SPI_CLOCK_POLARITY_LOW;
  spi_init_struct.clock_phase = SPI_CLOCK_PHASE_1EDGE;
  spi_init_struct.cs_mode_selection = SPI_CS_SOFTWARE_MODE;
  spi_init(SPI2, &spi_init_struct);

  /* add user code begin spi2_init 2 */
  // 添加片选引脚(PC3)初始化
  gpio_init_struct.gpio_pins = GPIO_PINS_3;
  gpio_init_struct.gpio_mode = GPIO_MODE_OUTPUT;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_pull = GPIO_PULL_UP;
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
  gpio_init(GPIOC, &gpio_init_struct);
  gpio_bits_set(GPIOC, GPIO_PINS_3); // 初始状态不选中
  /* add user code end spi2_init 2 */

  spi_enable(SPI2, TRUE);

  /* add user code begin spi2_init 3 */

  /* add user code end spi2_init 3 */
}

/* add user code begin 1 */
void spi2_cs_enable(void) {
    gpio_bits_reset(GPIOC, GPIO_PINS_3);
}

void spi2_cs_disable(void) {
    gpio_bits_set(GPIOC, GPIO_PINS_3);
}

uint8_t spi2_read_write_byte(uint8_t data) {
    uint32_t timeout = 100000;

    // 检查SPI状态 - 使用正确的标志名
    if(spi_i2s_flag_get(SPI2, SPI_I2S_TDBE_FLAG) == RESET) {
        printf("SPI TX buffer not empty!\r\n");
    }

    // 等待发送缓冲区为空
    while (spi_i2s_flag_get(SPI2, SPI_I2S_TDBE_FLAG) == RESET) {
        if (--timeout == 0) {
            printf("SPI TX timeout! Status: 0x%04X\r\n", SPI2->sts);
            return 0xFF;
        }
    }

    // 发送数据
    spi_i2s_data_transmit(SPI2, data);
    printf("SPI Sent: 0x%02X\r\n", data);

    timeout = 100000;
    // 等待接收缓冲区有数据
    while (spi_i2s_flag_get(SPI2, SPI_I2S_RDBF_FLAG) == RESET) {
        if (--timeout == 0) {
            printf("SPI RX timeout! Status: 0x%04X\r\n", SPI2->sts);
            return 0xFF;
        }
    }

    uint8_t ret = spi_i2s_data_receive(SPI2);
    printf("SPI Received: 0x%02X\r\n", ret);
    return ret;
}
/* add user code end 1 */
