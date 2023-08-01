set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)
set(CMAKE_ASM_COMPILER_WORKS 1)

# root of realtek_amebapro2_v0_example
set (prj_root "${CMAKE_CURRENT_SOURCE_DIR}")
# root of SDK

set(TOOLCHAINDIR "${prj_root}/toolchain")
set(ASDK_TOOLCHAIN "${TOOLCHAINDIR}/linux/asdk-10.3.0/linux/newlib")
set(TOOL_DIR "${ASDK_TOOLCHAIN}/bin")

message(STATUS "show the toolchain path ${TOOL_DIR}")

set(CMAKE_C_COMPILER "${TOOL_DIR}/arm-none-eabi-gcc" )
set(CMAKE_CXX_COMPILER "${TOOL_DIR}/arm-none-eabi-g++" )
set(CMAKE_ASM_COMPILER "${TOOL_DIR}/arm-none-eabi-gcc" )
set(CMAKE_OBJECOPY "${TOOL_DIR}/arm-none-eabi-objcopy" )
set(CMAKE_OBJEDUMP "${TOOL_DIR}/arm-none-eabi-objdump" )
set(CMAKE_STRIP "${TOOL_DIR}/arm-none-eabi-strip" )
set(CMAKE_AR "${TOOL_DIR}/arm-none-eabi-ar" )
set(CMAKE_AS "${TOOL_DIR}/arm-none-eabi-as" )
set(CMAKE_NM "${TOOL_DIR}/arm-none-eabi-nm" )
set(CMAKE_SIZE "${TOOL_DIR}/arm-none-eabi-size" )
#set(CMAKE_C_LINK_EXECUTABLE "${TOOL_DIR}/arm-none-eabi-gcc" )

add_definitions(-D__thumb2__ -DCONFIG_PLATFORM_8721D -DARM_MATH_ARMV8MML -D__FPU_PRESENT -D__ARM_ARCH_7M__=0 -D__ARM_ARCH_7EM__=0 -D__ARM_ARCH_8M_MAIN__=1 -D__ARM_ARCH_8M_BASE__=0 -D__ARM_FEATURE_FP16_SCALAR_ARITHMETIC=1 -D__DSP_PRESENT=1 -D__ARMVFP__)

add_definitions(-DCONFIG_USE_MBEDTLS_ROM_ALG -DCONFIG_FUNCION_O0_OPTIMIZE -DDM_ODM_SUPPORT_TYPE=32 -DLWIP_IPV6_ROUTE_TABLE_SUPPORT=1)

#set(CMAKE_C_FLAGS "-march=armv8-m.main+dsp+fp -mcpu=real-m500+fp -mthumb -mcmse -mfpu=fpv5-sp-d16 -mfp16-format=ieee -mfloat-abi=softfp ")
set(CMAKE_C_FLAGS "-march=armv8-m.main+dsp -mthumb -mcmse -mfpu=fpv5-sp-d16 -mfloat-abi=hard")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -ffunction-sections -fstack-usage -fdata-sections -fno-optimize-sibling-calls")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -gdwarf-3 -MMD -nostartfiles -nodefaultlibs -nostdlib ")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wpointer-arith -Wno-write-strings -Wno-maybe-uninitialized -fdiagnostics-color=always ")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_C_FLAGS} -std=gnu++17 -fno-rtti")

set(CMAKE_ASM_FLAGS "-march=armv8-m.main -mthumb -mfpu=fpv5-sp-d16 -mfloat-abi=softfp -x assembler-with-cpp")

list(
	APPEND GLOBAL_C_FLAGS
	-march=armv8-m.main+dsp
	-mthumb
	-mcmse
	-mfpu=fpv5-sp-d16
	-mfloat-abi=hard
	-O2
	-ffunction-sections
	-fstack-usage
	-fdata-sections
	-fno-optimize-sibling-calls
	-g
	-gdwarf-3
	-MMD
	-nostartfiles
	-nodefaultlibs
	-nostdlib
	-Wall
	-Wpointer-arith
	-Wno-write-strings
#	-Wno-maybe-uninitialized
	-fdiagnostics-color=always
	-Wno-stringop-truncation
)

list(
	APPEND GLOBAL_CPP_FLAGS
	-std=c++17
	-fno-rtti
)

#list(JOIN _wrapper " " function_wrapper)

#set(CMAKE_EXE_LINKER_FLAGS "${function_wrapper}" CACHE INTERNAL "")
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -nostartfiles -nodefaultlibs -nostdlib --specs=nosys.specs" CACHE INTERNAL "")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -nostartfiles -nodefaultlibs -nostdlib" CACHE INTERNAL "")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--gc-sections -Wl,--warn-section-align -Wl,--cref -Wl,--build-id=none -Wl,--use-blx" CACHE INTERNAL "")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Map=text.map -Wl,--no-enum-size-warning -Wl,--warn-common " CACHE INTERNAL "")
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -T ${CMAKE_CURRENT_SOURCE_DIR}/rtl8735b_fpga.ld -Wl,-Map=text.map " CACHE INTERNAL "")

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)



