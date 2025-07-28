#ifndef __BE2_SPI_H__
#define __BE2_SPI_H__

#include <stdint.h>
#include <stdbool.h>
#include "at32f402_405_wk_config.h"

typedef struct {
    float roll;
    float pitch;
    float yaw;
} lpms_data_t;

void lpms_be2_init(void);

bool lpms_enter_command_mode(void);
void lpms_set_output_format_euler(void);
void lpms_set_output_frequency(uint8_t hz);
void lpms_start_streaming(void);

bool lpms_read_euler_angles(lpms_data_t *data);

#endif
