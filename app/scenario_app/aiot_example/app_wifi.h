#ifndef __APP_WIFI_H__
#define __APP_WIFI_H__


typedef void (*TCP_RX_CB) (int32_t link_id, uint8_t *rx_data, int32_t rx_data_len);

int32_t app_wifi_init();
int32_t app_wifi_deinit();
int32_t app_wifi_connect(char *ssid, char *password);
int32_t app_wifi_disconnect();
int32_t app_tcp_open(char *ip_addr, int32_t port, uint32_t link_id);
int32_t app_tcp_close(int32_t link_id);
int32_t app_tcp_send(int32_t link_id, uint8_t *data, int32_t data_len);
int32_t app_tcp_send_file(char *server_ip, int server_port, uint32_t file_addr, uint32_t file_size, uint8_t type);

void app_wifi_check_rx(TCP_RX_CB rx_cb);

int32_t app_wifi_test();
int32_t app_wifi_scan_test();

#endif /* __APP_WIFI_H__ */

