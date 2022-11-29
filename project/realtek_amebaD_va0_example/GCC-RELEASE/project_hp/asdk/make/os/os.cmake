cmake_minimum_required(VERSION 3.6)

project(os)

set(dir "${sdk_root}/component/os/freertos")
set(dir_osdep "${sdk_root}/component/os/os_dep")
set(dir_freertos "${dir}/freertos_v10.2.0")
set(dir_freertos_port "${dir_freertos}/Source/portable/GCC/RTL8721D_HP/non_secure")

list(
    APPEND ${list}

    ${dir}/freertos_backtrace_ext.c
    ${dir}/freertos_service.c
    ${dir}/freertos_heap5_config.c

    # os_dep
    ${dir_osdep}/device_lock.c
    ${dir_osdep}/osdep_service.c
    ${dir_osdep}/psram_reserve.c

    # freertos_v10.2.0
    ${dir_freertos}/Source/tasks.c
    ${dir_freertos}/Source/list.c
    ${dir_freertos}/Source/croutine.c
    ${dir_freertos}/Source/queue.c
    ${dir_freertos}/Source/timers.c
    ${dir_freertos}/Source/event_groups.c
    ${dir_freertos}/Source/stream_buffer.c
    ${dir_freertos}/Source/portable/MemMang/heap_5.c

    # freertos port
    ${dir_freertos_port}/port.c
    ${dir_freertos_port}/portasm.c
)
