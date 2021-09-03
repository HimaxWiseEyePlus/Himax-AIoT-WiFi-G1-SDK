#include "embARC.h"
#include "embARC_debug.h"

/* app headers */
#include "app_i2c.h"
#include "app_api.h"
#include "app_wifi.h"

/* misc. */

#define SSID "your_wifi_ssid" //your wifi ssid
#define PASSWD "your_wifi_password" //you wifi password
#define SERVER_IP "192.168.1.100" //your tcp server ip

/* private functions */
static int32_t priv_wifi_send_result(uint32_t data_addr, uint32_t data_size, uint8_t data_type);


/* local variables */
static struct_algoResult algo_result;

void app_setup()
{
    app_algo_init();
    app_spi_init();
    app_i2c_cmd_init();
    app_sensor_init();
    app_wifi_init();
}

void app_main()
{
    uint32_t raw_img_addr, img_width, img_heigh;
    uint32_t jpg_addr, jpg_size;

    dbg_printf(DBG_LESS_INFO, "app_main starting...\n");
    app_setup();

    app_wifi_connect(SSID, PASSWD);
    app_tcp_open(SERVER_IP, 5555, 0);

    while(1)
    {
        /* I2C commands handling. e.g. OTA IntoUpg */
        app_i2c_cmd_polling();

        /* Capture Image */
        app_sensor_get_image(&raw_img_addr, &img_width, &img_heigh, &jpg_addr, &jpg_size);

        /* Run Algorithm */
        app_algo_run(raw_img_addr, img_width, img_heigh, &algo_result);

        /* Send out results by SPI */
        app_spi_write(jpg_addr, jpg_size, DATA_TYPE_JPG);
        app_spi_write((uint32_t)&algo_result, sizeof(struct_algoResult), DATA_TYPE_META_DATA);
        if (algo_result.humanPresence) {
            dbg_printf(DBG_LESS_INFO, "Human presence\n");
        }

        /* Send out results through WiFi */
        priv_wifi_send_result(jpg_addr, jpg_size, DATA_TYPE_JPG);
        priv_wifi_send_result((uint32_t)&algo_result, sizeof(struct_algoResult), DATA_TYPE_META_DATA);
    }
}

static int32_t priv_wifi_send_result(uint32_t data_addr, uint32_t data_size, uint8_t data_type)
{
#ifdef USE_APK
    unsigned char len_info[4] = {0};
    uint32_t link_id = -1;
#else
    unsigned char header[7] = {0};
    uint32_t link_id = 0;
#endif
    int32_t ret = -1;

    dbg_printf(DBG_LESS_INFO, "app_wifi_send_result> data_type = 0x%02X\n", data_type);

    /* Send JPEG. */
#ifdef USE_APK
    ret = app_tcp_open(TCP_SERV_IP, TCP_SERV_PORT, &link_id);
    if (ret != 0) {
    	dbg_printf(DBG_LESS_INFO, "app_wifi_send_result> app_tcp_open failed!\n");
    	return;
    }
    len_info[0] = (data_size>>24)&0xff;
    len_info[1] = (data_size>>16)&0xff;
    len_info[2] = (data_size>>8)&0xff;
    len_info[3] = (data_size)&0xff;
    dbg_printf(DBG_LESS_INFO, "len_info[0] = 0x%02x\n", len_info[0]);
    dbg_printf(DBG_LESS_INFO, "len_info[1] = 0x%02x\n", len_info[1]);
    dbg_printf(DBG_LESS_INFO, "len_info[2] = 0x%02x\n", len_info[2]);
    dbg_printf(DBG_LESS_INFO, "len_info[3] = 0x%02x\n", len_info[3]);
    ret = app_tcp_send(link_id, len_info, 4); //send size 
    ret = app_tcp_send(link_id, (unsigned char *)data_addr, data_size); //send payload
    ret = app_tcp_close(link_id);
#else /* USE_APK */
    header[0] = 0xC0;
    header[1] = 0x5A;
    header[2] = data_type;
    header[3] = (data_size)&0xff;
    header[4] = (data_size>>8)&0xff;
    header[5] = (data_size>>16)&0xff;
    header[6] = (data_size>>24)&0xff;
#if 0
    dbg_printf(DBG_LESS_INFO, "header[3] = 0x%02x\n", header[3]);
    dbg_printf(DBG_LESS_INFO, "header[4] = 0x%02x\n", header[4]);
    dbg_printf(DBG_LESS_INFO, "header[5] = 0x%02x\n", header[5]);
    dbg_printf(DBG_LESS_INFO, "header[6] = 0x%02x\n", header[6]);
#endif
    ret = app_tcp_send(link_id, header, 7); //send pkt header
    ret = app_tcp_send(link_id, (unsigned char *)data_addr, data_size); //send payload
#endif /* USE_APK */

    return ret;
}


