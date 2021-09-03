#ifndef __APP_API_H__
#define __APP_API_H__

#include "common.h"
#include "spi_protocol.h"
#include "tflitemicro_algo.h"

/* Sensor */
int32_t app_sensor_init();
uint8_t app_sensor_get_image(uint32_t *p_raw_addr, uint32_t *p_img_width, uint32_t *p_img_height, uint32_t *p_jpg_addr, uint32_t *p_jpg_size);

/* Algorithm */
int32_t app_algo_init();
int32_t app_algo_run(uint32_t image_addr, uint32_t image_width, uint32_t image_height, struct_algoResult *p_algo_result);

/* SPI */
int32_t app_spi_init();
int32_t app_spi_write(uint32_t addr, uint32_t size, SPI_CMD_DATA_TYPE data_type);

#endif

