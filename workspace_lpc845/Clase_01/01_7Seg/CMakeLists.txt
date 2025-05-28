# CROSS COMPILER SETTING
SET(CMAKE_SYSTEM_NAME Generic)
CMAKE_MINIMUM_REQUIRED (VERSION 3.10.0)

# THE VERSION NUMBER
SET (Tutorial_VERSION_MAJOR 1)
SET (Tutorial_VERSION_MINOR 0)

if (DEFINED ENV{SdkRootDirPath})
	SET(SdkRootDirPath "$ENV{SdkRootDirPath}")
endif()

if (NOT DEFINED SdkRootDirPath)
	MESSAGE(FATAL_ERROR "SDK path could not be determined. Please update SdkRootDirPath value from your CMake presets file(s).")
endif()

# ENABLE ASM
ENABLE_LANGUAGE(ASM)

# RESET FLAG VALUES
SET(CMAKE_ASM_FLAGS_DEBUG "")
SET(CMAKE_ASM_FLAGS_RELEASE "")
SET(CMAKE_C_FLAGS_DEBUG "")
SET(CMAKE_C_FLAGS_RELEASE "")
SET(CMAKE_CXX_FLAGS_DEBUG "")
SET(CMAKE_CXX_FLAGS_RELEASE "")
SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "")
SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "")

SET(CMAKE_STATIC_LIBRARY_PREFIX)
SET(CMAKE_STATIC_LIBRARY_SUFFIX)

SET(CMAKE_EXECUTABLE_LIBRARY_PREFIX)
SET(CMAKE_EXECUTABLE_LIBRARY_SUFFIX)

# CURRENT DIRECTORY
SET(ProjDirPath ${CMAKE_CURRENT_SOURCE_DIR})

SET(EXECUTABLE_OUTPUT_PATH ${ProjDirPath}/${CMAKE_BUILD_TYPE})
SET(LIBRARY_OUTPUT_PATH ${ProjDirPath}/${CMAKE_BUILD_TYPE})

set(CMAKE_USER_MAKE_RULES_OVERRIDE "user_override.cmake")

project(LPC845_Project)
set(MCUX_SDK_PROJECT_NAME LPC845_Project.elf)

include(${ProjDirPath}/flags.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} "${ProjDirPath}/component/uart/fsl_adapter_uart.h"
"${ProjDirPath}/component/uart/fsl_adapter_miniusart.c"
"${ProjDirPath}/startup/startup_lpc845.c"
"${ProjDirPath}/CMSIS/core_cm0plus.h"
"${ProjDirPath}/CMSIS/cmsis_version.h"
"${ProjDirPath}/CMSIS/cmsis_gcc.h"
"${ProjDirPath}/CMSIS/cmsis_compiler.h"
"${ProjDirPath}/CMSIS/m-profile/cmsis_gcc_m.h"
"${ProjDirPath}/CMSIS/m-profile/armv7m_mpu.h"
"${ProjDirPath}/source/semihost_hardfault.c"
"${ProjDirPath}/source/mtb.c"
"${ProjDirPath}/source/LPC845_Project.c"
"${ProjDirPath}/utilities/fsl_assert.h"
"${ProjDirPath}/utilities/fsl_assert.c"
"${ProjDirPath}/utilities/str/fsl_str.h"
"${ProjDirPath}/utilities/str/fsl_str.c"
"${ProjDirPath}/utilities/debug_console_lite/fsl_debug_console.h"
"${ProjDirPath}/utilities/debug_console_lite/fsl_debug_console.c"
"${ProjDirPath}/drivers/fsl_usart.h"
"${ProjDirPath}/drivers/fsl_usart.c"
"${ProjDirPath}/drivers/fsl_syscon_connections.h"
"${ProjDirPath}/drivers/fsl_swm_connections.h"
"${ProjDirPath}/drivers/fsl_swm.h"
"${ProjDirPath}/drivers/fsl_swm.c"
"${ProjDirPath}/drivers/fsl_reset.h"
"${ProjDirPath}/drivers/fsl_reset.c"
"${ProjDirPath}/drivers/fsl_power.h"
"${ProjDirPath}/drivers/fsl_power.c"
"${ProjDirPath}/drivers/fsl_iocon.h"
"${ProjDirPath}/drivers/fsl_gpio.h"
"${ProjDirPath}/drivers/fsl_gpio.c"
"${ProjDirPath}/drivers/fsl_common_arm.h"
"${ProjDirPath}/drivers/fsl_common_arm.c"
"${ProjDirPath}/drivers/fsl_common.h"
"${ProjDirPath}/drivers/fsl_common.c"
"${ProjDirPath}/drivers/fsl_clock.h"
"${ProjDirPath}/drivers/fsl_clock.c"
"${ProjDirPath}/device/system_LPC845.h"
"${ProjDirPath}/device/system_LPC845.c"
"${ProjDirPath}/device/fsl_device_registers.h"
"${ProjDirPath}/device/LPC845_features.h"
"${ProjDirPath}/device/LPC845_COMMON.h"
"${ProjDirPath}/device/LPC845.h"
"${ProjDirPath}/device/periph2/PERI_WWDT.h"
"${ProjDirPath}/device/periph2/PERI_WKT.h"
"${ProjDirPath}/device/periph2/PERI_USART.h"
"${ProjDirPath}/device/periph2/PERI_SYSCON.h"
"${ProjDirPath}/device/periph2/PERI_SWM.h"
"${ProjDirPath}/device/periph2/PERI_SPI.h"
"${ProjDirPath}/device/periph2/PERI_SCT.h"
"${ProjDirPath}/device/periph2/PERI_PMU.h"
"${ProjDirPath}/device/periph2/PERI_PINT.h"
"${ProjDirPath}/device/periph2/PERI_MTB.h"
"${ProjDirPath}/device/periph2/PERI_MRT.h"
"${ProjDirPath}/device/periph2/PERI_IOCON.h"
"${ProjDirPath}/device/periph2/PERI_INPUTMUX.h"
"${ProjDirPath}/device/periph2/PERI_I2C.h"
"${ProjDirPath}/device/periph2/PERI_GPIO.h"
"${ProjDirPath}/device/periph2/PERI_FLASH_CTRL.h"
"${ProjDirPath}/device/periph2/PERI_DMA.h"
"${ProjDirPath}/device/periph2/PERI_DAC.h"
"${ProjDirPath}/device/periph2/PERI_CTIMER.h"
"${ProjDirPath}/device/periph2/PERI_CRC.h"
"${ProjDirPath}/device/periph2/PERI_CAPT.h"
"${ProjDirPath}/device/periph2/PERI_ADC.h"
"${ProjDirPath}/device/periph2/PERI_ACOMP.h"
"${ProjDirPath}/board/pin_mux.h"
"${ProjDirPath}/board/pin_mux.c"
"${ProjDirPath}/board/peripherals.h"
"${ProjDirPath}/board/peripherals.c"
"${ProjDirPath}/board/clock_config.h"
"${ProjDirPath}/board/clock_config.c"
"${ProjDirPath}/board/board.h"
"${ProjDirPath}/board/board.c"
)




if(${LIBRARY_TYPE} STREQUAL "REDLIB")
    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE $ENV{ARMGCC_DIR}/redlib/include)
endif()

set(c_include_directories "${ProjDirPath}/board"
"${ProjDirPath}/source"
"${ProjDirPath}/component/uart"
"${ProjDirPath}/drivers"
"${ProjDirPath}/CMSIS"
"${ProjDirPath}/CMSIS/m-profile"
"${ProjDirPath}/device"
"${ProjDirPath}/utilities/debug_console_lite"
"${ProjDirPath}/utilities/str"
"${ProjDirPath}/utilities"
"${ProjDirPath}/device/periph2"
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE $<$<COMPILE_LANGUAGE:C>:${c_include_directories}>)

set(cpp_include_directories "")


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE $<$<COMPILE_LANGUAGE:CXX>:${cpp_include_directories}>)

set(include_files "")


target_precompile_headers(${MCUX_SDK_PROJECT_NAME} PRIVATE ${include_files})

set(link_directories "")


set(link_libraries "")


target_link_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE ${link_directories})
target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)
target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${TARGET_LINK_SYSTEM_LIBRARIES} ${link_libraries})
target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)





