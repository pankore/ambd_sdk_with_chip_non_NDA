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

#include <matter_dcts.h>
#include <matter_ota.h>
#include <matter_timers.h>
#include <matter_utils.h>
#include <matter_wifis.h>
#include <cmsis_compiler.h>
#include <stddef.h> /* for size_t */
#include <stdarg.h>
#include <platform_opts_bt.h>
#include <dct.h>
#include <wifi_structures.h>

#if CONFIG_BT_MATTER_ADAPTER
/** @brief  Config local address type: 0-pulic address, 1-static random address, 2-random resolvable private address */
#undef F_BT_LE_USE_RANDOM_ADDR
#define F_BT_LE_USE_RANDOM_ADDR      1
#endif /*CONFIG_BT_MATTER_ADAPTER*/

#ifdef __cplusplus
}
#endif
