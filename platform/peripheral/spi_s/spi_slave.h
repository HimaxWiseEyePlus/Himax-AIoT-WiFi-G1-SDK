/* ==========================================================================
* Synopsys DesignWare Sensor and Control IP Subsystem IO Software Driver and
* documentation (hereinafter, "Software") is an Unsupported proprietary work
* of Synopsys, Inc. unless otherwise expressly agreed to in writing between
* Synopsys and you.
*
* The Software IS NOT an item of Licensed Software or Licensed Product under
* any End User Software License Agreement or Agreement for Licensed Product
* with Synopsys or any supplement thereto. You are permitted to use and
* redistribute this Software in source and binary forms, with or without
* modification, provided that redistributions of source code must retain this
* notice. You may not view, use, disclose, copy or distribute this file or
* any information contained herein except pursuant to this license grant from
* Synopsys. If you do not agree with this notice, including the disclaimer
* below, then you are not authorized to use the Software.
*
* THIS SOFTWARE IS BEING DISTRIBUTED BY SYNOPSYS SOLELY ON AN "AS IS" BASIS
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE HEREBY DISCLAIMED. IN NO EVENT SHALL SYNOPSYS BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
* LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
* OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
* DAMAGE.
* ========================================================================== */
/*==========================================================================
* Library DW_DFSS-1.1.6
* ========================================================================== */

#ifndef SPI_SLAVE_H_
#define SPI_SLAVE_H_

#include <platform/inc/ioctl.h>

//for test non-interrupt transfer
#define NON_INT_USE 1
#define test_ERR_INT 1

/* SPI slave control commands */
#define IO_SPI_SLAVE_SET_SRL_MODE          (0x20)
#define IO_SPI_SLAVE_SET_TRANSFER_MODE     (0x21)
#define IO_SPI_SLAVE_SET_SCPH              (0x23)
#define IO_SPI_SLAVE_SET_SCPL              (0x24)
#define IO_SPI_SLAVE_SET_DFS               (0x25)
#define IO_SPI_SLAVE_SET_OUTPUT_ENABLE     (0x26)
#define IO_SPI_SLAVE_SET_RX_THRESHOLD      (0x28)
#define IO_SPI_SLAVE_SET_TX_THRESHOLD      (0x29)
#define IO_SPI_SLAVE_SET_PACKING           (0x30)

/* SPI shift register modes */
#define SPI_NORMAL_SRL_MODE         (0)
#define SPI_TEST_SRL_MODE           (1)

/* SPI transfer modes       */
#define SPI_TRANSMIT_RECEIVE_MODE   (0)
#define SPI_TRANSMIT_ONLY_MODE      (1)
#define SPI_RECEIVE_ONLY_MODE       (2)

typedef void (*spis_drv_cb_t) (void );

extern uint32_t io_spi_slave_open(uint32_t dev_id);
extern void io_spi_slave_close(uint32_t dev_id);
/* I2C master valid IOCTLs
  cmd                           arg type        arg value
  IO_SET_CB_RX                  io_cb_t         Callback function invoked from ISR when read successfully finishes
  IO_SET_CB_TX                  io_cb_t         Callback function invoked from ISR when write successfully finishes
  IO_SET_CB_ERR                 io_cb_t         Callback function invoked from ISR when read/write fails
  IO_SPI_SLAVE_SET_SRL_MODE     uint32_t        SPI shift register operation mode:
                                                0 = normal operation mode, Tx to Rx register loop disabled
                                                1 = test operation mode, Tx to Rx register loop enabled
  IO_SPI_SLAVE_SET_TRANSFER_MODE    uint32_t    SPI transfer mode:
                                                0 = transmit and receive
                                                1 = transmit only
                                                2 = receive only
  IO_SPI_SLAVE_SET_SCPH         uint32_t        SPI closk phase: inactive state of SPI clock is low (0) or high (1)
  IO_SPI_SLAVE_SET_SCPL         uint32_t        SPI clock polarity: SPI clock toggles in middle of first data bit (0) or at start of first data bit (1)
  IO_SPI_SLAVE_SET_DFS          uint32_t        SPI frame size in bits
  IO_SPI_SLAVE_SET_OUTPUT_ENABLE    uint32_t    Enables (1) or disables (0) slave output
  IO_SPI_SLAVE_SET_RX_THRESHOLD uint32_t        threshold value for the Rx FIFO
  IO_SPI_SLAVE_SET_TX_THRESHOLD uint32_t        threshold value for the Tx FIFO
  IO_SPI_SLAVE_SET_PACKING      uint32_t        1|0 = enable|disable data packing in Rx/Tx buffer

*/

extern void io_spi_slave_ioctl(uint32_t dev_id, uint32_t cmd, void *arg);
extern void io_spi_slave_read(uint32_t dev_id, uint8_t * data,
                  uint32_t size);
extern void io_spi_slave_read_pp(uint32_t dev_id, uint8_t * data_p1, uint8_t * data_p2,
                  uint32_t size);
extern void io_spi_slave_read_pp_halt(uint32_t dev_id);

extern void io_spi_slave_write(uint32_t dev_id, uint8_t * data,
                   uint32_t * size);
extern void io_spi_slave_write_ptl(uint32_t dev_id, uint8_t * data,
                   uint32_t * size, uint8_t data_type);
extern void io_spi_slave_write_ptl_cus(uint32_t dev_id, uint8_t * data, 
                   uint32_t * size, const void *header_buf, uint32_t header_len);
extern void io_spi_slave_write_halt(uint32_t dev_id);

extern void io_spi_slave_register_tx_cb(spis_drv_cb_t aWritecb);
extern void io_spi_slave_register_rx_cb(spis_drv_cb_t aReadcb);
extern void io_spi_slave_register_rx_evt_cb(spis_drv_cb_t aReadEvtcb);

#ifdef NON_INT_USE
extern void io_spi_slave_check_status(uint32_t dev_id, uint8_t *busy, uint8_t *tx_full, uint8_t *tx_empty, uint8_t *rx_not_empty, uint8_t *rx_full, uint8_t *err);
extern int8_t upper_fill_fifo(uint32_t dev_id, uint8_t * data, uint32_t * size, uint8_t first_flag);
extern int8_t upper_retrieve_fifo(uint32_t dev_id, uint8_t * data, uint32_t * size, uint8_t first_flag);
#endif
#ifdef test_ERR_INT
extern void set_err_interrupt(uint32_t dev_id);
#endif

#endif              /* SPI_SLAVE_H_ */
