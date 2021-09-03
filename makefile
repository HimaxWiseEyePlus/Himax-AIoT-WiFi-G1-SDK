## embARC application makefile template ##
### You can copy this file to your application folder
### and rename it to makefile.
##

##
# Selected OS
#freertos
#tx
##
#OS_SEL ?= tx

# Application name
APPL ?= WEI_FW

# Optimization Level
# Please Refer to toolchain_xxx.mk for this option
OLEVEL ?= O2

##
# Current Board And Core
##
BOARD ?= socket
BD_VER ?= 24
CUR_CORE ?= arcem9d_wei_r16
##
# select debugging jtag
##
JTAG ?= usb

##
# IC package select : LQFP128/WLCSP38/QFP100/QFN72
##
IC_PACKAGE_SEL = LQFP128

##
# Set toolchain
## gnu
TOOLCHAIN ?= gnu

##
# Set OS type
#RTOS
#TX
##
#OS_TYPE ?= TX

##
# Uncomment following options
# if you want to set your own heap and stack size
# Default settings see options.mk
##
#HEAPSZ ?= 29000
HEAPSZ ?= 32768
#HEAPSZ ?= 131072
#STACKSZ ?= 8192


##
# support TLS function?
##
#TLS_ENABLE ?= 1

##
# Uncomment following options
# if you want to add your own library into link process
# For example:
# If you want link math lib for gnu toolchain,
# you need to set the option to -lm
##
#APPL_LIBS ?=

##
# root dir of embARC
##
EMBARC_ROOT = .

##
# library support feature
# Add new library here
# The source code should be loacted in ~\library\{lib_name}\
# sensordp : for hwacc datapath
# security : for security library
##
LIB_SEL = security

##
# middleware support feature
# Add new middleware here
# The source code should be loacted in ~\middleware\{mid_name}\
##
MID_SEL =

##
# CIS_SENSOR support feature HM_COMMON, HM1245, HM2140, HM01B0_MONO, HM01B0_BAYER
# HM0360_MONO, HM0360_MONO_OSC, HM0360_BAYER, HM0360_BAYER_OSC, HM11B1, HM11B1_REVC
# HM0360_MONO_REVB, HM0360_MONO_OSC_REVB, HM0360_BAYER_REVB, HM0360_BAYER_OSC_REVB
##
CIS_SEL = HM_COMMON
##
# CIS_XSHUTDOWN_PIN_SEL XSHUTDOWN_SGPIO0(FPGA board), XSHUTDOWN_SGPIO1(Socket board, AIOT board)
##
CIS_XSHUTDOWN_PIN_SEL = XSHUTDOWN_SGPIO1

##
# application defines
# Add any definition to enable/disable any feature of application
# EX: -DCLI_ENABLE - Support CLI command for System/IP verification
#     -DEVENT_PROCESS - Support Event Create/Handler for System Usage
##
APPL_DEFINES =

# USING Pre-built BSP library
# 1 for enable
# other for disable
##
LIB_BSP_PREBUILT = 1

# USING Pre-built CV library
# 1 for enable
# other for disable
##
LIB_CV_PREBUILT = 1

# USING Pre-built AUDIO library
# 1 for enable
# other for disable
##
LIB_AUDIO_ALGO_PREBUILT = 1

# USING latest AUDIO library
# 1 for using latest AUDIO library
# other for old AUDIO library
##
USE_LATEST_LIB_AUDIO_ALGO = 0

# USING TONE AUDIO library
# 1 for enable
# other for disable
##
USE_TONE_LIB_AUDIO_ALGO = 0

# USING Pre-built ALANGO library
# 1 for enable
# other for disable
##
LIB_AUDIO_ALANGO_PREBUILT = 1

# USING Pre-built AZ_SDK library
# 1 for enable
# other for disable
##
LIB_AZ_SDK_PREBUILT = 1

# USING Pre-built NETXDUO library
# 1 for enable
# other for disable
##
LIB_NETXDUO_PREBUILT = 1




# Flag for pure board initial(no peripheral initial)
# 1 for enable
# other for disable
##
#PURE_BOARD_INIT = 1


##
# Integrate Support
# CLI_PERIPHERAL, CLI_HWACCBYTPG_LIB
# CLI_EXTERNAL
# CLI_HM1245_DP_LIB, CLI_HM2140_DP_LIB
# CLI_HM01B0_BAYER_DP_LIB
# CLI_HM01B0_MONO_DP_LIB
# CLI_EDM
# CLI_HM11B1_DP_LIB
# CLI_HM0360_RESIOSTREAM_DP_LIB
# CLI_HM0360_MONO_DP_LIB
# CLI_HM0360_BAYER_DP_LIB
# CLI_HM1055_DP_LIB
# CLI_PMU_TEST
# HWACCBYTPG_AUTO_TEST

# PS: As follow are scenario_app app name should be the same as directory name
# pmu_lib_test
# sample_code_prerolling_gpiowakeup
# sample_code_cdm_wakeup
# sample_code_gpiowakeup
# sample_code_periodical_wakeup_quickboot
# sample_code_rtos
# demo_scenario_human
# event_handler_testapp
# aiot_facedetect_allon
# aiot_facedetect_cdm
# aiot_humandetect_pir
# aiot_humandetect_smartdoorlock
# aiot_algodetect_analogwakeup
# aiot_algodetect_sensormdwakeup
# aiot_occupancy
# aiot_bodydetect_allon
# qnb_facedetect
# power_measure_app_human
# power_measure_app_ocs
# power_measure_dhrystone
# aiot_humandetect_tv
# aiot_humandetect_tv_cdm
# aiot_humandetect_tv_dut_test
# aiot_humandetect_tv_RTOS
# aiot_humandetect_aircondition
# audio_recognition
# emza_hog_test
# aiot_humandetect_surveillance
# send_frame_test
# tflm_sdk
# aiot_bodydetect_allon_tflite
# aiot_bodydetect_allon_tflite_code_gen (set ALGO_TYPE = TFLITE_MICRO_GOOGLE_PERSON)
# standalone_test
# aiot_chrome
# aiot_bodydetect_allon_tflite_HIMAX
# aiot_bodydetect_allon_tflite_HIMAX_code_gen (set ALGO_TYPE = TFLITE_MICRO_HIMAX_PERSON)
# aiot_surveillance_tflite_HIMAX
# aiot_amr_vip
# sample_code_algo
# slp2_slt
# sd_slt
# slp2_slt_nosensor
# sd_slt_nosensor
# mp3_libmad
# aiot_conference_call
# cambridge_app
# cambridge_app_master
# cambridge_app_slave
# tx_test
# hx_aiot_nb
##
APP_TYPE = aiot_example
#APP_TYPE = tflm_sdk
##
#  EMZA_SIMULATOR
#  EMZA_HUMAN_DETECT
#  EMZA_OCCUPANCY
#  EMZA_FACE_DETECT
#  EMZA_FACE_DETECT_V2
#  EMZA_FACE_DETECT_V4
#  EMZA_FACE_DETECT_V6
#  SNPS_CNN_FACEDETECT
#  DATA_BSS_SRAM
#  OKAO
#  OKAO_NO_AUDIO
#  PIXTALKS
#  EXPASOFT_BODY_DETECTION
#  TFLITE_MICRO_GOOGLE_PERSON
#  TFLITE_MICRO_HIMAX_PERSON
##
ALGO_TYPE = TFLITE_MICRO_GOOGLE_PERSON


LINKER_SCRIPT_FILE =
##
# Add new external device here
# The source code should be located in ~\external\{device_name}\
##
EXT_DEV_LIST += als/apds9301 \
			thermal/lm75a \
			pir/hcsr501 \
			wifi/esp8266 \
			imu/lsm9ds1 \
			qwiic/ms8607 \
			qwiic/air_quality \
			qwiic/bme280 \
			nb_iot/wnb303r  \

# application makefile
include $(EMBARC_ROOT)/app/app.mk

# include current project makefile
COMMON_COMPILE_PREREQUISITES += makefile

### Options above must be added before include options.mk ###
# include key embARC build system makefile
override EMBARC_ROOT := $(strip $(subst \,/,$(EMBARC_ROOT)))
include $(EMBARC_ROOT)/options/options.mk
