#include "common.h"
#include "embARC_debug.h"
#include "board_config.h"
#include "hx_drv_iomux.h"
#include "esp8266.h"

#include "app_wifi.h"


//#define WIFI_SW_UPG_CTRL
#define ESP8266_SEND_MAX  (2048)

static char mac[32];
static ESP8266_DEF g_esp8266;

int32_t app_wifi_init()
{
    ESP8266_DEF_PTR p_esp = &g_esp8266;
    int32_t ret = 0;

    hx_drv_iomux_set_pmux(IOMUX_PGPIO12, 3);
    hx_drv_iomux_set_pmux(IOMUX_PGPIO0, 3);
#ifdef WIFI_SW_UPG_CTRL
    dbg_printf(DBG_LESS_INFO, "set PGPIO_12 and PGPIO_14 to output.\n");
    hx_drv_iomux_set_pmux(IOMUX_PGPIO14, 3);
#else
    dbg_printf(DBG_LESS_INFO, "set PGPIO_14 to input.\n");
    hx_drv_iomux_set_pmux(IOMUX_PGPIO14, 2);
#endif

    /* pull down esp8266 EN pin. */
    hx_drv_iomux_set_outvalue(IOMUX_PGPIO12, 0);
    /* pull down esp8266 RST pin. */
    hx_drv_iomux_set_outvalue(IOMUX_PGPIO0, 0);

    board_delay_ms(100);

    /* raise esp8266 EN pin. */
    hx_drv_iomux_set_outvalue(IOMUX_PGPIO12, 1);
    /* raise esp8266 RST pin. */
    hx_drv_iomux_set_outvalue(IOMUX_PGPIO0, 1);
#ifdef SW_ESP_GPIO_0
    /* raise esp8266 GPIO_0 pin for running mode. */
    hx_drv_iomux_set_outvalue(IOMUX_PGPIO14, 1);
#endif

    board_delay_ms(100);

    do {
        ret = esp8266_init(p_esp, BOARD_WIFI_UART_BAUD);
        if (ret != 0) {
            dbg_printf(DBG_LESS_INFO, "esp8266_init(rate=%d) return %d\n", BOARD_WIFI_UART_BAUD, ret);
            break;
        }
        board_delay_ms(100);

        break;
    } while (0);

    return ret;
}

int32_t app_wifi_deinit()
{
    ESP8266_DEF_PTR p_esp = &g_esp8266;

    esp8266_deinit(p_esp);
    dbg_printf(DBG_LESS_INFO, "esp8266_deinit() done.\n");

    /* raise esp8266 EN pin. */
    hx_drv_iomux_set_outvalue(IOMUX_PGPIO12, 0);
    /* raise esp8266 RST pin. */
    hx_drv_iomux_set_outvalue(IOMUX_PGPIO0, 0);

    return 0;
}

int32_t app_wifi_test()
{
    ESP8266_DEF_PTR p_esp = &g_esp8266;
    int32_t ret = 0;

    ret = esp8266_test(p_esp);
    dbg_printf(DBG_LESS_INFO, "esp8266_test() return %d\n", ret);

    return ret;
}

int32_t app_wifi_scan_test()
{
    ESP8266_DEF_PTR p_esp = &g_esp8266;
    int32_t ret = 0;
    char *scan_results;

    scan_results = (char *)malloc(2048);
    ret = esp8266_wifi_mode_set(p_esp, ESP8266_STA, false);
    if (0 != ret) {
        dbg_printf(DBG_LESS_INFO, "ERROR! esp8266_wifi_scan() return %d\n", ret);
        return ret;
    }
    ret = esp8266_wifi_scan(p_esp, scan_results, 2048);
    dbg_printf(DBG_LESS_INFO, "esp8266_wifi_scan() return %d\n", ret);
    EMBARC_PRINTF("==== SCAN RESULT ====\n");
    EMBARC_PRINTF(CPNT_GRN);
    EMBARC_PRINTF("%s", scan_results);
    EMBARC_PRINTF(CPNT_END);
    EMBARC_PRINTF("\n\n");
    free(scan_results);

    return ret;
}

int32_t app_wifi_connect(char *ssid, char *password)
{
    ESP8266_DEF_PTR p_esp = &g_esp8266;
    char *at_ssid = NULL, *at_passwd = NULL;
    int32_t ret;

    at_ssid = (char *)malloc(strlen(ssid)+2);
    at_passwd = (char *)malloc(strlen(password)+2);
    sprintf(at_ssid, "\"%s\"", ssid);
    sprintf(at_passwd, "\"%s\"", password);
    dbg_printf(DBG_LESS_INFO, "at_ssid = %s\n", at_ssid);
    dbg_printf(DBG_LESS_INFO, "at_passwd = %s\n", at_passwd);

    do {
        board_delay_ms(500);
        ret = esp8266_wifi_connect(p_esp, at_ssid, at_passwd, false);
        dbg_printf(DBG_LESS_INFO, "esp8266_wifi_connect() return %d\n", ret);

        ret = esp8266_mac_get(p_esp, mac);
        dbg_printf(DBG_LESS_INFO, "esp8266_mac_get() return %d, mac = %s\n", ret, mac);
        board_delay_ms(500);

        //set to TCP multiple mode
        ret = esp8266_tcp_multi(p_esp, true);
        dbg_printf(DBG_LESS_INFO, "esp8266_tcp_multi() return %d\n", ret);

        //set to Passive Receive mode
        ret = esp8266_tcp_recv_mode_set(p_esp, true);
        dbg_printf(DBG_LESS_INFO, "esp8266_tcp_recv_mode_set(ESP8266_RECV_PASSIVE) return %d\n", ret);

        free(at_ssid);
        free(at_passwd);
        return 0;
    } while (0);

    free(at_ssid);
    free(at_passwd);

    return -1;
}

int32_t app_wifi_disconnect()
{
    ESP8266_DEF_PTR p_esp = &g_esp8266;
    int32_t ret;

    ret = esp8266_wifi_disconnect(p_esp);
    dbg_printf(DBG_LESS_INFO, "esp8266_wifi_disconnect() return %d\n", ret);

    return ret;
}

int32_t app_wifi_status()
{
    ESP8266_DEF_PTR p_esp = &g_esp8266;
    int32_t ret;

    ret = esp8266_conn_status(p_esp);
    dbg_printf(DBG_LESS_INFO, "app_wifi_status() return %d\n", ret);

    return ret;
}

int32_t app_wifi_multi_status(uint32_t link_id)
{
    ESP8266_DEF_PTR p_esp = &g_esp8266;
    int32_t ret;

    ret = esp8266_multi_conn_status(p_esp, link_id);
    dbg_printf(DBG_LESS_INFO, "app_wifi_status() return %d\n", ret);

    return ret;
}

int32_t app_tcp_open(char *ip_addr, int32_t port, uint32_t link_id)
{
    ESP8266_DEF_PTR p_esp = &g_esp8266;
    int32_t ret;

    ret = esp8266_tcp_multi_connect(p_esp, link_id, ip_addr, port);
    dbg_printf(DBG_LESS_INFO, "esp8266_tcp_multi_connect(%d, %s, %d) return %d\n", link_id, ip_addr, port, ret);

    return ret;
}

int32_t app_tcp_close(int32_t link_id)
{
    ESP8266_DEF_PTR p_esp = &g_esp8266;
    int32_t ret;

    ret = esp8266_tcp_multi_disconnect(p_esp, link_id);
    dbg_printf(DBG_LESS_INFO, "esp8266_tcp_multi_disconnect(%d) return %d\n", link_id, ret);

    return ret;
}

int32_t app_tcp_send(int32_t link_id, uint8_t *data, int32_t data_len)
{
    ESP8266_DEF_PTR p_esp = &g_esp8266;
    unsigned char *send_ptr = NULL;
    uint32_t sent_bytes = 0, send_len = 0;
    int32_t ret;

    while (sent_bytes < data_len) {
        send_ptr = (unsigned char *)(data + sent_bytes);
        if (data_len - sent_bytes < ESP8266_SEND_MAX) {
            send_len = data_len - sent_bytes;
        } else {
            send_len = ESP8266_SEND_MAX;
        }
        ret = esp8266_tcp_multi_write(p_esp, link_id, (char*)send_ptr, send_len);
        if (ret != 0) {
            dbg_printf(DBG_LESS_INFO, "esp8266_tcp_multi_write(%d, sent_bytes = %d, %d) return %d\n", link_id, sent_bytes,  send_len, ret);
            return ret;
        }
        sent_bytes += send_len;
    }

    return 0;
}

void app_wifi_check_rx(TCP_RX_CB rx_cb)
{
    ESP8266_DEF_PTR p_esp = &g_esp8266;
    int32_t len_ln0 = -1, len_ln1 = -1, len_ln2 = -1, len_ln3 = -1, len_ln4 = -1;
    uint32_t ret;

    ret = esp8266_tcp_passive_recv_get_len(p_esp, &len_ln0, &len_ln1, &len_ln2, &len_ln3, &len_ln4);

    if (len_ln0 > 0 || len_ln1 > 0 || len_ln2 > 0 || len_ln3 > 0 || len_ln4 > 0) {
        dbg_printf(DBG_LESS_INFO, "esp8266_tcp_passive_recv_get_len: length of link_id 0, 1, 2, 3, 4 = (%d, %d, %d, %d, %d)\n", len_ln0, len_ln1, len_ln2, len_ln3, len_ln4);
    }

    /* Get the Receive Data for Link_ID 0 */
    if (0 == ret && len_ln0 > 0) {
        uint8_t *rx_data;
        int32_t rx_len = 0;

        rx_data = (uint8_t *)malloc(len_ln0);
        ret = esp8266_tcp_multi_passive_read(p_esp, 0, (char*)rx_data, len_ln0, &rx_len);
        if (0 == ret && rx_len > 0) {
            rx_cb(0, rx_data, rx_len);
        }
        free(rx_data);
    }

    /* Get the Receive Data for Link_ID 1 */
    if (0 == ret && len_ln1 > 0) {
        uint8_t *rx_data;
        int32_t rx_len = 0;

        rx_data = (uint8_t *)malloc(len_ln1);
        ret = esp8266_tcp_multi_passive_read(p_esp, 1, (char*)rx_data, len_ln1, &rx_len);
        if (0 == ret && rx_len > 0) {
            rx_cb(1, rx_data, rx_len);
        }
        free(rx_data);
    }

    /* TODO: Should get the Receive Data for other Link_IDs. */
}

