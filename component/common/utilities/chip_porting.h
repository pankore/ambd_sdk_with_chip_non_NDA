/**
 * @brief High resolution sleep.
 *
 * http://pubs.opengroup.org/onlinepubs/9699919799/functions/nanosleep.html
 *
 * @note rmtp is ignored, as signals are not implemented.
 */
#ifdef __cplusplus
extern "C" {
#endif

#include <wifi_conf.h>

int _nanosleep( const struct timespec * rqtp, struct timespec * rmtp );
int _vTaskDelay( const TickType_t xTicksToDelay );
time_t _time( time_t * tloc );

// for AMBDConfig
int32_t initPref(void);
int32_t deinitPref(void);
int32_t registerPref(const char * ns);
int32_t registerPref2(char * ns);
int32_t clearPref(const char * ns);
int32_t deleteKey(const char *domain, const char *key);
BOOL checkExist(const char *domain, const char *key);
int32_t setPref_new(const char *domain, const char *key, uint8_t *value, size_t byteCount);
int32_t getPref_bool_new(const char *domain, const char *key, uint32_t *val);
int32_t getPref_u32_new(const char *domain, const char *key, uint32_t *val);
int32_t getPref_u64_new(const char *domain, const char *key, uint64_t *val);
int32_t getPref_str_new(const char *domain, const char *key, char * buf, size_t bufSize, size_t *outLen);
int32_t getPref_bin_new(const char *domain, const char *key, uint8_t * buf, size_t bufSize, size_t *outLen);

void wifi_btcoex_set_bt_on(void);
extern int CHIP_SetWiFiConfig(rtw_wifi_setting_t *config);
extern int CHIP_GetWiFiConfig(rtw_wifi_setting_t *config);
extern rtw_mode_t wifi_mode;

#ifdef __cplusplus
}
#endif