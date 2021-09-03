#include "common.h"
#include "ota_update.h"
#include "ota.h"

I2CCOMM_ERROR_E i2ccomm_cmd_process_ota_flow(void)
{
    int retval = 0;
    unsigned char cmd;
    bool udma_status=0;

    cmd = gRead_buf[I2CFMT_COMMAND_OFFSET];

    retval = hx_lib_i2ccomm_validate_checksum(gRead_buf);

    if (retval != I2CCOMM_NO_ERROR)
    {
        set_ota_result(cmd, OTA_ERROR_CHECKSUM);
        return I2CCOMM_ERROR_CHECKSUM;
    }

    switch (cmd)
    {
        case I2CCOMM_CMD_OTA_JUMP2UPG:
            dbg_printf(DBG_LESS_INFO, "===I2CCOMM_CMD_OTA_JUMP2UPG===\n");            
            udma_status = dmac_active();
            if(udma_status == 1)
                dmac_deinit();
            HX_UNCACHE_BIT_SET((uint32_t)0xB0000070,(uint32_t)INDICATION_FLAG_SIZE,0,(uint32_t)INDICATION_OTA);
            hx_lib_pm_cpu_rst();
            break;
        default:
            break;
    }

    return I2CCOMM_NO_ERROR;
}
