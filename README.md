# Matter (previously CHIP) on AmebaD

## Get AmebaD SDK (Tested on Ubuntu 20.04)

    mkdir ${HOME}/AmebaD_with_CHIP
    
    cd ${HOME}/AmebaD_with_CHIP
    
    git clone -b interop_testing_2022_01_18 https://github.com/pankore/ambd_sdk_with_chip_non_NDA.git
    
## Get Matter SDK

    cd ${HOME}/AmebaD_with_CHIP

    git clone https://github.com/project-chip/connectedhomeip.git


## Set Matter Build Environment 

    cd ${HOME}/AmebaD_with_CHIP/connectedhomeip

    source scripts/bootstrap.sh

    source scripts/activate.sh

    > Find more details to setup linux build environment
    > https://github.com/project-chip/connectedhomeip/blob/master/docs/guides/BUILDING.md

## Matter Ameba-D All Clusters Example

`cd ${HOME}/AmebaD_with_CHIP/connectedhomeip`

`$PWD/../ambd_sdk_with_chip_non_NDA/project/realtek_amebaD_va0_example/GCC-RELEASE/build.sh $PWD ninja $PWD/out all-clusters-app`
	
`cd out ; ninja`

## Matter Ameba-D Lighting Example

`cd ${HOME}/AmebaD_with_CHIP/connectedhomeip`

`$PWD/../ambd_sdk_with_chip_non_NDA/project/realtek_amebaD_va0_example/GCC-RELEASE/build.sh $PWD ninja $PWD/out lighting-app`
	
`cd out ; ninja`

## Flash Image on AmebaD EVB

Please refer [Application Note](https://github.com/pankore/ambd_sdk_with_chip_non_NDA/blob/master/doc/AN0400%20Ameba-D%20Application%20Note%20v14.pdf) Chapter 8 : Image Tool

    Image Tool Path : $(SDK_ROOT)/tools/AmebaD/Image_Tool/
    

## Run CHIP task on Ameba D (all-cluster-app example)
    enter command in console

	matter task will auto start after power on device.


## Test with [chip-tool](https://github.com/pankore/connectedhomeip/tree/master/examples/chip-tool)
Use standalone chip-tool app(linux) to communicate with the device.

In order to send commands to a device, it must be commissioned with the client. chip-tool currently only supports commissioning and remembering one device at a time. The configuration state is stored in `/tmp/chip_tool_config.ini`; deleting this and other `.ini` files in `/tmp` can sometimes resolve issues due to stale configuration.


### Commission a device over BLE

* Run CHIP-TOOL IP commissioning command `./chip-tool pairing ble-wifi ${NODE_ID_TO_ASSIGN} ${SSID} ${PASSWORD} 20202021 3840`
* For example: `./chip-tool pairing ble-wifi 12344321 testssid password 20202021 3840`

### Pair a device over IP

* Connect to AP using after matter task by `ATW0, ATW1, ATWC` commands
* Run CHIP-TOOL IP commissioning command `./chip-tool pairing onnetwork ${NODE_ID_TO_ASSIGN} 20202021`
* For example: `./chip-tool pairing onnetwork 12344321 20202021`

### Command for onoff cluster

Use PA_20 as output, connect a LED to this pin and GND.

* Run CHIP-TOOL on-off cluster command `./chip-tool onoff on 12344321 1`

* Run CHIP-TOOL on-off cluster command `./chip-tool onoff off 12344321 1`

## Test with [Python Controller](https://github.com/hank820/connectedhomeip/blob/master/docs/guides/python_chip_controller_building.md)
To build the Python Controller (linux), run the following command.

	./scripts/build_python.sh --chip_mdns platform

To launch Python Controller, activate the python environment first.
	
	source out/python_env/bin/activate
	chip-device-ctrl

### Commission a device over BLE
* Power on device and wait matter task run
* Run python controller BLE commissioning command `chip-device-ctrl > connect -ble 3840 20202021 135246`
* Provide network credentials `chip-device-ctrl > zcl NetworkCommissioning AddWiFiNetwork 135246 0 0 ssid=str:TESTSSID credentials=str:TESTPASSWD breadcrumb=0 timeoutMs=1000`
* Connect to AP `chip-device-ctrl > zcl NetworkCommissioning EnableNetwork 135246 0 0 networkID=str:TESTSSID breadcrumb=0 timeoutMs=1000`
* Close the BLE connection `chip-device-ctrl > close-ble`
* Resolve DNS-SD name and update address of the node in the device controller. `chip-device-ctrl > resolve 135246`

* On-Off cluster command `chip-device-ctrl >zcl OnOff On 135246 1 1`
* On-Off cluster command `chip-device-ctrl >zcl OnOff Off 135246 1 1`

### Pair a device over IP
* Power on device and wait matter task run
* Connect to AP using `ATW0, ATW1, ATWC` commands
* Run python controller IP commissioning command `chip-device-ctrl > connect -ip <IP> 20202021 135246`
* Resolve DNS-SD name and update address of the node in the device controller. `chip-device-ctrl > resolve 135246`
* On-Off cluster command `chip-device-ctrl >zcl OnOff On 135246 1 1`
* On-Off cluster command `chip-device-ctrl >zcl OnOff Off 135246 1 1`

## Matter Ameba-D All Clusters Example (by Makefile)

### Make Little CPU

`cd ${HOME}/AmebaD_with_CHIP/ambd_sdk_with_chip_non_NDA/project/realtek_amebaD_va0_example/GCC-RELEASE/project_lp`

`make all`

output : project/realtek_amebaD_va0_example/GCC-RELEASE/project_lp/asdk/image/km0_boot_all.bin

### Make libCHIP.a(matter core) and lib_main.a(matter example)

`cd ${HOME}/AmebaD_with_CHIP/ambd_sdk_with_chip_non_NDA/project/realtek_amebaD_va0_example/GCC-RELEASE/project_hp`

`make -C asdk lib_all`

output : ambd_sdk_with_chip_non_NDA/project/realtek_amebaD_va0_example/GCC-RELEASE/project_hp/asdk/lib/application

> libCHIP.a : matter core (generate by GN/ninja in connectedhomeip. Config by [chip/Makefile](https://github.com/pankore/ambd_sdk_with_chip_non_NDA/blob/main/project/realtek_amebaD_va0_example/GCC-RELEASE/project_hp/asdk/make/chip/Makefile))

> lib_main.a : matter example CHIP application (generate by [chip_main/Makefile](https://github.com/pankore/ambd_sdk_with_chip_non_NDA/blob/main/project/realtek_amebaD_va0_example/GCC-RELEASE/project_hp/asdk/make/chip_main/Makefile))

### Make Big CPU

`cd ${HOME}/AmebaD_with_CHIP/ambd_sdk_with_chip_non_NDA/project/realtek_amebaD_va0_example/GCC-RELEASE/project_hp`

`make all`

output : 

project/realtek_amebaD_va0_example/GCC-RELEASE/project_hp/asdk/image/km4_boot_all.bin

project/realtek_amebaD_va0_example/GCC-RELEASE/project_hp/asdk/image/km0_km4_image2.bin
