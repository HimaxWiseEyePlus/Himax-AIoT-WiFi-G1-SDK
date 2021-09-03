#ifndef INC_SPI_S_PROTOCOL_H_
#define INC_SPI_S_PROTOCOL_H_

#include "dev_common.h"
#include "spi_protocol.h"


#ifdef __cplusplus
extern "C" {
#endif

#ifdef SPI_SLAVE_HANDSHAKE_IC
#define HANDSHAKE_PIN_TX 4
#define HANDSHAKE_PIN_RX 5
#else
#define HANDSHAKE_PIN_TX 10
#define HANDSHAKE_PIN_RX 1
#endif



/**
 * \brief	initial spi slave device
 *
 * \param[in]	none
 * \return	status of operation, return 0 is pass
 */
extern int hx_drv_spi_slv_open();

/**
 * \brief	spi slave device read data with protocol
 *
 * \param[in]	SRAM_addr	 System memory address for data storage
 * \param[in]	img_size	 data size pointer, which will save size of read data
 * \return	status of operation, return 0 is pass, -1 is fail, -2 is spi not yet enable by I2C command
 */
extern int hx_drv_spi_slv_protocol_read_simple(uint32_t SRAM_addr, uint32_t *img_size );

/**
 * \brief	spi slave device read data with protocol(method with user-defined protocl)
 *
 * \param[in]	SRAM_addr	 System memory address for data storage
 * \param[in]	img_size	 data size pointer, which will save size of read data
 * \param[in]   header_len   header size pointer, which will save lenght of protocol header.
 * \return	status of operation, return 0 is pass, -1 is fail, -2 is spi not yet enable by I2C command
 */
extern int hx_drv_spi_slv_protocol_write_simple_cus(uint32_t SRAM_addr, uint32_t img_size, uint32_t header_len);

/**
 * \brief	spi slave device write data with protocol (non-blocking)
 *
 * \param[in]	SRAM_addr	 System memory address for data write out
 * \param[in]	img_size	 data size to write out
 * \return	status of operation, return 0 is pass, -1 is fail, -2 is spi not yet enable by I2C command
 */
extern int hx_drv_spi_slv_protocol_write_simple(uint32_t SRAM_addr, uint32_t img_size);

/**
 * \brief	spi slave device write data with protocol (non-blocking)
 *
 * \param[in]	SRAM_addr	 System memory address for data write out
 * \param[in]	img_size	 data size to write out
 * \param[in]	data_type	data type to write , reference SPI_CMD_DATA_TYPE
 * \return	status of operation, return 0 is pass, -1 is fail, -2 is spi not yet enable by I2C command
 */
extern int hx_drv_spi_slv_protocol_write_simple_ex(uint32_t SRAM_addr, uint32_t img_size, SPI_CMD_DATA_TYPE data_type);

/**
 * \brief	check spi slave device write status
 *
 * \param[in]	none
 * \return	status of operation, return 0 is okay to read/write, -1 is in busy status
 */
extern int hx_drv_spi_slv_protocol_write_busy_status();

/**
 * \brief	stop spi slave device from write operation
 *
 * \param[in]	none
 * \return	status of operation, return 0 is halt operation done, -1 is halt fail
 */
extern int hx_drv_spi_slv_protocol_write_halt();
#ifdef __cplusplus
}
#endif
#endif
