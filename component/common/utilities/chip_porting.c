#ifdef __cplusplus
 extern "C" {
#endif

#include <stddef.h>
#include <string.h>

#include "platform_opts.h"
#include "platform/platform_stdlib.h"
#include "errno.h"
#include <sntp/sntp.h>
#include "dct.h"
#include <wifi_conf.h>

#define MICROSECONDS_PER_SECOND    ( 1000000LL )                                   /**< Microseconds per second. */
#define NANOSECONDS_PER_SECOND     ( 1000000000LL )                                /**< Nanoseconds per second. */
#define NANOSECONDS_PER_TICK       ( NANOSECONDS_PER_SECOND / configTICK_RATE_HZ ) /**< Nanoseconds per FreeRTOS tick. */

extern int FreeRTOS_errno;
#define errno FreeRTOS_errno

bool UTILS_ValidateTimespec( const struct timespec * const pxTimespec )
{
    bool xReturn = FALSE;

    if( pxTimespec != NULL )
    {
        /* Verify 0 <= tv_nsec < 1000000000. */
        if( ( pxTimespec->tv_nsec >= 0 ) &&
            ( pxTimespec->tv_nsec < NANOSECONDS_PER_SECOND ) )
        {
            xReturn = TRUE;
        }
    }

    return xReturn;
}

int _nanosleep( const struct timespec * rqtp,
               struct timespec * rmtp )
{
    int iStatus = 0;
    TickType_t xSleepTime = 0;

    /* Silence warnings about unused parameters. */
    ( void ) rmtp;

    /* Check rqtp. */
    if( UTILS_ValidateTimespec( rqtp ) == FALSE )
    {
        errno = EINVAL;
        iStatus = -1;
    }

    if( iStatus == 0 )
    {
        /* Convert rqtp to ticks and delay. */
        if( UTILS_TimespecToTicks( rqtp, &xSleepTime ) == 0 )
        {
            vTaskDelay( xSleepTime );
        }
    }

    return iStatus;
}

int __clock_gettime(struct timespec * tp)
{
	unsigned int update_tick = 0;
	long update_sec = 0, update_usec = 0, current_sec = 0, current_usec = 0;
	unsigned int current_tick = xTaskGetTickCount();

	sntp_get_lasttime(&update_sec, &update_usec, &update_tick);
	//if(update_tick) {
		long tick_diff_sec, tick_diff_ms;

		tick_diff_sec = (current_tick - update_tick) / configTICK_RATE_HZ;
		tick_diff_ms = (current_tick - update_tick) % configTICK_RATE_HZ / portTICK_RATE_MS;
		update_sec += tick_diff_sec;
		update_usec += (tick_diff_ms * 1000);
        current_sec = update_sec + update_usec / 1000000;
        current_usec = update_usec % 1000000;
	//}
	//else {
		//current_sec = current_tick / configTICK_RATE_HZ;
	//}
	tp->tv_sec = current_sec;
    tp->tv_nsec = current_usec*1000;
	//sntp_set_lasttime(update_sec,update_usec,update_tick);
    //printf("update_sec %d update_usec %d update_tick %d tvsec %d\r\n",update_sec,update_usec,update_tick,tp->tv_sec);
}

time_t _time( time_t * tloc )
{
#if 0
    /* Read the current FreeRTOS tick count and convert it to seconds. */
    time_t xCurrentTime = ( time_t ) ( xTaskGetTickCount() / configTICK_RATE_HZ );
#else
    time_t xCurrentTime;
    struct timespec tp;

    __clock_gettime(&tp);
    xCurrentTime = tp.tv_sec;
#endif
    /* Set the output parameter if provided. */
    if( tloc != NULL )
    {
        *tloc = xCurrentTime;
    }

    return xCurrentTime;
}

extern void vTaskDelay( const TickType_t xTicksToDelay );
int _vTaskDelay( const TickType_t xTicksToDelay )
{
	vTaskDelay(xTicksToDelay);

	return 0;
}

/*
   module size is 4k, we set max module number as 12;
   if backup enabled, the total module number is 12 + 1*12 = 24, the size is 96k;
   if wear leveling enabled, the total module number is 12 + 2*12 + 3*12 = 36, the size is 288k"
*/
#define DCT_BEGIN_ADDR_MATTER   MATTER_KVS_BEGIN_ADDR
#define MODULE_NUM              MATTER_KVS_MODULE_NUM
#define VARIABLE_NAME_SIZE      MATTER_KVS_VARIABLE_NAME_SIZE
#define VARIABLE_VALUE_SIZE     MATTER_KVS_VARIABLE_VALUE_SIZE

#define DCT_BEGIN_ADDR_MATTER2  MATTER_KVS_BEGIN_ADDR2
#define MODULE_NUM2             MATTER_KVS_MODULE_NUM2
#define VARIABLE_NAME_SIZE2     MATTER_KVS_VARIABLE_NAME_SIZE2
#define VARIABLE_VALUE_SIZE2    MATTER_KVS_VARIABLE_VALUE_SIZE2

#define ENABLE_BACKUP           MATTER_KVS_ENABLE_BACKUP
#define ENABLE_WEAR_LEVELING    MATTER_KVS_ENABLE_WEAR_LEVELING

int32_t initPref(void)
{
    int32_t ret;
    ret = dct_init(DCT_BEGIN_ADDR_MATTER, MODULE_NUM, VARIABLE_NAME_SIZE, VARIABLE_VALUE_SIZE, ENABLE_BACKUP, ENABLE_WEAR_LEVELING);
    if (ret != 0)
        printf("dct_init fail\n");
    else
        printf("dct_init success\n");

    ret = dct_init2(DCT_BEGIN_ADDR_MATTER2, MODULE_NUM2, VARIABLE_NAME_SIZE2, VARIABLE_VALUE_SIZE2, ENABLE_BACKUP, ENABLE_WEAR_LEVELING);
    if (ret != 0)
        printf("dct_init2 fail\n");
    else
        printf("dct_init2 success\n");

    return ret;
}

int32_t deinitPref(void)
{
    int32_t ret;
    ret = dct_format(DCT_BEGIN_ADDR_MATTER, MODULE_NUM, VARIABLE_NAME_SIZE, VARIABLE_VALUE_SIZE, ENABLE_BACKUP, ENABLE_WEAR_LEVELING);
    if (ret != 0)
        printf("dct_format fail\n");
    else
        printf("dct_format success\n");

    ret = dct_format2(DCT_BEGIN_ADDR_MATTER2, MODULE_NUM2, VARIABLE_NAME_SIZE2, VARIABLE_VALUE_SIZE2, ENABLE_BACKUP, ENABLE_WEAR_LEVELING);
    if (ret != 0)
        printf("dct_format2 fail\n");
    else
        printf("dct_format2 success\n");

    return ret;
}

int32_t registerPref(char * ns)
{
    int32_t ret;
    ret = dct_register_module(ns);
    if (ret != 0)
        printf("dct_register_module %s fail\n",ns);
    else
        printf("dct_register_module %s success\n",ns);

    return ret;
}

int32_t registerPref2(char * ns)
{
    int32_t ret;
    ret = dct_register_module2(ns);
    if (ret != 0)
        printf("dct_register_module2 %s fail\n",ns);
    else
        printf("dct_register_module2 %s success\n",ns);

    return ret;
}


int32_t clearPref(char * ns)
{
    int32_t ret;
    ret = dct_unregister_module(ns);
    if (ret != 0)
        printf("dct_unregister_module %s fail\n",ns);
    else
        printf("dct_unregister_module %s success\n",ns);

    return ret;
}

int32_t deleteKey(char *domain, char *key)
{
    dct_handle_t handle;
    int32_t ret = -1;

	if ( (strcmp(domain,"chip-factory")==0) || (strcmp(domain,"chip-config")==0) || (strcmp(domain,"chip-counters")==0))
	{

	    ret = dct_open_module(&handle, domain);
	    if (DCT_SUCCESS != ret)
	    {
	        printf("%s : dct_open_module(%s) failed\n",__FUNCTION__,key);
	        goto exit;
	    }

	    ret = dct_delete_variable(&handle, key);
	    if(ret == DCT_ERR_NOT_FIND || ret == DCT_SUCCESS)
	        ret = DCT_SUCCESS;
	    else
	        printf("%s : dct_delete_variable(%s) failed\n",__FUNCTION__,key);

	    dct_close_module(&handle);
	}
	else
	{
	    ret = dct_open_module2(&handle, key);
	    if (ret != DCT_SUCCESS){
	        printf("%s : dct_open_module2(%s) failed\n",__FUNCTION__,key);
	        goto exit;
	    }

	    ret = dct_delete_variable(&handle, key);
	    if(ret == DCT_ERR_NOT_FIND || ret == DCT_SUCCESS)
	        ret = DCT_SUCCESS;
	    else
	        printf("%s : dct_delete_variable(%s) failed\n",__FUNCTION__,key);

	    dct_close_module2(&handle);
	    dct_unregister_module2(key);
	}

exit:
    return (DCT_SUCCESS == ret ? 1 : 0);
}

bool checkExist(char *domain, char *key)
{
    dct_handle_t handle;
    int32_t ret = -1;
    uint16_t DataLen = 0;
    uint8_t *str = malloc(sizeof(uint8_t) * VARIABLE_VALUE_SIZE2);

	if ( (strcmp(domain,"chip-factory")==0) || (strcmp(domain,"chip-config")==0) || (strcmp(domain,"chip-counters")==0))
	{
	    ret = dct_open_module(&handle, domain);
	    if (ret != DCT_SUCCESS){
	        printf("%s : dct_open_module(%s) failed\n",__FUNCTION__,domain);
	        goto exit;
	    }

	    ret = dct_get_variable_new(&handle, key, str, &DataLen);

	    if(ret == DCT_ERR_NOT_FIND)
	        printf("%s not found.\n", key);
	    else if(ret == DCT_SUCCESS)
	        printf("%s found.\n", key);

	    dct_close_module(&handle);
	}
	else
	{
	    ret = dct_open_module2(&handle, key);
	    if (ret != DCT_SUCCESS){
	        printf("%s : dct_open_module2(%s) failed\n",__FUNCTION__,key);
	        goto exit;
	    }

	    ret = dct_get_variable_new2(&handle, key, str, &DataLen);

	    if(ret == DCT_ERR_NOT_FIND)
	        printf("%s not found.\n", key);
	    else if(ret == DCT_SUCCESS)
	        printf("%s found.\n", key);

	    dct_close_module2(&handle);
	}

exit:
    free(str);
    return (DCT_SUCCESS == ret ? 1 : 0);
}

int32_t setPref_new(char *domain, char *key, uint8_t *value, size_t byteCount)
{
    dct_handle_t handle;
    int32_t ret = -1;
    uint32_t prefSize;

    //if (byteCount > VARIABLE_VALUE_SIZE-4)
    //{
    //    printf("ERROR : byteCount(%d) large than VARIABLE_VALUE_SIZE(%d)\n", byteCount,VARIABLE_VALUE_SIZE-4);
    //    goto exit;
    //}

	if(byteCount < 64 )
    {
	    ret = registerPref(domain);
	    if (DCT_SUCCESS != ret)
	    {
	        printf("%s : registerPref(%s) failed\n",__FUNCTION__,domain);
	        goto exit;
	    }

	    ret = dct_open_module(&handle, domain);
	    if (DCT_SUCCESS != ret)
	    {
	        printf("%s : dct_open_module(%s) failed\n",__FUNCTION__,domain);
	        goto exit;
	    }

	    ret = dct_set_variable_new(&handle, key, (char *)value, (uint16_t)byteCount);
	    if (DCT_SUCCESS != ret)
	        printf("%s : dct_set_variable(%s) failed\n",__FUNCTION__,key);

	    dct_close_module(&handle);
	}
	else
	{
	    ret = registerPref2(key);
	    if (DCT_SUCCESS != ret)
	    {
	        printf("%s : registerPref2(%s) failed\n",__FUNCTION__,key);
	        goto exit;
	    }

	    ret = dct_open_module2(&handle, key);
	    if (DCT_SUCCESS != ret)
	    {
	        printf("%s : dct_open_module2(%s) failed\n",__FUNCTION__,key);
	        goto exit;
	    }

	    ret = dct_set_variable_new2(&handle, key, (char *)value, (uint16_t)byteCount);
	    if (DCT_SUCCESS != ret)
	        printf("%s : dct_set_variable2(%s) failed\n",__FUNCTION__,key);

	    dct_close_module2(&handle);
	}

exit:
    return (DCT_SUCCESS == ret ? 1 : 0);
}

int32_t getPref_bool_new(char *domain, char *key, uint32_t *val)
{
    dct_handle_t handle;
    int32_t ret = -1;
    uint16_t len = 0;

    ret = dct_open_module(&handle, domain);
    if (DCT_SUCCESS != ret)
    {
        printf("%s : dct_open_module(%s) failed\n",__FUNCTION__,domain);
        goto exit;
    }

    len = sizeof(uint32_t);
    ret = dct_get_variable_new(&handle, key, (char *)val, &len);
    if (DCT_SUCCESS != ret)
        printf("%s : dct_get_variable(%s) failed\n",__FUNCTION__,key);

    dct_close_module(&handle);

exit:
    return (DCT_SUCCESS == ret ? 1 : 0);
}

int32_t getPref_u32_new(char *domain, char *key, uint32_t *val)
{
    dct_handle_t handle;
    int32_t ret = -1;
    uint16_t len = 0;

    ret = dct_open_module(&handle, domain);
    if (DCT_SUCCESS != ret)
    {
        printf("%s : dct_open_module(%s) failed\n",__FUNCTION__,domain);
        goto exit;
    }

    len = sizeof(uint32_t);
    ret = dct_get_variable_new(&handle, key, (char *)val, &len);
    if (DCT_SUCCESS != ret)
        printf("%s : dct_get_variable(%s) failed\n",__FUNCTION__,key);

    dct_close_module(&handle);

exit:
    return (DCT_SUCCESS == ret ? 1 : 0);
}

int32_t getPref_u64_new(char *domain, char *key, uint64_t *val)
{
    dct_handle_t handle;
    int32_t ret = -1;
    uint16_t len = 0;

    ret = dct_open_module(&handle, domain);
    if (DCT_SUCCESS != ret)
    {
        printf("%s : dct_open_module(%s) failed\n",__FUNCTION__,domain);
        goto exit;
    }

    len = sizeof(uint64_t);
    ret = dct_get_variable_new(&handle, key, (char *)val, &len);
    if (DCT_SUCCESS != ret)
        printf("%s : dct_get_variable(%s) failed\n",__FUNCTION__,key);

    dct_close_module(&handle);

exit:
    return (DCT_SUCCESS == ret ? 1 : 0);
}

int32_t getPref_str_new(char *domain, char *key, char * buf, size_t bufSize, size_t *outLen)
{
    dct_handle_t handle;
    int32_t ret = -1;
    uint16_t _bufSize = bufSize;

	if(bufSize < 64)
	{
	    ret = dct_open_module(&handle, domain);
	    if (DCT_SUCCESS != ret)
	    {
	        printf("%s : dct_open_module(%s) failed\n",__FUNCTION__,domain);
	        goto exit;
	    }

	    ret = dct_get_variable_new(&handle, key, buf, &_bufSize);
	    if (DCT_SUCCESS != ret)
	        printf("%s : dct_get_variable(%s) failed\n",__FUNCTION__,key);

	    outLen = bufSize;

	    dct_close_module(&handle);
	}
	else
	{
	    ret = dct_open_module2(&handle, key);
	    if (DCT_SUCCESS != ret)
	    {
	        printf("%s : dct_open_module2(%s) failed\n",__FUNCTION__,key);
	        goto exit;
	    }

	    ret = dct_get_variable_new2(&handle, key, buf, &_bufSize);
	    if (DCT_SUCCESS != ret)
	        printf("%s : dct_get_variable2(%s) failed\n",__FUNCTION__,key);

	    outLen = bufSize;

	    dct_close_module2(&handle);
	}

exit:
    return (DCT_SUCCESS == ret ? 1 : 0);
}

int32_t getPref_bin_new(char *domain, char *key, uint8_t * buf, size_t bufSize, size_t *outLen)
{
    dct_handle_t handle;
    int32_t ret = -1;
    uint16_t _bufSize = bufSize;

	if(bufSize < 64)
	{
	    ret = dct_open_module(&handle, domain);
	    if (DCT_SUCCESS != ret)
	    {
	        printf("%s : dct_open_module(%s) failed\n",__FUNCTION__,domain);
	        goto exit;
	    }

	    ret = dct_get_variable_new(&handle, key, (char *)buf, &_bufSize);
	    if (DCT_SUCCESS != ret)
	        printf("%s : dct_get_variable(%s) failed\n",__FUNCTION__,key);

	    outLen = bufSize;

	    dct_close_module(&handle);
	}
	else
	{
	    ret = dct_open_module2(&handle, key);
	    if (DCT_SUCCESS != ret)
	    {
	        printf("%s : dct_open_module2(%s) failed\n",__FUNCTION__,key);
	        goto exit;
	    }

	    ret = dct_get_variable_new2(&handle, key, (char *)buf, &_bufSize);
	    if (DCT_SUCCESS != ret)
	        printf("%s : dct_get_variable2(%s) failed\n",__FUNCTION__,key);

	    outLen = bufSize;

	    dct_close_module2(&handle);
	}

exit:
    return (DCT_SUCCESS == ret ? 1 : 0);
}

rtw_wifi_setting_t chip_wifi_setting = {0};
int CHIP_SetWiFiConfig(rtw_wifi_setting_t *config)
{
	//printf("%s %d %s %sToDo\r\n", __func__,__LINE__, config->ssid, config->password);
	if(config)
		memcpy(&chip_wifi_setting, config, sizeof(chip_wifi_setting));
	return 0;
}

int CHIP_GetWiFiConfig(rtw_wifi_setting_t *config)
{
	//printf("%s %d %s %sToDo\r\n", __func__,__LINE__, chip_wifi_setting.ssid, chip_wifi_setting.password);
	if(config)
		memcpy(config, &chip_wifi_setting, sizeof(chip_wifi_setting));
	//printf("%s %d %s %sToDo\r\n", __func__,__LINE__, config->ssid, config->password);
	return 0;
}


#ifdef __cplusplus
}
#endif
