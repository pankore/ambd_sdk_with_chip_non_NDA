/************************** 
* Matter DCT Related 
**************************/
#include "platform_opts.h"
#include "platform/platform_stdlib.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "stddef.h"
#include "string.h"
#include "stdbool.h"
#include "dct.h"
#include "chip_porting.h"

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

s32 initPref(void)
{
    s32 ret;
    ret = dct_init(DCT_BEGIN_ADDR_MATTER, MODULE_NUM, VARIABLE_NAME_SIZE, VARIABLE_VALUE_SIZE, ENABLE_BACKUP, ENABLE_WEAR_LEVELING);
    if (ret != DCT_SUCCESS)
        printf("dct_init failed with error: %d\n", ret);
    else
        printf("dct_init success\n");

    ret = dct_init2(DCT_BEGIN_ADDR_MATTER2, MODULE_NUM2, VARIABLE_NAME_SIZE2, VARIABLE_VALUE_SIZE2, ENABLE_BACKUP, ENABLE_WEAR_LEVELING);
    if (ret != DCT_SUCCESS)
        printf("dct_init2 failed with error: %d\n", ret);
    else
        printf("dct_init2 success\n");

    return ret;
}

s32 deinitPref(void)
{
    s32 ret;
    ret = dct_format(DCT_BEGIN_ADDR_MATTER, MODULE_NUM, VARIABLE_NAME_SIZE, VARIABLE_VALUE_SIZE, ENABLE_BACKUP, ENABLE_WEAR_LEVELING);
    if (ret != DCT_SUCCESS)
        printf("dct_format failed with error: %d\n", ret);
    else
        printf("dct_format success\n");

    ret = dct_format2(DCT_BEGIN_ADDR_MATTER2, MODULE_NUM2, VARIABLE_NAME_SIZE2, VARIABLE_VALUE_SIZE2, ENABLE_BACKUP, ENABLE_WEAR_LEVELING);
    if (ret != DCT_SUCCESS)
        printf("dct_format2 failed with error: %d\n", ret);
    else
        printf("dct_format2 success\n");

    return ret;
}

s32 registerPref()
{
    s32 ret;
    char ns[15];

    for (size_t i=0; i<MODULE_NUM; i++)
    {
        snprintf(ns, 15, "matter_kvs1_%d", i+1); 
        ret = dct_register_module(ns);
        if (ret != DCT_SUCCESS)
            goto exit;
        else
            printf("dct_register_module %s success\n", ns);
    }

exit:
    if (ret != DCT_SUCCESS)
        printf("DCT1 modules registration failed");
    return ret;
}

s32 registerPref2()
{
    s32 ret;
    char ns[15];

    for (size_t i=0; i<MODULE_NUM2; i++)
    {
        snprintf(ns, 15, "matter_kvs2_%d", i+1); 
        ret = dct_register_module2(ns);
        if (ret != DCT_SUCCESS)
            goto exit;
        else
            printf("dct_register_module2 %s success\n", ns);
    }

exit:
    if (ret != DCT_SUCCESS)
        printf("DCT2 modules registration failed");
    return ret;
}

s32 clearPref()
{
    s32 ret;
    char ns[15];

    for (size_t i=0; i<MODULE_NUM; i++)
    {
        snprintf(ns, 15, "matter_kvs1_%d", i+1); 
        ret = dct_unregister_module(ns);
        if (ret != DCT_SUCCESS)
            goto exit;
        else
            printf("dct_unregister_module %s success\n", ns);
    }

exit:
    if (ret != DCT_SUCCESS)
        printf("DCT1 modules unregistration failed");
    return ret;
}

s32 clearPref2()
{
    s32 ret;
    char ns[15];

    for (size_t i=0; i<MODULE_NUM2; i++)
    {
        snprintf(ns, 15, "matter_kvs2_%d", i+1); 
        ret = dct_unregister_module2(ns);
        if (ret != DCT_SUCCESS)
            goto exit;
        else
            printf("dct_unregister_module2 %s success\n", ns);
    }

exit:
    if (ret != DCT_SUCCESS)
        printf("DCT2 modules unregistration failed");
    return ret;
}

s32 deleteKey(const char *domain, const char *key)
{
    dct_handle_t handle;
    s32 ret;
    char ns[15];

    // Loop over DCT1 modules
    for (size_t i=0; i<MODULE_NUM; i++)
    {
        snprintf(ns, 15, "matter_kvs1_%d", i+1); 
        ret = dct_open_module(&handle, ns);
        if (ret != DCT_SUCCESS)
        {
            printf("%s : dct_open_module(%s) failed with error: %d\n" ,__FUNCTION__, ns, ret);
            goto exit;
        }
        ret = dct_delete_variable(&handle, key);
        dct_close_module(&handle);
        if (ret != DCT_SUCCESS)
            return ret;
    }

    // Loop over DCT2 modules
    for (size_t i=0; i<MODULE_NUM2; i++)
    {
        snprintf(ns, 15, "matter_kvs2_%d", i+1); 
        ret = dct_open_module2(&handle, ns);
        if (ret != DCT_SUCCESS)
        {
            printf("%s : dct_open_module2(%s) failed with error: %d\n" ,__FUNCTION__, ns, ret);
            goto exit;
        }
        ret = dct_delete_variable2(&handle, key);
        dct_close_module2(&handle);
        if (ret != DCT_SUCCESS)
            return ret;
    }

exit:
    return ret;
}

bool checkExist(const char *domain, const char *key)
{
    dct_handle_t handle;
    s32 ret;
    uint16_t len = 0;
    u8 *str = malloc(sizeof(u8) * VARIABLE_VALUE_SIZE2-4); // use the bigger buffer size
    char ns[15];

    // Loop over DCT1 modules
    for (size_t i=0; i<MODULE_NUM; i++)
    {
        snprintf(ns, 15, "matter_kvs1_%d", i+1); 
        ret = dct_open_module(&handle, ns);
        if (ret != DCT_SUCCESS)
        {
            printf("%s : dct_open_module(%s) failed with error: %d\n" ,__FUNCTION__, ns, ret);
            goto exit;
        }

        len = sizeof(u32);
        ret = dct_get_variable_new(&handle, key, (char *)str, &len);
        if(ret == DCT_SUCCESS)
        {
            printf("checkExist key=%s found.\n", key);
            dct_close_module(&handle);
            goto exit;
        }

        len = sizeof(u64);
        ret = dct_get_variable_new(&handle, key, (char *)str, &len);
        if(ret == DCT_SUCCESS)
        {
            printf("checkExist key=%s found.\n", key);
            dct_close_module(&handle);
            goto exit;
        }

        dct_close_module(&handle);
    }

    // Loop over DCT2 modules
    for (size_t i=0; i<MODULE_NUM2; i++)
    {
        snprintf(ns, 15, "matter_kvs2_%d", i+1); 
        ret = dct_open_module2(&handle, ns);
        if (ret != DCT_SUCCESS)
        {
            printf("%s : dct_open_module2(%s) failed with error : %d\n" ,__FUNCTION__, ns, ret);
            goto exit;
        }

        len = VARIABLE_VALUE_SIZE2-4;
        ret = dct_get_variable_new2(&handle, key, str, &len);
        if(ret == DCT_SUCCESS)
        {
            printf("checkExist key=%s found.\n", key);
            dct_close_module2(&handle);
            goto exit;
        }

        dct_close_module2(&handle);
    }

exit:
    free(str);
    return (ret == DCT_SUCCESS) ? true : false;
}

s32 setPref_new(const char *domain, const char *key, u8 *value, size_t byteCount)
{
    dct_handle_t handle;
    s32 ret;
    char ns[15];

    if (byteCount <= 64)
    {
        // Loop over DCT1 modules
        for (size_t i=0; i<MODULE_NUM; i++)
        {
            snprintf(ns, 15, "matter_kvs1_%d", i+1); 
            ret = dct_open_module(&handle, ns);
            if (ret != DCT_SUCCESS)
            {
                printf("%s : dct_open_module(%s) failed with error: %d\n" ,__FUNCTION__, ns, ret);
                goto exit;
            }

            if (dct_remain_variable(&handle) > 0)
            {
                ret = dct_set_variable_new(&handle, key, (char *)value, (uint16_t)byteCount);
                if (ret != DCT_SUCCESS)
                {
                    printf("%s : dct_set_variable(%s) failed with error: %d\n" ,__FUNCTION__, key, ret);
                    dct_close_module(&handle);
                    goto exit;
                }
                dct_close_module(&handle);
                break;
            }
            dct_close_module(&handle);
        }
    }
    else
    {
        // Loop over DCT2 modules
        for (size_t i=0; i<MODULE_NUM2; i++)
        {
            snprintf(ns, 15, "matter_kvs2_%d", i+1); 
            ret = dct_open_module2(&handle, ns);
            if (ret != DCT_SUCCESS)
            {
                printf("%s : dct_open_module2(%s) failed with error: %d\n" ,__FUNCTION__, ns, ret);
                goto exit;
            }

            if (dct_remain_variable2(&handle) > 0)
            {
                ret = dct_set_variable_new2(&handle, key, (char *)value, (uint16_t)byteCount);
                if (ret != DCT_SUCCESS)
                {
                    printf("%s : dct_set_variable2(%s) failed with error: %d\n" ,__FUNCTION__, key, ret);
                    dct_close_module2(&handle);
                    goto exit;
                }
                dct_close_module2(&handle);
                break;
            }
            dct_close_module2(&handle);
        }
    }

exit:
    return ret;
}

s32 getPref_bool_new(const char *domain, const char *key, u8 *val)
{
    dct_handle_t handle;
    s32 ret;
    uint16_t len = sizeof(u8);
    char ns[15];

    // Loop over DCT1 modules
    for (size_t i=0; i<MODULE_NUM; i++)
    {
        snprintf(ns, 15, "matter_kvs1_%d", i+1); 
        ret = dct_open_module(&handle, ns);
        if (ret != DCT_SUCCESS)
        {
            printf("%s : dct_open_module(%s) failed with error: %d\n" ,__FUNCTION__, ns, ret);
            goto exit;
        }
        ret = dct_get_variable_new(&handle, key, (char *)val, &len);
        dct_close_module(&handle);
        if (ret == DCT_SUCCESS)
        {
            return ret;
        }
    }

    // Loop over DCT2 modules
    for (size_t i=0; i<MODULE_NUM2; i++)
    {
        snprintf(ns, 15, "matter_kvs2_%d", i+1); 
        ret = dct_open_module2(&handle, ns);
        if (ret != DCT_SUCCESS)
        {
            printf("%s : dct_open_module2(%s) failed with error: %d\n" ,__FUNCTION__, ns, ret);
            goto exit;
        }
        ret = dct_get_variable_new2(&handle, key, (char *)val, &len);
        dct_close_module2(&handle);
        if (ret == DCT_SUCCESS)
        {
            return ret;
        }
    }

exit:
    return ret;
}

s32 getPref_u32_new(const char *domain, const char *key, u32 *val)
{
    dct_handle_t handle;
    s32 ret;
    uint16_t len = sizeof(u32);
    char ns[15];

    // Loop over DCT1 modules
    for (size_t i=0; i<MODULE_NUM; i++)
    {
        snprintf(ns, 15, "matter_kvs1_%d", i+1); 
        ret = dct_open_module(&handle, ns);
        if (ret != DCT_SUCCESS)
        {
            printf("%s : dct_open_module(%s) failed with error: %d\n" ,__FUNCTION__, ns, ret);
            goto exit;
        }
        ret = dct_get_variable_new(&handle, key, (char *)val, &len);
        dct_close_module(&handle);
        if (ret == DCT_SUCCESS)
        {
            return ret;
        }
    }

    // Loop over DCT2 modules
    for (size_t i=0; i<MODULE_NUM2; i++)
    {
        snprintf(ns, 15, "matter_kvs2_%d", i+1); 
        ret = dct_open_module2(&handle, ns);
        if (ret != DCT_SUCCESS)
        {
            printf("%s : dct_open_module2(%s) failed with error: %d\n" ,__FUNCTION__, ns, ret);
            goto exit;
        }
        ret = dct_get_variable_new2(&handle, key, (char *)val, &len);
        dct_close_module2(&handle);
        if (ret == DCT_SUCCESS)
        {
            return ret;
        }
    }

exit:
    return ret;
}

s32 getPref_u64_new(const char *domain, const char *key, u64 *val)
{
    dct_handle_t handle;
    s32 ret;
    uint16_t len = sizeof(u64);
    char ns[15];

    // Loop over DCT1 modules
    for (size_t i=0; i<MODULE_NUM; i++)
    {
        snprintf(ns, 15, "matter_kvs1_%d", i+1); 
        ret = dct_open_module(&handle, ns);
        if (ret != DCT_SUCCESS)
        {
            printf("%s : dct_open_module(%s) failed with error: %d\n" ,__FUNCTION__, ns, ret);
            goto exit;
        }
        ret = dct_get_variable_new(&handle, key, (char *)val, &len);
        dct_close_module(&handle);
        if (ret == DCT_SUCCESS)
        {
            return ret;
        }
    }

    // Loop over DCT2 modules
    for (size_t i=0; i<MODULE_NUM2; i++)
    {
        snprintf(ns, 15, "matter_kvs2_%d", i+1); 
        ret = dct_open_module2(&handle, ns);
        if (ret != DCT_SUCCESS)
        {
            printf("%s : dct_open_module2(%s) failed with error: %d\n" ,__FUNCTION__, ns, ret);
            goto exit;
        }
        ret = dct_get_variable_new2(&handle, key, (char *)val, &len);
        dct_close_module2(&handle);
        if (ret == DCT_SUCCESS)
        {
            return ret;
        }
    }

exit:
    return ret;
}

s32 getPref_str_new(const char *domain, const char *key, char * buf, size_t bufSize, size_t *outLen)
{
    dct_handle_t handle;
    s32 ret;
    char ns[15];

    // Loop over DCT1 modules
    for (size_t i=0; i<MODULE_NUM; i++)
    {
        snprintf(ns, 15, "matter_kvs1_%d", i+1); 
        ret = dct_open_module(&handle, ns);
        if (ret != DCT_SUCCESS)
        {
            printf("%s : dct_open_module(%s) failed with error: %d\n" ,__FUNCTION__, ns, ret);
            goto exit;
        }
        ret = dct_get_variable_new(&handle, key, buf, &bufSize);
        dct_close_module(&handle);
        if (ret == DCT_SUCCESS)
        {
            *outLen = bufSize;
            return ret;
        }
    }

    // Loop over DCT2 modules
    for (size_t i=0; i<MODULE_NUM2; i++)
    {
        snprintf(ns, 15, "matter_kvs2_%d", i+1); 
        ret = dct_open_module2(&handle, ns);
        if (ret != DCT_SUCCESS)
        {
            printf("%s : dct_open_module2(%s) failed with error: %d\n" ,__FUNCTION__, ns, ret);
            goto exit;
        }
        ret = dct_get_variable_new2(&handle, key, buf, &bufSize);
        dct_close_module2(&handle);
        if (ret == DCT_SUCCESS)
        {
            *outLen = bufSize;
            return ret;
        }
    }

exit:
    return ret;
}

s32 getPref_bin_new(const char *domain, const char *key, u8 * buf, size_t bufSize, size_t *outLen)
{
    dct_handle_t handle;
    s32 ret;
    char ns[15];

    // Loop over DCT1 modules
    for (size_t i=0; i<MODULE_NUM; i++)
    {
        snprintf(ns, 15, "matter_kvs1_%d", i+1); 
        ret = dct_open_module(&handle, ns);
        if (ret != DCT_SUCCESS)
        {
            printf("%s : dct_open_module(%s) failed with error: %d\n" ,__FUNCTION__, ns, ret);
            goto exit;
        }
        ret = dct_get_variable_new(&handle, key, (char *)buf, &bufSize);
        dct_close_module(&handle);
        if (ret == DCT_SUCCESS)
        {
            *outLen = bufSize;
            return ret;
        }
    }

    // Loop over DCT2 modules
    for (size_t i=0; i<MODULE_NUM2; i++)
    {
        snprintf(ns, 15, "matter_kvs2_%d", i+1); 
        ret = dct_open_module2(&handle, ns);
        if (ret != DCT_SUCCESS)
        {
            printf("%s : dct_open_module2(%s) failed with error: %d\n" ,__FUNCTION__, ns, ret);
            goto exit;
        }
        ret = dct_get_variable_new2(&handle, key, (char *)buf, &bufSize);
        dct_close_module2(&handle);
        if (ret == DCT_SUCCESS)
        {
            *outLen = bufSize;
            return ret;
        }
    }

exit:
    return ret;
}

#ifdef __cplusplus
}
#endif
