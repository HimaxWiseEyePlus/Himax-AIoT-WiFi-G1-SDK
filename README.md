# Himax-AIoT-WiFi-G1 Platform EVB USER GUIDE

Himax-AIoT-WiFi-G1 platforms EVB includes Himax WE-I Plus MCU, image sensor and rich peripheral supports. The details are given in the following paragraph.


## Table of contents

- [Himax-AIoT-WiFi-G1 Platform EVB](#himax-aiot-nb-g3-platform-evb)

- [System Requirement](#system-requirement)
  - [Hardware Environment Setup](#system-requirement)
  - [Software Tools](#system-requirement)

- [Himax-AIoT-WiFi-G1 Platform EVB Startup](#himax-aiot-nb-g3-platform-evb-startup)
  - [Startup Flowchart](#startup-flowchart)
  - [Power ON EVB](#power-on-evb)
  - [Flash Image via OTA tool](#flash-image-via-ota-tool)
  - [Boot from i2c](#boot-from-i2c)
  - [Check UART Message Output](#check-uart-message-output)

- [Retrieve detection result on Himax-AioT-WiFi-G1](#retrieve-detection-result-on-himax-aiot-wifi-g1)
  - [PC tool](#pc-tool)
  - [Wifi](#wifi)
  
- [TensorFlow Lite for Microcontroller Example](#tensorflow-lite-for-microcontroller-example)
  - [TFLM Model Path](#tflm-model-path)
  - [TFLM Example Person Detection INT8](#tflm-example-person-detection-int8)


## Himax-AIoT-WiFi-G1 Platform EVB

![alt text](docs/images/Himax-AIoT-WiFi-G1_Board.png)

  1.	Himax WE-I Plus chip
  3.	HM0360 AoS<sup>TM</sup> VGA camera
  4.    Microphones (L/R) at back side
  5.	3-Axis Accelerometer
  6.	3-Axis Gyroscope
  7.	3-Axis Magnetometer
  8.	Reset Button
  9.	GREEN LED x2 and RED LEDx1 and BLUE LEDx1
  10.	Micro-USB cable (I2C/SPI/Flash Download)

  <a href="docs/HX6539-A_HWUG(WiFi-ESP12F)_preliminary.pdf" target="_blank">Hardware user guide PDF</a>

## System Requirement
  - Operating system
    - Ubuntu 20.04 LTS

  - Hardware Environment Setup
    - Prepare a micro-USB cable to connect to EVB with PC(used as Power/UART)

  - Software Tools
    - OTA tool (Windows application, image flash)
    - HMX-AIOT-FT4222H_GUI (Windows application, I2C/CLK/SPI)
    - UART Terminal Application such as [TeraTerm](https://ttssh2.osdn.jp/index.html.en).

## Himax-AIoT-WiFi-G1 Platform EVB Startup
Use the following procedure to startup the Himax-AIoT-WiFi-G1 platform EVB.
<br>

### ***Startup Flowchart***

  ![alt text](docs/images/Himax-AIoT-WiFi-G1_FlowChart.png)

### ***Power ON EVB***
 - Power supply by usb or battery(AAA*4)


### ***Flash Image via OTA tool***
> All following steps are done in Windows environment
  
  - ***Step 1:*** Follow image below to switch pins on ***Himax-AIoT-WiFi-G1 Platform EVB***.
  
    ![alt text](docs/images/Himax-AIoT-WiFi-G1_OTA_PinSetting.png) 

  - ***Step 2:*** Connect with PC USB to power on ***Himax-AIoT-WiFi-G1 Platform EVB***.

  - ***Step 3:*** Check if you can see [UART Message Output](#check-uart-message-output).
    If no, refer to [Boot from i2c](#check-uart-message-output) to flash image.     

  - ***Step 4:*** Use `/tools/OTA_Tools/ota.exe` to download EVB image.
    - ***Step 4.1:***
    Open OTA.exe<br>
    ![alt text](docs/images/Himax-AIoT-WiFi-G1_OTATool_MainMenu.png)
   
    
    - ***Step 4.2:***
      Select `1.Command->1.Intoupg`<br>
      ![alt text](docs/images/Himax-AIoT-WiFi-G1_OTATool_SubMenu.png)
    
        If sucessful, the highlight message <span style="color:red">"Please use OTA UPGRADE TOOL Version : x.x.x"</span> will be shown in `TeraTerm`. Now the device is ready for OTA flash programming.
        ![alt text](docs/images/Himax-AIoT-WiFi-G1_OTATool_OTAMsgReady.png)

    - ***Step 4.3:*** 
        Switch back to OTA tool and select `2.Upgrade->1.FORCE UPGRADE` to start upgrade image.

        ![alt text](docs/images/Himax-AIoT-WiFi-G1_OTATool_Upgrad.png)

    - ***Step 4.4:*** Wait for a while, the device will reboot when flash process complete.
 
### ***Boot from i2c***
> If OTA process isn't accessible, boot from i2c to reload 2nd bootloader to fix problem.

- ***Step 1:*** Connect with PC USB to power on ***Himax-AIoT-WiFi-G1 Platform EVB***. 
- ***Step 2:*** Turn pin #1 switch on & press `Reset button` to reset.  

    ![alt text](docs/images/Himax-AIoT-WiFi-G1_BootFromI2C_PinSetting.png)

- ***Step 3:*** Excute OTA.exe on `/tools/OTA_Tools/` and select <br><span style="color:red">3.Other Tools->1.Boot from I2C</span>.

- ***Step 4:*** Enter the input files path manually:<br>`BootUpFromI2C\HX6537-A09TDIG-1111K_QFN72_Debug\sign_formal_PA8530_EM9D_2nd_Bootloader.bin`.<br> 

    ![alt text](docs/images/Himax-AIoT-WiFi-G1_BootFromI2C_SendFile.png)

    The board will reboot & enter 2nd boot loader, you will see the message <span style="color:red">"Please use OTA UPGRADE TOOL Version : x.x.x"</span> that is the same as normal OTA process. Now, just follow normal [Flash Image via OTA tool](#flash-image-via-ota-tool) section to complete process.

    ![alt text](docs/images/Himax-AIoT-WiFi-G1_BootFromI2C_WaitOTAMsg.png)

### ***Check UART Message Output***
  - Serial Terminal Emulation Application Setting

    |   |  |
    | :------------ |:---------------:|
    | Baud Rate  | 115200 bps |
    | Data | 8 bit |
    | Parity  | none  |
    | Stop  | 1 bit  |
    | Flow control | none |

  The system will output the following message to the UART console. Please setup UART terminal tool setting as (115200/8/N/1).

  - Display Log Message
  In the example, we use TeraTerm to see the output message.

    - TeraTerm New Connection

        ![alt text](docs/images/Himax-AIoT-WiFi-G1_TeraTerm_NewConnect.png)

    - TeraTerm Select COM Port

        ![alt text](docs/images/Himax-AIoT-WiFi-G1_TeraTerm_SelectCOMPort.png)

## Retrieve detection result on Himax-AioT-WiFi-G1
### ***PC Tool***
1. Excute GUI tool HMX_FT4222H_GUI.exe under \tools\HMX_FT4222H_GUI\GUI_binary\.
2. Select 'SPI slave' tab.
3. In Receive Image/Data field, select 'ALGO' then click the 'Receive' button.
![alt text](docs/images/Himax-AIoT-WiFi-G1_PC_Tool_1.png)
4. The GUI PC_Tool will pop a new window 'Simple_FRAMES' to display the captured image and the person detection result.
![alt text](docs/images/Himax-AIoT-WiFi-G1_PC_Tool_2.png)
5. In the 'Simple_FRAMES' window, the left-top indicator disply whether human is presence or not.
    
    Green (human detect); Red (no human detect).
    ![alt text](docs/images/Himax-AIoT-WiFi-G1_PC_Tool_3_Human.png)
    ![alt text](docs/images/Himax-AIoT-WiFi-G1_PC_Tool_4_NoHuman.png)

### ***WiFi***

1. Setup the TCP Server Example on PC.
  - ***1.1.*** Connect your Win10 PC to WiFi AP.
  - ***1.2.*** Use ipconfig to get the IP Address of your PC.
  
    ![alt text](docs/images/Himax-AIoT-WiFi-G1_python_tcp_server_get_ip.png)
  
  - ***1.3.*** Execute ***tcp_server_example.py*** on ***tools/tcp_server_example/***
    ![alt text](docs/images/Himax-AIoT-WiFi-G1_python_tcp_server_excution.png)
  - ***1.4.*** The tcp_server_example is ready to receive data.

2. Setup Himax-AIoT-WiFi-G1.
  - ***2.1.*** Download the SDK.
  - ***2.2.*** According to your WiFi environment to config the settings in to app/scenario_app/aiot_example/app_main.c:
      > #define SSID "your_wifi_ssid" //your wifi ssid <br>
      > #define PASSWD "your_wifi_password" //you wifi password
  - ***2.3.*** Set the SERVER_IP to the IP got from ***Step 1.2***
      > #define SERVER_IP "192.168.1.100" //your tcp server ip
  - ***2.4.*** Build the image and upgrade to Himax-AIoT-WF-G1([Flash Image via OTA tool](#flash-image-via-ota-tool)).
  - ***2.5.*** After Himax-AIoT-WF-G1 boot up, it should connect to the TCP Server(PC).

3. Himax-AIoT-WF-G1 continues to sending JPGs and human-detection result to the TCP Server(PC).
![alt text](docs/images/Himax-AIoT-WiFi-G1_python_tcp_server_running.png)
![alt text](docs/images/Himax-AIoT-WiFi-G1_python_tcp_server_human_presense.png)
![alt text](docs/images/Himax-AIoT-WiFi-G1_python_tcp_server_no_human.png)


## TensorFlow Lite for Microcontroller Example 

### TFLM Model Path
  - Put your training model in Himax-AIoT-NB-G3-SDK-Azure-RTOS-main\himax_aiot_nb_g3-master\app\scenario_app\hx_aiot_nb_g3 folder
 
### TFLM Example Person Detection INT8

  To generate person detection example flash binary for Himax AIoT Platform EVB:
  1. Based on the flow of [person detection example](https://github.com/tensorflow/tensorflow/tree/master/tensorflow/lite/micro/examples/person_detection_experimental#person-detection-example) to generate flash image. 
  2. Download image binary to Himax-AIoT-WiFi-G1 EVB, detail steps can be found at [Flash Image via OTA tool](#flash-image-via-ota-tool).
  3. Person detection example message will be shown on the terminal application. 

 
