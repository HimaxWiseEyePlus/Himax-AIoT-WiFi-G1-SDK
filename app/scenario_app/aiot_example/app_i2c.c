#include "embARC.h"
#include "embARC_debug.h"


#include "i2c_comm.h"
#include "ota_update.h"

/* I2C macro define */
#define DATA_SFT_OFFSET_0       0
#define DATA_SFT_OFFSET_8       8
#define DATA_SFT_OFFSET_16      16
#define DATA_SFT_OFFSET_24      24

/* I2C prototypes */
static void i2ccomm_callback_fun_tx(void *param);
static void i2ccomm_callback_fun_rx(void *param);
static void i2ccomm_callback_fun_err(void *param);

/* I2C local variables */
static uint8_t i2c_rx_ready = false;
uint8_t gRead_buf[I2CCOMM_MAX_RBUF_SIZE];
uint8_t gWrite_buf[I2CCOMM_MAX_RBUF_SIZE];


static I2CCOMM_CFG_T gI2CCOMM_cfg = {
    BOARD_DFSS_IICS_SLVADDR,
    i2ccomm_callback_fun_tx,
    i2ccomm_callback_fun_rx,
    i2ccomm_callback_fun_err,
};

static void i2ccomm_callback_fun_tx(void *param)
{
    return;
}

static void i2ccomm_callback_fun_rx(void *param)
{
    i2c_rx_ready = true;
    return;
}

static void i2ccomm_callback_fun_err(void *param)
{
    return;
}

void app_i2c_cmd_init()
{
    I2CCOMM_STATUS_E status = I2CCOMM_STATUS_UNKNOWN;

    dbg_printf(DBG_LESS_INFO, "app_i2c_cmd_init...\n");
    hx_lib_i2ccomm_init(gI2CCOMM_cfg);
    hx_lib_i2ccomm_status(&status);
    if (status == I2CCOMM_STATUS_OPEN)
    {
        dbg_printf(DBG_LESS_INFO,"I2C is already Open.\n");
        return;
    }

    hx_lib_i2ccomm_register_cb(i2ccomm_callback_fun_rx, i2ccomm_callback_fun_tx);
    hx_lib_i2ccomm_start(gRead_buf, I2CCOMM_MAX_RBUF_SIZE);
}

void app_i2c_cmd_polling()
{
    if (i2c_rx_ready) {
        uint8_t feature_type = 0;

#if 0
        int32_t i;

        dbg_printf(DBG_LESS_INFO,"app_i2c_loop> i2c_rx_ready\n");
        for (i = 0; i < 8; i++) {
            dbg_printf(DBG_LESS_INFO,"gRead_buf[%d] = 0x%02x\n", i, gRead_buf[i]);
        }
#endif
        feature_type = gRead_buf[I2CFMT_FEATURE_OFFSET];
        dbg_printf(DBG_LESS_INFO, "[MSG] I2CCOMM_SYS_FEATURE: 0x%x \n", feature_type);

        switch (feature_type)
        {
            case I2CCOMM_FEATURE_OTA_RESERVED:
                i2ccomm_cmd_process_ota_flow();
                break;
            case I2CCOMM_FEATURE_CUSTOMER_MIN:
                dbg_printf(DBG_LESS_INFO, "[MSG] I2CCOMM_FEATURE_CUSTOMER_MIN: 0x%x \n", feature_type);
                break;
            default:
                break;
        }

        memset(gRead_buf, 0xFF, 4);
        i2c_rx_ready = false;
        hx_lib_i2ccomm_enable_read(gRead_buf, I2CCOMM_MAX_RBUF_SIZE);
    }
}

