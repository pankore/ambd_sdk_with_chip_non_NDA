cmake_minimum_required(VERSION 3.6)

find_package(Python3 REQUIRED)

project(app_ntz)

enable_language(C ASM)

set(prj_root_tmp "${CMAKE_CURRENT_SOURCE_DIR}/../../..")
set(sdk_root_tmp "${CMAKE_CURRENT_SOURCE_DIR}/../../../../..")
get_filename_component(prj_root ${prj_root_tmp} REALPATH)
get_filename_component(sdk_root ${sdk_root_tmp} REALPATH)
set(km0_image "${prj_root}/GCC-RELEASE/project_lp/asdk/image")
set(prj_hp "${prj_root}/GCC-RELEASE/project_hp")
set(app_ntz application.ntz)
set(app_ntz_pure application.ntz_pure)
set(list app_ntz_sources)
set(list_inc_path app_ntz_inc)
set(list_c_flags app_ntz_flags)
set(list_code2sram code2sram)
set(list_code2sram_network code2sram_network)

set(freertos "freertos_v10.2.0")
set(lwip "lwip_v2.1.2")

set(utility_dir "${prj_hp}/asdk/gnu_utility")
set(prepend_tool "${utility_dir}/prepend_header.sh")
set(pad_tool "${utility_dir}/pad.sh")

if(MATTER_ALL_CLUSTERS_APP)
    ADD_DEFINITIONS(-DMATTER_ALL_CLUSTERS_APP=1)
    ADD_DEFINITIONS(-DMATTER_OTA_REQUESTOR_APP=1)
    if(matter_enable_rpc)
        set (matter_enable_shell 0)
    else()
        set (matter_enable_shell 1)
    endif(matter_enable_rpc)
endif(MATTER_ALL_CLUSTERS_APP)

if(MATTER_LIGHTING_APP)
    ADD_DEFINITIONS(-DMATTER_LIGHTING_APP=1)
    ADD_DEFINITIONS(-DMATTER_OTA_REQUESTOR_APP=1)
    if(matter_enable_rpc)
        set (matter_enable_shell 0)
    else()
        set (matter_enable_shell 1)
    endif(matter_enable_rpc)
endif()

if(MATTER_OTA_REQUESTOR_APP)
    ADD_DEFINITIONS(-DMATTER_OTA_REQUESTOR_APP=1)
endif()

if(MATTER_OTA_PROVIDER_APP)
    ADD_DEFINITIONS(-DMATTER_OTA_PROVIDER_APP=1)
endif()

if(MATTER_LIGHT_SWITCH_APP)
    ADD_DEFINITIONS(-DMATTER_LIGHT_SWITCH_APP=1)
    ADD_DEFINITIONS(-DMATTER_OTA_REQUESTOR_APP=1)
    if(matter_enable_rpc)
        set (matter_enable_shell 0)
    else()
        set (matter_enable_shell 1)
    endif(matter_enable_rpc)
endif()

if(MATTER_CHEF_APP)
    ADD_DEFINITIONS(-DMATTER_CHEF_APP=1)
endif()

include(config.cmake)
include(includepath.cmake)
include(make/make.cmake)

if(DEFINED ameba_matter_root)
    SET(CHIP_ROOT "${ameba_matter_root}")
    include(${ameba_matter_root}/src/app/chip_data_model.cmake)
    include(${matter_example_path}/chip_main.cmake)
    include(${matter_config_path}/chip.cmake)
else()
    include(${prj_hp}/asdk/make/chip_main/chip_main.cmake)
    include(${prj_hp}/asdk/make/chip/chip.cmake)
endif()

add_dependencies(chip_main chip-gn)

#copy_ld_img2(ld/rlx8721d_img2_is.ld) copy_ld_img3
set( ld_script ${prj_hp}/asdk/rlx8721d_test.ld )

add_executable(
	${app_ntz}
	${app_ntz_sources}
)

target_include_directories(
	${app_ntz}
	PUBLIC

	${inc_path}
	${app_ntz_inc}
)

list(
	APPEND app_ntz_flags
	CONFIG_PLATFORM_8721D
	CONFIG_USE_MBEDTLS_ROM_ALG
	CONFIG_FUNCION_O0_OPTIMIZE
	DM_ODM_SUPPORT_TYPE=32
	MBEDTLS_CONFIG_FILE=<mbedtls_config.h>
	LWIP_IPV6_ROUTE_TABLE_SUPPORT=1
	CONFIG_ENABLE_AMEBA_SNTP=1
)

target_compile_definitions(
	${app_ntz}
	PRIVATE

	${app_ntz_flags}
)

target_link_libraries(
	${app_ntz}
	_application_hp
	_btgap
	_cmsis_dsp
	_dct
	_wlan
	_wps
	_httpc
	_httpd
	_mdns
	_tftp
	_usbd
	_usbh
	_user
	_websocket
	-Wl,--start-group chip_main CHIP -Wl,--end-group
	m
	stdc++
	c
	gcc
	nosys
)

set(TOOLCHAINDIR "${prj_root}/GCC-RELEASE/project_hp/toolchain")
set(ASDK_TOOLCHAIN "${TOOLCHAINDIR}/linux/asdk-10.3.0/linux/newlib")
target_link_options(
	${app_ntz}
	PUBLIC
	"LINKER:SHELL:-L ${ASDK_TOOLCHAIN}/lib"
	"LINKER:SHELL:-L ${prj_hp}/asdk/lib"
	"LINKER:SHELL:-L ${prj_hp}/asdk/lib/application"
	"LINKER:SHELL:-L ${sdk_root}/component/common/bluetooth/realtek/sdk/board/amebad/lib"
	"LINKER:SHELL:-T ${ld_script}"
	"LINKER:SHELL:-Map=text.map"
)

add_custom_command(TARGET ${app_ntz} PRE_BUILD

	COMMAND echo > ram_size.txt
	COMMAND echo > obj_list.txt
)

#RENAME CODE2SRAM
foreach(_file ${code2sram})
	string(CONCAT _file_obj ${CMAKE_CURRENT_BINARY_DIR} "/CMakeFiles/${app_ntz}.dir" ${_file} ".obj")
	add_custom_command(TARGET ${app_ntz} PRE_LINK
		COMMAND ${CMAKE_OBJCOPY} --prefix-section .image2.ram.text ${_file_obj} ${_file_obj}
	)
endforeach()

#RENAME NETWORK CODE2SRAM
foreach(_file_net ${code2sram_network})
	string(CONCAT _file_obj ${CMAKE_CURRENT_BINARY_DIR} "/CMakeFiles/${app_ntz}.dir" ${_file_net} ".obj")
	add_custom_command(TARGET ${app_ntz} PRE_LINK
		COMMAND ${CMAKE_OBJCOPY} --prefix-section .image2.net.ram.text ${_file_obj} ${_file_obj}
	)
endforeach()

#prepare ram_size.txt and obj_list.txt
foreach(_file ${app_ntz_sources})
	string(CONCAT _file_obj ${CMAKE_CURRENT_BINARY_DIR} "/CMakeFiles/${app_ntz}.dir" ${_file} ".obj")
	add_custom_command(TARGET ${app_ntz} PRE_LINK
		COMMAND ${CMAKE_SIZE} -t ${_file_obj} >> ram_size.txt
		COMMAND echo ${_file_obj} >> obj_list.txt
	)
endforeach()

add_custom_command(TARGET ${app_ntz} PRE_LINK
    COMMAND echo "COPY CHIP library ..."
    COMMAND cp -f ${CHIP_OUTPUT}/lib/libCHIP.a ${LIB_ROOT}
)

add_custom_command(TARGET ${app_ntz} POST_BUILD

	COMMAND echo "========= linker img2_ns start ========="
	COMMAND cp $<TARGET_FILE:${app_ntz}> target_image2.axf
	COMMAND ${CMAKE_NM} target_image2.axf | sort > target_image2.map
	COMMAND ${CMAKE_OBJEDUMP} -d target_image2.axf > target_image2.asm

	COMMAND rm -rf image && mkdir -p image
	COMMAND cp target_image2.axf image/target_image2.axf
	COMMAND cp target_image2.axf image/target_pure_image2.axf
	COMMAND cp target_image2.map image
	COMMAND cp target_image2.asm image
	COMMAND cp ram_size.txt image
	COMMAND cp obj_list.txt image
	COMMAND find .. -type f -name "text.map" | xargs -i cp {} image/text_image2_ns.map

	COMMAND ${CMAKE_STRIP} image/target_pure_image2.axf
	COMMAND ${CMAKE_NM} image/target_pure_image2.axf | sort > image/target_pure_image2.map

	COMMAND ${CMAKE_OBJCOPY} -j .ram_image2.entry -j .ram_image2.text -j .ram_image2.data -Obinary image/target_pure_image2.axf image/ram_2.bin
	COMMAND ${CMAKE_OBJCOPY} -j .xip_image2.text -Obinary image/target_pure_image2.axf image/xip_image2.bin
	COMMAND ${CMAKE_OBJCOPY} -j .psram_image2.text -j .psram_image2.data -Obinary image/target_pure_image2.axf image/psram_2.bin

	# BT TRACE
	#COMMAND ${CMAKE_OBJCOPY} -j .bluetooth_trace.text -Obinary output/${app_ntz_pure}.axf APP.trace

	COMMAND echo "========== Image Info HEX =========="
	COMMAND ${CMAKE_SIZE} -A --radix=16 image/target_image2.axf
	COMMAND ${CMAKE_SIZE} -t --radix=16 image/target_image2.axf
	COMMAND echo "========== Image Info HEX =========="

	COMMAND echo "========== Image Info DEC =========="
	COMMAND ${CMAKE_SIZE} -A --radix=10 image/target_image2.axf
	COMMAND ${CMAKE_SIZE} -t --radix=10 image/target_image2.axf
	COMMAND echo "========== Image Info DEC =========="

	COMMAND echo "========== Image manipulating start =========="
	COMMAND ${prepend_tool} image/ram_2.bin  __ram_image2_text_start__  image/target_image2.map
	COMMAND ${prepend_tool} image/xip_image2.bin  __flash_text_start__  image/target_image2.map
	COMMAND ${prepend_tool} image/psram_2.bin  __psram_image2_text_start__  image/target_image2.map
	COMMAND cp ${prj_hp}/asdk/code_analyze.py .
	COMMAND chmod 777 code_analyze.py
	COMMAND python3 code_analyze.py

	COMMAND cat image/xip_image2_prepend.bin image/ram_2_prepend.bin image/psram_2_prepend.bin  > image/km4_image2_all.bin

	COMMAND ${pad_tool} image/km4_image2_all.bin
	COMMAND cat ${km0_image}/km0_image2_all.bin image/km4_image2_all.bin > image/km0_km4_image2.bin
	COMMAND echo "Image manipulating ... Done "
	COMMAND echo "========== Image manipulating end =========="


)

if (matter_generate_ota_image)
file(READ ${prj_root}/GCC-RELEASE/matter_ota_settings.json JSON_STRING)
string(JSON VendorID GET ${JSON_STRING} VendorID)
string(JSON ProductID GET ${JSON_STRING} ProductID)
string(JSON Version GET ${JSON_STRING} Version)
string(JSON VersionString GET ${JSON_STRING} VersionString)
string(JSON DigestAlgorithm GET ${JSON_STRING} DigestAlgorithm)

set(ota_imagetool_dir ${ameba_matter_root}/src/app)
add_custom_command(TARGET ${app_ntz} POST_BUILD
    COMMAND python3 ../../python_custom_ecdsa_D_gcc.py ${matter_output_path}/asdk/image/km0_km4_image2.bin ${matter_output_path}/asdk/image/OTA_ALL.bin
    COMMAND echo "========= Generating OTA_ALL.bin Complete =========="
    COMMAND cd ${ota_imagetool_dir}
    COMMAND python3 ./ota_image_tool.py create -v ${VendorID} -p ${ProductID} -vn ${Version} -vs ${VersionString} -da ${DigestAlgorithm} ${matter_output_path}/asdk/image/OTA_ALL.bin ${matter_output_path}/asdk/image/MATTER_OTA_ALL.bin
    COMMAND echo "========= Generating MATTER_OTA_ALL.bin Complete =========="
    COMMAND python3 ./ota_image_tool.py show ${matter_output_path}/asdk/image/MATTER_OTA_ALL.bin
)
endif(matter_generate_ota_image)

