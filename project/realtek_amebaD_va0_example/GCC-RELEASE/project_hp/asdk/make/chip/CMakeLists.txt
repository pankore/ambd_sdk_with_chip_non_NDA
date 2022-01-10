cmake_minimum_required(VERSION 3.6)

project(chip)

set(sdk_root "${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../..")
set(prj_root "${CMAKE_CURRENT_SOURCE_DIR}/../../../../..")
set(dir_chip "${ameba_matter_root}")
set(dir_output "${dir_chip}/examples/all-clusters-app/ameba/build/chip")
set(target chip)
set(list chip_sources)

message(STATUS "Building \"${target}\" folder")

list(
    APPEND GLOBAL_CFLAGS

    -DCHIP_PROJECT=1

    -DCONFIG_PLATFORM_8721D
    -DCONFIG_USE_MBEDTLS_ROM_ALG
    -DCONFIG_FUNCION_O0_OPTIMIZE
    -DDM_ODM_SUPPORT_TYPE=32

    -DCHIP_DEVICE_LAYER_TARGET=Ameba
    -DMBEDTLS_CONFIG_FILE=\"mbedtls_config.h\"

    -DLWIP_IPV6_ND=1
    -DLWIP_IPV6_SCOPES=0
    -DLWIP_PBUF_FROM_CUSTOM_POOLS=0
    -DLWIP_IPV6_ROUTE_TABLE_SUPPORT=1

    -DCHIP_DEVICE_LAYER_NONE=0
    -DCHIP_SYSTEM_CONFIG_USE_ZEPHYR_NET_IF=0
    -DCHIP_SYSTEM_CONFIG_USE_BSD_IFADDRS=0
    -DCHIP_SYSTEM_CONFIG_USE_ZEPHYR_SOCKET_EXTENSIONS=0


    -DCHIP_SYSTEM_CONFIG_USE_LWIP=1
    -DCHIP_SYSTEM_CONFIG_USE_SOCKETS=0
    -DCHIP_SYSTEM_CONFIG_USE_NETWORK_FRAMEWORK=0
)

list(
    APPEND CHIP_CFLAGS

    ${GLOBAL_CFLAGS}
)

list(
    APPEND CHIP_CXXFLAGS

    #-std=gnu++11
    #-std=c++14
    #-fno-rtti
    -DFD_SETSIZE=10

    -Wno-sign-compare
    -Wno-unused-function
    -Wno-unused-but-set-variable
    -Wno-unused-variable
    -Wno-deprecated-declarations
    -Wno-unused-parameter
    -Wno-format

    ${GLOBAL_CFLAGS}
)

target_link_libraries(
	${target}
	${chip_sources} 
)
add_custom_command(
    TARGET ${target}
    PRE_BUILD

    COMMAND echo "INSTALL CHIP..."
    COMMAND mkdir -p ${dir_output}
	COMMAND echo                                   > ${dir_output}/args.gn
	COMMAND echo "import(\"//args.gni\")"          >> ${dir_output}/args.gn
	COMMAND echo target_cflags_c  = foreach(word ${CHIP_CFLAGS} \"${word}\") | sed -e 's/=\"/=\\"/g;s/\"\"/\\"\"/g;'  >> ${dir_output}/args.gn
	COMMAND echo target_cflags_cc = foreach(word ${CHIP_CXXFLAGS} \"${word}\") | sed -e 's/=\"/=\\"/g;s/\"\"/\\"\"/g;'   >> ${dir_output}/args.gn
	COMMAND echo ameba_ar = \"arm-none-eabi-ar\"    >> ${dir_output}/args.gn
	COMMAND echo ameba_cc = \"arm-none-eabi-gcc\"   >> ${dir_output}/args.gn
	COMMAND echo ameba_cxx = \"arm-none-eabi-c++\"  >> ${dir_output}/args.gn
	COMMAND echo ameba_cpu = \"ameba\"               >> ${dir_output}/args.gn
	COMMAND cd ${dir_chip} && PW_ENVSETUP_QUIET=1 . scripts/activate.sh
	COMMAND export PATH=${BASEDIR}/project/realtek_amebaD_va0_example/GCC-RELEASE/project_hp/toolchain/linux/asdk-10.3.0/linux/newlib/bin:${PATH}
	COMMAND mkdir -p ${dir_chip}/config/ameba/components/chip
	COMMAND cd ${dir_chip}/config/ameba/components/chip && gn gen --check --fail-on-unused-args ${dir_chip}/examples/all-clusters-app/ameba/build/chip
	COMMAND cd ${dir_chip}/config/ameba/components/chip ; ninja -C ${dir_chip}/examples/all-clusters-app/ameba/build/chip
    COMMAND cp -f ${dir_output}/lib/* ${CMAKE_CURRENT_SOURCE_DIR}/../../lib/application/
)

target_include_directories(
    ${target}
    PUBLIC

    ${dir_chip}/config/ameba
    ${dir_chip}/src/include
    ${dir_chip}/src/lib
    ${dir_chip}/src
    ${dir_chip}/src/system
    ${dir_chip}/src/app
    ${dir_chip}/third_party/nlassert/repo/include
    ${dir_chip}/third_party/nlio/repo/include
    ${dir_chip}/third_party/nlunit-test/repo/src
    ${BASEDIR}/component/common/bluetooth/realtek/sdk/example/bt_matter_adapter
)

