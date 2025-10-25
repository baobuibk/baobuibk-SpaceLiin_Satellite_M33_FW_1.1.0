set(TARGET_NAME M1_SysApp)

file(GLOB_RECURSE SRC_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.s"
)

if(SRC_FILES)
    add_library(${TARGET_NAME} STATIC ${SRC_FILES})

    target_include_directories(${TARGET_NAME} PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${PROJECT_GLOBAL_INCLUDE_DIRS}
    )

    target_link_libraries(${TARGET_NAME} PUBLIC exp_sdk_common)

else()
    message(WARNING "${TARGET_NAME} has no source files.")
    add_library(${TARGET_NAME} INTERFACE)
endif()