/**
 * @brief High resolution sleep.
 *
 * http://pubs.opengroup.org/onlinepubs/9699919799/functions/nanosleep.html
 *
 * @note rmtp is ignored, as signals are not implemented.
 */
#ifndef MATTER_WIFIS_H_
#define MATTER_WIFIS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <wifi_conf.h>
#include <lwip_netconf.h>

#define JOIN_HANDSHAKE_DONE (uint32_t)(1 << 7)

typedef enum{
    MATTER_WIFI_EVENT_CONNECT = 0,
    MATTER_WIFI_EVENT_FOURWAY_HANDSHAKE_DONE = 1,
    MATTER_WIFI_EVENT_DISCONNECT = 2,
    MATTER_WIFI_EVENT_DHCP6_DONE = 3,
} matter_wifi_event;

extern uint32_t rtw_join_status;

void wifi_btcoex_set_bt_on(void);
extern int CHIP_SetWiFiConfig(rtw_wifi_setting_t *config);
extern int CHIP_GetWiFiConfig(rtw_wifi_setting_t *config);
extern rtw_mode_t wifi_mode;

/******************************************************
 *               WiFi Structure
 ******************************************************/
#if defined(CONFIG_AUTO_RECONNECT) && CONFIG_AUTO_RECONNECT
struct matter_wifi_autoreconnect_param {
    rtw_security_t security_type;
    char *ssid;
    int ssid_len;
    char *password;
    int password_len;
    int key_id;
};
#endif /* CONFIG_AUTO_RECONNECT */

/******************************************************
 *               WiFi Security
 ******************************************************/

#define MATTER_WIFI_VERSION_11B       0x01
#define MATTER_WIFI_VERSION_11G       0x02
#define MATTER_WIFI_VERSION_11A       0x04
#define MATTER_WIFI_VERSION_11N       0x18  // 0x8: 2.4G, 0x10: 5Gs
#define MATTER_WIFI_VERSION_11AC      0x40
#define MATTER_WIFI_VERSION_11AX      0x80
#define MATTER_WIFI_VERSION_11AH      0x100

extern u32 apNum;
typedef int (*chip_connmgr_callback)(void *object);
void chip_connmgr_set_callback_func(chip_connmgr_callback p, void *data);
void matter_wifi_scan_networks(void);
void matter_wifi_scan_networks_with_ssid(const unsigned char *ssid, size_t length);
rtw_scan_result_t *matter_get_scan_results(void);
void matter_wifi_autoreconnect_hdl(rtw_security_t security_type, char *ssid, int ssid_len,
                                   char *password, int password_len, int key_id);
void matter_wifi_set_autoreconnect(u8 mode);
int matter_wifi_connect(char *ssid, rtw_security_t security_type, char *password,
                        int ssid_len, int password_len, int key_id, void *semaphore);
int matter_get_sta_wifi_info(rtw_wifi_setting_t *pSetting);
int matter_wifi_disconnect(void);
int matter_wifi_on(rtw_mode_t mode);
int matter_wifi_set_mode(rtw_mode_t mode);
int matter_wifi_is_connected_to_ap(void);
int matter_wifi_is_open_security (void);
int matter_wifi_is_ready_to_transceive(rtw_interface_t interface);
int matter_wifi_is_up(rtw_interface_t interface);
int matter_wifi_is_station_mode(void);
void matter_lwip_dhcp(void);
void matter_lwip_dhcp6(void);
void matter_lwip_releaseip(void);
int matter_wifi_get_ap_bssid(unsigned char*);
int matter_wifi_sta_get_network_mode(rtw_network_mode_t *pmode);
int matter_wifi_sta_get_security_type(uint32_t *wifi_security);
int matter_wifi_sta_get_channel_number(uint8_t *ch);
int matter_wifi_sta_get_rssi(int *prssi);
int matter_wifi_sta_get_ap_bssid(unsigned char *bssid);
int matter_wifi_sta_get_wifi_version(uint8_t *mode);
int matter_wifi_get_mac_address(char *mac);
int matter_wifi_get_last_error(void);
#if LWIP_VERSION_MAJOR > 2 || LWIP_VERSION_MINOR > 0
#if LWIP_IPV6
uint8_t *matter_LwIP_GetIPv6_linklocal(uint8_t idx);
uint8_t *matter_LwIP_GetIPv6_global(uint8_t idx);
#endif // LWIP_IPV6
#endif // LWIP_VERSION_MAJOR > 2 || LWIP_VERSION_MINOR > 0
unsigned char *matter_LwIP_GetIP(uint8_t idx);
unsigned char *matter_LwIP_GetGW(uint8_t idx);
uint8_t *matter_LwIP_GetMASK(uint8_t idx);
int matter_wifi_get_setting(unsigned char wlan_idx, rtw_wifi_setting_t *psetting);
void matter_wifi_reg_event_handler(matter_wifi_event event_cmds, rtw_event_handler_t handler_func, void *handler_user_data);

#ifdef __cplusplus
}
#endif

#endif //MATTER_WIFIS_H_
