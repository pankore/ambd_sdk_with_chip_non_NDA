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

typedef u32 TickType_t;
int _nanosleep( const struct timespec * rqtp, struct timespec * rmtp );
int _vTaskDelay( const TickType_t xTicksToDelay );
time_t _time( time_t * tloc );
void matter_rtc_init(void);
long long matter_rtc_read(void);
void matter_rtc_write(long long time);
uint64_t ameba_get_clock_time(void);
void matter_timer_init(void);
#if defined(CONFIG_ENABLE_AMEBA_SNTP) && (CONFIG_ENABLE_AMEBA_SNTP == 1)
bool matter_sntp_rtc_is_sync(void);
void matter_sntp_get_current_time(time_t *current_sec, time_t *current_usec);
void matter_sntp_init(void);
#endif
#ifdef __cplusplus
}
#endif
