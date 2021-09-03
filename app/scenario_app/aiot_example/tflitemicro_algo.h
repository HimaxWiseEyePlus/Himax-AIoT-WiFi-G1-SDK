/*
 * tflitemicro_algo.h
 *
 *  Created on: 2020�~5��27��
 *      Author: 902447
 */

#ifndef SCENARIO_APP_SAMPLE_CODE_PERIODICAL_WAKEUP_QUICKBOOT_TFLITEMICRO_ALGO_H_
#define SCENARIO_APP_SAMPLE_CODE_PERIODICAL_WAKEUP_QUICKBOOT_TFLITEMICRO_ALGO_H_

#ifdef SCALABLE_METADATA
    #include "algo_metadata.h"
#else
    #include "bodydetect_meta.h"
#endif

#ifdef __cplusplus
extern "C" {
//#include <app_macro_cfg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "embARC.h"
#include "embARC_debug.h"
#include "board_config.h"
#include "hx_drv_pmu.h"
#include "powermode.h"
#endif

int tflitemicro_algo_init();
int tflitemicro_algo_run(uint32_t image_addr, uint32_t image_width, uint32_t image_height, struct_algoResult *algoresult);
void tflitemicro_algo_exit();
#ifdef __cplusplus
}
#endif



#endif /* SCENARIO_APP_SAMPLE_CODE_PERIODICAL_WAKEUP_QUICKBOOT_TFLITEMICRO_ALGO_H_ */
