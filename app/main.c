#if (!defined OS_FREERTOS) && (!defined OS_TX)
/**************************************************************************************************
    (C) COPYRIGHT, Himax Technologies, Inc. ALL RIGHTS RESERVED
    ------------------------------------------------------------------------
    File        : main.c
    Project     : WEI
    DATE        : 2018/10/01
    AUTHOR      : 902452
    BRIFE       : main function
    HISTORY     : Initial version - 2018/10/01 created by Will
    			: V1.0			  - 2018/11/13 support CLI
**************************************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "embARC.h"
#include "embARC_debug.h"
#include "board_config.h"
#include "arc_timer.h"
#include "hx_drv_spi_s.h"
#include "spi_slave_protocol.h"
#include "hardware_config.h"

#ifdef TLS_ENABLE
#include "tls.h"
#include "hx_drv_basic_def.h"
#include "hx_hal_sec.h"
#endif
#include "hx_drv_CIS_common.h"
#if defined(CIS_HM0360_MONO_REVB) || defined(CIS_HM0360_MONO_OSC_REVB) || defined(CIS_HM0360_BAYER_REVB) || defined(CIS_HM0360_BAYER_OSC_REVB)
#include "hx_drv_hm0360.h"
#endif
#ifdef CIS_HM11B1
#include "hx_drv_hm11b1.h"
#endif
#ifdef CIS_HM01B0_MONO
#include "hx_drv_hm01b0.h"
#endif
#ifdef CIS_XSHUT_SGPIO0
#define DEAULT_XHSUTDOWN_PIN    CIS_XHSUTDOWN_IOMUX_SGPIO0
#else
#define DEAULT_XHSUTDOWN_PIN    CIS_XHSUTDOWN_IOMUX_SGPIO1
#endif

#ifdef EVENT_HANDLER_TESTAPP
#include "event_handler_testapp.h"

int main(void)
{
	event_handler_testapp();

	return 0;
}
#endif

/******************************************************/
// ifeq ($(APP_TYPE), CLI_PERIPHERAL)
/******************************************************/
#ifdef LIB_CLI
#include "cli.h"
#include "hx_drv_dp.h"
/** main entry */
int main(void)
{
#ifdef TLS_ENABLE
	SEC_ERROR_E status;
	Hx_System_Info system_info;

	EMBARC_PRINTF("himax_sec_init\n");
	system_info.sec_system_mem = 0x20000000;
	system_info.cpu_clk = CLK_CPU;
	status = himax_sec_init(&system_info);
	if(0 != status){
		EMBARC_PRINTF("errors : himax_sec_init\n");
		return status;
	}
	EMBARC_PRINTF("himax_sec_init success\n");

	int32_t rc=0;
	sslKeys_t *keys = NULL;
	ssl_t *ssl = NULL;
	uint8_t *client_receive_app_data = NULL;
	uint32_t receive_data_len = 0;
#ifdef USE_CLIENT_SIDE_SSL
	//================= client side============================
	rc = TLS_key_exchange_client(&ssl,&keys);
	if(0 != rc){
		EMBARC_PRINTF("errors : TLS_key_exchange_client\n");
		goto error;
	}

	rc = TLS_send_data(ssl,client_app_data,strlen((const char *)client_app_data));
	if(0 != rc){
		EMBARC_PRINTF("errors : TLS_send_data\n");
		goto error;
	}

	rc = TLS_receive_data(ssl,&client_receive_app_data,&receive_data_len);
	if(0 != rc){
		EMBARC_PRINTF("errors : TLS_receive_data\n");
		goto error;
	}
	psTraceBytes("server app data",client_receive_app_data,receive_data_len);

#else
	//================= server side============================
	rc = TLS_key_exchange_server(&ssl,&keys);
	if(0 != rc){
		EMBARC_PRINTF("errors : TLS_key_exchange_server\n");
		goto error;
	}

	rc = TLS_receive_data(ssl,&client_receive_app_data,&receive_data_len);
	if(0 != rc){
		EMBARC_PRINTF("errors : TLS_receive_data\n");
		goto error;
	}
	psTraceBytes("client app data",client_receive_app_data,receive_data_len);

	rc = TLS_send_data(ssl,server_app_data,strlen((const char *)server_app_data));
	if(0 != rc){
		EMBARC_PRINTF("errors : TLS_send_data\n");
		goto error;
	}

#endif

error:
	cleanup(ssl, keys);
#endif

#ifdef CLI_ENABLE
#ifdef LIB_CLI
	EMBARC_PRINTF("[CLI Program V1.0]\n");

	/*disable INTC side effect period is accurate*/
//	int_disable(BOARD_SYS_TIMER_INTNO); /* disable timer interrupt (method 1 to stop capture frame)*/
//	int_disable(BOARD_STD_TIMER_ID); /* disable timer interrupt (method 1 to stop capture frame)*/
#if defined(CIS_HM0360_MONO_OSC_REVB) || defined(CIS_HM0360_BAYER_OSC_REVB)
	hx_drv_dp_set_mclk_src(DP_MCLK_SRC_INTERNAL, DP_MCLK_SRC_INT_SEL_RC36M);
#else
	hx_drv_dp_set_mclk_src(DP_MCLK_SRC_INTERNAL, DP_MCLK_SRC_INT_SEL_XTAL);
#endif
#if defined(CIS_HM01B0_MONO) || defined(CIS_HM01B0_BAYER) || defined(CIS_COMMON) || defined(CIS_HM1245) || defined(CIS_HM2140) || defined(CIS_HM0360_MONO) || defined(CIS_HM0360_MONO_OSC) || defined(CIS_HM0360_BAYER) || defined(CIS_HM0360_BAYER_OSC) || defined(CIS_HM0360_MONO_REVB) || defined(CIS_HM0360_MONO_OSC_REVB) || defined(CIS_HM0360_BAYER_REVB) || defined(CIS_HM0360_BAYER_OSC_REVB)
#ifdef IC_PACKAGE_WLCSP38
	hx_drv_sensorctrl_set_wlcsp_sharepin(SENSORCTRL_WLCSP_SHAREPIN_HM01B0);
#endif
	DP_MCLK_SRC_INT_EXT_E clk_int_ext;
	DP_MCLK_SRC_INT_SEL_E clk_int_src;
	hx_drv_dp_get_mclk_src(&clk_int_ext, &clk_int_src);
	EMBARC_PRINTF("mclk_int_ext=%d,mclk_int_src=%d\n",clk_int_ext, clk_int_src);
    if(clk_int_src == DP_MCLK_SRC_INT_SEL_XTAL)
    {
    	EMBARC_PRINTF("mclk DIV2, xshutdown_pin=%d\n",DEAULT_XHSUTDOWN_PIN);
#if defined(CIS_HM01B0_MONO) || defined(CIS_HM01B0_BAYER)
#if (HM01B0_MCLK_MHZ == 24)
    	hx_drv_cis_init(CIS_XHSUTDOWN_IOMUX_NONE, SENSORCTRL_MCLK_DIV1);
#elif (HM01B0_MCLK_MHZ == 18)
    	hx_drv_dp_set_mclk_src(DP_MCLK_SRC_INTERNAL, DP_MCLK_SRC_INT_SEL_RC36M);
    	hx_drv_cis_init(CIS_XHSUTDOWN_IOMUX_NONE, SENSORCTRL_MCLK_DIV2);
#elif (HM01B0_MCLK_MHZ == 12)
    	hx_drv_cis_init(CIS_XHSUTDOWN_IOMUX_NONE, SENSORCTRL_MCLK_DIV2);
#endif
#elif defined(HM0360_MONO_OSC) || defined(HM0360_BAYER_OSC) || defined(CIS_HM0360_MONO_OSC_REVB) || defined(CIS_HM0360_BAYER_OSC_REVB)
    	hx_drv_cis_init(DEAULT_XHSUTDOWN_PIN, SENSORCTRL_MCLK_NO);
#else
#if defined(CIS_HM0360_MONO_REVB) || defined(CIS_HM0360_BAYER_REVB)
#if(HM0360_MCLK_MHZ == 12)
    	hx_drv_cis_init(DEAULT_XHSUTDOWN_PIN, SENSORCTRL_MCLK_DIV2);
#elif(HM0360_MCLK_MHZ == 24)
    	hx_drv_cis_init(DEAULT_XHSUTDOWN_PIN, SENSORCTRL_MCLK_DIV1);
#endif
#else
    	hx_drv_cis_init(DEAULT_XHSUTDOWN_PIN, SENSORCTRL_MCLK_DIV2);
#endif
#endif
    }else{
    	EMBARC_PRINTF("mclk DIV3, xshutdown_pin=%d\n",DEAULT_XHSUTDOWN_PIN);
#if defined(CIS_HM01B0_MONO) || defined(CIS_HM01B0_BAYER)
    	hx_drv_cis_init(CIS_XHSUTDOWN_IOMUX_NONE, SENSORCTRL_MCLK_DIV3);
#else
    	hx_drv_cis_init(DEAULT_XHSUTDOWN_PIN, SENSORCTRL_MCLK_DIV3);
#endif
    }
#endif
#ifdef CIS_HM0360_FPGA
	hx_drv_hm0360fpga_init(DEAULT_XHSUTDOWN_PIN);
#endif
#ifdef CIS_HM11B1
#ifdef IC_PACKAGE_WLCSP38
	hx_drv_sensorctrl_set_wlcsp_sharepin(SENSORCTRL_WLCSP_SHAREPIN_HM11B1);
#endif
	hx_drv_hm11b1_init(DEAULT_XHSUTDOWN_PIN);
#endif

    CLI_InitMenu();
#ifdef CLI_ENABLE_PASSWORD
    CLI_EnablePassword(FALSE);
    CLI_EnablePassword(TRUE);
#endif
	CLI_CommandLoop();
#endif
#endif
	while(1)
	{
		board_delay_ms(1);
	}
	return E_SYS;
}
#endif


#ifdef DEMO_SCENARIO_PIR
#include "demo_scenario_pir.h"

/** main entry */
int main(void)
{
	app_demo_scenario_pir();
}
#endif

#ifdef DEMO_SCENARIO_LIVE_JPEG
#include "demo_live_jpeg.h"

/** main entry */
int main(void)
{
	app_demo_live_jpeg();
}
#endif

#ifdef DEMO_SCENARIO_CNN
#include "demo_scenario_cnn.h"

/** main entry */
int main(void)
{
	DP_MCLK_SRC_INT_EXT_E clk_int_ext;
	DP_MCLK_SRC_INT_SEL_E clk_int_src;
	hx_drv_dp_set_mclk_src(DP_MCLK_SRC_INTERNAL, DP_MCLK_SRC_INT_SEL_XTAL);
	hx_drv_dp_get_mclk_src(&clk_int_ext, &clk_int_src);
	EMBARC_PRINTF("mclk_int_ext=%d,mclk_int_src=%d\n",clk_int_ext, clk_int_src);
    if(clk_int_src == DP_MCLK_SRC_INT_SEL_XTAL)
    {
    	EMBARC_PRINTF("mclk DIV2, xshutdown_pin=%d\n",DEAULT_XHSUTDOWN_PIN);
    	hx_drv_cis_init(DEAULT_XHSUTDOWN_PIN, SENSORCTRL_MCLK_DIV2);
    }else{
    	EMBARC_PRINTF("mclk DIV3, xshutdown_pin=%d\n",DEAULT_XHSUTDOWN_PIN);
    	hx_drv_cis_init(DEAULT_XHSUTDOWN_PIN, SENSORCTRL_MCLK_DIV3);
    }

	app_demo_scenario_cnn();
}
#endif

#ifdef AIOT_CNNFACEDETECT_ALLON
#include "app_aiot_cnnfacedetect_allon.h"

/** main entry */
int main(void)
{
	app_aiot_cnnfacedetect_allon();
}
#endif

#ifdef DEMO_SCENARIO_HUMAN
#include "event_handler.h"
#include "demo_scenario_human.h"

int main(void)
{
	app_demo_scenario_human();

	return 0;
}
#endif

#ifdef AIOT_FACEDETECT_ALLON
#include <aiot_facedetect_allon/app_aiot_facedetect_allon.h>

int main(void)
{
	app_aiot_facedetect();

	return 0;
}
#endif

#ifdef AIOT_FACEDETECT_CDM
#include <aiot_facedetect_cdm/app_aiot_facedetect_cdm.h>

int main(void)
{
	app_iot_facedetect_cdm();

	return 0;
}
#endif

#ifdef SAMPLECODE_CDM_WAKEUP
#include <app_aiot_humandetect_cdm.h>

int main(void)
{
	app_iot_humandetect_cdm();

	return 0;
}
#endif

#ifdef AIOT_HUMANDETECT_PIR
#include <aiot_humandetect_pir/app_aiot_humandetect_pir.h>

int main(void)
{
	app_iot_humandetect_pir();

	return 0;
}
#endif

#ifdef AIOT_HUMANDETECT_SMARTDOORLOCK
#include <aiot_humandetect_smartdoorlock/app_aiot_humandetect_smartdoorlock.h>

int main(void)
{
	printf("\033[31;43m \n11 pix Smart Door Lock algo version %d\n \033[0m", pix_aos_2d_facekit_version());
	app_iot_humandetect_pir();

	return 0;
}
#endif

#ifdef AIOT_ALGODETECT_ANALOGWAKEUP
#include <aiot_algodetect_analogwakeup/app_aiot_algodetect_analogwakeup.h>

int main(void)
{
	app_iot_algodetect_analogwakeup();

	return 0;
}
#endif

#ifdef AIOT_OCCUPANCY
#include <aiot_occupancy/app_aiot_occupancy.h>

int main(void)
{
	app_aiot_occupancy();

	return 0;
}
#endif

#ifdef QNB_FACEDETECT
#include "event_handler.h"
#include "qnb_facedetect.h"

int main(void)
{
	app_qnb_facedetect();

	return 0;
}
#endif

#ifdef AIOT_ALGODETECT_SENSORMDWAKEUP
#include <app_aiot_algodetect_sensormd_wakeup.h>

int main(void)
{
	app_iot_algodetect_sensormd_wakeup();

	return 0;
}
#endif

#ifdef PMU_LIB_TEST
#include "pmu_lib_test.h"
int main(void)
{
#ifdef PMU_LIB_TEST_CDM
	test_humandet_cdm();
#endif
#ifdef PMU_LIB_TEST_DPIR
	test_humandet_dpir();
#endif
#ifdef PMU_LIB_TEST_APIR
	test_humandet_apir();
#endif
#ifdef PMU_LIB_TEST_OCS
	test_humandet_ocs();
#endif
#ifdef PMU_LIB_TEST_CPURESET
	test_cpu_rst();
#endif
#ifdef PMU_LIB_TEST_INT1_FORCV
	setup_int1_path_forcv(2000, 1);
#endif
#ifdef PMU_LIB_TEST_INT1_FORCDM
	setup_int1_path_forcdm(2000, 1, 0);
#endif
#ifdef PMU_LIB_TEST_HW5X5JPEG_FORCV
	setup_hw5x5jpeg_forcv(2000, 1);
#endif
#ifdef PMU_LIB_TEST_DPIR_ONLY
	test_dpir_only();
#endif
#ifdef PMU_LIB_TEST_BYPASS_FORCV
	setup_bypass_forcv(1000, 1);
#endif
#ifdef PMU_LIB_TEST_APIR_ONLY
	test_apir_only();
#endif
#ifdef PMU_LIB_TEST_CHIPRESET
	test_chipreset_rst();
#endif
#ifdef PMU_LIB_TEST_CDM_CHIPRESET_MIX
	test_humandet_cdm_chipreset();
#endif
#ifdef PMU_LIB_TEST_DPIR_CHIPRESET_MIX
	test_humandet_dpir_chipreset();
#endif
#ifdef PMU_LIB_TEST_APIR_CHIPRESET_MIX
	test_humandet_apir_chipreset();
#endif
#ifdef PMU_LIB_TEST_CDM_DPIR_APIR_CHIPRESET_MIX
	test_humandet_all_chipreset();
#endif
#ifdef PMU_LIB_TEST_CDM_CLASSRTC
	test_humandet_cdm_rtcclass_chipreset();
#endif
#ifdef PMU_LIB_TEST_DPIR_CLASSRTC
	test_humandet_dpir_rtcclass_chipreset();
#endif
#ifdef PMU_LIB_TEST_APIR_CLASSRTC
	test_humandet_apir_rtcclass_chipreset();
#endif
#ifdef PMU_LIB_TEST_CDM_APIR_CLASSRTC
	test_humandet_cdm_apir_rtcclass_chipreset();
#endif
#ifdef PMU_LIB_TEST_CDM_CLASSRTC_CPURESET
	test_humandet_cdm_rtcclass_chipreset();
#endif
#ifdef PMU_LIB_TEST_DPIR_CLASSRTC_CPURESET
	test_humandet_dpir_rtcclass_chipreset();
#endif
#ifdef PMU_LIB_TEST_APIR_CLASSRTC_CPURESET
	test_humandet_apir_rtcclass_chipreset();
#endif
#ifdef PMU_LIB_TEST_CDM_CPURESET_MIX
	test_humandet_cdm_chipreset();
#endif
#ifdef PMU_LIB_TEST_DPIR_CPURESET_MIX
	test_humandet_dpir_chipreset();
#endif
#ifdef PMU_LIB_TEST_APIR_CPURESET_MIX
	test_humandet_apir_chipreset();
#endif
#ifdef PMU_LIB_TEST_CDM_DPIR_APIR_CPURESET_MIX
	test_humandet_all_chipreset();
#endif
#ifdef PMU_LIB_TEST_CDM_APIR_CLASSRTC_CPURESET
	test_humandet_cdm_apir_rtcclass_chipreset();
#endif
#ifdef WLCSP_HM01B0_TEST
	wlcsp38_hm01b0_test();
#endif
#ifdef PMU_LIB_TEST_CDM_NOBOOTFLOW
	test_humandet_cdm_nobootflow();
#endif
#ifdef PMU_LIB_TEST_SCENARIO12_BOOTFLOW
	test_scenario12_bootflow();
#endif
#ifdef PMU_LIB_TEST_SCENARIO12_NOBOOTFLOW
	test_scenario12_nobootflow();
#endif
#ifdef PMU_LIB_TEST_APIR_NOBOOTFLOW
	test_humandet_apir_nobootflow();
#endif
#ifdef PMU_LIB_TEST_DPIR_BOOTCAP
	test_humandet_dpir_bootcap();
#endif
#ifdef PMU_LIB_TEST_OCS_BOOTCAP
	test_humandet_ocs_bootcap();
#endif
#ifdef PMU_LIB_TEST_SCENARIO12_NOBOOTFLOW_SENDSPIALL
	test_scenario12_nobootflow_sendspiall();
#endif
#ifdef PMU_LIB_TEST_CDM_NONAOS
	test_humandet_cdm_nonaos();
#endif
#ifdef PMU_LIB_TEST_CDM_HM0360_XSLEEP_ONLY
	test_humandet_cdm_xsleeponly();
#endif
#ifdef PMU_LIB_TEST_CDM_HM0360_XSLEEP_USE_TRIGGER
	test_humandet_cdm_xsleep_use_we1_trigger();
#endif
#ifdef PMU_LIB_TEST_BOOTROM_CASE1
//	1.	SLP1 WDT reboot + without Skip boot flow + Bit 13: pclk/hclk source pre_selection (0:rc_36m)+ Boot Speed Disable PLL
	sim_bootrom_case1();
#endif
#ifdef PMU_LIB_TEST_BOOTROM_CASE2
//  2.	SLP1 WDT reboot + without Skip boot flow + Bit 13: pclk/hclk source pre_selection (0:rc_36m)+ Boot Speed 400M/50M(AHB1/SSI)
	sim_bootrom_case2();
#endif
#ifdef PMU_LIB_TEST_BOOTROM_CASE3
//	3.	SLP1 WDT reboot + without Skip boot flow + Bit 13: pclk/hclk source pre_selection (0:rc_36m)+ Boot Speed 400M/25M(AHB1/SSI)
	sim_bootrom_case3();
#endif
#ifdef PMU_LIB_TEST_BOOTROM_CASE4
//	4.	SLP1 WDT reboot + without Skip boot flow + Bit 13: pclk/hclk source pre_selection (0:rc_36m)+ Boot Speed 200M/25M(AHB1/SSI)

	sim_bootrom_case4();

#endif
#ifdef PMU_LIB_TEST_BOOTROM_CASE5
	//5.	SLP1 WDT reboot + without Skip boot flow + Bit 13: pclk/hclk source pre_selection (1:xtal_24m_post) + Boot Speed Disable PLL
	sim_bootrom_case5();
#endif
#ifdef PMU_LIB_TEST_BOOTROM_CASE6
	//6.	SLP1 WDT reboot + without Skip boot flow + Bit 13: pclk/hclk source pre_selection (1:xtal_24m_post) + Boot Speed 400M/50M(AHB1/SSI)
	sim_bootrom_case6();
#endif
#ifdef PMU_LIB_TEST_BOOTROM_CASE7
	//7.	SLP1 WDT reboot + without Skip boot flow + Bit 13: pclk/hclk source pre_selection (1:xtal_24m_post) + Boot Speed 400M/25M(AHB1/SSI)
	sim_bootrom_case7();
#endif
#ifdef PMU_LIB_TEST_BOOTROM_CASE8
	//8.	SLP1 WDT reboot + without Skip boot flow + Bit 13: pclk/hclk source pre_selection (1:xtal_24m_post) + Boot Speed 200M/25M(AHB1/SSI)
	sim_bootrom_case8();
#endif
#ifdef PMU_LIB_TEST_BOOTROM_CASE9
	//9.	SLP1 WDT reboot + with Skip boot flow + Bit 13: pclk/hclk source pre_selection (1:xtal_24m_post)+ Boot Speed 400M/50M(AHB1/SSI)
	sim_bootrom_case9();
#endif
#ifdef PMU_LIB_TEST_PERIODICAL_QUICK_WAKEUP
	periodical_quick_wakeup();
#endif
	return 0;
}
#endif

#ifdef POWER_MEASURE_APP_HUMAN
#include "event_handler.h"
#include <power_measure_app_human/demo_scenario_human.h>

int main(void)
{
	app_demo_scenario_human();

	return 0;
}
#endif

#ifdef POWER_MEASURE_APP_OCS
#include <power_measure_app_ocs/app_aiot_occupancy.h>

int main(void)
{
	app_aiot_occupancy();

	return 0;
}
#endif

//
#ifdef SAMPLECODE_GPIOWAKEUP
#include <app_aiot_algodetect_gpiowakeup.h>

int main(void)
{
	app_iot_algodetect_gpiowakeup();

	return 0;
}
#endif

#ifdef AIOT_CHROME
#include <app_aiot_chrome.h>

int main(void)
{
	app_iot_chrome();

	return 0;
}
#endif

#ifdef SLP2_SLT
#include <slp2_slt/slp2_slt.h>

int main(void)
{
	app_slp2_slt();

	return 0;
}
#endif

#ifdef SLP2_SLT_NOSENSOR
#include <slp2_slt_nosensor/slp2_slt_nosensor.h>

int main(void)
{
	app_slp2_slt_nosensor();

	return 0;
}
#endif

#ifdef SD_SLT
#include <sd_slt/sd_slt.h>

int main(void)
{
	app_sd_slt();

	return 0;
}
#endif

#ifdef SD_SLT_NOSENSOR
#include <sd_slt_nosensor/sd_slt_nosensor.h>

int main(void)
{
	app_sd_slt_nosensor();

	return 0;
}
#endif


#ifdef HWACCBYTPG_AUTO_TEST
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>

#include "embARC_error.h"
#include "io_config_ext.h"
#include "hx_drv_iomux.h"
#include "sensor_dp_lib.h"
#include "tpgdp_lib.h"
#ifdef HWACCBYTPG_AUTO_HOG
#include "hog_test.h"
#endif
#ifdef HWACCBYTPG_AUTO_HOGDMA_NOBND
#include "hogdma_test.h"
#endif
#ifdef HWACCBYTPG_AUTO_HOGDMA_BND
#include "hogdma_test.h"
#endif
#ifdef HWACCBYTPG_AUTO_RS
#include "rs_test.h"
#endif
#ifdef HWACCBYTPG_AUTO_JPEG
#include "jpeg_test.h"
#endif
#ifdef HWACCBYTPG_AUTO_HW5X5
#include "hw5x5_test.h"
#endif
#ifdef HWACCBYTPG_AUTO_HW2X2
#include "hw2x2_test.h"
#endif
#ifdef HWACCBYTPG_AUTO_HW5X5_JPEG
#include "hw5x5_jpeg_test.h"
#endif
#ifdef HWACCBYTPG_AUTO_CDM
#include "cdm_test.h"
#endif
#ifdef HWACCBYTPG_AUTO_HW2X2_CDM
#include "hw2x2_cdm_test.h"
#endif
#ifdef BYPASS_TEST
#include "bypass_test.h"
#endif
#ifdef EDM_CRC_TEST
#include "edm_crc_test.h"
#endif

#define DP_FROM_XTAL
int main(void)
{
	DP_DP_CLK_SRC_E dp_clk;
	EMBARC_PRINTF("AUTOTEST Start\n");
#ifdef DP_FROM_XTAL
	hx_drv_dp_set_dp_clk_src(DP_CLK_SRC_XTAL_24M_POST);
	hx_drv_dp_get_dp_clk_src(&dp_clk);
	EMBARC_PRINTF("dp_clk=%d\n",dp_clk);
#endif

#ifdef HWACCBYTPG_AUTO_HOG
		HOG_AUTOTEST_ALL();
#endif
#ifdef HWACCBYTPG_AUTO_HOGDMA_NOBND
		HOGDMA_NOBND_AUTOTEST_ALL();
#endif
#ifdef HWACCBYTPG_AUTO_HOGDMA_BND
		HOGDMA_BND_AUTOTEST_ALL();
#endif
#ifdef HWACCBYTPG_AUTO_RS
		RS_AUTOTEST_ALL();
#endif
#ifdef HWACCBYTPG_AUTO_JPEG
		JPEG_AUTOTEST_ALL();
#endif
#ifdef HWACCBYTPG_AUTO_HW5X5
		HW5X5_AUTOTEST_ALL();
#endif
#ifdef HWACCBYTPG_AUTO_HW2X2
		HW2X2_AUTOTEST_ALL();
#endif
#ifdef HWACCBYTPG_AUTO_HW5X5_JPEG
		HW5X5_JPEG_AUTOTEST_ALL();
#endif
#ifdef HWACCBYTPG_AUTO_CDM
		CDM_AUTOTEST_ALL();
#endif
#ifdef HWACCBYTPG_AUTO_HW2X2_CDM
		HW2X2_CDM_AUTOTEST_ALL();
#endif
#ifdef BYPASS_TEST
		BYPASS_AUTOTEST_ALL();
#endif
#ifdef EDM_CRC_TEST
		EDM_CRC_AUTOTEST_ALL();
#endif
		EMBARC_PRINTF("AUTOTEST End\n");
}

#endif//HWACCBYTPG_AUTO_TEST

#ifdef AIOT_CONFERENCE_CALL

#ifdef AUDIO
#include <audio_recognition/app_audio_recognition.h>
#endif
#include <aiot_conference_call/app_aiot_humandetect.h>

int main(void)
{
    app_iot_humandetect();

    return 0;
}
#endif //AIOT_CONFERENCE_CALL


#ifdef MP3_LIBMAD
int main(void)
{
	printf("mp3_libmad main \n");
	mp3_libmad();
	return 0;
}
#endif


#ifdef AIOT_HUMANDETECT_TV

#ifdef AUDIO
#include <audio_recognition/app_audio_recognition.h>
#endif
#include <aiot_humandetect_tv/app_aiot_humandetect.h>

int main(void)
{
    app_iot_humandetect();

    return 0;
}
#endif //AIOT_HUMANDETECT_TV

#ifdef AIOT_HUMANDETECT_TV_CDM

#ifdef AUDIO
#include <audio_recognition/app_audio_recognition.h>
#endif
#include <aiot_humandetect_tv_cdm/app_aiot_humandetect_tv_cdm.h>

int main(void)
{
    app_iot_humandetect_tv_cdm();

    return 0;
}
#endif //AIOT_HUMANDETECT_TV_CDM

#ifdef AIOT_HUMANDETECT_AIRCONDITION

#ifdef AUDIO
#include <audio_recognition/app_audio_recognition.h>
#endif
#include <aiot_humandetect_aircondition/app_aiot_humandetect.h>

int main(void)
{
    app_iot_humandetect();

    return 0;
}
#endif //AIOT_HUMANDETECT_AIRCONDITION

#ifdef AIOT_PEOPLE_COUNTING_VIP

#ifdef AUDIO
#include <audio_recognition/app_audio_recognition.h>
#endif
#include <aiot_people_counting_vip/app_aiot_humandetect.h>

int main(void)
{
    app_iot_humandetect();

    return 0;
}
#endif //AIOT_PEOPLE_COUNTING_VIP

#ifdef AIOT_AMR_VIP

#ifdef AUDIO
#include <audio_recognition/app_audio_recognition.h>
#endif
#include <aiot_amr_vip/app_aiot_humandetect.h>

int main(void)
{
    app_iot_humandetect();

    return 0;
}
#endif //AIOT_AMR_VIP

#ifdef AIOT_CROPNET_VIP

#ifdef AUDIO
#include <audio_recognition/app_audio_recognition.h>
#endif
#include <aiot_cropnet_vip/app_aiot_humandetect.h>

int main(void)
{
    app_iot_humandetect();

    return 0;
}
#endif //AIOT_CROPNET_VIP

#ifdef EMZA_HOG_TEST

#include "test_capture_frame_do_hog.h"
int main(void)
{
	setup_capture_frame();

	return 0;
}
#endif

#ifdef STANDALONE_TEST

#include "hello_main.h"
/** main entry */
int main(void)
{
    app_exe_standalone_sample();
}
#endif

#ifdef AUDIO_RECOGNITION
#include <audio_recognition/app_audio_recognition.h>

/** main entry */
int main(void)
{
#ifdef ALANGO_STANDALONE
    dbg_printf(DBG_LESS_INFO, "alango_standalone_init\n");
    alango_standalone_init();
#endif
    app_audio_recognition_init();
}
#endif

#ifdef POWER_MEASUREMENT_DHRYSTONE
#include "power_measure_dhrystone.h"
/** main entry */
int main(void)
{
    dbg_printf(DBG_LESS_INFO, "power_measurement_dhrystone\n");
    app_power_measurement_dhrystone();
}
#endif

#ifdef SEND_FRAME_TEST
#include "test_capture_frame_send.h"
int main(void)
{
	setup_capture_frame();

	return 0;
}
#endif

#ifdef TFLM_SDK
#include "test_lib_tflm.h"
int main(void)
{

	test_sdk();

	return 0;
}
#endif

#ifdef AIOT_BODYDETECT_ALLON
#include <aiot_bodydetect_allon/app_aiot_bodydetect_allon.h>

int main(void)
{
	app_aiot_bodydetect();

	return 0;
}
#endif

#ifdef AIOT_BODYDETECT_ALLON_GOOGLE
#include <aiot_bodydetect_allon_tflite/app_aiot_bodydetect_allon.h>

int main(void)
{
	app_aiot_bodydetect_google();
	//app_main();

	return 0;
}
#endif

#ifdef APP_MAIN
#include <app_main/app_main.h>

int main(void)
{
	app_main();

	return 0;
}
#endif

#ifdef AIOT_BODYDETECT_ALLON_GOOGLE_CODE_GEN
#include <aiot_bodydetect_allon_tflite_code_gen/app_aiot_bodydetect_allon.h>

int main(void)
{
	app_aiot_bodydetect_google_code_gen();

	return 0;
}
#endif

#ifdef AIOT_MOBILENETV2SSD_MNIST
#include <app_aiot_mobilenetv2ssd_mnist.h>

int main(void)
{
	app_aiot_mobilenetv2ssd_mnsit();

	return 0;
}
#endif

#ifdef AIOT_BODYDETECT_ALLON_HIMAX
#include <aiot_bodydetect_allon_tflite_HIMAX/app_aiot_bodydetect_allon.h>

int main(void)
{
	app_aiot_bodydetect_himax();

	return 0;
}
#endif

#ifdef AIOT_BODYDETECT_ALLON_HIMAX_CODE_GEN
#include <aiot_bodydetect_allon_tflite_HIMAX_code_gen/app_aiot_bodydetect_allon.h>

int main(void)
{
	app_aiot_bodydetect_himax_code_gen();

	return 0;
}
#endif

#ifdef AIOT_SURVEILLANCE_HIMAX
#include <app_aiot_surveillance.h>

int main(void)
{
	app_iot_prerolling_gpiowakeup();

	return 0;
}
#endif

#ifdef SAMPLECODE_PREROLLING_GPIOWAKEUP
#include <app_aiot_prerolling_gpiowakeup.h>

int main(void)
{
	app_iot_prerolling_gpiowakeup();

	return 0;
}
#endif

#ifdef SAMPLECODE_PERIODICAL_WAKEUP_QUICKBOOT
#include <app_periodical_wakeup_quickboot.h>

int main(void)
{
	app_periodical_wakeup_quickboot();

	return 0;
}
#endif

#ifdef AIOT_ALGO_TEMPLATE_HIMAX
#include <sample_code_algo/app_aiot_algo_template.h>

int main(void)
{
    app_aiot_algo_template_himax();

    return 0;
}
#endif

#ifdef CAPSULE
#include <app_capsule.h>

int main(void)
{
	app_capsule();

	return 0;
}
#endif

#ifdef PEGA
int main(void)
{
	pega();

    return 0;
}
#endif 

/*if (!defined OS_FREERTOS) && (!defined OS_TX)*/
/** @} */
#elif (defined OS_FREERTOS)
// for freeRtos test
/* ------------------------------------------
 * Copyright (c) 2017, Synopsys, Inc. All rights reserved.

 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:

 * 1) Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.

 * 2) Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation and/or
 * other materials provided with the distribution.

 * 3) Neither the name of the Synopsys, Inc., nor the names of its contributors may
 * be used to endorse or promote products derived from this software without
 * specific prior written permission.

 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
--------------------------------------------- */

/**
 * \defgroup	EMBARC_APP_FREERTOS_KERNEL	embARC FreeRTOS Kernel Example
 * \ingroup	EMBARC_APPS_TOTAL
 * \ingroup	EMBARC_APPS_OS_FREERTOS
 * \brief	embARC Example for testing FreeRTOS task switch and interrupt/exception handling
 *
 * \details
 * ### Extra Required Tools
 *
 * ### Extra Required Peripherals
 *
 * ### Design Concept
 *     This example is designed to show the functionality of FreeRTOS.
 *
 * ### Usage Manual
 *     Test case for show how FreeRTOS is working by task switching and interrupt/exception processing.
 *     ![ScreenShot of freertos-demo under freertos](pic/images/example/emsk/emsk_freertos_demo.jpg)
 *
 * ### Extra Comments
 *
 */

/**
 * \file
 * \ingroup	EMBARC_APP_FREERTOS_KERNEL
 * \brief	main source file of the freertos demo
 */

/**
 * \addtogroup	EMBARC_APP_FREERTOS_KERNEL
 * @{
 */

#include "embARC.h"
#include "embARC_debug.h"
#include <stdlib.h>


/******************************************************/
// ifeq ($(APP_TYPE), CLI_PERIPHERAL)
/******************************************************/
#ifdef LIB_CLI
#include <stdio.h>
#include <string.h>
#include "board_config.h"
#include "arc_timer.h"
#include "hx_drv_spi_s.h"
#include "spi_slave_protocol.h"
#include "hardware_config.h"

#ifdef TLS_ENABLE
#include "tls.h"
#include "hx_drv_basic_def.h"
#include "hx_hal_sec.h"
#endif
#include "hx_drv_CIS_common.h"
#ifdef CIS_HM0360_FPGA
#include "hx_drv_hm0360_fpga.h"
#endif
#ifdef CIS_HM11B1
#include "hx_drv_hm11b1.h"
#endif
#ifdef CIS_XSHUT_SGPIO0
#define DEAULT_XHSUTDOWN_PIN    CIS_XHSUTDOWN_IOMUX_SGPIO0
#else
#define DEAULT_XHSUTDOWN_PIN    CIS_XHSUTDOWN_IOMUX_SGPIO1
#endif

#include "cli.h"
#include "hx_drv_dp.h"
/** main entry */
int main(void)
{
#ifdef TLS_ENABLE
	SEC_ERROR_E status;
	Hx_System_Info system_info;

	EMBARC_PRINTF("himax_sec_init\n");
	system_info.sec_system_mem = 0x20000000;
	system_info.cpu_clk = CLK_CPU;
	status = himax_sec_init(&system_info);
	if(0 != status){
		EMBARC_PRINTF("errors : himax_sec_init\n");
		return status;
	}
	EMBARC_PRINTF("himax_sec_init success\n");

	int32_t rc=0;
	sslKeys_t *keys = NULL;
	ssl_t *ssl = NULL;
	uint8_t *client_receive_app_data = NULL;
	uint32_t receive_data_len = 0;
#ifdef USE_CLIENT_SIDE_SSL
	//================= client side============================
	rc = TLS_key_exchange_client(&ssl,&keys);
	if(0 != rc){
		EMBARC_PRINTF("errors : TLS_key_exchange_client\n");
		goto error;
	}

	rc = TLS_send_data(ssl,client_app_data,strlen((const char *)client_app_data));
	if(0 != rc){
		EMBARC_PRINTF("errors : TLS_send_data\n");
		goto error;
	}

	rc = TLS_receive_data(ssl,&client_receive_app_data,&receive_data_len);
	if(0 != rc){
		EMBARC_PRINTF("errors : TLS_receive_data\n");
		goto error;
	}
	psTraceBytes("server app data",client_receive_app_data,receive_data_len);

#else
	//================= server side============================
	rc = TLS_key_exchange_server(&ssl,&keys);
	if(0 != rc){
		EMBARC_PRINTF("errors : TLS_key_exchange_server\n");
		goto error;
	}

	rc = TLS_receive_data(ssl,&client_receive_app_data,&receive_data_len);
	if(0 != rc){
		EMBARC_PRINTF("errors : TLS_receive_data\n");
		goto error;
	}
	psTraceBytes("client app data",client_receive_app_data,receive_data_len);

	rc = TLS_send_data(ssl,server_app_data,strlen((const char *)server_app_data));
	if(0 != rc){
		EMBARC_PRINTF("errors : TLS_send_data\n");
		goto error;
	}

#endif

error:
	cleanup(ssl, keys);
#endif

#ifdef CLI_ENABLE
#ifdef LIB_CLI
	EMBARC_PRINTF("[CLI Program V1.0]\n");

	/*disable INTC side effect period is accurate*/
//	int_disable(BOARD_SYS_TIMER_INTNO); /* disable timer interrupt (method 1 to stop capture frame)*/
//	int_disable(BOARD_STD_TIMER_ID); /* disable timer interrupt (method 1 to stop capture frame)*/
	hx_drv_dp_set_mclk_src(DP_MCLK_SRC_INTERNAL, DP_MCLK_SRC_INT_SEL_XTAL);
#if defined(CIS_HM01B0_MONO) || defined(CIS_HM01B0_BAYER) || defined(CIS_COMMON) || defined(CIS_HM1245) || defined(CIS_HM2140) || defined(CIS_HM0360_MONO) || defined(CIS_HM0360_MONO_OSC) || defined(CIS_HM0360_BAYER) || defined(CIS_HM0360_BAYER_OSC)
#ifdef IC_PACKAGE_WLCSP38
	hx_drv_sensorctrl_set_wlcsp_sharepin(SENSORCTRL_WLCSP_SHAREPIN_HM01B0);
#endif
	DP_MCLK_SRC_INT_EXT_E clk_int_ext;
	DP_MCLK_SRC_INT_SEL_E clk_int_src;
	hx_drv_dp_get_mclk_src(&clk_int_ext, &clk_int_src);
	EMBARC_PRINTF("mclk_int_ext=%d,mclk_int_src=%d\n",clk_int_ext, clk_int_src);
    if(clk_int_src == DP_MCLK_SRC_INT_SEL_XTAL)
    {
    	EMBARC_PRINTF("mclk DIV2, xshutdown_pin=%d\n",DEAULT_XHSUTDOWN_PIN);
#if defined(CIS_HM01B0_MONO) || defined(CIS_HM01B0_BAYER)
    	hx_drv_cis_init(CIS_XHSUTDOWN_IOMUX_NONE, SENSORCTRL_MCLK_DIV2);
#elif defined(HM0360_MONO_OSC) || defined(HM0360_BAYER_OSC)
    	hx_drv_cis_init(DEAULT_XHSUTDOWN_PIN, SENSORCTRL_MCLK_NO);
#else
    	hx_drv_cis_init(DEAULT_XHSUTDOWN_PIN, SENSORCTRL_MCLK_DIV2);
#endif
    }else{
    	EMBARC_PRINTF("mclk DIV3, xshutdown_pin=%d\n",DEAULT_XHSUTDOWN_PIN);
#if defined(CIS_HM01B0_MONO) || defined(CIS_HM01B0_BAYER)
    	hx_drv_cis_init(CIS_XHSUTDOWN_IOMUX_NONE, SENSORCTRL_MCLK_DIV3);
#else
    	hx_drv_cis_init(DEAULT_XHSUTDOWN_PIN, SENSORCTRL_MCLK_DIV3);
#endif
    }
#endif
#ifdef CIS_HM0360_FPGA
	hx_drv_hm0360fpga_init(DEAULT_XHSUTDOWN_PIN);
#endif
#ifdef CIS_HM11B1
#ifdef IC_PACKAGE_WLCSP38
	hx_drv_sensorctrl_set_wlcsp_sharepin(SENSORCTRL_WLCSP_SHAREPIN_HM11B1);
#endif
	hx_drv_hm11b1_init(DEAULT_XHSUTDOWN_PIN);
#endif

    CLI_InitMenu();
#ifdef CLI_ENABLE_PASSWORD
    CLI_EnablePassword(FALSE);
    CLI_EnablePassword(TRUE);
#endif
	CLI_CommandLoop();
#endif
#endif
	while(1)
	{
		board_delay_ms(1);
	}
	return E_SYS;
}
#endif

#ifdef AIOT_HUMANDETECT_TV

#ifdef AUDIO
#include <audio_recognition/app_audio_recognition.h>
#endif
#include <aiot_humandetect_tv_RTOS/app_aiot_humandetect.h>

int main(void)
{
    app_iot_humandetect();

    return 0;
}
#endif //AIOT_HUMANDETECT_TV

#ifdef AIOT_HUMANDETECT_AIRCONDITION

#ifdef AUDIO
#include <audio_recognition/app_audio_recognition.h>
#endif
#include <aiot_humandetect_aircondition/app_aiot_humandetect.h>

int main(void)
{
    app_iot_humandetect();

    return 0;
}
#endif //AIOT_HUMANDETECT_AIRCONDITION

#ifdef AIOT_PEOPLE_COUNTING_VIP

#ifdef AUDIO
#include <audio_recognition/app_audio_recognition.h>
#endif
#include <aiot_people_counting_vip/app_aiot_humandetect.h>

int main(void)
{
    app_iot_humandetect();

    return 0;
}
#endif //AIOT_PEOPLE_COUNTING_VIP


#ifdef AIOT_AMR_VIP

#ifdef AUDIO
#include <audio_recognition/app_audio_recognition.h>
#endif
#include <aiot_amr_vip/app_aiot_humandetect.h>

int main(void)
{
    app_iot_humandetect();

    return 0;
}
#endif //AIOT_AMR_VIP

#ifdef AIOT_CROPNET_VIP

#ifdef AUDIO
#include <audio_recognition/app_audio_recognition.h>
#endif
#include <aiot_cropnet_vip/app_aiot_humandetect.h>

int main(void)
{
    app_iot_humandetect();

    return 0;
}
#endif //AIOT_CROPNET_VIP

#ifdef AIOT_MOBILENETV2SSD_MNIST
#include <app_aiot_mobilenetv2ssd_mnist.h>

int main(void)
{
	app_aiot_mobilenetv2ssd_mnsit();

	return 0;
}
#endif //AIOT_MOBILENETV2SSD_MNIST

#ifdef SAMPLE_CODE_RTOS
#include <sample_code_rtos/app_aiot_humandetect.h>
int main(void)
{
    app_iot_humandetect();

    return 0;
}
#endif //SAMPLE_CODE_RTOS


#ifdef CAMBRIDGE_APP
#include "cambridge_app.h"

int main(void)
{
	cambridge_app();

	return 0;
}
#endif


#ifdef CAMBRIDGE_APP_MASTER
#include "cambridge_app_master.h"

int main(void)
{
	cambridge_app_master();

	return 0;
}
#endif

#ifdef CAMBRIDGE_APP_SLAVE
#include "cambridge_app_slave.h"

int main(void)
{
	cambridge_app_slave();

	return 0;
}
#endif

#ifdef PEGA
int main(void)
{
	pega();

    return 0;
}
#endif

#if 0
static void task1(void *par);
static void task2(void *par);
static void trap_exception(void *p_excinf);
static void soft_interrupt(void *p_excinf);

/*
 * use the last external interrupt as software trigger interrupt
 * pls do not conflict with other interrupt, e.g.
 * INTNO 16 is for timer 0 (cannot be used)
 * INTNO 17 is for timer 1 (cnnnot be used)
 * the INTNO for console
 */
#define SWI_INTNO		(NUM_EXC_ALL - 1)


#define TSK_PRIOR_HI		(configMAX_PRIORITIES-1)
#define TSK_PRIOR_LO		(configMAX_PRIORITIES-2)

// Task IDs
static TaskHandle_t task1_handle = NULL;
static TaskHandle_t task2_handle = NULL;

// Mutex IDs
static SemaphoreHandle_t mux1_id;

// Semaphores
static SemaphoreHandle_t sem1_id;

// Events
static EventGroupHandle_t evt1_cb;
#define EVENT_WAIT_BITS		0x00000001

// Queues
static QueueHandle_t dtq1_id;

// Local parameters
static volatile unsigned int start = 0;
static unsigned int perf_id = 0xFF;

//#define ENABLE_DETAILED_RESULT_OUTPUT

// Change this macro if you want to run more or less times
#define MAX_SAMPLES		200

#define TASK_DELAY_MS		2

typedef struct _sample {
	unsigned int t_measure;
	unsigned int t_t2_t1;
	unsigned int t_int_t1;
	unsigned int t_t1_t2;
	unsigned int t_t1_int;
	unsigned int t_int_nest;
	unsigned int t_nest_int;
	unsigned int t_mux_rls;
	unsigned int t_sem_snd;
	unsigned int t_evt_snd;
	unsigned int t_dtq_wri;
} sample;


static sample x;
static sample x_aver = {0};

static sample sample_array[MAX_SAMPLES];
static int sample_count = 0;

/** performance timer initialization */
static void perf_init(unsigned int id)
{
	if (timer_present(id) == 0) {
		EMBARC_PRINTF("perf timer %d is not present\r\n", id);
		while (1);
	}

	timer_start(id, TIMER_CTRL_NH, 0xFFFFFFFF);

	perf_id = id;
}

/** performance timer start */
static void perf_start(void)
{
	if (timer_current(perf_id, (void *)(&start)) < 0) {
		start = 0;
	}
}

/** performance timer end, and return the time passed */
static unsigned int perf_end(void)
{
	unsigned int end = 0;

	if (timer_current(perf_id, (void *)(&end)) < 0) {
		return 0;
	}

	if (start < end) {
		return (end - start);
	} else {
		return (0xFFFFFFFF - start + end);
	}
}

/**
 * \brief  call FreeRTOS API, create and start tasks
 */
int main(void)
{
	EMBARC_PRINTF("Benchmark CPU Frequency: %d Hz\r\n", BOARD_CPU_CLOCK);
	EMBARC_PRINTF("Benchmark will run %d times, please wait about %d ms\r\n", MAX_SAMPLES, (TASK_DELAY_MS*MAX_SAMPLES));

	// Register interrupts
	int_handler_install(SWI_INTNO, (EXC_HANDLER)soft_interrupt);
	//int_pri_set(SWI_INTNO, INT_PRI_MIN);
	int_enable(SWI_INTNO);

	exc_handler_install(EXC_NO_TRAP, trap_exception); /*!< install the exception handler */

	vTaskSuspendAll();

	// Create Tasks
	if (xTaskCreate(task1, "task1", 128, (void *)1, TSK_PRIOR_LO, &task1_handle)
	    != pdPASS) {	/*!< FreeRTOS xTaskCreate() API function */
		EMBARC_PRINTF("create task1 error\r\n");
		return -1;
	}

	if (xTaskCreate(task2, "task2", 128, (void *)2, TSK_PRIOR_HI, &task2_handle)
	    != pdPASS) {	/*!< FreeRTOS xTaskCreate() API function */
		EMBARC_PRINTF("create task2 error\r\n");
		return -1;
	}

	// Create Mutex
	mux1_id = xSemaphoreCreateMutex();

	// Create Semaphores
	sem1_id = xSemaphoreCreateBinary();
	xSemaphoreGive(sem1_id);

	// Create Events
	evt1_cb = xEventGroupCreate();

	// Create Queues
	dtq1_id = xQueueCreate(1, sizeof(uint32_t));

	xTaskResumeAll();

	vTaskSuspend(NULL);
	vTaskSuspend(NULL);

	return 0;
}


/**
 * \brief  task1 in FreeRTOS
 * \details Call vTaskDelayUntil() to execute task1 with a fixed period 1 second.
 * \param[in] *par
 */
static void task1(void *par)
{
	uint32_t queue_data = 1;
	TickType_t xLastExecutionTime;
	xLastExecutionTime = xTaskGetTickCount(); /*!< initialize current tick */
	EMBARC_PRINTF("task1 start !!!\r\n");
	while (1) {
		//// 1-E: Task2 --> Task1
		x.t_t2_t1 = perf_end();
		//// 2-S: Task1 --> Int
		perf_start();
		_arc_aux_write(AUX_IRQ_HINT, SWI_INTNO); /*!< activate soft_interrupt */
		//// 5-E: Int --> Task1
		x.t_int_t1 = perf_end();

		// task delay, to control benchmark run speed
		vTaskDelayUntil( &xLastExecutionTime, TASK_DELAY_MS);
		// Task 1 acquire mutex first
		xSemaphoreTake(mux1_id, portMAX_DELAY);
		// Task 1 acquire semaphore first
		xSemaphoreTake(sem1_id, portMAX_DELAY);

		//// 6-S: Task1 --> Task2
		perf_start();
		vTaskResume(task2_handle);

		//// 8-S: Mutex Release -> Mutex Acquire
		perf_start();
		// Task 1 release mutex, task 2 acquire it
		xSemaphoreGive(mux1_id);

		//// 9-S: Sem Post -> Sem Acquire
		perf_start();
		// Task 1 post sem, task 2 acquire it
		xSemaphoreGive(sem1_id);

		//// 10-S: Event Write -> Event Read
		perf_start();
		// Task 1 write event, task 2 read it
		xEventGroupSetBits(evt1_cb, EVENT_WAIT_BITS);

		//// 11-S: Queue Write -> Queue Read
		perf_start();
		// Task 1 write queue, task 2 read it
		xQueueSend(dtq1_id, (void *)(&queue_data),  portMAX_DELAY);
		queue_data ++;
	}

}

/**
 * \brief  task2 in FreeRTOS
 * \details Print information in task2 and suspend task2.
 * \param[in] *par
 */
static void task2(void *par)
{
	static uint32_t perf_times = 0;
	uint32_t queue_data = 0;
	perf_init(TIMER_1);
	EMBARC_PRINTF("task2 start !!!\r\n");
	while (1) {
		//// 1-S: Task2 --> Task1
		perf_start();
		vTaskSuspend(task2_handle); /*!< suspend task2 */
		//// 6-E: Task1 --> Task2
		x.t_t1_t2 = perf_end();
		//// 7-S: Benchmark extra cost
		perf_start();
		//// 7-E: Benchmark extra cost
		x.t_measure = perf_end();

		// Task 2 wait for task 1 release mutex
		xSemaphoreTake(mux1_id, portMAX_DELAY);
		//// 8-E: Mutex Release -> Mutex Acquire
		x.t_mux_rls = perf_end();
		xSemaphoreGive(mux1_id);

		// Task 2 wait for task 1 release sem
		xSemaphoreTake(sem1_id, portMAX_DELAY);
		//// 9-E: Sem Post -> Sem Acquire
		x.t_sem_snd = perf_end();
		xSemaphoreGive(sem1_id);

		// Task 2 wait for task 1 set event
		xEventGroupWaitBits(evt1_cb, EVENT_WAIT_BITS, pdTRUE, pdFALSE, portMAX_DELAY);

		//// 10-E: Event Write -> Event Read
		x.t_evt_snd = perf_end();

		// Task 2 read queue
		xQueueReceive(dtq1_id, (void *)(&queue_data), portMAX_DELAY);
		//// 11-E: Queue Write -> Queue Read
		x.t_dtq_wri = perf_end();

		if (perf_times) {
			if (sample_count < MAX_SAMPLES) {
				sample_array[sample_count] = x;
			} else {
				/* Suspend task 1 */
				vTaskSuspend(task1_handle);

				for (int i = 0; i < MAX_SAMPLES; i ++) {
					x_aver.t_measure += sample_array[i].t_measure;
					x_aver.t_t2_t1 += sample_array[i].t_t2_t1;
					x_aver.t_int_t1 += sample_array[i].t_int_t1;
					x_aver.t_t1_t2 += sample_array[i].t_t1_t2;
					x_aver.t_t1_int += sample_array[i].t_t1_int;
					x_aver.t_int_nest += sample_array[i].t_int_nest;
					x_aver.t_nest_int += sample_array[i].t_nest_int;
					x_aver.t_mux_rls += sample_array[i].t_mux_rls;
					x_aver.t_sem_snd += sample_array[i].t_sem_snd;
					x_aver.t_evt_snd += sample_array[i].t_evt_snd;
					x_aver.t_dtq_wri += sample_array[i].t_dtq_wri;
				}

				x_aver.t_measure /= MAX_SAMPLES;
				x_aver.t_t2_t1 /= MAX_SAMPLES;
				x_aver.t_int_t1 /= MAX_SAMPLES;
				x_aver.t_t1_t2 /= MAX_SAMPLES;
				x_aver.t_t1_int /= MAX_SAMPLES;
				x_aver.t_int_nest /= MAX_SAMPLES;
				x_aver.t_nest_int /= MAX_SAMPLES;
				x_aver.t_mux_rls /= MAX_SAMPLES;
				x_aver.t_sem_snd /= MAX_SAMPLES;
				x_aver.t_evt_snd /= MAX_SAMPLES;
				x_aver.t_dtq_wri /= MAX_SAMPLES;

				EMBARC_PRINTF("\r\n");
				EMBARC_PRINTF("Average benchmark result list as follows:\r\n");
				EMBARC_PRINTF("extra measurement cost : %d cycles\r\n", x_aver.t_measure);
				EMBARC_PRINTF("task2     ->  task1    : %d cycles\r\n", x_aver.t_t2_t1);
				EMBARC_PRINTF("task1     ->  int      : %d cycles\r\n", x_aver.t_t1_int);
				EMBARC_PRINTF("int       ->  nest int : %d cycles\r\n", x_aver.t_int_nest);
				EMBARC_PRINTF("nest int  ->  int      : %d cycles\r\n", x_aver.t_nest_int);
				EMBARC_PRINTF("int       ->  task1    : %d cycles\r\n", x_aver.t_int_t1);
				EMBARC_PRINTF("task1     ->  task2    : %d cycles\r\n", x_aver.t_t1_t2);
				EMBARC_PRINTF("mux: tsk1 ->  tsk2     : %d cycles\r\n", x_aver.t_mux_rls);
				EMBARC_PRINTF("sem: tsk1 ->  tsk2     : %d cycles\r\n", x_aver.t_sem_snd);
				EMBARC_PRINTF("evt: tsk1 ->  tsk2     : %d cycles\r\n", x_aver.t_evt_snd);
				EMBARC_PRINTF("dtq: tsk1 ->  tsk2     : %d cycles\r\n", x_aver.t_dtq_wri);
				EMBARC_PRINTF("\r\n");

#ifdef ENABLE_DETAILED_RESULT_OUTPUT
				EMBARC_PRINTF("Detailed benchmark result table list as follows:\r\n");
				EMBARC_PRINTF("t_measure t_t2_t1 t_t1_int t_int_nest t_nest_int t_int_t1 t_t1_t2 t_mux_rls t_sem_snd t_evt_snd t_dtq_wri\r\n");

				for (int i = 0; i < MAX_SAMPLES; i ++) {
					EMBARC_PRINTF("%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\r\n", \
					              sample_array[i].t_measure, sample_array[i].t_t2_t1, \
					              sample_array[i].t_t1_int, sample_array[i].t_int_nest, \
					              sample_array[i].t_nest_int, sample_array[i].t_int_t1, \
					              sample_array[i].t_t1_t2, sample_array[i].t_mux_rls, \
					              sample_array[i].t_sem_snd, sample_array[i].t_evt_snd, \
					              sample_array[i].t_dtq_wri);
				}

#endif
				exit(0); /* Exit benchmark application */
			}

			sample_count ++;
		}

		perf_times ++;
	}
}

/**
 * \brief  trap exception
 * \details Call xTaskResumeFromISR() to resume task2 that can be called from within ISR.
 * If resuming the task2 should result in a context switch, call vPortYieldFromIsr() to generate task switch request.
 * \param[in] *p_excinf
 */
static void trap_exception(void *p_excinf)
{
	//// 3-E: Int --> Nest Int
	x.t_int_nest = perf_end();
	//// 4-S: Nest Int --> Int
	perf_start();
}

/**
 * \brief  soft interrupt
 * \details Call trap_s instruction to raise the exception.
 * \param[in] *p_excinf
 */
static void soft_interrupt(void *p_exinf)
{
	//// 2-E: Task1 --> Int
	x.t_t1_int = perf_end();
	//// 3-S: Int --> Nest Int
	perf_start();
	Asm("trap_s 1");
	//// 4-E: Nest Int --> Int
	x.t_nest_int = perf_end();
	//// 5-S: Int --> Task1
	perf_start();
}

/** @} */
#endif
/*elif defined(OS_FREERTOS) end*/
#elif (defined OS_TX)

#include "embARC.h"
#include "embARC_debug.h"
#include "tx_api.h"

#ifdef TX_TEST
int main(void)
{
	tx_test();
	return 0;
}
#endif


#ifdef HX_AIOT_NB
int main(void)
{
	hx_aiot_nb();
	return 0;
}
#endif
/*elfi (defined OS_TX)*/
#endif
