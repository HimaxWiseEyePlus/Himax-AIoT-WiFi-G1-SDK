#ifndef _COMMON_H
#define _COMMON_H

#include "stdio.h"
#include "stdlib.h"
#include "arc_builtin.h"
#include "embARC_toolchain.h"
#include "embARC_debug.h"

//#include "hx_drv_iomux.h"
//#include "hx_drv_uart.h"
//#include "spi_slave_protocol.h"
//#include "uart_protocol.h"
#include "board_config.h"
#include "io_config.h"
//#include "spi_protocol.h"
//#include "peripheral_cmd_handler.h"
#include "board.h"
//#include "app_algo.h"

#include "arc.h"
#include "arc_timer.h"

/* Color Print */
#define CPNT_RED "\033[1;31m"
#define CPNT_GRN "\033[1;32m"
#define CPNT_BLU "\033[1;34m"
#define CPNT_END "\033[m"

#ifdef __Xdmac
#include "arc_udma.h"
#endif

#endif