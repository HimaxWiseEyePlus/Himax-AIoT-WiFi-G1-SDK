/*
 * algo_custom_config.h
 *
 *  Created on: 2019¦~8¤ë15¤é
 *      Author: 902447
 */

#ifndef SCENARIO_APP_ALGO_CUSTOM_CONFIG_H_
#define SCENARIO_APP_ALGO_CUSTOM_CONFIG_H_

#include "board.h"
#include "board_config.h"

#define ALGO_CONFIG_TABLE_VERSION  0x0001
#define MAX_ALGO_PARA_LEN   1024

typedef enum EMBARC_PACKED
{
	WE1AlgoCfg_Type_EMZA_HUMANDETECT,
	WE1AlgoCfg_Type_EMZA_OCCUPANCY_SENSOR,
	WE1AlgoCfg_Type_EMZA_FACE_DETECT,
	WE1AlgoCfg_Type_CNN_FACE_DETECT,
}WE1AlgoCfg_Type_e;

typedef struct
{
	uint16_t	   table_version; 	/**< Table Version */
	uint16_t	   totalLen; 	    /**< Table Total Len */
	uint16_t       table_Len;       /**< sizeof(WE1AppCfg_TableInfo_t)*u32CateCount + sizeof(WE1AppCfg_TableHeader_t) */
	uint8_t		   table_cate_count;/**< category count*/
	uint8_t		   table_crc;/**< Table CRC*/
	uint16_t	   table_checksum; 	/**< checksum calculate from table header not include checksum */
}EMBARC_PACKED WE1AlgoCfg_TableHeader_t;

typedef struct
{
	WE1AlgoCfg_Type_e table_type;
	uint16_t u16Offset;
	uint16_t u16Len;
}EMBARC_PACKED WE1AlgoCfg_TableInfo_t;

typedef struct
{
	uint8_t act_len;
	uint8_t para[MAX_ALGO_PARA_LEN];
}EMBARC_PACKED WE1AlgoCfg_Para_t;


typedef struct
{
	WE1AlgoCfg_TableHeader_t table_header;/**< Table Header */

	WE1AlgoCfg_TableInfo_t algo_table_info;/**< Algorithm Table information */

	WE1AlgoCfg_Para_t algo_table_cfg;
}EMBARC_PACKED WE1AlgoCfg_CustTable_t;

#endif /* SCENARIO_APP_ALGO_CUSTOM_CONFIG_H_ */
