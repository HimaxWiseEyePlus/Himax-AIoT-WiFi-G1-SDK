/**
 * @file hx_drv_spi_s.h
 * @author himax/902452
 * @brief 
 * @version V1.0.0
 * @date 2018-10-05
 * 
 * @copyright (C) COPYRIGHT, Himax, Inc. ALL RIGHTS RESERVED
 * 
 */

#ifndef INC_HX_DRV_SPI_S_H_
#define INC_HX_DRV_SPI_S_H_

#include "dev_common.h"

/**
 * \defgroup	DEVICE_HAL_SPI_SLV_OBJ	SPI Slave Device Object Instance
 * \ingroup	DEVICE_HAL_SPI_SLV
 * \brief	Macros for spi slave instance.
 * \details	Definitions for spi slave instance.
 * @{
 */
#define DFSS_SPI_SLV_NUM		(1)	/*!< Datafusion SPI slave valid number */

#define DFSS_SPI_SLV_0_ID		0	/*!< SPI 0 id macro (slave node) */
#define DFSS_SPI_SLV_ALL_ID	DFSS_SPI_SLV_NUM

typedef enum USE_DFSS_SPI_SLV_S
{
	USE_DFSS_SPI_SLV_0 = DFSS_SPI_SLV_0_ID,    		/*!< enable use datafusion subsystem spi slave 0 */
	USE_DFSS_SPI_SLV_ALL = DFSS_SPI_SLV_ALL_ID,		/*!< enable use datafusion subsystem spi slave all */
	USE_DFSS_SPI_SLV_MAX
}USE_DFSS_SPI_SLV_E;

/** @} */


/**
 * \defgroup	DEVICE_HAL_SPI_SLV_CTRLCMD		SPI Slave Device Control Commands
 * \ingroup	DEVICE_HAL_SPI_SLV
 * \brief	Definitions for spi slave control command, used in \ref dev_spi::spi_control "SPI IO Control"
 * \details	These commands defined here can be used in user code directly.
 * - Parameters Usage
 *   - For passing parameters like integer, just use uint32_t/int32_t to directly pass values
 *   - For passing parameters for a structure, please use pointer to pass values
 *   - For getting some data, please use pointer to store the return data
 * - Common Return Values
 *   - \ref E_OK,	Control device successfully
 *   - \ref E_CLSED,	Device is not opened
 *   - \ref E_OBJ,	Device object is not valid or not exists
 *   - \ref E_PAR,	Parameter is not valid for current control command
 *   - \ref E_SYS,	Control device failed, due to hardware issues such as device is disabled
 *   - \ref E_CTX,	Control device failed, due to different reasons like in transfer state
 *   - \ref E_NOSPT,	Control command is not supported or not valid
 * - Usage Comment
 *   - For SPI poll or interrupt read/write/transfer operations, only 1 operation can be triggered.
 *     If there is a operation is running, any other operation will return \ref E_CTX
 *   - If SPI is in transfer, then the following operations may return \ref E_CTX. Like
 *     \ref SPI_SLV_CMD_SET_CLK_MODE, \ref SPI_SLV__CMD_SET_TXINT_BUF, \ref SPI_SLV_CMD_SET_RXINT_BUF,
 *     \ref SPI_SLV_CMD_SET_TXINT, \ref SPI_SLV__CMD_SET_RXINT, \ref SPI_SLV_CMD_ABORT_TX, \ref SPI_SLV_CMD_ABORT_RX,
 *     \ref SPI_SLV_CMD_FLUSH_TX, \ref SPI_SLV__CMD_FLUSH_RX, \ref SPI_SLV_CMD_SET_DFS, \ref SPI_SLV_CMD_TRANSFER_POLLING,
 *     \ref SPI_SLV_CMD_TRANSFER_INT, \ref SPI_SLV__CMD_ABORT_XFER, \ref SPI_SLV_CMD_MST_SEL_DEV, \ref SPI_SLV_CMD_MST_DSEL_DEV,
 *     \ref SPI_SLV_CMD_MST_SET_FREQ and \ref dev_spi::spi_write "SPI Poll Write" or \ref dev_spi::spi_read "SPI Poll Read".
 * @{
 */

/** Define SPI control commands for common usage */
#define DEV_SET_SPI_SYSCMD(cmd)		DEV_SET_SYSCMD((cmd))
/** Define SPI control commands for master usage */
//#define DEV_SET_SPI_MST_SYSCMD(cmd)	DEV_SET_SYSCMD(0x00001000|(cmd))
/** Define SPI control commands for slave usage */
#define DEV_SET_SPI_SLV_SYSCMD(cmd)	DEV_SET_SYSCMD(0x00002000|(cmd))


/* ++++ Common commands for SPI Device ++++ */
/**
 * Get \ref dev_spi_info::status "current device status"
 * - Param type : uint32_t *
 * - Param usage : store result of current status
 * - Return value explanation :
 */
#define SPI_CMD_GET_STATUS			DEV_SET_SPI_SYSCMD(0)
/**
 * set the \ref dev_spi_info::clk_mode "clock mode" of spi slave transfer
 * - Param type : uint32_t
 * - Param usage : spi slave clock mode to choose clock phase and clock polarity
 * - Return value explanation :
 */
#define SPI_CMD_SET_CLK_MODE			DEV_SET_SPI_SYSCMD(1)
/**
 * set spi slave \ref dev_spi_info::dfs "data frame size"
 * - Param type : uint32_t
 * - Param usage : should > 0
 * - Return value explanation : If dfs is not supported, then return \ref E_SYS
 */
#define SPI_CMD_SET_DFS				DEV_SET_SPI_SYSCMD(2)
/**
 * set the \ref dev_spi_info::dummy "dummy data" during spi slave transfer
 * - Param type : uint32_t
 * - Param usage : dummy data to transfer
 * - Return value explanation :
 */
#define SPI_CMD_SET_DUMMY_DATA			DEV_SET_SPI_SYSCMD(3)
/**
 * Set \ref dev_spi_cbs::tx_cb "spi slave transmit success callback" function
 * when all required bytes are transmitted for interrupt method
 * - Param type : \ref DEV_CALLBACK * or NULL
 * - Param usage : transmit success callback function for spi
 * - Return value explanation :
 */
#define SPI_CMD_SET_TXCB			DEV_SET_SPI_SYSCMD(4)
/**
 * Set \ref dev_spi_cbs::rx_cb "spi slave receive success callback" function
 * when all required bytes are received for interrupt method
 * - Param type : \ref DEV_CALLBACK * or NULL
 * - Param usage : receive success callback function for spi
 * - Return value explanation :
 */
#define SPI_CMD_SET_RXCB			DEV_SET_SPI_SYSCMD(5)
/**
 * Set \ref dev_spi_cbs::xfer_cb "spi slave transfer success callback" function
 * when all required transfer are done for interrupt method
 * - Param type : \ref DEV_CALLBACK * or NULL
 * - Param usage : transfer success callback function for spi
 * - Return value explanation :
 */
#define SPI_CMD_SET_XFERCB			DEV_SET_SPI_SYSCMD(6)
/**
 * Set \ref dev_spi_cbs::err_cb "spi slave transfer error callback" function
 * when something error happened for interrupt method
 * - Param type : \ref DEV_CALLBACK * or NULL
 * - Param usage : transfer error callback function for spi
 * - Return value explanation :
 */
#define SPI_CMD_SET_ERRCB			DEV_SET_SPI_SYSCMD(7)
/**
 * Set buffer in interrupt transmit, and it will set \ref dev_spi_info::xfer "spi slave tranfer".
 * - SPI master and slave mode use case  \n
 *    For both master and slave mode, if you set tx buffer to NULL, when tx interrupt is enabled and entered into tx interrupt,
 * it will automatically disable the tx interrupt, so when you want to transfer something, you need to set the
 * tx buffer to Non-NULL and enable tx interrupt, when the tx buffer is sent, it will disable the tx interrupt
 * and call tx callback function if available.
 * - Param type : DEV_BUFFER * or NULL
 * - Param usage : buffer structure pointer, if param is NULL, then it will set xfer to empty
 * - Return value explanation :
 */
#define SPI_CMD_SET_TXINT_BUF			DEV_SET_SPI_SYSCMD(8)
/**
 * Set buffer in interrupt receive, and it will set \ref dev_spi_info::xfer "spi slave tranfer".
 * - SPI master mode use case  \n
 *   Similar to \ref SPI_CMD_SET_TXINT_BUF
 * - SPI slave mode use case   \n
 *   Similiar to \ref SPI_CMD_SET_TXINT_BUF
 * - Param type : DEV_BUFFER * or NULL
 * - Param usage : buffer structure pointer, if param is NULL, then it will set xfer to empty
 * - Return value explanation :
 */
#define SPI_CMD_SET_RXINT_BUF			DEV_SET_SPI_SYSCMD(9)
/**
 * Enable or disable transmit interrupt,
 * for master mode, only one of tx and rx interrupt can be enabled,
 * if tx interrupt is enabled, then rx interrupt can't be enabled.
 * - Param type : uint32_t
 * - Param usage : enable(none-zero) or disable(zero) flag
 * - Return value explanation :
 */
#define SPI_CMD_SET_TXINT			DEV_SET_SPI_SYSCMD(10)
/**
 * Enable or disable receive interrupt,
 * for master mode, only one of tx and rx interrupt can be enabled,
 * if rx interrupt is enabled, then tx interrupt can't be enabled.
 * - Param type : uint32_t
 * - Param usage : enable(none-zero) or disable(zero) flag
 * - Return value explanation :
 */
#define SPI_CMD_SET_RXINT			DEV_SET_SPI_SYSCMD(11)
/**
 * start the transfer by polling
 * - Param type : \ref DEV_SPI_TRANSFER *
 * - Param usage :
 * - Return value explanation :
 */
#define SPI_CMD_TRANSFER_POLLING		DEV_SET_SPI_SYSCMD(12)
/**
 * start the transfer by interrupt
 * - Param type : \ref DEV_SPI_TRANSFER * or NULL
 * - Param usage : If NULL, it will disable transfer interrupt, if not NULL, it will enable transfer interrupt
 * - Return value explanation :
 */
#define SPI_CMD_TRANSFER_INT			DEV_SET_SPI_SYSCMD(13)
/**
 * Abort current interrupt transmit operation if tx interrupt enabled,
 * it will disable transmit interrupt, and set \ref DEV_IN_TX_ABRT
 * in \ref dev_spi_info::status "status" variable,
 * and call the transmit callback function, when tx callback is finished,
 * it will clear \ref DEV_IN_TX_ABRT and return
 * - Param type : NULL
 * - Param usage :
 * - Return value explanation :
 */
#define SPI_CMD_ABORT_TX			DEV_SET_SPI_SYSCMD(14)
/**
 * Abort current interrupt receive operation if rx interrupt enabled,
 * it will disable receive interrupt, and set \ref DEV_IN_TX_ABRT
 * in \ref dev_spi_info::status "status" variable,
 * and call the receive callback function, when rx callback is finished,
 * it will clear \ref DEV_IN_TX_ABRT and return
 * - Param type : NULL
 * - Param usage :
 * - Return value explanation :
 */
#define SPI_CMD_ABORT_RX			DEV_SET_SPI_SYSCMD(15)
/**
 * Abort current interrupt transfer operation if transfer is issued,
 * it will disable transfer interrupt, and set \ref DEV_IN_XFER_ABRT
 * in \ref dev_spi_info::status "status" variable,
 * and call the transfer callback function, when xfer callback is finished,
 * it will clear \ref DEV_IN_XFER_ABRT and return
 * - Param type : NULL
 * - Param usage :
 * - Return value explanation :
 */
#define SPI_CMD_ABORT_XFER			DEV_SET_SPI_SYSCMD(16)
/**
 * Do a software reset for SPI device, it will stop current transfer,
 * and clear error state and bring device to normal state, set next condition to STOP
 * - Param type : NULL
 * - Param usage :
 * - Return value explanation :
 */
#define SPI_CMD_RESET				DEV_SET_SPI_SYSCMD(17)
/**
 * Flush spi slave tx fifo, this will clear the data in tx fifo
 * - Param type : NULL
 * - Param usage :
 * - Return value explanation :
 */
#define SPI_CMD_FLUSH_TX			DEV_SET_SPI_SYSCMD(18)
/**
 * Flush spi slave rx fifo, this will clear the data in rx fifo
 * - Param type : NULL
 * - Param usage :
 * - Return value explanation :
 */
#define SPI_CMD_FLUSH_RX			DEV_SET_SPI_SYSCMD(19)
/**
 * Enable spi slave device
 * - Param type : NULL
 * - Param usage : param is not required
 * - Return value explanation :
 */
#define SPI_CMD_ENA_DEV				DEV_SET_SPI_SYSCMD(20)
/**
 * Disable spi slave device, when device is disabled,
 * only \ref SPI_CMD_ENA_DEV, \ref SPI_CMD_DIS_DEV,
 * \ref SPI_CMD_GET_STATUS and \ref SPI_CMD_RESET
 * commands can be executed, other commands will return \ref E_SYS
 * - Param type : NULL
 * - Param usage : param is not required
 * - Return value explanation :
 */
#define SPI_CMD_DIS_DEV				DEV_SET_SPI_SYSCMD(21)
/**
 * Get how many bytes space in spi slave are available to transmit,
 * this can be used in interrupt callback functions,
 * cooperate with \ref dev_spi::spi_write "spi_write" API to realize non-blocked write
 * - Param type : int32_t *
 * - Param usage : store the write available bytes, > 0 for available bytes, 0 for not available
 * - Return value explanation :
 */
#define SPI_CMD_GET_TXAVAIL			DEV_SET_SPI_SYSCMD(22)
/**
 * Get how many bytes in spi slave are available to receive,
 * this can be used in interrupt callback functions,
 * cooperate with \ref dev_spi::spi_read "spi_read" API to realize non-blocked read
 * - Param type : int32_t *
 * - Param usage : store the read available bytes, > 0 for available bytes, 0 for not available
 * - Return value explanation :
 */
#define SPI_CMD_GET_RXAVAIL			DEV_SET_SPI_SYSCMD(23)

#define SPI_CMD_ERR_INT_TEST		DEV_SET_SPI_SYSCMD(24)
/* ++++ Master only commands for SPI Device ++++ */

/* ++++ Slave only commands for SPI Device ++++ */

/* TODO: HIMAX@WILL add spi slave related CMDs */

/** @} */

/**
 * \defgroup	DEVICE_HAL_SPI_CALLBACK	SPI Interrupt callback functions
 * \ingroup	DEVICE_HAL_SPI
 * \brief	callback function structure for SPI device
 * @{
 */
typedef struct dev_spi_slv_cbs {
	DEV_CALLBACK tx_cb;	/*!< spi slave data transmit success required bytes callback */
	DEV_CALLBACK rx_cb;	/*!< spi slave data receive success required bytes callback */
	DEV_CALLBACK err_cb;	/*!< spi slave error callback */
	DEV_CALLBACK xfer_cb;	/*!< transfer callback */
} DEV_SPI_SLV_CBS, *DEV_SPI_SLV_CBS_PTR;
/** @} */

/**
 * \defgroup	DEVICE_HAL_SPI_CLK_MODE	SPI Clock Mode
 * \ingroup	DEVICE_HAL_SPI
 * \brief	spi slave clock mode settings
 * @{
 */
/** SPI Clock Mode */
typedef enum spi_slv_clk_mode {
	SPI_S_CPOL_0_CPHA_0 = 0,	/*!< Inactive state of serial clock is low, serial clock toggles in middle of first data bit */
	SPI_S_CPOL_0_CPHA_1 = 1,	/*!< Inactive state of serial clock is low, serial clock toggles at start of first data bit  */
	SPI_S_CPOL_1_CPHA_0 = 2,	/*!< Inactive state of serial clock is high, serial clock toggles in middle of first data bit */
	SPI_S_CPOL_1_CPHA_1 = 3,	/*!< Inactive state of serial clock is high, serial clock toggles at start of first data bit */

	SPI_S_CLK_MODE_0    = SPI_S_CPOL_0_CPHA_0,	/*!< Equal to \ref SPI_CPOL_0_CPHA_0 */
	SPI_S_CLK_MODE_1    = SPI_S_CPOL_0_CPHA_1,	/*!< Equal to \ref SPI_CPOL_0_CPHA_1 */
	SPI_S_CLK_MODE_2    = SPI_S_CPOL_1_CPHA_0,	/*!< Equal to \ref SPI_CPOL_1_CPHA_0 */
	SPI_S_CLK_MODE_3    = SPI_S_CPOL_1_CPHA_1	/*!< Equal to \ref SPI_CPOL_1_CPHA_1 */
} SPI_SLV_CLK_MODE;

//typedef SPI_SLV_CLK_MODE				DEV_SPI_SLV_CLK_MODE;

/*!< Default SPI device clock mode */
#define SPI_SLV_CLK_MODE_DEFAULT		SPI_S_CPOL_0_CPHA_0
/** @} */


/**
 * \defgroup	DEVICE_HAL_SPI_DEVSTRUCT	SPI Device Structure
 * \ingroup	DEVICE_HAL_SPI
 * \brief	contains definitions of spi slave device structure.
 * \details	this structure will be used in user implemented code, which was called
 * 	Device Driver Implement Layer for spi slave to realize in user code.
 * @{
 */
typedef struct dev_spi_slv_transfer DEV_SPI_SLV_TRANSFER, *DEV_SPI_SLV_TRANSFER_PTR;
/**
 * \brief	spi slave read and write data structure used by \ref SPI_CMD_TRANSFER
 * 	spi slave write then read data
 *
 */
struct dev_spi_slv_transfer {
	DEV_SPI_SLV_TRANSFER *next;
	/* Calc by software */
	/** tot_len = (tx_totlen>rx_totlen)?tx_totlen:rx_totlen */
	uint32_t tot_len;
	/* Set by user */
	uint8_t *tx_buf;
	uint32_t tx_ofs;
	uint32_t tx_len;
	uint8_t *rx_buf;
	uint32_t rx_ofs;
	uint32_t rx_len;
	/* Should auto set to proper value during set buffer value */
	uint32_t tx_idx;
	uint32_t tx_totlen;	/** tx_totlen = tx_len + tx_ofs */
	uint32_t rx_idx;
	uint32_t rx_totlen;	/** rx_totlen = rx_len + rx_ofs */
};

/** Set tx buffer of device spi slave transfer */
#define DEV_SPI_SLV_XFER_SET_TXBUF(xfer, buf, ofs, len)		{		\
					(xfer)->tx_buf = (uint8_t *)(buf);	\
					(xfer)->tx_len = (uint32_t)(len);	\
					(xfer)->tx_ofs = (uint32_t)(ofs);	\
					(xfer)->tx_idx = 0;			\
					(xfer)->tx_totlen = ( (uint32_t)(len) 	\
							+ (uint32_t)(ofs) ) ;	\
				}

/** Set rx buffer of device spi slave transfer */
#define DEV_SPI_SLV_XFER_SET_RXBUF(xfer, buf, ofs, len)		{		\
					(xfer)->rx_buf = (uint8_t *)(buf);	\
					(xfer)->rx_len = (uint32_t)(len);	\
					(xfer)->rx_ofs = (uint32_t)(ofs);	\
					(xfer)->rx_idx = 0;			\
					(xfer)->rx_totlen = ( (uint32_t)(len) 	\
							+ (uint32_t)(ofs) ) ;	\
				}

/** Calculate total length of current transfer without next transfer */
#define DEV_SPI_SLV_XFER_CALC_TOTLEN(xfer)		(xfer)->tot_len =	\
				((xfer)->tx_totlen > (xfer)->rx_totlen) ? (xfer)->tx_totlen : (xfer)->rx_totlen ;

/** Set next SPI transfer */
#define DEV_SPI_SLV_XFER_SET_NEXT(xfer, next_xfer)	(xfer)->next = (next_xfer);

/** Init spi slave transfer */
#define DEV_SPI_SLV_XFER_INIT(xfer)					{		\
					(xfer)->tx_idx = 0;			\
					(xfer)->rx_idx = 0;			\
					(xfer)->tx_totlen = ((xfer)->tx_len  	\
							+ (xfer)->tx_ofs) ;	\
					(xfer)->rx_totlen = ((xfer)->rx_len  	\
							+ (xfer)->rx_ofs) ;	\
					DEV_SPI_SLV_XFER_CALC_TOTLEN(xfer);		\
				}
/**
 * \brief	spi slave information struct definition
 * \details	informations about spi slave open state, working state,
 * 	frequency, spi slave registers, working method, interrupt number
 */
typedef struct dev_spi_slv_info {
	void *spi_ctrl;		/*!< spi slave control related */
	uint32_t status;	/*!< current working status, refer to \ref DEVICE_HAL_COMMON_DEVSTATUS, this should be \ref DEV_ENABLED for first open */
	uint32_t freq;		/*!< spi slave working baudrate */
	uint8_t mode;		/*!< spi slave working mode (master/slave) */
	uint8_t clk_mode;	/*!< spi slave clock phase and polarity, this should be \ref SPI_CLK_MODE_DEFAULT for first open */
	uint8_t opn_cnt;	/*!< spi slave open count, open it will increase 1, close it will decrease 1, 0 for close, >0 for open */
	uint8_t slave;		/*!< current selected slave device no, start from 0, this should be \ref SPI_SLAVE_NOT_SELECTED for first open */
	uint8_t dfs;		/*!< data frame size, this should be \ref SPI_DFS_DEFAULT for first open */

	DEV_SPI_SLV_TRANSFER xfer;	/*!< spi slave transfer, this should be set to all zero for first open */
	DEV_SPI_SLV_CBS spi_cbs;	/*!< spi slave callbacks, for both master and slave mode, this should be all NULL for first open */
	void *extra;		/*!< a extra pointer to get hook to applications which should not used by bsp developer,
					this should be NULL for first open and you can \ref DEV_SPI_INFO_SET_EXTRA_OBJECT "set"
					or \ref DEV_SPI_INFO_GET_EXTRA_OBJECT "get" the extra information pointer */
	uint32_t dummy;		/*!< dummy write data when send and receive, this should be \ref SPI_DUMMY_DEFAULT for first open */
} DEV_SPI_SLV_INFO, * DEV_SPI_SLV_INFO_PTR;

/** Set extra information pointer of spi slave info */
//#define DEV_SPI_SLV_INFO_SET_EXTRA_OBJECT(spi_info_ptr, extra_info)		(spi_info_ptr)->extra = (void *)(extra_info)
/** Get extra information pointer of spi slave info */
//#define DEV_SPI_SLV_INFO_GET_EXTRA_OBJECT(spi_info_ptr)			((spi_info_ptr)->extra)



/**
 * \brief	spi slave device interface definition
 * \details	define spi slave device interface, like spi slave information structure,
 * 	fuctions to get spi slave info, open/close/control spi, send/receive data by spi
 * \note	all this details are implemented by user in user porting code
 */
typedef struct dev_spi_slv {
	DEV_SPI_SLV_INFO spi_info;						/*!< spi slave device information */
	int32_t (*spi_open) (uint32_t mode, uint32_t param);		/*!< open spi slave device in master/slave mode, \
									when in master mode, param stands for frequency, \
									when in slave mode, param stands for clock mode */
	int32_t (*spi_close) (void);					/*!< close spi slave device */
	int32_t (*spi_control) (uint32_t ctrl_cmd, void *param);	/*!< control spi slave device */
	int32_t (*spi_write) (const void *data, uint32_t len);		/*!< send data to spi slave device (blocking method) */
	int32_t (*spi_write_ptl) (const void *data, uint32_t len, uint8_t data_type);		/*!< send data to spi slave device (blocking method) */
	int32_t (*spi_write_ptl_cus) (const void *header_buf, uint32_t header_len,  /*!< send data to spi slave device (method with user-defined protocol) */
								 const void *data, uint32_t len); 
	int32_t (*spi_write_halt) ();		/*!< send data to spi slave device (blocking method) */
	int32_t (*spi_read) (void *data, uint32_t len);			/*!< read data from spi slave device (blocking method) */
	int32_t (*spi_read_pp) (void *data1, void *data2, uint32_t len);			/*!< read data from spi slave device (recursive ping pong method) len should be smaller than 256KB */
	int32_t (*spi_read_pp_halt) ();			/*!< read data ping pong mode halt  */


} DEV_SPI_SLV, * DEV_SPI_SLV_PTR;

/**
 * \fn		int32_t (* dev_spi::spi_open) (uint32_t mode, uint32_t param)
 * \details	open an spi slave device with selected mode (master or slave) with defined \ref param
 * \param[in]	mode	working mode (\ref DEV_MASTER_MODE "master" or \ref DEV_SLAVE_MODE "slave")
 * \param[in]	param	When mode is \ref DEV_MASTER_MODE, param stands for \ref dev_spi_info::freq "frequency",
 * 			when mode is \ref DEV_SLAVE_MODE, param stands for \ref dev_spi_info::clk_mode "slave clock mode"
 * \retval	E_OK	Open successfully without any issues
 * \retval	E_OPNED	If device was opened before with different parameters,
 *			then just increase the \ref dev_spi_info::opn_cnt "opn_cnt" and return \ref E_OPNED
 * \retval	E_OBJ	Device object is not valid
 * \retval	E_SYS	Device is opened for different mode before, if you want to open it with different mode, you need to fully close it first.
 * \retval	E_PAR	Parameter is not valid
 * \retval	E_NOSPT	Open settings are not supported
 */

/**
 * \fn		int32_t (* dev_spi::spi_close) (void)
 * \details	close an spi slave device, just decrease the \ref dev_spi_info::opn_cnt "opn_cnt",
 *      if \ref dev_spi_info::opn_cnt "opn_cnt" equals 0, then close the device
 * \retval	E_OK	Close successfully without any issues(including scenario that device is already closed)
 * \retval	E_OPNED	Device is still opened, the device \ref dev_spi_info::opn_cnt "opn_cnt" decreased by 1
 * \retval	E_OBJ	Device object is not valid
 */

/**
 * \fn		int32_t (* dev_spi::spi_control) (uint32_t ctrl_cmd, void *param)
 * \details	control an spi slave device by \ref ctrl_cmd, with passed \ref param.
 * 	you can control spi slave device using predefined spi slave control commands defined using \ref DEV_SET_SYSCMD
 * 	(which must be implemented by bsp developer), such as \ref SPI_CMD_FLUSH_TX "flush tx" and \ref DEVICE_HAL_SPI_CTRLCMD "more".
 * 	And you can also control spi slave device using your own specified commands defined using \ref DEV_SET_USRCMD,
 * 	but these specified commands should be defined in your own spi slave device driver implementation.
 * \param[in]		ctrl_cmd	\ref DEVICE_HAL_SPI_CTRLCMD "control command", to change or get some thing related to spi
 * \param[in,out]	param		parameters that maybe argument of the command,
 * 					or return values of the command, must not be NULL
 * \retval	E_OK	Control device successfully
 * \retval	E_CLSED	Device is not opened
 * \retval	E_OBJ	Device object is not valid or not exists
 * \retval	E_PAR	Parameter is not valid for current control command
 * \retval	E_SYS	Control device failed, due to hardware issues, such as device is disabled
 * \retval	E_CTX	Control device failed, due to different reasons like in transfer state
 * \retval	E_NOSPT	Control command is not supported or not valid
 */

/**
 * \fn		int32_t (* dev_spi::spi_write) (const void *data, uint32_t len)
 * \details	send \ref data through spi slave with defined \ref len to slave device .
 * \param[in]	data	pointer to data need to send by spi
 * \param[in]	len	length of data to be sent
 * \retval	>0	Byte count that was successfully sent for poll method
 * \retval	E_OBJ	Device object is not valid or not exists
 * \retval	E_PAR	Parameter is not valid
 * \retval	E_CTX	Device is still in transfer state
 * \retval	E_SYS	Can't write data to hardware due to hardware issues, such as device is disabled
 */

/**
 * \fn		int32_t (* dev_spi::spi_read) (void *data, uint32_t len)
 * \details	receive \ref data of defined \ref len through spi from slave device .
 * \param[out]	data	pointer to data need to received by spi
 * \param[in]	len	length of data to be received
 * \retval	>0	Byte count that was successfully received for poll method
 * \retval	E_OBJ	Device object is not valid or not exists
 * \retval	E_CTX	Device is still in transfer state
 * \retval	E_PAR	Parameter is not valid
 * \retval	E_SYS	Can't receive data from hardware due to hardware issues, such as device is disabled
 */
/** @} */

typedef void (*hx_drv_spi_s_cb_t) (void );

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief	get an \ref dev_spi slave "spi device" by spi slave device id.
 * 	For how to use spi slave device hal refer to \ref dev_spi slave "Functions in spi slave device structure"
 * \param[in]	spi_id	id of spi, defined by user
 * \retval	!NULL	pointer to an \ref dev_spi slave "spi slave device structure"
 * \retval	NULL	failed to find the spi slave device by \ref spi_id
 * \note	need to implemented by user in user code
 */
extern DEV_SPI_SLV_PTR hx_drv_spi_slv_get_dev(USE_DFSS_SPI_SLV_E spi_id);

extern int hx_drv_spi_slv_init(USE_DFSS_SPI_SLV_E spi_id);

extern int hx_drv_spi_slv_deinit(USE_DFSS_SPI_SLV_E spi_id);

extern int hx_drv_spi_slv_register_tx_cb(hx_drv_spi_s_cb_t aWritecb);

extern int hx_drv_spi_slv_register_rx_cb(hx_drv_spi_s_cb_t aReadcb);

extern int hx_drv_spi_slv_register_rx_evt_cb(hx_drv_spi_s_cb_t aEvtReadcb);

#ifdef __cplusplus
}
#endif
#endif /* INC_HX_DRV_SPI_S_H_ */
