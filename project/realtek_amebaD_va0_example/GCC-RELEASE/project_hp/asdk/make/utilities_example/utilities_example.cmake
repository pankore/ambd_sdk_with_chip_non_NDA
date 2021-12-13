cmake_minimum_required(VERSION 3.6)

project(utilities_example)

set(dir "${sdk_root}/component/common/example")

# Temporary setting of macros
set(CONFIG_AUDIO_EN OFF)        # Unable to find file
set(CONFIG_USB_OTG_EN OFF)      # Unable to find file
set(CONFIG_LINKKIT_AWSS OFF)    # Unable to find file
set(CONFIG_USB_DEVICE_EN OFF)
set(CONFIG_USB_HOST_EN OFF)
set(CONFIG_USBH_UVC OFF)

list(
    APPEND ${list}

    ${dir}/example_entry.c
    ${dir}/cm_backtrace/example_cm_backtrace.c
    ${dir}/bcast/example_bcast.c
    #${dir}/eap/example_eap.c
    ${dir}/get_beacon_frame/example_get_beacon_frame.c
    ${dir}/high_load_memory_use/example_high_load_memory_use.c
    ${dir}/http_client/example_http_client.c
    ${dir}/http_download/example_http_download.c
    ${dir}/mcast/example_mcast.c
    ${dir}/mdns/example_mdns.c
    ${dir}/nonblock_connect/example_nonblock_connect.c
    ${dir}/rarp/example_rarp.c
    ${dir}/sntp_showtime/example_sntp_showtime.c
    ${dir}/socket_select/example_socket_select.c
    ${dir}/ssl_download/example_ssl_download.c
    ${dir}/ssl_server/example_ssl_server.c
    ${dir}/tcp_keepalive/example_tcp_keepalive.c
    ${dir}/uart_atcmd/example_uart_atcmd.c
    ${dir}/wifi_mac_monitor/example_wifi_mac_monitor.c
    ${dir}/wlan_fast_connect/example_wlan_fast_connect.c
    #$(dir}/wlan_repeater/example_wlan_repeater.c
    ${dir}/wlan_scenario/example_wlan_scenario.c
    ${dir}/websocket_client/example_wsclient.c
    ${dir}/websocket_server/example_ws_server.c
    ${dir}/xml/example_xml.c
    ${dir}/mqtt/example_mqtt.c
    #${dir}/coap/example_coap.c
    #${dir}/coap_client/example_coap_client.c
    #${dir}/coap_server/example_coap_server.c
    ${dir}/httpd/example_httpd.c
    ${dir}/httpc/example_httpc.c
    ${dir}/ota_http/example_ota_http.c
    #${dir}/ota_https/example_ota_https.c
    ${dir}/ota_sdcard/example_ota_sdcard.c
    ${dir}/dct/example_dct.c
    #${dir}/wifi_manager/example_wifi_manager.c
    ${dir}/wifi_roaming/example_wifi_roaming.c
    #${dir}/wifi_roaming_plus/example_wifi_roaming_plus.c
    #${dir}/wifi_connection_priority/example_wifi_conn_pri_cond.c
    ${dir}/uart_firmware_update/example_uart_update.c
    #${dir}/ipv6/example_ipv6.c
    ${dir}/cJSON/example_cJSON.c
    #${dir}/sdcard_upload_httpd/example_sdcard_upload_httpd.c
    ${dir}/socket_tcp_trx/example_socket_tcp_trx_1.c
    ${dir}/socket_tcp_trx/example_socket_tcp_trx_2.c

    # MMF AUDIO EXAMPLE
    #${dir}/media_framework/example_media_framework.c
    #${dir}/media_framework/mmf2_example_audioloop_init.c
    #${dir}/media_framework/mmf2_example_pcmu_array_rtsp_init.c
    #${dir}/media_framework/mmf2_example_aac_array_rtsp_init.c
    #${dir}/media_framework/mmf2_example_a_init.c
    #${dir}/media_framework/mmf2_example_rtp_aad_init.c
    #${dir}/media_framework/mmf2_example_2way_audio_init.c
    #${dir}/media_framework/mmf2_example_aacloop_init.c
    #${dir}/media_framework/mmf2_example_g711loop_init.c

    #${dir}/audio_sport/audio_amr/example_audio_amr.c
    #${dir}/audio_sport/audio_mp3/example_audio_mp3.c
    #${dir}/audio_sport/audio_ac3/example_audio_ac3.c
    #${dir}/audio_sport/audio_helix_aac/example_audio_helix_aac.c
    #${dir}/audio_sport/audio_helix_mp3/example_audio_helix_mp3.c
    #${dir}/audio_sport/audio_m4a_selfparse/example_audio_m4a_selfparse.c
    #${dir}/audio_sport/audio_m4a/example_audio_m4a.c
    #${dir}/audio_sport/audio_pcm_upload/example_audio_pcm_upload.c
    #${dir}/audio_sport/audio_hls/example_audio_hls.c
    #${dir}/audio_sport/audio_recorder/example_audio_recorder.c
    #${dir}/audio_sport/audio_flac/example_audio_flac.c
    #${dir}/audio_sport/audio_amr_flash/example_audio_amr_flash.c
    #${dir}/audio_sport/audio_amr_flash/audio_play.c
    #${dir}/audio_sport/audio_signal_generate/example_audio_signal_generate.c
    #${dir}/audio_sport/audio_tts/example_audio_tts.c
    ${dir}/fatfs/example_fatfs.c
    ${dir}/dct/example_dct.c
    #${dir}/competitive_headphones/example_competitive_headphones.c
    #${dir}/competitive_headphones/headphone_rl6548_audio.c
    #${dir}/competitive_headphones/headphone_burst.c
    #${dir}/competitive_headphones/headphone_packet_compensation.c
    #${dir}/competitive_headphones_dongle/example_competitive_headphones_dongle.c
    ${dir}/matter_task/example_matter.c
)

if(${CONFIG_AUDIO_EN})
list(
    APPEND ${list}

    ${dir}/audio_sport/audio_amr/example_audio_amr.c
    ${dir}/audio_sport/audio_mp3/example_audio_mp3.c
    ${dir}/audio_sport/audio_helix_aac/example_audio_helix_aac.c
    ${dir}/audio_sport/audio_helix_mp3/example_audio_helix_mp3.c
    ${dir}/audio_sport/audio_m4a_selfparse/example_audio_m4a_selfparse.c
    #${dir}/audio_sport/audio_m4a/example_audio_m4a.c
    #${dir}/audio_sport/audio_pcm_upload/example_audio_pcm_upload.c
    #${dir}/audio_sport/audio_hls/example_audio_hls.c
    ${dir}/audio_sport/audio_flac/example_audio_flac.c
    ${dir}/audio_sport/audio_ac3/example_audio_ac3.c
    ${dir}/audio_sport/audio_ac3/a52dec.c
    ${dir}/audio_sport/audio_recorder/example_audio_recorder.c
    ${dir}/audio_sport/audio_amr_flash/example_audio_amr_flash.c
    ${dir}/audio_sport/audio_amr_flash/audio_play.c
    ${dir}/audio_sport/example_audio_ekho.c
    ${dir}/audio_sport/audio_signal_generate/example_audio_signal_generate.c
    #${dir}/audio_sport/audio_tts_cybron/example_audio_tts_cyberon.c
)
endif()

if(${CONFIG_USB_OTG_EN} AND ${CONFIG_USB_DEVICE_EN})
list(
    APPEND ${list}

    ${dir}/usbd_audio/example_usbd_audio_sp.c
    ${dir}/usbd_audio/example_usbd_audio.c
    ${dir}/usbd_msc/example_usbd_msc.c
    ${dir}/usbd_cdc_acm/example_usbd_cdc_acm.c
    ${dir}/usbd_vendor/example_usbd_vendor.c
)
endif()

if(${CONFIG_USB_OTG_EN} AND ${CONFIG_USB_HOST_EN})
list(
    APPEND ${list}

    ${dir}/usbh_msc/example_usbh_msc.c
    ${dir}/usbh_uvc/example_usbh_uvc.c
    ${dir}/usbh_vendor/example_usbh_vendor.c
    ${dir}/usbh_cdc_acm/example_usbh_cdc_acm.c
)
endif()

if(${CONFIG_LINKKIT_AWSS})
list(
    APPEND ${list}

    ${dir}/linkkit/linkkit_awss_example.c
)
endif()


list(
	APPEND ${list_inc_path}
    #${sdk_root}/component/common/network/coap/include
    ${sdk_root}/component/common/application/mqtt/MQTTClient
    ${sdk_root}/component/common/audio/flac
    ${sdk_root}/component/common/bluetooth/realtek/sdk
    ${sdk_root}/component/common/drivers/sdio/realtek/sdio_host/inc
    ${sdk_root}/component/common/network/libcoap/include
)

#if(${CONFIG_AUDIO_EN})
#target_include_directories(
#    ${target}
#    PUBLIC
#    ${sdk_root}/component/common/audio/libav
#    ${sdk_root}/component/common/audio/a52dec/src
#    ${sdk_root}/component/common/audio/a52dec/include
#    ${sdk_root}/component/common/audio/a52dec/libao
#)
#endif()

#if(${CONFIG_USB_DEVICE_EN})
#target_include_directories(
#    ${target}
#    PUBLIC
#    ${sdk_root}/component/soc/realtek/amebad/fwlib/usb_otg/device/inc
#    ${sdk_root}/component/common/drivers/usb/device/audio/inc
#    ${sdk_root}/component/common/drivers/usb/device/cdc_acm/inc
#    ${sdk_root}/component/common/drivers/usb/device/msc/inc
#    ${sdk_root}/component/common/drivers/usb/device/vendor/inc
#)
#endif()

#if(${CONFIG_USB_HOST_EN})
#target_include_directories(
#    ${target}
#    PUBLIC
#    ${sdk_root}/component/soc/realtek/amebad/fwlib/usb_otg/host/inc
#    ${sdk_root}/component/common/drivers/usb/host/msc/inc
#    ${sdk_root}/component/common/drivers/usb/host/uvc/inc
#    ${sdk_root}/component/common/drivers/usb/host/vendor/inc
#    ${sdk_root}/component/common/drivers/usb/host/cdc_acm/inc
#)

#if(${CONFIG_USBH_UVC})
#target_include_directories(
#    ${target}
#    PUBLIC
#    ${sdk_root}/component/common/video/v4l2/inc
#    ${sdk_root}/component/common/media/rtp_codec
#    ${sdk_root}/component/common/media/mmfv2
#)
#endif()
#endif()

#if(${CONFIG_LINKKIT_AWSS})
#target_include_directories(
#    ${target}
#    PUBLIC
#    ${sdk_root}/component/common/application/linkkit/v1.3.0/include
#    ${sdk_root}/component/common/application/linkkit/v1.3.0/include/imports
#    ${sdk_root}/component/common/application/linkkit/v1.3.0/include/exports
#    ${sdk_root}/component/common/application/linkkit/v1.3.0/include/platform
#)
#endif()
