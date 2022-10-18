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
#include "chip_porting.h"

#include "mbedtls/aes.h"

#define MICROSECONDS_PER_SECOND    ( 1000000LL )                                   /**< Microseconds per second. */
#define NANOSECONDS_PER_SECOND     ( 1000000000LL )                                /**< Nanoseconds per second. */
#define NANOSECONDS_PER_TICK       ( NANOSECONDS_PER_SECOND / configTICK_RATE_HZ ) /**< Nanoseconds per FreeRTOS tick. */

int FreeRTOS_errno = 0;
#define errno FreeRTOS_errno

bool UTILS_ValidateTimespec( const struct timespec * const pxTimespec );


int UTILS_TimespecToTicks( const struct timespec * const pxTimespec, TickType_t * const pxResult )
{
    int iStatus = 0;
    int64_t llTotalTicks = 0;
    long lNanoseconds = 0;

    /* Check parameters. */
    if( ( pxTimespec == NULL ) || ( pxResult == NULL ) )
    {
        iStatus = EINVAL;
    }
    else if( ( iStatus == 0 ) && ( UTILS_ValidateTimespec( pxTimespec ) == FALSE ) )
    {
        iStatus = EINVAL;
    }

    if( iStatus == 0 )
    {
        /* Convert timespec.tv_sec to ticks. */
        llTotalTicks = ( int64_t ) configTICK_RATE_HZ * ( pxTimespec->tv_sec );

        /* Convert timespec.tv_nsec to ticks. This value does not have to be checked
         * for overflow because a valid timespec has 0 <= tv_nsec < 1000000000 and
         * NANOSECONDS_PER_TICK > 1. */
        lNanoseconds = pxTimespec->tv_nsec / ( long ) NANOSECONDS_PER_TICK +                  /* Whole nanoseconds. */
                       ( long ) ( pxTimespec->tv_nsec % ( long ) NANOSECONDS_PER_TICK != 0 ); /* Add 1 to round up if needed. */

        /* Add the nanoseconds to the total ticks. */
        llTotalTicks += ( int64_t ) lNanoseconds;

        /* Check for overflow */
        if( llTotalTicks < 0 )
        {
            iStatus = EINVAL;
        }
        else
        {
            /* check if TickType_t is 32 bit or 64 bit */
            uint32_t ulTickTypeSize = sizeof( TickType_t );

            /* check for downcast overflow */
            if( ulTickTypeSize == sizeof( uint32_t ) )
            {
                if( llTotalTicks > UINT_MAX )
                {
                    iStatus = EINVAL;
                }
            }
        }

        /* Write result. */
        *pxResult = ( TickType_t ) llTotalTicks;
    }

    return iStatus;
}

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
#define ENABLE_DCT_ENCRYPTION   MATTER_KVS_ENABLE_ENCRYPTION // replace with macro from platform_opts or something

#if ENABLE_DCT_ENCRYPTION
#if defined(MBEDTLS_CIPHER_MODE_CTR)
    mbedtls_aes_context aes;

    // key length 32 bytes for 256 bit encrypting, it can be 16 or 24 bytes for 128 and 192 bits encrypting mode
    unsigned char key[] = {0xff, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0xff, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};

    int dct_encrypt(unsigned char* input_to_encrypt, int input_len, unsigned char* encrypt_output)
    {
        size_t nc_off = 0;
        unsigned char nonce_counter[16] = {0};
        unsigned char stream_block[16] = {0};
        int ret = mbedtls_aes_crypt_ctr(&aes, input_len, &nc_off, nonce_counter, stream_block, input_to_encrypt, encrypt_output);
        // Add return value check
    }

    int dct_decrypt(unsigned char* input_to_decrypt, int input_len, unsigned char* decrypt_output)
    {
        size_t nc_off1 = 0;
        unsigned char nonce_counter1[16] = {0};
        unsigned char stream_block1[16] = {0};
        int ret = mbedtls_aes_crypt_ctr(&aes, input_len, &nc_off1, nonce_counter1, stream_block1, input_to_decrypt, decrypt_output);
        // Add return value check
    }

#else
#error "MBEDTLS_CIPHER_MODE_CTR must be enabled to perform DCT flash encryption" 
#endif // MBEDTLS_CIPHER_MODE_CTR
#endif // ENABLE_DCT_ENCRYPTION

const char *matter_domain[19] =
{
    "chip-factory",
    "chip-config",
    "chip-counters",
    "chip-fabric-1",
    "chip-fabric-2",
    "chip-fabric-3",
    "chip-fabric-4",
    "chip-fabric-5",
    "chip-acl",
    "chip-groupmsgcounters",
    "chip-attributes",
    "chip-bindingtable",
    "chip-ota",
    "chip-failsafe",
    "chip-sessionresumption",
    "chip-deviceinfoprovider",
    "chip-groupdataprovider",
    "chip-others2",
    "chip-others"
};

/*
 * allocate into predefined domains
 * needs to be more robust, but currently it gets the job done
 * chip-groupdataproviders is put into their respective chip-fabric-x
 */

/*
 * domainAllocator allocates key-value data in domain 1
 */
const char* domainAllocator(const char *domain)
{
    //chip-factory
    if(strcmp(domain, "chip-factory") == 0)
        return matter_domain[0];
    //chip-config
    if(strcmp(domain, "chip-config") == 0)
        return matter_domain[1];
    //chip-counters
    if(strcmp(domain, "chip-counters") == 0)
        return matter_domain[2];
    // chip-acl
    if(strncmp(domain+4, "ac", 2) == 0)
    {
        // acl extension
        if(strncmp(domain+7, "1", 1) == 0)
            return matter_domain[17];

        return matter_domain[8];
    }

    if(domain[0] == 'f')
    {
        // chip-groupdataprovider
        if(strncmp(domain+4, "g", 1) == 0) 
            return matter_domain[16];

        // chip-fabrics
        switch(atoi(&domain[2]))
        {
            case 1:
                return matter_domain[3];
                break;
            case 2:
                return matter_domain[4];
                break;
            case 3:
                return matter_domain[5];
                break;
            case 4:
                return matter_domain[6];
                break;
            case 5:
                return matter_domain[7];
                break;
        }
    }
    // chip-groupmsgcounters
    if((strcmp(domain, "g/gdc") == 0) || (strcmp(domain, "g/gcc") == 0))
        return matter_domain[9];
    // chip-attributes
    if(strncmp(domain, "g/a", 3) == 0)
        return matter_domain[10];
    // chip-bindingtable
    if(strncmp(domain, "g/bt", 4) == 0)
        return matter_domain[11];
    // chip-ota
    if(strncmp(domain, "g/o", 3) == 0)
        return matter_domain[12];
    // chip-failsafe
    if(strncmp(domain, "g/fs", 4) == 0)
        return matter_domain[13];
    // chip-sessionresumption
    if((strncmp(domain, "g/s", 3) == 0) && strcmp(domain, "g/sri") != 0)
        return matter_domain[14];
    // chip-deviceinfoprovider
    if(strncmp(domain, "g/userlbl", 9) == 0)
        return matter_domain[15];
    // chip-others2
    // Store KV pairs that can't fit in chip-others (>64bytes)
    if((strcmp(domain, "wifi-pass") == 0) || (strcmp(domain, "g/sri") == 0))
        return matter_domain[17];
    // chip-others
    // store FabricTable, FailSafeContextKey, GroupFabricList, FabricIndexInfo, IMEventNumber in chip-others
    return matter_domain[18];
}

/*
 * Assigns DCT region to an allocated domain
 * retval:
 *      1 => DCT1 region
 *      2 => DCT2 region
 */
uint8_t allocateRegion(const char *allocatedDomain)
{
    if (strncmp(allocatedDomain, "chip-fabric", 11) == 0)
        return 2;
    if (strcmp(allocatedDomain, "chip-others2") == 0)
        return 2;
    else
        return 1;
}

int32_t initPref(void)
{
    int32_t ret;
    ret = dct_init(DCT_BEGIN_ADDR_MATTER, MODULE_NUM, VARIABLE_NAME_SIZE, VARIABLE_VALUE_SIZE, ENABLE_BACKUP, ENABLE_WEAR_LEVELING);
    if (ret != 0)
        printf("dct_init failed with error: %d\n", ret);
    else
        printf("dct_init success\n");

    ret = dct_init2(DCT_BEGIN_ADDR_MATTER2, MODULE_NUM2, VARIABLE_NAME_SIZE2, VARIABLE_VALUE_SIZE2, ENABLE_BACKUP, ENABLE_WEAR_LEVELING);
    if (ret != 0)
        printf("dct_init2 failed with error: %d\n", ret);
    else
        printf("dct_init2 success\n");

#if ENABLE_DCT_ENCRYPTION
    // Initialize mbedtls aes context and set encryption key
    mbedtls_aes_init(&aes);
    mbedtls_aes_setkey_enc(&aes, key, 256);
#endif

exit:
    return ret;
}

int32_t deinitPref(void)
{
    int32_t ret;
    ret = dct_format(DCT_BEGIN_ADDR_MATTER, MODULE_NUM, VARIABLE_NAME_SIZE, VARIABLE_VALUE_SIZE, ENABLE_BACKUP, ENABLE_WEAR_LEVELING);
    if (ret != 0)
        printf("dct_format failed with error: %d\n", ret);
    else
        printf("dct_format success\n");

    ret = dct_format2(DCT_BEGIN_ADDR_MATTER2, MODULE_NUM2, VARIABLE_NAME_SIZE2, VARIABLE_VALUE_SIZE2, ENABLE_BACKUP, ENABLE_WEAR_LEVELING);
    if (ret != 0)
        printf("dct_format2 failed with error: %d\n", ret);
    else
        printf("dct_format2 success\n");

#if ENABLE_DCT_ENCRYPTION
    // free aes context
    mbedtls_aes_free(&aes);
#endif

    return ret;
}

int32_t registerPref(const char * ns)
{
    int32_t ret;
    ret = dct_register_module(ns);
    if (ret != 0)
        printf("dct_register_module %s failed with error: %d\n",ns, ret);
    else
        printf("dct_register_module %s success\n",ns);

    return ret;
}

int32_t registerPref2(const char * ns)
{
    int32_t ret;
    ret = dct_register_module2(ns);
    if (ret != 0)
        printf("dct_register_module2 %s failed with error: %d\n",ns, ret);
    else
        printf("dct_register_module2 %s success\n",ns);

    return ret;
}


int32_t clearPref(const char * ns)
{
    int32_t ret;
    ret = dct_unregister_module(ns);
    if (ret != 0)
        printf("dct_unregister_module %s failed with error: %d\n",ns, ret);
    else
        printf("dct_unregister_module %s success\n",ns);

    return ret;
}

int32_t deleteKey(const char *domain, const char *key)
{
    dct_handle_t handle;
    int32_t ret = -1;
    const char *allocatedDomain = domainAllocator(domain);
    uint8_t allocatedRegion = allocateRegion(allocatedDomain);

    if (allocatedRegion == 1)
    {
        ret = dct_open_module(&handle, allocatedDomain);
        if (DCT_SUCCESS != ret)
        {
            printf("%s : dct_open_module(%s) failed with error: %d\n" ,__FUNCTION__, allocatedDomain, ret);
            goto exit;
        }

        ret = dct_delete_variable(&handle, key);
        dct_close_module(&handle);
        if(DCT_SUCCESS != ret)
        {
            printf("%s : dct_delete_variable(%s) failed with error: %d\n" ,__FUNCTION__, key, ret);

            if (DCT_ERR_NOT_FIND == ret) // Only enter here if variable is not found in region1
            {
                // Now we search for this variable in chip-others2
                allocatedDomain = matter_domain[17];

                ret = dct_open_module2(&handle, allocatedDomain);
                if (DCT_SUCCESS != ret)
                {
                    printf("%s : dct_open_module2(%s) failed with error: %d\n" ,__FUNCTION__, allocatedDomain, ret);
                    goto exit;
                }

                ret = dct_delete_variable2(&handle, key);
                if(DCT_SUCCESS != ret)
                    printf("%s : dct_delete_variable(%s) failed with error: %d\n" ,__FUNCTION__, key, ret);

                dct_close_module2(&handle);
            }
        }

    }
    else
    {
        ret = dct_open_module2(&handle, allocatedDomain);
        if (DCT_SUCCESS != ret){
            printf("%s : dct_open_module2(%s) failed with error: %d\n" ,__FUNCTION__, allocatedDomain, ret);
            goto exit;
        }

        ret = dct_delete_variable2(&handle, key);
        if(DCT_SUCCESS != ret)
            printf("%s : dct_delete_variable(%s) failed with error: %d\n" ,__FUNCTION__, key, ret);

        dct_close_module2(&handle);
        //dct_unregister_module2(key);
    }

exit:
    return ret;
}

bool checkExist(const char *domain, const char *key)
{
    dct_handle_t handle;
    int32_t ret = -1;
    uint16_t len = 0;
    uint8_t found = 0;
    uint8_t *str = malloc(sizeof(uint8_t) * VARIABLE_VALUE_SIZE-4);
    const char *allocatedDomain = domainAllocator(domain);
    uint8_t allocatedRegion = allocateRegion(allocatedDomain);

    if (allocatedRegion == 1)
    {
        ret = dct_open_module(&handle, allocatedDomain);
        if (ret != DCT_SUCCESS){
            printf("%s : dct_open_module(%s) failed with error: %d\n" ,__FUNCTION__, allocatedDomain, ret);
            goto exit;
        }

        if(found == 0)
        {
#if ENABLE_DCT_ENCRYPTION
            len = 68;
#else
            len = 64;
#endif
            ret = dct_get_variable_new(&handle, key, (char *)str, &len);
            if(ret == DCT_SUCCESS)
            {
                printf("checkExist key=%s found.\n", key);
                found = 1;
            }
        }

        if(found == 0)
        {
#if ENABLE_DCT_ENCRYPTION
            len = 68;
#else
            len = 64;
#endif
            ret = dct_get_variable_new(&handle, key, (char *)str, &len);
            if(ret == DCT_SUCCESS)
            {
                printf("checkExist key=%s found.\n", key);
                found = 1;
            }
        }

        dct_close_module(&handle);

        // Find in chip-others2 if not found
        if(found == 0)
        {
            allocatedDomain = matter_domain[17];
            ret = dct_open_module2(&handle, allocatedDomain);
            if (ret != DCT_SUCCESS){
                printf("%s : dct_open_module2(%s) failed with error : %d\n" ,__FUNCTION__, allocatedDomain, ret);
                goto exit;
            }

#if ENABLE_DCT_ENCRYPTION
            len = 404;
#else
            len = 400;
#endif
            ret = dct_get_variable_new2(&handle, key, str, &len);
            if(ret == DCT_SUCCESS)
            {
                printf("checkExist key=%s found.\n", key);
                found = 1;
            }

            dct_close_module2(&handle);
        }
    }
    else
    {
        ret = dct_open_module2(&handle, allocatedDomain);
        if (ret != DCT_SUCCESS){
            printf("%s : dct_open_module2(%s) failed with error : %d\n" ,__FUNCTION__, allocatedDomain, ret);
            goto exit;
        }

#if ENABLE_DCT_ENCRYPTION
        len = 404;
#else
        len = 400;
#endif
        ret = dct_get_variable_new2(&handle, key, str, &len);
        if(ret == DCT_SUCCESS)
        {
            printf("checkExist key=%s found.\n", key);
            found = 1;
        }

        dct_close_module2(&handle);
    }

    if(found == 0)
        printf("checkExist key=%s not found. ret=%d\n",key ,ret);

exit:
    free(str);
    return found;
}

int32_t setPref_new(const char *domain, const char *key, uint8_t *value, size_t byteCount)
{
    dct_handle_t handle;
    int32_t ret = -1;
    const char *allocatedDomain;
    uint8_t allocatedRegion;
    allocatedDomain = domainAllocator(domain);
    allocatedRegion = allocateRegion(allocatedDomain);

#if ENABLE_DCT_ENCRYPTION
    uint8_t data_to_encrypt[404] = {0};
    uint8_t encrypted_data[404] = {0};

    memcpy(data_to_encrypt, &byteCount, sizeof(size_t));
    memcpy(data_to_encrypt + sizeof(size_t), value, byteCount);
#endif

    if (allocatedRegion == 1)
    {
        if (byteCount > 64) // This is a **new unknown key** that should be stored in chip-others2 inside region2
        {
            allocatedDomain = matter_domain[17];

            ret = dct_open_module2(&handle, allocatedDomain);
            if (DCT_SUCCESS != ret)
            {
                printf("%s : dct_open_module2(%s) failed with error: %d\n" ,__FUNCTION__, allocatedDomain, ret);
                goto exit;
            }

#if ENABLE_DCT_ENCRYPTION
            dct_encrypt(data_to_encrypt, 404, encrypted_data);
            ret = dct_set_variable_new2(&handle, key, (char *)encrypted_data, 404);
#else
            ret = dct_set_variable_new2(&handle, key, (char *)value, (uint16_t)byteCount);
#endif
            if (DCT_SUCCESS != ret)
                printf("%s : dct_set_variable2(%s) failed with error: %d\n" ,__FUNCTION__, key, ret);

            dct_close_module2(&handle);
        }
        else
        {
            ret = dct_open_module(&handle, allocatedDomain);
            if (DCT_SUCCESS != ret)
            {
                printf("%s : dct_open_module(%s) failed with error: %d\n" ,__FUNCTION__, allocatedDomain, ret);
                goto exit;
            }

#if ENABLE_DCT_ENCRYPTION
            printf("data_to_encrypt after addr: %p\n", data_to_encrypt);
            dct_encrypt(data_to_encrypt, 68, encrypted_data);
            ret = dct_set_variable_new(&handle, key, (char *)encrypted_data, 68);
#else
            ret = dct_set_variable_new(&handle, key, (char *)value, (uint16_t)byteCount);
#endif
            if (DCT_SUCCESS != ret)
                printf("%s : dct_set_variable(%s) failed with error: %d\n" ,__FUNCTION__, key, ret);

            dct_close_module(&handle);
        }
    }
    else
    {
        ret = dct_open_module2(&handle, allocatedDomain);
        if (DCT_SUCCESS != ret)
        {
            printf("%s : dct_open_module2(%s) failed with error: %d\n" ,__FUNCTION__, allocatedDomain, ret);
            goto exit;
        }

#if ENABLE_DCT_ENCRYPTION
        dct_encrypt(data_to_encrypt, 404, encrypted_data);
        ret = dct_set_variable_new2(&handle, key, (char *)encrypted_data, 404);
#else
        ret = dct_set_variable_new2(&handle, key, (char *)value, (uint16_t)byteCount);
#endif
        if (DCT_SUCCESS != ret)
            printf("%s : dct_set_variable2(%s) failed with error: %d\n" ,__FUNCTION__, key, ret);

        dct_close_module2(&handle);
    }

exit:
    return (DCT_SUCCESS == ret ? 1 : 0);
}

int32_t getPref_bool_new(const char *domain, const char *key, uint8_t *val)
{
    dct_handle_t handle;
    int32_t ret = -1;
    uint16_t len = 0;
    const char *allocatedDomain = domainAllocator(domain);
    uint8_t allocatedRegion = allocateRegion(allocatedDomain);
#if ENABLE_DCT_ENCRYPTION
    uint8_t encrypted_data[404] = {0};
    uint8_t decrypted_data[404] = {0};
#endif

    if (allocatedRegion == 1)
    {
        ret = dct_open_module(&handle, allocatedDomain);
        if (DCT_SUCCESS != ret)
        {
            printf("%s : dct_open_module(%s) failed with error: %d\n" ,__FUNCTION__, allocatedDomain, ret);
            goto exit;
        }

#if ENABLE_DCT_ENCRYPTION
        len = 68;
        memset(encrypted_data, 0, len);
        ret = dct_get_variable_new(&handle, key, (char *)encrypted_data, &len);
#else
        len = sizeof(uint8_t);
        ret = dct_get_variable_new(&handle, key, (char *)val, &len);
#endif
        dct_close_module(&handle);

        if (DCT_SUCCESS != ret)
        {
            printf("%s : dct_get_variable(%s) failed with error: %d\n" ,__FUNCTION__, key, ret);

            if (DCT_ERR_NOT_FIND == ret) // Only enter here if variable is not found in region1
            {
                // Now we search for this variable in chip-others2
                allocatedDomain = matter_domain[17];

                ret = dct_open_module2(&handle, allocatedDomain);
                if (DCT_SUCCESS != ret)
                {
                    printf("%s : dct_open_module2(%s) failed with error: %d\n" ,__FUNCTION__, allocatedDomain, ret);
                    goto exit;
                }

#if ENABLE_DCT_ENCRYPTION
                len = 404;
                memset(encrypted_data, 0, len);
                ret = dct_get_variable_new2(&handle, key, (char *)encrypted_data, &len);
#else
                len = sizeof(uint8_t);
                ret = dct_get_variable_new2(&handle, key, (char *)val, &len);
#endif
                if (DCT_SUCCESS != ret)
                {
                    printf("%s : dct_get_variable2(%s) failed with error: %d\n" ,__FUNCTION__, key, ret);
                    dct_close_module2(&handle);
                    goto exit;
                }
                else
                    allocatedRegion = 2; // Variable is found in region2

                dct_close_module2(&handle);
            }
        }
    }
    else
    {
        ret = dct_open_module2(&handle, allocatedDomain);
        if (DCT_SUCCESS != ret)
        {
            printf("%s : dct_open_module2(%s) failed with error: %d\n" ,__FUNCTION__, allocatedDomain, ret);
            goto exit;
        }

#if ENABLE_DCT_ENCRYPTION
        len = 404;
        memset(encrypted_data, 0, len);
        ret = dct_get_variable_new2(&handle, key, (char *)encrypted_data, &len);
#else
        len = sizeof(uint8_t);
        ret = dct_get_variable_new2(&handle, key, (char *)val, &len);
#endif
        if (DCT_SUCCESS != ret)
        {
            printf("%s : dct_get_variable2(%s) failed with error: %d\n" ,__FUNCTION__, key, ret);
            dct_close_module2(&handle);
            goto exit;
        }

        dct_close_module2(&handle);
    }

#if ENABLE_DCT_ENCRYPTION
    if (DCT_SUCCESS == ret)
    {
        size_t decrypt_len;
        if (allocatedRegion == 1)
            decrypt_len = 68;
        else
            decrypt_len = 404;
        dct_decrypt(encrypted_data, decrypt_len, decrypted_data);

        size_t val_len = 0;
        memset(val, 0, sizeof(uint8_t));
        memcpy(&val_len, decrypted_data, sizeof(size_t));
        memcpy(val, decrypted_data + sizeof(size_t), val_len);
    }
#endif

exit:
    return ret;
}

int32_t getPref_u32_new(const char *domain, const char *key, uint32_t *val)
{
    dct_handle_t handle;
    int32_t ret = -1;
    uint16_t len = 0;
    const char *allocatedDomain = domainAllocator(domain);
    uint8_t allocatedRegion = allocateRegion(allocatedDomain);
#if ENABLE_DCT_ENCRYPTION
    uint8_t encrypted_data[404] = {0};
    uint8_t decrypted_data[404] = {0};
#endif

    if (allocatedRegion == 1)
    {
        ret = dct_open_module(&handle, allocatedDomain);
        if (DCT_SUCCESS != ret)
        {
            printf("%s : dct_open_module(%s) failed with error: %d\n" ,__FUNCTION__, allocatedDomain, ret);
            goto exit;
        }

#if ENABLE_DCT_ENCRYPTION
        len = 68;
        memset(encrypted_data, 0, len);
        ret = dct_get_variable_new(&handle, key, (char *)encrypted_data, &len);
#else
        len = sizeof(uint32_t);
        ret = dct_get_variable_new(&handle, key, (char *)val, &len);
#endif
        dct_close_module(&handle);

        if (DCT_SUCCESS != ret)
        {
            printf("%s : dct_get_variable(%s) failed with error: %d\n" ,__FUNCTION__, key, ret);

            if (DCT_ERR_NOT_FIND == ret) // Only enter here if variable is not found in region1
            {
                // Now we search for this variable in chip-others2
                allocatedDomain = matter_domain[17];

                ret = dct_open_module2(&handle, allocatedDomain);
                if (DCT_SUCCESS != ret)
                {
                    printf("%s : dct_open_module2(%s) failed with error: %d\n" ,__FUNCTION__, allocatedDomain, ret);
                    goto exit;
                }

#if ENABLE_DCT_ENCRYPTION
                len = 404;
                memset(encrypted_data, 0, len);
                ret = dct_get_variable_new2(&handle, key, (char *)encrypted_data, &len);
#else
                len = sizeof(uint32_t);
                ret = dct_get_variable_new2(&handle, key, (char *)val, &len);
#endif
                if (DCT_SUCCESS != ret)
                {
                    printf("%s : dct_get_variable2(%s) failed with error: %d\n" ,__FUNCTION__, key, ret);
                    dct_close_module2(&handle);
                    goto exit;
                }
                else
                    allocatedRegion = 2; // Variable is found in region2

                dct_close_module2(&handle);
            }
        }
    }
    else
    {
        ret = dct_open_module2(&handle, allocatedDomain);
        if (DCT_SUCCESS != ret)
        {
            printf("%s : dct_open_module2(%s) failed with error: %d\n" ,__FUNCTION__, allocatedDomain, ret);
            goto exit;
        }

#if ENABLE_DCT_ENCRYPTION
        len = 404;
        memset(encrypted_data, 0, len);
        ret = dct_get_variable_new2(&handle, key, (char *)encrypted_data, &len);
#else
        len = sizeof(uint32_t);
        ret = dct_get_variable_new2(&handle, key, (char *)val, &len);
#endif
        if (DCT_SUCCESS != ret)
        {
            printf("%s : dct_get_variable2(%s) failed with error: %d\n" ,__FUNCTION__, key, ret);
            dct_close_module2(&handle);
            goto exit;
        }

        dct_close_module2(&handle);
    }

#if ENABLE_DCT_ENCRYPTION
    if (DCT_SUCCESS == ret)
    {
        size_t decrypt_len;
        if (allocatedRegion == 1)
            decrypt_len = 68;
        else
            decrypt_len = 404;
        dct_decrypt(encrypted_data, decrypt_len, decrypted_data);
        size_t val_len = 0;
        memset(val, 0, sizeof(uint32_t));
        memcpy(&val_len, decrypted_data, sizeof(size_t));
        memcpy(val, decrypted_data + sizeof(size_t), val_len);
    }
#endif

exit:
    return ret;
}

int32_t getPref_u64_new(const char *domain, const char *key, uint64_t *val)
{
    dct_handle_t handle;
    int32_t ret = -1;
    uint16_t len = 0;
    const char *allocatedDomain = domainAllocator(domain);
    uint8_t allocatedRegion = allocateRegion(allocatedDomain);
#if ENABLE_DCT_ENCRYPTION
    uint8_t encrypted_data[404] = {0};
    uint8_t decrypted_data[404] = {0};
#endif

    if (allocatedRegion == 1)
    {
        ret = dct_open_module(&handle, allocatedDomain);
        if (DCT_SUCCESS != ret)
        {
            printf("%s : dct_open_module(%s) failed with error: %d\n" ,__FUNCTION__, allocatedDomain, ret);
            goto exit;
        }

#if ENABLE_DCT_ENCRYPTION
        len = 68;
        memset(encrypted_data, 0, len);
        ret = dct_get_variable_new(&handle, key, (char *)encrypted_data, &len);
#else
        len = sizeof(uint64_t);
        ret = dct_get_variable_new(&handle, key, (char *)val, &len);
#endif
        dct_close_module(&handle);

        if (DCT_SUCCESS != ret)
        {
            printf("%s : dct_get_variable(%s) failed with error: %d\n" ,__FUNCTION__, key, ret);

            if (DCT_ERR_NOT_FIND == ret) // Only enter here if variable is not found in region1
            {
                // Now we search for this variable in chip-others2
                allocatedDomain = matter_domain[17];

                ret = dct_open_module2(&handle, allocatedDomain);
                if (DCT_SUCCESS != ret)
                {
                    printf("%s : dct_open_module2(%s) failed with error: %d\n" ,__FUNCTION__, allocatedDomain, ret);
                    goto exit;
                }

#if ENABLE_DCT_ENCRYPTION
                len = 404;
                memset(encrypted_data, 0, len);
                ret = dct_get_variable_new2(&handle, key, (char *)encrypted_data, &len);
#else
                len = sizeof(uint64_t);
                ret = dct_get_variable_new2(&handle, key, (char *)val, &len);
#endif
                if (DCT_SUCCESS != ret)
                {
                    printf("%s : dct_get_variable2(%s) failed with error: %d\n" ,__FUNCTION__, key, ret);
                    dct_close_module2(&handle);
                    goto exit;
                }
                else
                    allocatedRegion = 2; // Variable is found in region2

                dct_close_module2(&handle);
            }
        }
    }
    else
    {
        ret = dct_open_module2(&handle, allocatedDomain);
        if (DCT_SUCCESS != ret)
        {
            printf("%s : dct_open_module2(%s) failed with error: %d\n" ,__FUNCTION__, allocatedDomain, ret);
            goto exit;
        }

#if ENABLE_DCT_ENCRYPTION
        len = 404;
        memset(encrypted_data, 0, len);
        ret = dct_get_variable_new2(&handle, key, (char *)encrypted_data, &len);
#else
        len = sizeof(uint64_t);
        ret = dct_get_variable_new2(&handle, key, (char *)val, &len);
#endif
        if (DCT_SUCCESS != ret)
        {
            printf("%s : dct_get_variable2(%s) failed with error: %d\n" ,__FUNCTION__, key, ret);
            dct_close_module2(&handle);
            goto exit;
        }

        dct_close_module2(&handle);
    }

#if ENABLE_DCT_ENCRYPTION
    if (DCT_SUCCESS == ret)
    {
        size_t decrypt_len;
        if (allocatedRegion == 1)
            decrypt_len = 68;
        else
            decrypt_len = 404;
        dct_decrypt(encrypted_data, decrypt_len, decrypted_data);

        size_t val_len = 0;
        memset(val, 0, sizeof(uint64_t));
        memcpy(&val_len, decrypted_data, sizeof(size_t));
        memcpy(val, decrypted_data + sizeof(size_t), val_len);
    }
#endif

exit:
    return ret;
}

int32_t getPref_str_new(const char *domain, const char *key, char * buf, size_t bufSize, size_t *outLen)
{
    dct_handle_t handle;
    int32_t ret = -1;
    uint16_t _bufSize = bufSize;
    const char *allocatedDomain = domainAllocator(domain);
    uint8_t allocatedRegion = allocateRegion(allocatedDomain);
#if ENABLE_DCT_ENCRYPTION
    uint8_t encrypted_data[404] = {0};
    uint8_t decrypted_data[404] = {0};
#endif

    if (allocatedRegion == 1)
    {
        ret = dct_open_module(&handle, allocatedDomain);
        if (DCT_SUCCESS != ret)
        {
            printf("%s : dct_open_module(%s) failed with error: %d\n",__FUNCTION__, allocatedDomain, ret);
            goto exit;
        }

#if ENABLE_DCT_ENCRYPTION
        _bufSize = 68;
        memset(encrypted_data, 0, _bufSize);
        ret = dct_get_variable_new(&handle, key, encrypted_data, &_bufSize);
#else
        ret = dct_get_variable_new(&handle, key, buf, &_bufSize);
#endif
        dct_close_module(&handle);

        if (DCT_SUCCESS != ret)
        {
            printf("%s : dct_get_variable(%s) failed with error: %d\n",__FUNCTION__, key, ret);
#if !ENABLE_DCT_ENCRYPTION
            *outLen = _bufSize;
#endif

            if (DCT_ERR_NOT_FIND == ret) // Only enter here if variable is not found in region1
            {
                // Now we search for this variable in chip-others2
                allocatedDomain = matter_domain[17];

                ret = dct_open_module2(&handle, allocatedDomain);
                if (DCT_SUCCESS != ret)
                {
                    printf("%s : dct_open_module2(%s) failed with error: %d\n" ,__FUNCTION__, allocatedDomain, ret);
                    goto exit;
                }

#if ENABLE_DCT_ENCRYPTION
                _bufSize = 404;
                memset(encrypted_data, 0, _bufSize);
                ret = dct_get_variable_new2(&handle, key, encrypted_data, &_bufSize);
#else
                ret = dct_get_variable_new2(&handle, key, buf, &_bufSize);
#endif
                if (DCT_SUCCESS != ret)
                {
                    printf("%s : dct_get_variable2(%s) failed with error: %d\n" ,__FUNCTION__, key, ret);
                    dct_close_module2(&handle);
                    *outLen = bufSize;
                    goto exit;
                }

                allocatedRegion = 2; // Variable is found in region2
#if !ENABLE_DCT_ENCRYPTION
                *outLen = _bufSize;
#endif
                dct_close_module2(&handle);
            }
        }
#if !ENABLE_DCT_ENCRYPTION
        else
            *outLen = _bufSize;
#endif
    }
    else
    {
        ret = dct_open_module2(&handle, allocatedDomain);
        if (DCT_SUCCESS != ret)
        {
            printf("%s : dct_open_module2(%s) failed with error: %d\n" ,__FUNCTION__, allocatedDomain, ret);
            goto exit;
        }

#if ENABLE_DCT_ENCRYPTION
        _bufSize = 404;
        memset(encrypted_data, 0, _bufSize);
        ret = dct_get_variable_new2(&handle, key, (char *)encrypted_data, &_bufSize);
#else
        ret = dct_get_variable_new2(&handle, key, buf, &_bufSize);
#endif
        if (DCT_SUCCESS != ret)
        {
            printf("%s : dct_get_variable2(%s) failed with error: %d\n" ,__FUNCTION__, key, ret);
            dct_close_module2(&handle);
            *outLen = bufSize;
            goto exit;
        }

#if !ENABLE_DCT_ENCRYPTION
        *outLen = _bufSize;
#endif

        dct_close_module2(&handle);
    }

#if ENABLE_DCT_ENCRYPTION
    if (DCT_SUCCESS == ret)
    {
        size_t decrypt_len;
        if (allocatedRegion == 1)
            decrypt_len = 68;
        else
            decrypt_len = 404;
        dct_decrypt(encrypted_data, decrypt_len, decrypted_data);

        size_t val_len = 0;
        memset(buf, 0, bufSize);
        memcpy(&val_len, decrypted_data, sizeof(size_t));

        // Buffer provided is too small, copy data up to provided buffer size only
        if (bufSize < val_len)
        {
            memcpy(buf, decrypted_data + sizeof(size_t), bufSize);
            ret = -8; // buffer too small
            *outLen = bufSize;
        }
        // Copy data of length val_len as per normal
        else
        {
            memcpy(buf, decrypted_data + sizeof(size_t), val_len);
            *outLen = val_len;
        }
    }
#endif

exit:
    return ret;
}

int32_t getPref_bin_new(const char *domain, const char *key, uint8_t * buf, size_t bufSize, size_t *outLen)
{
    dct_handle_t handle;
    int32_t ret = -1;
    uint16_t _bufSize = bufSize;
    const char *allocatedDomain = domainAllocator(domain);
    uint8_t allocatedRegion = allocateRegion(allocatedDomain);
#if ENABLE_DCT_ENCRYPTION
    uint8_t encrypted_data[404] = {0};
    uint8_t decrypted_data[404] = {0};
#endif

    if (allocatedRegion == 1)
    {
        ret = dct_open_module(&handle, allocatedDomain);
        if (DCT_SUCCESS != ret)
        {
            printf("%s : dct_open_module(%s) failed with error: %d\n" ,__FUNCTION__, allocatedDomain, ret);
            goto exit;
        }

#if ENABLE_DCT_ENCRYPTION
        _bufSize = 68;
        memset(encrypted_data, 0, _bufSize);
        ret = dct_get_variable_new(&handle, key, (char *)encrypted_data, &_bufSize);
#else
        ret = dct_get_variable_new(&handle, key, (char *)buf, &_bufSize);
#endif
        dct_close_module(&handle);

        if (DCT_SUCCESS != ret)
        {
            printf("%s : dct_get_variable(%s) failed with error: %d\n" ,__FUNCTION__, key, ret);
#if !ENABLE_DCT_ENCRYPTION
            *outLen = _bufSize;
#endif

            if (DCT_ERR_NOT_FIND == ret) // Only enter here if variable is not found in region1
            {
                // Now we search for this variable in chip-others2
                allocatedDomain = matter_domain[17];

                ret = dct_open_module2(&handle, allocatedDomain);
                if (DCT_SUCCESS != ret)
                {
                    printf("%s : dct_open_module2(%s) failed with error: %d\n" ,__FUNCTION__, allocatedDomain, ret);
                    goto exit;
                }

#if ENABLE_DCT_ENCRYPTION
                _bufSize = 404;
                memset(encrypted_data, 0, _bufSize);
                ret = dct_get_variable_new2(&handle, key, encrypted_data, &_bufSize);
#else
                ret = dct_get_variable_new2(&handle, key, buf, &_bufSize);
#endif
                if (DCT_SUCCESS != ret)
                {
                    printf("%s : dct_get_variable2(%s) failed with error: %d\n" ,__FUNCTION__, key, ret);
                    dct_close_module2(&handle);
                    *outLen = bufSize;
                    goto exit;
                }

                allocatedRegion = 2; // Variable is found in region2
#if !ENABLE_DCT_ENCRYPTION
                *outLen = _bufSize;
#endif

                dct_close_module2(&handle);
            }
        }
#if !ENABLE_DCT_ENCRYPTION
        else
            *outLen = _bufSize;
#endif
    }
    else
    {
        ret = dct_open_module2(&handle, allocatedDomain);
        if (DCT_SUCCESS != ret)
        {
            printf("%s : dct_open_module2(%s) failed with error: %d\n" ,__FUNCTION__, allocatedDomain, ret);
            goto exit;
        }

#if ENABLE_DCT_ENCRYPTION
        _bufSize = 404;
        memset(encrypted_data, 0, _bufSize);
        ret = dct_get_variable_new2(&handle, key, (char *)encrypted_data, &_bufSize);
#else
        ret = dct_get_variable_new2(&handle, key, (char *)buf, &_bufSize);
#endif
        if (DCT_SUCCESS != ret)
        {
            printf("%s : dct_get_variable2(%s) failed with error: %d\n" ,__FUNCTION__, key, ret);
            dct_close_module2(&handle);
            *outLen = bufSize;
            goto exit;
        }

#if !ENABLE_DCT_ENCRYPTION
        *outLen = _bufSize;
#endif

        dct_close_module2(&handle);
    }

#if ENABLE_DCT_ENCRYPTION
    if (DCT_SUCCESS == ret)
    {
        size_t decrypt_len;
        if (allocatedRegion == 1)
            decrypt_len = 68;
        else
            decrypt_len = 404;
        dct_decrypt(encrypted_data, decrypt_len, decrypted_data);

        size_t val_len = 0;
        memset(buf, 0, bufSize);
        memcpy(&val_len, decrypted_data, sizeof(size_t));

        // Buffer provided is too small, copy data up to provided buffer size only
        if (bufSize < val_len)
        {
            memcpy(buf, decrypted_data + sizeof(size_t), bufSize);
            ret = -8; // buffer too small
            *outLen = bufSize;
        }
        // Copy data of length val_len as per normal
        else
        {
            memcpy(buf, decrypted_data + sizeof(size_t), val_len);
            *outLen = val_len;
        }
    }
#endif

exit:
    return ret;
}

/************************** Matter WiFi Related ********************************/
uint32_t apNum = 0; // no of total AP scanned
static rtw_scan_result_t matter_userdata[65] = {0};
static char *matter_ssid;

chip_connmgr_callback chip_connmgr_callback_func = NULL;
void *chip_connmgr_callback_data = NULL;
void chip_connmgr_set_callback_func(chip_connmgr_callback p, void *data)
{
    chip_connmgr_callback_func = p;
    chip_connmgr_callback_data = data;
}

void print_matter_scan_result( rtw_scan_result_t* record )
{
    RTW_API_INFO("%s\t ", ( record->bss_type == RTW_BSS_TYPE_ADHOC ) ? "Adhoc" : "Infra");
    RTW_API_INFO(MAC_FMT, MAC_ARG(record->BSSID.octet));
    RTW_API_INFO(" %d\t ", record->signal_strength);
    RTW_API_INFO(" %d\t  ", record->channel);
    RTW_API_INFO(" %d\t  ", record->wps_type);
    RTW_API_INFO("%s\t\t ", ( record->security == RTW_SECURITY_OPEN ) ? "Open" :
                                 ( record->security == RTW_SECURITY_WEP_PSK ) ? "WEP" :
                                 ( record->security == RTW_SECURITY_WPA_TKIP_PSK ) ? "WPA TKIP" :
                                 ( record->security == RTW_SECURITY_WPA_AES_PSK ) ? "WPA AES" :
                                 ( record->security == RTW_SECURITY_WPA_MIXED_PSK ) ? "WPA Mixed" :
                                 ( record->security == RTW_SECURITY_WPA2_AES_PSK ) ? "WPA2 AES" :
                                 ( record->security == RTW_SECURITY_WPA2_TKIP_PSK ) ? "WPA2 TKIP" :
                                 ( record->security == RTW_SECURITY_WPA2_MIXED_PSK ) ? "WPA2 Mixed" :
                                 ( record->security == RTW_SECURITY_WPA_WPA2_TKIP_PSK) ? "WPA/WPA2 TKIP" :
                                 ( record->security == RTW_SECURITY_WPA_WPA2_AES_PSK) ? "WPA/WPA2 AES" :
                                 ( record->security == RTW_SECURITY_WPA_WPA2_MIXED_PSK) ? "WPA/WPA2 Mixed" :
                                 ( record->security == RTW_SECURITY_WPA_TKIP_ENTERPRISE ) ? "WPA TKIP Enterprise" :
                                 ( record->security == RTW_SECURITY_WPA_AES_ENTERPRISE ) ? "WPA AES Enterprise" :
                                 ( record->security == RTW_SECURITY_WPA_MIXED_ENTERPRISE ) ? "WPA Mixed Enterprise" :
                                 ( record->security == RTW_SECURITY_WPA2_TKIP_ENTERPRISE ) ? "WPA2 TKIP Enterprise" :
                                 ( record->security == RTW_SECURITY_WPA2_AES_ENTERPRISE ) ? "WPA2 AES Enterprise" :
                                 ( record->security == RTW_SECURITY_WPA2_MIXED_ENTERPRISE ) ? "WPA2 Mixed Enterprise" :
                                 ( record->security == RTW_SECURITY_WPA_WPA2_TKIP_ENTERPRISE ) ? "WPA/WPA2 TKIP Enterprise" :
                                 ( record->security == RTW_SECURITY_WPA_WPA2_AES_ENTERPRISE ) ? "WPA/WPA2 AES Enterprise" :
                                 ( record->security == RTW_SECURITY_WPA_WPA2_MIXED_ENTERPRISE ) ? "WPA/WPA2 Mixed Enterprise" :
                                 "Unknown");

    RTW_API_INFO(" %s ", record->SSID.val);
    RTW_API_INFO("\r\n");
}

static rtw_result_t matter_scan_result_handler( rtw_scan_handler_result_t* malloced_scan_result )
{
    if (malloced_scan_result->scan_complete != RTW_TRUE)
    {
        if (malloced_scan_result->ap_details.SSID.len != 0)
        {
            rtw_scan_result_t* record = &malloced_scan_result->ap_details;
            record->SSID.val[record->SSID.len] = 0; /* Ensure the SSID is null terminated */

            RTW_API_INFO("%d\t ", ++apNum);
            print_matter_scan_result(record);

            if(malloced_scan_result->user_data)
                memcpy((void *)((char *)malloced_scan_result->user_data+(apNum-1)*sizeof(rtw_scan_result_t)), (char *)record, sizeof(rtw_scan_result_t));
        }
    } 
    else
    {
        if (chip_connmgr_callback_func && chip_connmgr_callback_data)
        {
            // inform matter
            chip_connmgr_callback_func(chip_connmgr_callback_data);
        }
        else
        {
            printf("chip_connmgr_callback_func is NULL\r\n");
            apNum = 0;
            return RTW_ERROR;
        }
    }
    return RTW_SUCCESS;
}

static rtw_result_t matter_scan_with_ssid_result_handler( rtw_scan_handler_result_t* malloced_scan_result )
{
    if (malloced_scan_result->scan_complete != RTW_TRUE)
    {
        rtw_scan_result_t* record = &malloced_scan_result->ap_details;
        record->SSID.val[record->SSID.len] = 0; /* Ensure the SSID is null terminated */

        if((malloced_scan_result->user_data) && (!strcmp(matter_ssid, record->SSID.val)))
        {
            RTW_API_INFO("%d\t ", ++apNum);
            memcpy((void *)((char *)malloced_scan_result->user_data+(apNum-1)*sizeof(rtw_scan_result_t)), (char *)record, sizeof(rtw_scan_result_t));
            print_matter_scan_result(record);
        }
    } 
    else
    {
        if (chip_connmgr_callback_func && chip_connmgr_callback_data)
        {
            // inform matter
            chip_connmgr_callback_func(chip_connmgr_callback_data);
            vPortFree(matter_ssid);
        }
        else
        {
            printf("chip_connmgr_callback_func is NULL\r\n");
            apNum = 0;
            vPortFree(matter_ssid);
            return RTW_ERROR;
        }
    }
    return RTW_SUCCESS;
}

void matter_scan_networks(void)
{
    volatile int ret = RTW_SUCCESS;
    apNum = 0; // reset counter at the start of scan
    if((ret = wifi_scan_networks(matter_scan_result_handler, matter_userdata)) != RTW_SUCCESS)
    {
        printf("ERROR: wifi scan failed\n\r");
    }
}

void matter_scan_networks_with_ssid(const unsigned char *ssid, size_t length)
{
    volatile int ret = RTW_SUCCESS;
    apNum = 0; // reset counter at the start of scan
    matter_ssid = (char*) pvPortMalloc(length+1);
    memset(matter_ssid, 0, length+1);
    memcpy(matter_ssid, ssid, length);
    matter_ssid[length] = '\0';
    if((ret = wifi_scan_networks(matter_scan_with_ssid_result_handler, matter_userdata)) != RTW_SUCCESS)
    {
        printf("ERROR: wifi scan failed\n\r");
    }
}

void matter_get_scan_results(rtw_scan_result_t *result_buf, uint8_t scanned_num)
{
    memcpy(result_buf, matter_userdata, sizeof(rtw_scan_result_t) * scanned_num);
}


#ifdef __cplusplus
}
#endif
