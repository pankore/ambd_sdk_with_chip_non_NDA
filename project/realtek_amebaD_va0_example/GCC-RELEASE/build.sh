#!/usr/bin/env bash

#   ./build.sh -g -r {MATTER DIR} {BUILD METHOD} {OUTPUT DIR} {APP_NAME} 
#   -g and -r are optional flags, input them as the first two arguments
#       -g: Generate MATTER_OTA_ALL.bin
#       -r: Enable RPC

## Build options
while getopts :gr opt; do
    case $opt in
        g) export MATTER_GENERATE_OTA_IMAGE=1 ;;
        r) export MATTER_ENABLE_RPC=1 ;;
        :) echo "Missing argument for option -$OPTARG"; exit 1;;
       \?) echo "Unknown option -$OPTARG"; exit 1;;
    esac
done
shift $(( OPTIND - 1 ))

# If RPC is not enabled, then enable Matter Shell, Vice Versa
if [[ -z "${MATTER_ENABLE_RPC}" ]]; then
    export MATTER_ENABLE_RPC=0
    echo "MATTER RPC DISABLED"
    echo "MATTER SHELL ENABLED"
else
    echo "MATTER RPC ENABLED"
    echo "MATTER SHELL DISABLED"
fi

ENABLE_PERSISTENTSTORAGE_AUDIT=0
export MATTER_ENABLE_PERSISTENTSTORAGE_AUDIT=$ENABLE_PERSISTENTSTORAGE_AUDIT

BUILD_FILE_DIR=`test -d ${0%/*} && cd ${0%/*}; pwd`
CMAKE_ROOT=$BUILD_FILE_DIR/project_hp
LP_IMAGE=$BUILD_FILE_DIR/project_lp/asdk/image
HP_IMAGE=$BUILD_FILE_DIR/project_hp/asdk/image

## Unzip toolchain
cd $CMAKE_ROOT/toolchain
mkdir linux
if [ -z "$(ls -A $CMAKE_ROOT/toolchain/linux)" ]; then
   cat asdk/asdk-10.3.0-linux-newlib-build-3638-x86_64.tar.bz2.part* > asdk/asdk-10.3.0-linux-newlib-build-3638-x86_64.tar.bz2
    tar -jxvf asdk/asdk-10.3.0-linux-newlib-build-3638-x86_64.tar.bz2 -C linux/
    rm asdk/asdk-10.3.0-linux-newlib-build-3638-x86_64.tar.bz2
else
   echo "Toolchain $(ls -A $CMAKE_ROOT/toolchain/linux) is found at $CMAKE_ROOT/toolchain/linux."
fi

# AMEBAD build
export MATTER_PLATFORM_8721D=1

## AMEBA_MATTER to be exported or manually keyed in.
if [ ! -z ${AMEBA_MATTER} ]; then
    echo "Matter SDK is located at: ${AMEBA_MATTER}"
elif [ -d "$1" ]; then
    echo "Matter SDK is located at: "$1""
    export AMEBA_MATTER="$1"
else
    echo "Error: Unknown path for Matter SDK."
    exit
fi

export MATTER_CONFIG_PATH=${AMEBA_MATTER}/config/ameba

APP_SELECT="$4"
echo "********** You are building $APP_SELECT **********"

if [ $APP_SELECT == "all-clusters-app" ]; then
    export MATTER_EXAMPLE_PATH=${AMEBA_MATTER}/examples/all-clusters-app/ameba
    export MATTER_ENABLE_OTA_REQUESTOR=1
elif [ $APP_SELECT == "all-clusters-minimal-app" ]; then
    export MATTER_EXAMPLE_PATH=${AMEBA_MATTER}/examples/all-clusters-minimal-app/ameba
    export MATTER_ENABLE_OTA_REQUESTOR=1
elif [ $APP_SELECT == "air-purifier-app" ]; then
    export MATTER_EXAMPLE_PATH=${AMEBA_MATTER}/examples/air-purifier-app/ameba
    export MATTER_ENABLE_OTA_REQUESTOR=1
elif [ $APP_SELECT == "lighting-app" ]; then
    export MATTER_EXAMPLE_PATH=${AMEBA_MATTER}/examples/lighting-app/ameba
    export MATTER_ENABLE_OTA_REQUESTOR=1
elif [ $APP_SELECT == "pigweed-app" ]; then
    export MATTER_EXAMPLE_PATH=${AMEBA_MATTER}/examples/pigweed-app/ameba
elif [ $APP_SELECT == "ota-provider-app" ]; then
    export MATTER_EXAMPLE_PATH=${AMEBA_MATTER}/examples/ota-provider-app/ameba
elif [ $APP_SELECT == "ota-requestor-app" ]; then
    export MATTER_EXAMPLE_PATH=${AMEBA_MATTER}/examples/ota-requestor-app/ameba
    export MATTER_ENABLE_OTA_REQUESTOR=1
elif [ $APP_SELECT == "light-switch-app" ]; then
    export MATTER_EXAMPLE_PATH=${AMEBA_MATTER}/examples/light-switch-app/ameba
    export MATTER_ENABLE_OTA_REQUESTOR=1
elif [ $APP_SELECT == "chef-app" ]; then
    export MATTER_EXAMPLE_PATH=${AMEBA_MATTER}/examples/chef/ameba
else
    export MATTER_EXAMPLE_PATH=${AMEBA_MATTER}/examples/all-clusters-app/ameba
    export MATTER_ENABLE_OTA_REQUESTOR=1
fi
echo "MATTER_EXAMPLE_PATH at: ${MATTER_EXAMPLE_PATH}"

## Check output directory
if [ ! -z "$3" ]; then
    export MATTER_OUTPUT="$3"
    mkdir -p "$MATTER_OUTPUT"
fi
cd "$MATTER_OUTPUT"

function exe_cmake()
{
	if [ $APP_SELECT == "all-clusters-app" ]; then
	    exe_cmake_all
	elif [ $APP_SELECT == "lighting-app" ]; then
	    exe_cmake_light
	elif [ $APP_SELECT == "pigweed-app" ]; then
	    exe_cmake_pigweed
	elif [ $APP_SELECT == "ota-requestor-app" ]; then
	    exe_cmake_otar
	elif [ $APP_SELECT == "ota-provider-app" ]; then
	    exe_cmake_otap
	elif [ $APP_SELECT == "light-switch-app" ]; then
	    exe_cmake_switch
	elif [ $APP_SELECT == "chef-app" ]; then
	    exe_cmake_chef
	else
	    exe_cmake_all
	fi
}

# -DCMAKE_EXPORT_COMPILE_COMMANDS=1 will generate a compile_commands.json file under the output directory
# To use LSP, copy this compile_commands.json file to the parent directory of both
# ambd_sdk_with_chip_non_NDA and connectedhomeip
function exe_cmake_all()
{
	echo "Build all"
	cmake $CMAKE_ROOT -G"$BUILD_METHOD" -DCMAKE_TOOLCHAIN_FILE=$CMAKE_ROOT/toolchain.cmake -DMATTER_ALL_CLUSTERS_APP=ON -DCMAKE_EXPORT_COMPILE_COMMANDS=1
}

function exe_cmake_light()
{
	echo "Build light"
	cmake $CMAKE_ROOT -G"$BUILD_METHOD" -DCMAKE_TOOLCHAIN_FILE=$CMAKE_ROOT/toolchain.cmake -DMATTER_LIGHTING_APP=ON -DCMAKE_EXPORT_COMPILE_COMMANDS=1
}

function exe_cmake_pigweed()
{
	echo "Build pigweed"
	cmake $CMAKE_ROOT -G"$BUILD_METHOD" -DCMAKE_TOOLCHAIN_FILE=$CMAKE_ROOT/toolchain.cmake -DMATTER_PIGWEED_APP=ON -DCMAKE_EXPORT_COMPILE_COMMANDS=1
}

function exe_cmake_otar()
{
	echo "Build OTA-R"
	cmake $CMAKE_ROOT -G"$BUILD_METHOD" -DCMAKE_TOOLCHAIN_FILE=$CMAKE_ROOT/toolchain.cmake -DMATTER_OTA_REQUESTOR_APP=ON -DCMAKE_EXPORT_COMPILE_COMMANDS=1
}

function exe_cmake_otap()
{
	echo "Build OTA-P"
	cmake $CMAKE_ROOT -G"$BUILD_METHOD" -DCMAKE_TOOLCHAIN_FILE=$CMAKE_ROOT/toolchain.cmake -DMATTER_OTA_PROVIDER_APP=ON -DCMAKE_EXPORT_COMPILE_COMMANDS=1
}

function exe_cmake_switch()
{
	echo "Build switch"
	cmake $CMAKE_ROOT -G"$BUILD_METHOD" -DCMAKE_TOOLCHAIN_FILE=$CMAKE_ROOT/toolchain.cmake -DMATTER_LIGHT_SWITCH_APP=ON -DCMAKE_EXPORT_COMPILE_COMMANDS=1
}

function exe_cmake_chef()
{
	echo "Build chef"
	cmake $CMAKE_ROOT -G"$BUILD_METHOD" -DCMAKE_TOOLCHAIN_FILE=$CMAKE_ROOT/toolchain.cmake -DMATTER_CHEF_APP=ON -DCMAKE_EXPORT_COMPILE_COMMANDS=1
}

## Decide meta build method
if [[ "$2" == "ninja" || "$2" == "Ninja" ]]; then
	BUILD_METHOD="Ninja"
	exe_cmake
	#ninja
else
	BUILD_METHOD="Unix Makefiles"
    exe_cmake
	#make
fi

## Copy bootloaders
if [ ! -d "$MATTER_OUTPUT/asdk/bootloader" ]; then
        mkdir -p $MATTER_OUTPUT/asdk/bootloader
fi

if [ -a "$LP_IMAGE/km0_boot_all.bin" ]; then
    cp $LP_IMAGE/km0_boot_all.bin $MATTER_OUTPUT/asdk/bootloader/km0_boot_all.bin
else
    echo "Error: km0_boot_all.bin can not be found."
fi

if [ -a "$HP_IMAGE/km4_boot_all.bin" ]; then
    cp $HP_IMAGE/km4_boot_all.bin $MATTER_OUTPUT/asdk/bootloader/km4_boot_all.bin
else
    echo "Error: km4_boot_all.bin can not be found."
fi
