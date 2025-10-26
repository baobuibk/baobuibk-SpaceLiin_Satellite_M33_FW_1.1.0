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

set(MCUX_SDK_PROJECT_NAME satelite_exp_v100.elf)

if (NOT DEFINED SdkRootDirPath)
    SET(SdkRootDirPath ${ProjDirPath}/../../../../..)
endif()

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

# USER DEFINE DEV DIRECTORY
set(UserSourceDirPath ${ProjDirPath}/../Source)

add_executable(${MCUX_SDK_PROJECT_NAME}
    # main.c
    "${UserSourceDirPath}/main.c"

    # FreeRTOS
    # "${UserSourceDirPath}/MiddleWare/FreeRTOS/FreeRTOSConfig.h"

    # MCUXpresso Autogen
    "${UserSourceDirPath}/BoardSupport/autogen/board.c"
    "${UserSourceDirPath}/BoardSupport/autogen/pin_mux.c"
    "${UserSourceDirPath}/BoardSupport/autogen/clock_config.c"

    # User define
    # App
    "${UserSourceDirPath}/App/app.c"

    "${UserSourceDirPath}/App/system_data/system_data.c"

    "${UserSourceDirPath}/App/task/task_experiment/task_experiment.c"

    "${UserSourceDirPath}/App/task/task_cmd_line/task_cmd_line.c"

    "${UserSourceDirPath}/App/task/task_cmd_line/command/command.c"

    "${UserSourceDirPath}/App/task/task_cmd_line/command/cmd_test_can/cmd_test_can.c"

    "${UserSourceDirPath}/App/task/task_cmd_line/command/cmd_test_spi/cmd_test_spi.c"

    "${UserSourceDirPath}/App/task/task_test_can/task_test_can.c"

    "${UserSourceDirPath}/App/task/task_update_onboard_adc/task_update_onboard_adc.c"

    "${UserSourceDirPath}/App/task/task_experiment/task_experiment.c"

    # BoardSupport
    "${UserSourceDirPath}/BoardSupport/bsp.c"
    

    "${UserSourceDirPath}/BoardSupport/bsp_core/bsp_core.c"

    "${UserSourceDirPath}/BoardSupport/bsp_peripheral/bsp_debug_console/bsp_debug_console.c"

    "${UserSourceDirPath}/BoardSupport/bsp_peripheral/bsp_libcsp_can/bsp_libcsp_can.c"

    # "${UserSourceDirPath}/BoardSupport/bsp_peripheral/bsp_spi_sensor/bsp_spi_sensor.c"

    "${UserSourceDirPath}/BoardSupport/bsp_peripheral/bsp_i2c_sensor/bsp_i2c_sensor.c"

    "${UserSourceDirPath}/BoardSupport/bsp_peripheral/bsp_laser/bsp_laser.c"

    "${UserSourceDirPath}/BoardSupport/bsp_peripheral/bsp_expander/bsp_expander.c"

    "${UserSourceDirPath}/BoardSupport/bsp_peripheral/bsp_heater/bsp_heater.c"

    "${UserSourceDirPath}/BoardSupport/bsp_peripheral/bsp_onboard_adc/bsp_onboard_adc.c"

    "${UserSourceDirPath}/BoardSupport/bsp_peripheral/bsp_photo/bsp_photo.c"

    # Components

    "${UserSourceDirPath}/Drivers/Components/delay/nxp/nxp_delay.c"

    "${UserSourceDirPath}/Drivers/Components/ring_buffer/ring_buffer.c"

    "${UserSourceDirPath}/Drivers/Components/ring_char_buffer/ring_char_buffer.c"

    "${UserSourceDirPath}/Drivers/Components/sp_gpio/port/NXP/sp_gpio_port.c"

    "${UserSourceDirPath}/Drivers/Components/dio/nxp/do.c"

    "${UserSourceDirPath}/Drivers/Components/uart_stdio/uart_stdio.c"

    "${UserSourceDirPath}/Drivers/Components/sp_uart/port/NXP/sp_uart_port.c"

    "${UserSourceDirPath}/Drivers/Components/sp_spi/port/NXP/sp_spi_port.c"

    "${UserSourceDirPath}/Drivers/Components/spi/spi_io.c"
    "${UserSourceDirPath}/Drivers/Components/spi/nxp/spi_io_nxp.c"

    "${UserSourceDirPath}/Drivers/Components/sp_i2c/port/NXP/sp_i2c_port.c"

    "${UserSourceDirPath}/Drivers/Components/i2c/i2c_io.c"
    "${UserSourceDirPath}/Drivers/Components/i2c/nxp/nxp_i2c_io.c"

    "${UserSourceDirPath}/Drivers/Components/can_stdio/can_stdio.c"

    "${UserSourceDirPath}/Drivers/Components/cmd_line/cmd_line.c"

    "${UserSourceDirPath}/Drivers/Components/ntc/ntc.c"

    # Devices
    "${UserSourceDirPath}/Drivers/Devices/tca6416/tca6416.c"

    "${UserSourceDirPath}/Drivers/Devices/pca9685/pca9685.c"

    "${UserSourceDirPath}/Drivers/Devices/AD4114/ad4114.c"

    "${UserSourceDirPath}/Drivers/Devices/ADG1414BR/adg1414.c"

    "${UserSourceDirPath}/Drivers/Devices/MCP4902/mcp4902.c"

    # MiddleWare
    "${UserSourceDirPath}/MiddleWare/FlightSystem/os/freertos/osDelay.c"
    "${UserSourceDirPath}/MiddleWare/FlightSystem/os/freertos/osMalloc.c"
    "${UserSourceDirPath}/MiddleWare/FlightSystem/os/freertos/osQueue.c"
    "${UserSourceDirPath}/MiddleWare/FlightSystem/os/freertos/osScheduler.c"
    "${UserSourceDirPath}/MiddleWare/FlightSystem/os/freertos/osSemphr.c"
    "${UserSourceDirPath}/MiddleWare/FlightSystem/os/freertos/osThread.c"

    "${UserSourceDirPath}/MiddleWare/FlightSystem/drivers/stm32/cpu.c"
    "${UserSourceDirPath}/MiddleWare/FlightSystem/drivers/stm32/i2c.c"
    "${UserSourceDirPath}/MiddleWare/FlightSystem/drivers/stm32/init.c"
    "${UserSourceDirPath}/MiddleWare/FlightSystem/drivers/stm32/mem.c"

    "${UserSourceDirPath}/Drivers/Components/devices/lt8722/lt8722.c"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/..

    # User define
    # App
    ${UserSourceDirPath}/App
    ${UserSourceDirPath}/App/system_data
    ${UserSourceDirPath}/App/task
    ${UserSourceDirPath}/App/task/task_experiment
    ${UserSourceDirPath}/App/task/task_cmd_line
    ${UserSourceDirPath}/App/task/task_cmd_line/command
    ${UserSourceDirPath}/App/task/task_cmd_line/command/cmd_test_can
    ${UserSourceDirPath}/App/task/task_cmd_line/command/cmd_test_spi

    ${UserSourceDirPath}/App/task/task_test_can
    ${UserSourceDirPath}/App/task/task_update_onboard_adc
    ${UserSourceDirPath}/App/task/task_experiment

    # BoardSupport
    ${UserSourceDirPath}/BoardSupport
    ${UserSourceDirPath}/BoardSupport/autogen

    ${UserSourceDirPath}/BoardSupport/bsp_board

    ${UserSourceDirPath}/BoardSupport/bsp_core

    ${UserSourceDirPath}/BoardSupport/bsp_peripheral/bsp_debug_console
    ${UserSourceDirPath}/BoardSupport/bsp_peripheral/bsp_libcsp_can
    # ${UserSourceDirPath}/BoardSupport/bsp_peripheral/bsp_spi_sensor
    ${UserSourceDirPath}/BoardSupport/bsp_peripheral/bsp_i2c_sensor
    ${UserSourceDirPath}/BoardSupport/bsp_peripheral/bsp_laser
    ${UserSourceDirPath}/BoardSupport/bsp_peripheral/bsp_expander
    ${UserSourceDirPath}/BoardSupport/bsp_peripheral/bsp_heater
    ${UserSourceDirPath}/BoardSupport/bsp_peripheral/bsp_onboard_adc
    ${UserSourceDirPath}/BoardSupport/bsp_peripheral/bsp_photo

    # Component
    ${UserSourceDirPath}/Drivers/Components/error_codes
    ${UserSourceDirPath}/Drivers/Components/ring_buffer
    ${UserSourceDirPath}/Drivers/Components/ring_char_buffer
    ${UserSourceDirPath}/Drivers/Components/delay

    ${UserSourceDirPath}/Drivers/Components/sp_gpio
    ${UserSourceDirPath}/Drivers/Components/sp_gpio/port
    ${UserSourceDirPath}/Drivers/Components/sp_gpio/port/NXP

    ${UserSourceDirPath}/Drivers/Components/dio
    ${UserSourceDirPath}/Drivers/Components/dio/nxp

    ${UserSourceDirPath}/Drivers/Components/sp_spi
    ${UserSourceDirPath}/Drivers/Components/sp_spi/port
    ${UserSourceDirPath}/Drivers/Components/sp_spi/port/NXP

    ${UserSourceDirPath}/Drivers/Components/spi
    ${UserSourceDirPath}/Drivers/Components/spi/nxp

    ${UserSourceDirPath}/Drivers/Components/sp_i2c
    ${UserSourceDirPath}/Drivers/Components/sp_i2c/port
    ${UserSourceDirPath}/Drivers/Components/sp_i2c/port/NXP

    ${UserSourceDirPath}/Drivers/Components/i2c
    ${UserSourceDirPath}/Drivers/Components/i2c/nxp

    ${UserSourceDirPath}/Drivers/Components/uart_stdio
    ${UserSourceDirPath}/Drivers/Components/can_stdio
    ${UserSourceDirPath}/Drivers/Components/cmd_line

    ${UserSourceDirPath}/Drivers/Components/sp_uart
    ${UserSourceDirPath}/Drivers/Components/sp_uart/port
    ${UserSourceDirPath}/Drivers/Components/sp_uart/port/NXP

    ${UserSourceDirPath}/Drivers/Components/ntc

    # ${UserSourceDirPath}/Drivers/Components/devices/lt8722

    # Devices
    ${UserSourceDirPath}/Drivers/Devices/tca6416
    ${UserSourceDirPath}/Drivers/Devices/pca9685
    ${UserSourceDirPath}/Drivers/Devices/AD4114
    ${UserSourceDirPath}/Drivers/Devices/ADG1414BR
    ${UserSourceDirPath}/Drivers/Devices/MCP4902

    # MiddleWare
    # ${UserSourceDirPath}/MiddleWare/FreeRTOS

    # ${UserSourceDirPath}/MiddleWare/FlightSystem/os/include
    ${UserSourceDirPath}/MiddleWare/FlightSystem/os/include/os
    ${UserSourceDirPath}/MiddleWare/FlightSystem/os/include/wrapper

    # ${UserSourceDirPath}/MiddleWare/FlightSystem/drivers/include
    # ${UserSourceDirPath}/MiddleWare/FlightSystem/drivers/stm32/include

    # Configs
    ${UserSourceDirPath}/Configs
    # ${UserSourceDirPath}/Configs/csp
    ${UserSourceDirPath}/Configs/flight
)

set_source_files_properties("${UserSourceDirPath}/Configs/FreeRTOSConfig.h" PROPERTIES COMPONENT_CONFIG_FILE "middleware_freertos-kernel_template")

include(${SdkRootDirPath}/devices/MIMX9352/all_lib_device.cmake)

# In đường dẫn SDK để kiểm tra trực quan
message(STATUS "SdkRootDirPath = ${SdkRootDirPath}")

# Sanity check: chắc chắn file driver tồn tại trong SDK
if(NOT EXISTS "${SdkRootDirPath}/devices/MIMX9352/drivers/fsl_lpspi.c")
  message(FATAL_ERROR "Không thấy fsl_lpspi.c tại: ${SdkRootDirPath}/devices/MIMX9352/drivers
  -> Kiểm tra lại SdkRootDirPath hoặc bộ SDK MIMX9352.")
endif()


get_target_property(__cur_src ${MCUX_SDK_PROJECT_NAME} SOURCES)
string(REGEX MATCH "fsl_lpspi.c" __has_lpspi "${__cur_src}")
if(NOT __has_lpspi)
  message(WARNING "Component manager chưa add fsl_lpspi.c -> ép add thủ công.")
  target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    "${SdkRootDirPath}/devices/MIMX9352/drivers/fsl_lpspi.c")
  target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    "${SdkRootDirPath}/devices/MIMX9352/drivers"
    "${SdkRootDirPath}/devices/MIMX9352")
endif()

IF(NOT DEFINED TARGET_LINK_SYSTEM_LIBRARIES)  
    SET(TARGET_LINK_SYSTEM_LIBRARIES "-lm -lc -lgcc -lnosys")  
ENDIF()  

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${TARGET_LINK_SYSTEM_LIBRARIES})

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)

ADD_CUSTOM_COMMAND(TARGET ${MCUX_SDK_PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_OBJCOPY}
-Obinary ${EXECUTABLE_OUTPUT_PATH}/${MCUX_SDK_PROJECT_NAME} ${EXECUTABLE_OUTPUT_PATH}/sdk20-app.bin)

