# Himax AIoT Platform Board with NB-IoT(TensorFlow Lite for Microcontrollers)
It is a modified version of the [TensorFlow Lite for Microcontrollers](https://github.com/tensorflow/tensorflow/tree/master/tensorflow/lite/micro) for use with Himax-AIoT-NB-G3 Boards. Each example in the package has been tested in Ubuntu 20.04 LTS environment.

Following examples are included :
- person detection INT8 example

## Table of contents
  - [Prerequisites](#prerequisites)
  - [Deploy to Himax-AIoT-NB-G3](#deploy-to-himax-aiot-NB-G3)    

## Prerequisites
- Make Tool version

  A `make` tool is required for deploying Tensorflow Lite Micro applications, See
[Check make tool version](https://github.com/tensorflow/tensorflow/blob/master/tensorflow/lite/micro/tools/make/targets/arc/README.md#make-tool)
section for proper environment.

- Development Toolkit

  Install the toolkits listed below:

  - GNU Development Toolkit

    See
[ARC GNU Tool Chain](https://github.com/foss-for-synopsys-dwc-arc-processors/toolchain) section for more detail, current released GNU version is [GNU Toolchain for ARC Processors, 2020.09](https://github.com/foss-for-synopsys-dwc-arc-processors/toolchain/releases/download/arc-2020.09-release/arc_gnu_2020.09_prebuilt_elf32_le_linux_install.tar.gz). After download and extract toolkit to local space, please remember to add it to environment PATH. For example:

    ```
    export PATH=[location of your ARC_GNU_ROOT]/bin:$PATH
    ```

- curl command

  Installing curl for Ubuntu Linux.
  ```
  sudo apt update
  sudo apt upgrade
  sudo apt install curl
  ```
- Serial Terminal Emulation Application

  There are 2 main purposes for Himax-AIoT-NB-G3 Debug UART port, print application output and burn application to flash by using xmodem send application binary.

## Deploy to Himax-AIoT-NB-G3

Default building toolchain in makefile is Metaware Development toolkit, if you are trying to build example with GNU toolkit. please change the `ARC_TOOLCHAIN` define in `Makefile` like this

```
ARC_TOOLCHAIN ?= gnu
```

Before Build person detection INT8 example and flash image, you need to Himax-AIoT-NB-G3-SDK-Azure-RTOS-main\image_gen_linux directory
```
chmod 777 *
```
Build person detection INT8 example and flash image. The resulted flash image will be named `WEI_FW_gnu_arcem9d_wei_r16.img` and saved under `Himax-AIoT-NB-G3-SDK-Azure-RTOS-main\himax_aiot_nb_g3-master\obj_socket_24\gnu_arcem9d_wei_r16`by default.
```
make clean && make && make flash 
```
After flash image generated, please download the flash image file to Himax-AIoT-NB-G3 EVB by UART, details are described [here](https://github.com/HimaxWiseEyePlus/Himax-AIoT-NB-G3-SDK-Azure-RTOS/tree/master/Himax-AIoT-NB-G3_user_guide#flash-image-update)
