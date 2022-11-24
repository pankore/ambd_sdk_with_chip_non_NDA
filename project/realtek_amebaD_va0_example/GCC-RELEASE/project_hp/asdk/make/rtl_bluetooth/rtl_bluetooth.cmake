cmake_minimum_required(VERSION 3.6)

project(rtl_bluetooth)

set(dir "${sdk_root}/component/common/bluetooth/realtek")
set(dir_sdk "${sdk_root}/component/common/bluetooth/realtek/sdk")
set(matter_bt_dir "${sdk_root}/component/common/application/matter/common/bluetooth/bt_matter_adapter")

if(${CONFIG_BT_EN})

if(${CONFIG_BT_MESH_PROVISIONER})

list(
    APPEND ${list}

    # Mesh Provisioner
    ${dir_sdk}/board/amebad/src/vendor_cmd/vendor_cmd.c 
)

if(${CONFIG_BT_MESH_PROVISIONER_RTK_DEMO})
    # Unable to find file
endif()	

elseif(${CONFIG_BT_MESH_DEVICE})
list(
    APPEND ${list}

    # Mesh Device
    ${dir_mesh}/lib/cmd/mesh_data_uart.c 
	${dir_mesh}/lib/cmd/mesh_user_cmd_parse.c 
    ${dir_mesh}/lib/profile/datatrans_server.c 
    ${dir_mesh}/lib/profile/datatrans_service.c 
	${dir_mesh}/lib/model/generic_on_off_server.c 
	${dir_mesh}/lib/model/delay_execution.c 
	${dir_mesh}/lib/model/delay_msg_rsp.c 
	${dir_mesh}/lib/model/generic_transition_time.c 
    ${dir_mesh}/lib/model/realtek/datatrans_model.c
    ${dir_mesh}/lib/model/realtek/ping_control.c 
    ${dir_mesh}/lib/model/realtek/tp_control.c 
    ${dir_mesh}/lib/model/health_server.c 
	${dir_mesh}/lib/model/light_lightness_setup_server.c 
	${dir_mesh}/lib/model/light_lightness_server.c 
	${dir_mesh}/lib/model/light_xyl_setup_server.c 
	${dir_mesh}/lib/model/light_xyl_server.c 
	${dir_mesh}/lib/model/light_ctl_server.c 
	${dir_mesh}/lib/model/light_ctl_setup_server.c 
	${dir_mesh}/lib/model/light_ctl_temperature_server.c
	${dir_mesh}/lib/model/light_ctl_temperature_server.c
	${dir_mesh}/lib/model/light_hsl_hue_server.c 
	${dir_mesh}/lib/model/light_hsl_saturation_server.c 
	${dir_mesh}/lib/model/light_hsl_server.c 
	${dir_mesh}/lib/model/light_hsl_setup_server.c 
	${dir_mesh}/lib/model/time_server.c 
	${dir_mesh}/lib/model/time_setup_server.c 
    ${dir_mesh}/lib/model/scene_server.c 
    ${dir_mesh}/lib/model/scene_setup_server.c 
    ${dir_mesh}/lib/model/scheduler_server.c 
    ${dir_mesh}/lib/model/scheduler_setup_server.c 
    ${dir_mesh}/device/bt_mesh_device_app_task.c 
    ${dir_mesh}/device/device_app.c 
    ${dir_mesh}/lib/common/ping_app.c 
    ${dir_mesh}/lib/common/datatrans_app.c 
	${dir_mesh}/lib/common/scene_server_app.c 
    ${dir_mesh}/lib/common/scheduler_server_app.c 
	${dir_mesh}/lib/common/time_server_app.c 
	${dir_mesh}/lib/common/light_server_app.c 
    ${dir_mesh}/lib/cmd/mesh_cmd.c 
    ${dir_mesh}/lib/cmd/test_cmd.c 
    ${dir_mesh}/device/device_cmd.c
    ${dir_mesh}/device/bt_mesh_device_app_main.c 
    ${dir_sdk}/board/amebad/src/vendor_cmd/vendor_cmd.c 
    ${dir_mesh_user_api}/device/bt_mesh_device_api.c 
    ${dir_mesh_user_api}/bt_mesh_user_api.c
)

if(${CONFIG_BT_MESH_DEVICE_RTK_DEMO})
    # Unable to find file
endif()

elseif(${CONFIG_BT_MESH_PROVISIONER_MULTIPLE_PROFILE})
    # Unable to find file
if(${CONFIG_BT_MESH_PROVISIONER_RTK_DEMO})
    # Unable to find file
endif()

elseif(${CONFIG_BT_MESH_DEVICE_MULTIPLE_PROFILE})
    # Unable to find file
if (${CONFIG_BT_MESH_DEVICE_RTK_DEMO})
    # Unable to find file
endif()

endif()

list(
    APPEND ${list}

    ${dir}/sdk/board/common/os/freertos/osif_freertos.c 
    ${dir}/sdk/board/amebad/src/platform_utils.c 
    ${dir}/sdk/board/common/src/cycle_queue.c 
    ${dir}/sdk/board/common/src/trace_task.c 
    ${dir}/sdk/board/common/src/hci_process.c
    ${dir}/sdk/board/common/src/hci_adapter.c
    ${dir}/sdk/board/common/src/bt_uart_bridge.c 
    ${dir}/sdk/board/amebad/src/trace_uart.c 
	${dir}/sdk/board/amebad/src/rtk_coex.c 
	${dir}/sdk/board/amebad/src/vendor_cmd/vendor_cmd.c 
	${dir}/sdk/board/amebad/src/hci/hci_uart.c 
    ${dir}/sdk/board/amebad/src/hci/hci_board.c 
    ${dir}/sdk/board/amebad/src/hci/bt_fwconfig.c 
    ${dir}/sdk/board/amebad/src/hci/bt_normal_patch.c 
	${dir}/sdk/board/amebad/src/hci/bt_mp_patch.c 
	${dir}/sdk/bt_example_entry.c

    # bt_beacon 

    # bt_config

    # bt_airsync_config

    # bt_breeze
    # Unable to find file

    # ble_peripheral
    ${dir}/sdk/src/ble/profile/server/simple_ble_service.c 
    ${dir}/sdk/src/ble/profile/server/bas.c 
    ${dir}/sdk/example/ble_peripheral/app_task.c 
    ${dir}/sdk/example/ble_peripheral/ble_app_main.c 
    ${dir}/sdk/example/ble_peripheral/peripheral_app.c 
    ${dir}/sdk/example/ble_peripheral/ble_peripheral_at_cmd.c

    # ble_central

    # ble_scatternet

    # ble_matter
    ${matter_bt_dir}/bt_matter_adapter_app_main.c
    ${matter_bt_dir}/bt_matter_adapter_app_task.c
    ${matter_bt_dir}/bt_matter_adapter_peripheral_app.c
    ${matter_bt_dir}/bt_matter_adapter_service.c
    ${matter_bt_dir}/bt_matter_adapter_wifi.c
)
endif()
