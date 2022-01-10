cmake_minimum_required(VERSION 3.6)

project(make)

set(sdk_root "${CMAKE_CURRENT_SOURCE_DIR}/../../../../../..")
set(prj_root "${CMAKE_CURRENT_SOURCE_DIR}/../../../..")

#set(toolchain_dir "${CMAKE_CURRENT_SOURCE_DIR}/../../toolchain")
#set(asdk_toolchain "${toolchain_dir}/linux/asdk-10.3.0/linux/newlib")
#set(FROMELF "${asdk_toolchain}/bin/arm-none-eabi-objcopy")

#set(CONFIG_BT_EN ON)
#set(CONFIG_LINKKIT_AWSS OFF)
#set(CONFIG_WIFI_EN ON)
#set(CONFIG_MBED_TLS_ENABLED ON)

if(${CONFIG_BT_EN})
#    add_subdirectory(rtl_bluetooth)    # no common/bluetooth folder
endif()

if(${CONFIG_WIFI_EN})
    add_subdirectory(api)
    add_subdirectory(network)
    #add_subdirectory(wlan) # pre-built library
endif()

    add_subdirectory(utilities)
#    add_subdirectory(test)
    add_subdirectory(os)

if(${CONFIG_MBED_TLS_ENABLED})
    add_subdirectory(mbedtls)
endif()

if(${CONFIG_GUI_EN})
#    add_subdirectory(ui)
endif()

if(${CONFIG_LINKKIT_AWSS})
#    add_subdirectory(linkkit)
endif()

    add_subdirectory(app)
    add_subdirectory(target)
    add_subdirectory(drivers)
#    add_subdirectory(cmsis)    # no cmsis/source folder
    add_subdirectory(mbed)
    add_subdirectory(utilities_example)
    add_subdirectory(file_system)
    add_subdirectory(project)
    add_subdirectory(application)
#    add_subdirectory(chip)
    add_subdirectory(chip_main)
