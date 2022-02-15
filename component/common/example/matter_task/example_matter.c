#include <platform_opts.h>
#include <platform_stdlib.h>

#if defined(CONFIG_EXAMPLE_MATTER) && CONFIG_EXAMPLE_MATTER

extern void ChipTest(void);

static void example_matter_task_thread(void *pvParameters)
{
    vTaskDelay(5000);
    ChipTest();

    vTaskDelete(NULL);
    return;
}

void example_matter_task(void)
{
    // TODO: This is a temporary work around, lighting-app requires priority of tskIDLE_PRIORITY + 2 to avoid hard fault
#if (MATTER_LIGHTING_APP)
    if(xTaskCreate(example_matter_task_thread, ((const char*)"example_matter_task_thread"), 1024, NULL, tskIDLE_PRIORITY + 2, NULL) != pdPASS)
        printf("\n\r%s xTaskCreate(example_matter_task_thread) failed", __FUNCTION__);
#else
    if(xTaskCreate(example_matter_task_thread, ((const char*)"example_matter_task_thread"), 1024, NULL, tskIDLE_PRIORITY + 1, NULL) != pdPASS)
        printf("\n\r%s xTaskCreate(example_matter_task_thread) failed", __FUNCTION__);
#endif // #if (MATTER_LIGHTING_APP)
}

#endif // #if (CONFIG_EXAMPLE_MATTER)
