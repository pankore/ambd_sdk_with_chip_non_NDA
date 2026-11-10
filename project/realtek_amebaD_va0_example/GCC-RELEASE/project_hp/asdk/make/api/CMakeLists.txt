cmake_minimum_required(VERSION 3.6)

project(api)

set(sdk_root "${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../..")
set(prj_root "${CMAKE_CURRENT_SOURCE_DIR}/../../../../..")
set(dir "${sdk_root}/component/common/api")
set(target api)
set(list api_sources)

message(STATUS "Building \"${target}\" folder")

list(
    APPEND ${list}

    ${dir}/wifi_interactive_mode.c
    ${dir}/lwip_netconf.c

    # at_cmd
    ${dir}/at_cmd/atcmd_sys.c
    ${dir}/at_cmd/atcmd_wifi.c
    ${dir}/at_cmd/atcmd_lwip.c
    ${dir}/at_cmd/log_service.c
    ${dir}/at_cmd/atcmd_mp.c
    ${dir}/at_cmd/atcmd_mp_ext2.c

    # network
    ${dir}/network/src/wlan_network.c
    ${dir}/network/src/ttcp.c
    ${dir}/network/src/ping_test.c

    # wifi    
    ${dir}/wifi/wifi_util.c
    ${dir}/wifi/wifi_conf.c
    ${dir}/wifi/wifi_promisc.c
    ${dir}/wifi/wifi_ind.c
    ${dir}/wifi/wifi_simple_config.c
    ${dir}/wifi/rtw_wpa_supplicant/wpa_supplicant/wifi_eap_config.c
    ${dir}/wifi/rtw_wpa_supplicant/wpa_supplicant/wifi_p2p_config.c
    ${dir}/wifi/rtw_wpa_supplicant/wpa_supplicant/wifi_wps_config.c
    #${dir}/wifi/rtw_wpa_supplicant/src/crypto/tls_polarssl.c
)

if(${CONFIG_BT_EN})
list(
    APPEND ${list}
    # at_cmd
    ${dir}/at_cmd/atcmd_mp_ext2.c
)
endif()	

if(${CONFIG_LINKKIT_AWSS})
list(
    APPEND ${list}
    # at_cmd
    ${dir}/at_cmd/atcmd_linkkit.c
)
endif()

add_executable(
    ${target}
    ${api_sources}
)

target_include_directories(
    ${target}
    PUBLIC
    ${sdk_root}/component/common/network/libcoap/include
)
