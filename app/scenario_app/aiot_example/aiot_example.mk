override SCENARIO_APP_SUPPORT_LIST := $(APP_TYPE)

APPL_DEFINES += -DHX6539_WIFI_ESP12F_V10
APPL_DEFINES += -DAPP_MAIN

ifneq ("$(ALGO_TYPE)","")
ifeq ("$(ALGO_TYPE)","TFLITE_MICRO_GOOGLE_PERSON")
LIB_SEL += tflitemicro_25
endif
else #default algo
override ALGO_TYPE := DATA_BSS_SRAM
endif

LIB_SEL += audio tflm_sdk_lib
LIB_SEL += sensordp pwrmgmt
LIB_SEL += i2c_comm ota

APPL_DEFINES += -DI2C_COMM
ifeq ($(BOARD), fpga)
else
APPL_DEFINES += -DSPI_MASTER_SEND
endif

# ----------------------------------
# 	Use scalable metadata format
# ----------------------------------
# APPL_DEFINES += -DSCALABLE_METADATA
