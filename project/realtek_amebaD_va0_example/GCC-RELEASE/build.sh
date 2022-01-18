#!/usr/bin/env bash

#   ./build.sh {MATTER DIR} {BUILD METHOD} {OUTPUT DIR} {APP_NAME} {rpc (optional)}

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

if [ "$4" == "all-clusters-app" ]; then
    export MATTER_EXAMPLE_PATH=${AMEBA_MATTER}/examples/all-clusters-app/ameba
    export MATTER_ENABLE_OTA_REQUESTOR=1
elif [ "$4" == "lighting-app" ]; then
    export MATTER_EXAMPLE_PATH=${AMEBA_MATTER}/examples/lighting-app/ameba
    export MATTER_ENABLE_OTA_REQUESTOR=1
elif [ "$4" == "pigweed-app" ]; then
    export MATTER_EXAMPLE_PATH=${AMEBA_MATTER}/examples/pigweed-app/ameba
    export MATTER_ENABLE_RPC=1
elif [ "$4" == "ota-provider-app" ]; then
    export MATTER_EXAMPLE_PATH=${AMEBA_MATTER}/examples/ota-provider-app/ameba
elif [ "$4" == "ota-requestor-app" ]; then
    export MATTER_EXAMPLE_PATH=${AMEBA_MATTER}/examples/ota-requestor-app/ameba
    export MATTER_ENABLE_OTA_REQUESTOR=1
else
    export MATTER_EXAMPLE_PATH=${AMEBA_MATTER}/examples/all-clusters-app/ameba
fi
echo "MATTER_EXAMPLE_PATH at: ${MATTER_EXAMPLE_PATH}"

if [ "$5" == "rpc" ]; then
    export MATTER_ENABLE_RPC=1
fi

## Check output directory
if [ ! -z "$3" ]; then
    export MATTER_OUTPUT="$3"
    mkdir -p "$MATTER_OUTPUT"
fi
cd "$MATTER_OUTPUT"

function exe_cmake()
{
	if [ "$4" == "all-clusters-app" ]; then
	    exe_cmake_all
	elif [ "$4" == "lighting-app" ]; then
	    exe_cmake_light
	elif [ "$4" == "ota-requestor-app" ]; then
	    exe_cmake_otar
	elif [ "$4" == "ota-provider-app" ]; then
	    exe_cmake_otap
	else
	    exe_cmake_all
	fi
}

function exe_cmake_all()
{
	echo "Build all"
	cmake $CMAKE_ROOT -G"$BUILD_METHOD" -DCMAKE_TOOLCHAIN_FILE=$CMAKE_ROOT/toolchain.cmake -DMATTER_ALL_CLUSTERS_APP=1
}

function exe_cmake_light()
{
	echo "Build light"
	cmake $CMAKE_ROOT -G"$BUILD_METHOD" -DCMAKE_TOOLCHAIN_FILE=$CMAKE_ROOT/toolchain.cmake -DMATTER_LIGHTING_APP=1
}

function exe_cmake_otar()
{
	echo "Build OTA-R"
	cmake $CMAKE_ROOT -G"$BUILD_METHOD" -DCMAKE_TOOLCHAIN_FILE=$CMAKE_ROOT/toolchain.cmake -DMATTER_OTA_REQUESTOR_APP=1
}

function exe_cmake_otap()
{
	echo "Build OTA-P"
	cmake $CMAKE_ROOT -G"$BUILD_METHOD" -DCMAKE_TOOLCHAIN_FILE=$CMAKE_ROOT/toolchain.cmake -DMATTER_OTA_PROVIDER_APP=1
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
