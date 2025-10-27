# CROSS COMPILER SETTING
SET(CMAKE_SYSTEM_NAME Generic)
CMAKE_MINIMUM_REQUIRED (VERSION 3.10.0)

# THE VERSION NUMBER
SET (MCUXPRESSO_CMAKE_FORMAT_MAJOR_VERSION 2)
SET (MCUXPRESSO_CMAKE_FORMAT_MINOR_VERSION 0)

if(CMAKE_SCRIPT_MODE_FILE)
  message("${MCUXPRESSO_CMAKE_FORMAT_MAJOR_VERSION}")
  return()
endif()


# ENABLE ASM
ENABLE_LANGUAGE(ASM)

SET(CMAKE_STATIC_LIBRARY_PREFIX)
SET(CMAKE_STATIC_LIBRARY_SUFFIX)

SET(CMAKE_EXECUTABLE_LIBRARY_PREFIX)
SET(CMAKE_EXECUTABLE_LIBRARY_SUFFIX)

# CURRENT DIRECTORY
SET(ProjDirPath ${CMAKE_CURRENT_SOURCE_DIR})

SET(EXECUTABLE_OUTPUT_PATH ${ProjDirPath}/${CMAKE_BUILD_TYPE})
SET(LIBRARY_OUTPUT_PATH ${ProjDirPath}/${CMAKE_BUILD_TYPE})


project(satelite_exp_v100)

set(MCUX_BUILD_TYPES debug release)
set(MCUX_SDK_PROJECT_NAME payexp_m33.elf)


if (NOT DEFINED SdkRootDirPath)
    SET(SdkRootDirPath ${ProjDirPath}/../../../../..)
endif()

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

# USER DEFINE DEV DIRECTORY
set(UserSourceDirPath ${ProjDirPath}/../)
set(SRC_ROOT "${ProjDirPath}/..")
add_executable(${MCUX_SDK_PROJECT_NAME}

    # ==== Core ====
    "${SRC_ROOT}/Core/Src/board.c"
    "${SRC_ROOT}/Core/Src/clock_config.c"
    "${SRC_ROOT}/Core/Src/main.c"
    "${SRC_ROOT}/Core/Src/MIMX9352cm33_it.c"
    "${SRC_ROOT}/Core/Src/pin_mux.c"

    # ==== 01_DEV / M0_App ====
    "${SRC_ROOT}/01_DEV/M0_App/Boot/boot_manager.c"
    "${SRC_ROOT}/01_DEV/M0_App/OS/Kernel/rtos.c"
    "${SRC_ROOT}/01_DEV/M0_App/OS/Task/rtos_tasks.c"

    # ==== 01_DEV / M1_SysApp ====
    "${SRC_ROOT}/01_DEV/M1_SysApp/Interface/RemoteCall/remote_call.c"
    "${SRC_ROOT}/01_DEV/M1_SysApp/Interface/SimpleShell/simple_shell.c"
    "${SRC_ROOT}/01_DEV/M1_SysApp/Services/cmdline.c"
    "${SRC_ROOT}/01_DEV/M1_SysApp/system_data/m33_command.c"
    "${SRC_ROOT}/01_DEV/M1_SysApp/system_data/m33_data.c"
    "${SRC_ROOT}/01_DEV/M1_SysApp/system_data/command_list.c"
    "${SRC_ROOT}/01_DEV/M1_SysApp/task_update_onboard_adc/task_update_onboard_adc.c"
    "${SRC_ROOT}/01_DEV/M1_SysApp/task_temperature/app_temperature.c"
    "${SRC_ROOT}/01_DEV/M1_SysApp/task_experiment/task_experiment.c"
    "${SRC_ROOT}/01_DEV/M1_SysApp/task_system_control/task_system_control.c"

    # ==== 01_DEV / M2_BSP ====
    "${SRC_ROOT}/01_DEV/M2_BSP/remoteproc/m33_remoteproc.c"
    "${SRC_ROOT}/01_DEV/M2_BSP/remoteproc/rsc_table.c"
    "${SRC_ROOT}/01_DEV/M2_BSP/rpmsg/m33_rpmsg.c"
    #     # ---- BSP Core & Board ----
    "${SRC_ROOT}/01_DEV/M2_BSP/bsp_board/bsp.c"
    "${SRC_ROOT}/01_DEV/M2_BSP/bsp_core/bsp_core.c"

    # ---- BSP Peripheral ----
    "${SRC_ROOT}/01_DEV/M2_BSP/bsp_peripheral/bsp_debug_console/bsp_debug_console.c"
    "${SRC_ROOT}/01_DEV/M2_BSP/bsp_peripheral/bsp_expander/bsp_expander.c"
    "${SRC_ROOT}/01_DEV/M2_BSP/bsp_peripheral/bsp_heater/bsp_heater.c"
    "${SRC_ROOT}/01_DEV/M2_BSP/bsp_peripheral/bsp_i2c_sensor/bsp_i2c_sensor.c"
    "${SRC_ROOT}/01_DEV/M2_BSP/bsp_peripheral/bsp_laser/bsp_laser.c"
    #"${SRC_ROOT}/01_DEV/M2_BSP/bsp_peripheral/bsp_libcsp_can/bsp_libcsp_can.c"
     "${SRC_ROOT}/01_DEV/M2_BSP/bsp_peripheral/bsp_onboard_adc/bsp_onboard_adc.c"
    "${SRC_ROOT}/01_DEV/M2_BSP/bsp_peripheral/bsp_photo/bsp_photo.c"
    "${SRC_ROOT}/01_DEV/M2_BSP/bsp_peripheral/bsp_power/bsp_power.c"
    "${SRC_ROOT}/01_DEV/M2_BSP/bsp_peripheral/bsp_spi_sensor/bsp_spi_sensor.c"

    # ==== 01_DEV / M3_Devices ====
    "${SRC_ROOT}/01_DEV/M3_Devices/components/delay/nxp/nxp_delay.c"
    "${SRC_ROOT}/01_DEV/M3_Devices/components/dio/nxp/do.c"
    "${SRC_ROOT}/01_DEV/M3_Devices/components/spi/spi_io.c"
    "${SRC_ROOT}/01_DEV/M3_Devices/components/spi/nxp/spi_io_nxp.c"
    "${SRC_ROOT}/01_DEV/M3_Devices/components/i2c/i2c_io.c"
    "${SRC_ROOT}/01_DEV/M3_Devices/components/i2c/nxp/nxp_i2c_io.c"
     "${SRC_ROOT}/01_DEV/M3_Devices/components/ntc/ntc.c"
     "${SRC_ROOT}/01_DEV/M3_Devices/components/sp_spi/port/NXP/sp_spi_port.c"
    "${SRC_ROOT}/01_DEV/M3_Devices/components/sp_gpio/port/NXP/sp_gpio_port.c"
 
    "${SRC_ROOT}/01_DEV/M3_Devices/components/sp_uart/port/NXP/sp_uart_port.c"

    "${SRC_ROOT}/01_DEV/M3_Devices/devices/AD4114/ad4114.c"
    "${SRC_ROOT}/01_DEV/M3_Devices/devices/ADG1414BR/adg1414.c"
    "${SRC_ROOT}/01_DEV/M3_Devices/devices/AS3004204/as3004.c"
    "${SRC_ROOT}/01_DEV/M3_Devices/devices/LT8722/lt8722.c"
    "${SRC_ROOT}/01_DEV/M3_Devices/devices/MCP4902/mcp4902.c"
    "${SRC_ROOT}/01_DEV/M3_Devices/devices/pca9685/pca9685.c"
    "${SRC_ROOT}/01_DEV/M3_Devices/devices/RV3129/rv3129.c"
    "${SRC_ROOT}/01_DEV/M3_Devices/devices/tca6416/tca6416.c"

    # ==== 01_DEV / M4_Middlewares ====
    "${SRC_ROOT}/01_DEV/M4_Middlewares/os/freertos/osDelay.c"
    "${SRC_ROOT}/01_DEV/M4_Middlewares/os/freertos/osMalloc.c"
    "${SRC_ROOT}/01_DEV/M4_Middlewares/os/freertos/osQueue.c"
    "${SRC_ROOT}/01_DEV/M4_Middlewares/os/freertos/osScheduler.c"
    "${SRC_ROOT}/01_DEV/M4_Middlewares/os/freertos/osSemphr.c"
    "${SRC_ROOT}/01_DEV/M4_Middlewares/os/freertos/osThread.c"
    "${SRC_ROOT}/01_DEV/M4_Middlewares/printf/printf.c"
    "${SRC_ROOT}/01_DEV/M4_Middlewares/ring_char_buffer/ring_char_buffer.c"
    "${SRC_ROOT}/01_DEV/M4_Middlewares/ring_buffer/ring_buffer.c"


    # ==== 01_DEV / M5_Utils ====
    "${SRC_ROOT}/01_DEV/M5_Utils/CRC/crc16_xmodem.c"
    "${SRC_ROOT}/01_DEV/M5_Utils/DateTime/date_time.c"

    # (Các module M6_Tools, M7_Tests hiện chỉ có CMakeLists.txt, không có file .c)
)
target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/..

    # User define
    # App
    # Core
    "${SRC_ROOT}/Core/Inc"
    "${SRC_ROOT}/Core/Src"
    "${SRC_ROOT}/Core/Startup"

    # M0_App
    "${SRC_ROOT}/01_DEV/M0_App"
    "${SRC_ROOT}/01_DEV/M0_App/Boot"
    "${SRC_ROOT}/01_DEV/M0_App/Config"
    "${SRC_ROOT}/01_DEV/M0_App/OS/Kernel"
    "${SRC_ROOT}/01_DEV/M0_App/OS/Task"

    # M1_SysApp
    "${SRC_ROOT}/01_DEV/M1_SysApp/"
    "${SRC_ROOT}/01_DEV/M1_SysApp/Interface/RemoteCall"
    "${SRC_ROOT}/01_DEV/M1_SysApp/Interface/SimpleShell"
    "${SRC_ROOT}/01_DEV/M1_SysApp/Services"
    "${SRC_ROOT}/01_DEV/M1_SysApp/system_data"
    "${SRC_ROOT}/01_DEV/M1_SysApp/task_update_onboard_adc"
    "${SRC_ROOT}/01_DEV/M1_SysApp/task_temperature"
    "${SRC_ROOT}/01_DEV/M1_SysApp/task_experiment"
    "${SRC_ROOT}/01_DEV/M1_SysApp/task_system_control"

    # M2_BSP
    "${SRC_ROOT}/01_DEV/M2_BSP"
    "${SRC_ROOT}/01_DEV/M2_BSP/remoteproc"
    "${SRC_ROOT}/01_DEV/M2_BSP/rpmsg"
    "${SRC_ROOT}/01_DEV/M2_BSP"
    "${SRC_ROOT}/01_DEV/M2_BSP/bsp_board"
    "${SRC_ROOT}/01_DEV/M2_BSP/bsp_core"

    "${SRC_ROOT}/01_DEV/M2_BSP/bsp_peripheral"
    "${SRC_ROOT}/01_DEV/M2_BSP/bsp_peripheral/bsp_debug_console"
    "${SRC_ROOT}/01_DEV/M2_BSP/bsp_peripheral/bsp_expander"
    "${SRC_ROOT}/01_DEV/M2_BSP/bsp_peripheral/bsp_heater"
    "${SRC_ROOT}/01_DEV/M2_BSP/bsp_peripheral/bsp_i2c_sensor"
    "${SRC_ROOT}/01_DEV/M2_BSP/bsp_peripheral/bsp_laser"
    "${SRC_ROOT}/01_DEV/M2_BSP/bsp_peripheral/bsp_libcsp_can"
    "${SRC_ROOT}/01_DEV/M2_BSP/bsp_peripheral/bsp_onboard_adc"
    "${SRC_ROOT}/01_DEV/M2_BSP/bsp_peripheral/bsp_photo"
    "${SRC_ROOT}/01_DEV/M2_BSP/bsp_peripheral/bsp_power"
    "${SRC_ROOT}/01_DEV/M2_BSP/bsp_peripheral/bsp_spi_sensor"

    # M3_Devices
    "${SRC_ROOT}/01_DEV/M3_Devices/components/delay"
    "${SRC_ROOT}/01_DEV/M3_Devices/components/dio"
    "${SRC_ROOT}/01_DEV/M3_Devices/components/i2c"
    "${SRC_ROOT}/01_DEV/M3_Devices/components/dio/nxp"
    "${SRC_ROOT}/01_DEV/M3_Devices/components/spi"
    "${SRC_ROOT}/01_DEV/M3_Devices/components/spi/nxp"
    "${SRC_ROOT}/01_DEV/M3_Devices/components/sp_uart"
    "${SRC_ROOT}/01_DEV/M3_Devices/components/sp_uart/port"
    "${SRC_ROOT}/01_DEV/M3_Devices/components/sp_uart/port/NXP"
    "${SRC_ROOT}/01_DEV/M3_Devices/components/ntc"
    "${SRC_ROOT}/01_DEV/M3_Devices/components/sp_spi"
    "${SRC_ROOT}/01_DEV/M3_Devices/components/sp_spi/port"
    "${SRC_ROOT}/01_DEV/M3_Devices/components/sp_spi/port/NXP"

    "${SRC_ROOT}/01_DEV/M3_Devices/components/sp_gpio"
    "${SRC_ROOT}/01_DEV/M3_Devices/components/sp_gpio/port"
    "${SRC_ROOT}/01_DEV/M3_Devices/components/sp_gpio/port/NXP"

    "${SRC_ROOT}/01_DEV/M3_Devices/devices/AD4114"
    "${SRC_ROOT}/01_DEV/M3_Devices/devices/ADG1414BR"
    "${SRC_ROOT}/01_DEV/M3_Devices/devices/AS3004204"
    "${SRC_ROOT}/01_DEV/M3_Devices/devices/LT8722"
    "${SRC_ROOT}/01_DEV/M3_Devices/devices/MCP4902"
    "${SRC_ROOT}/01_DEV/M3_Devices/devices/pca9685"
    "${SRC_ROOT}/01_DEV/M3_Devices/devices/RV3129"
    "${SRC_ROOT}/01_DEV/M3_Devices/devices/tca6416"

    # M4_Middlewares
    "${SRC_ROOT}/01_DEV/M4_Middlewares/os/freertos"
    "${SRC_ROOT}/01_DEV/M4_Middlewares/os/include/os"
    "${SRC_ROOT}/01_DEV/M4_Middlewares/os/include/wrapper"
    "${SRC_ROOT}/01_DEV/M4_Middlewares/printf"
    "${SRC_ROOT}/01_DEV/M4_Middlewares/ring_char_buffer/"
    "${SRC_ROOT}/01_DEV/M4_Middlewares/ring_buffer"


    # M5_Utils
    "${SRC_ROOT}/01_DEV/M5_Utils"
    "${SRC_ROOT}/01_DEV/M5_Utils/CRC"
    "${SRC_ROOT}/01_DEV/M5_Utils/DateTime"
    "${SRC_ROOT}/01_DEV/M5_Utils/Define"

    # SDK của NXP cho rpmsg_lite, fsl_common.h
    "${SDK_ROOT_PATH}/devices/MIMX9352"
    "${SDK_ROOT_PATH}/devices/MIMX9352/drivers"
    "${SDK_ROOT_PATH}/middleware/multicore/rpmsg_lite/lib/include"
    "${SDK_ROOT_PATH}/middleware/multicore/rpmsg_lite/lib/include/environment/freertos"
    "${SDK_ROOT_PATH}/middleware/multicore/rpmsg_lite/lib/include/platform/imx93_m33"
)

set_source_files_properties("${UserSourceDirPath}/Configs/FreeRTOSConfig.h" PROPERTIES COMPONENT_CONFIG_FILE "middleware_freertos-kernel_template")

include(${SdkRootDirPath}/devices/MIMX9352/all_lib_device.cmake)

IF(NOT DEFINED TARGET_LINK_SYSTEM_LIBRARIES)  
    SET(TARGET_LINK_SYSTEM_LIBRARIES "-lm -lc -lgcc -lnosys")  
ENDIF()  

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${TARGET_LINK_SYSTEM_LIBRARIES})

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)

ADD_CUSTOM_COMMAND(TARGET ${MCUX_SDK_PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_OBJCOPY}
-Obinary ${EXECUTABLE_OUTPUT_PATH}/${MCUX_SDK_PROJECT_NAME} ${EXECUTABLE_OUTPUT_PATH}/sdk20-app.bin)

