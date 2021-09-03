#include "common.h"
#include "hx_drv_tflm.h"
#ifdef SPI_MASTER_SEND
#include "spi_master_protocol.h"
#else
#include "spi_slave_protocol.h"
#endif
#include "app_api.h"


/* local variables */
static hx_drv_sensor_image_config_t g_pimg_config;


/********************** Sensor *************************/
int32_t app_sensor_init()
{
    dbg_printf(DBG_LESS_INFO, "app_sensor_init...\n");

    if (hx_drv_sensor_initial(&g_pimg_config) != HX_DRV_LIB_PASS)
    {
        dbg_printf(DBG_LESS_INFO, "hx_drv_sensor_initial error\n");
        return -1;
    }

    dbg_printf(DBG_LESS_INFO, "app_sensor_init done.\n");

    return 0;
}

uint8_t app_sensor_get_image(uint32_t *p_raw_addr, uint32_t *p_img_width, uint32_t *p_img_height, uint32_t *p_jpg_addr, uint32_t *p_jpg_size)
{
    /* capture image by sensor */
    hx_drv_sensor_capture(&g_pimg_config);
#if 0
    dbg_printf(DBG_LESS_INFO, "\ng_pimg_config.jpeg_address:0x%x size : %d \n",g_pimg_config.jpeg_address,g_pimg_config.jpeg_size);
    dbg_printf(DBG_LESS_INFO, "img_width: %d img_height : %d \n",g_pimg_config.img_width,g_pimg_config.img_height);
#endif

    /* RAW info */
    *p_raw_addr = g_pimg_config.raw_address;
    *p_img_width = g_pimg_config.img_width;
    *p_img_height = g_pimg_config.img_height;

    /* JPG info */
    *p_jpg_addr = g_pimg_config.jpeg_address;
    *p_jpg_size = g_pimg_config.jpeg_size;

    return 0;
}

/********************** Algorithm *************************/
int32_t app_algo_init()
{
    int32_t ret;

    dbg_printf(DBG_LESS_INFO, "app_algo_init...\n");
    ret = tflitemicro_algo_init();

    return ret;
}

int32_t app_algo_run(uint32_t image_addr, uint32_t image_width, uint32_t image_height, struct_algoResult *p_algo_result)
{
    int32_t ret;

    memset(p_algo_result, 0, sizeof(struct_algoResult));
    ret = tflitemicro_algo_run(image_addr, image_width, image_height, p_algo_result);

    return ret;
}

/********************** SPI *************************/
int32_t app_spi_init()
{
    int32_t ret;

    dbg_printf(DBG_LESS_INFO, "app_spi_init...\n");
#ifdef SPI_MASTER_SEND
    ret = hx_drv_spi_mst_open();
    dbg_printf(DBG_LESS_INFO, "SPI master ");
#else
    ret = hx_drv_spi_slv_open();
    dbg_printf(DBG_LESS_INFO, "SPI slave ");
#endif
    return ret;
}

int32_t app_spi_write(uint32_t addr, uint32_t size, SPI_CMD_DATA_TYPE data_type)
{
#ifdef SPI_MASTER_SEND
    return hx_drv_spi_mst_protocol_write_sp(addr, size, data_type);
#else
    return hx_drv_spi_slv_protocol_write_simple_ex(addr, size, data_type);
#endif
}

