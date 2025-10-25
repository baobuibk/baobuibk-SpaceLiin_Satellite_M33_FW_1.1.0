add_library(exp_sdk_common INTERFACE)

target_include_directories(exp_sdk_common INTERFACE
    ${PROJECT_GLOBAL_INCLUDE_DIRS}

    ${SDK_ROOT_PATH}/devices/MIMX9352/utilities/debug_console
    ${SDK_ROOT_PATH}/devices/MIMX9352/
    ${SDK_ROOT_PATH}/devices/MIMX9352/drivers
    ${SDK_ROOT_PATH}/CMSIS/Core/Include
    ${SDK_ROOT_PATH}/components/serial_manager
    ${SDK_ROOT_PATH}/components/uart
    ${SDK_ROOT_PATH}/components/lists
    ${SDK_ROOT_PATH}/rtos/freertos/freertos-kernel/include
    ${SDK_ROOT_PATH}/middleware/multicore/rpmsg_lite/lib/include
    ${SDK_ROOT_PATH}/middleware/multicore/rpmsg_lite/lib/include/environment/freertos
    ${SDK_ROOT_PATH}/middleware/multicore/rpmsg_lite/lib/include/platform/imx93_m33
    ${ProjDirPath}/../01_DEV/M2_BSP/rpmsg
    ${ProjDirPath}/../01_DEV/M2_BSP/rpmsg
    ${ProjDirPath}/../01_DEV/M3_Devices/components/spi
)

# target_compile_definitions(exp_sdk_common INTERFACE DEBUG USE_UART_LOG)
# target_compile_options(exp_sdk_common INTERFACE -Wall)
