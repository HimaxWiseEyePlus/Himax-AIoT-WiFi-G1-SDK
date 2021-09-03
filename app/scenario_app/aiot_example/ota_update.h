#ifndef __OTA_UPDATE_H__
#define __OTA_UPDATE_H__

#include "i2c_comm.h"

extern unsigned char gRead_buf[I2CCOMM_MAX_RBUF_SIZE];
extern unsigned char gWrite_buf[I2CCOMM_MAX_RBUF_SIZE];

I2CCOMM_ERROR_E i2ccomm_cmd_process_ota_flow(void);

#endif

