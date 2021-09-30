###########################
# QT SOURCE BUILD
###########################

cmake_minimum_required(VERSION 3.19 FATAL_ERROR)

if(CMAKE_ARGC LESS 3)
    message(FATAL_ERROR "Usage: cmake -P cmake/qt_source_build.cmake <ENV_FILE_PATH> <BIN_DIR_PATH>")
endif()

set(QT_VERSION          5.15)
set(QT_VERSION_PATCH    ${QT_VERSION}.2)
set(CONFIG_FILE         .env.config)
set(QT_BUILD_SCRIPT     ./qt_vulkan_source_build.sh)

# NOTE
#
# This builds qt from source to enable vulkan instance and functions at configure time.
# It may take up to an hour or less depending on the system memory.
if(NOT QT_BUILD_COMPLETE)
    execute_process(
        # Copy Environment Variables Manifest
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/${CONFIG_FILE}
        ${CMAKE_CURRENT_BINARY_DIR}/${CONFIG_FILE}

        # Copy Qt Source Build Script
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/${QT_BUILD_SCRIPT}
        ${CMAKE_CURRENT_BINARY_DIR}/${QT_BUILD_SCRIPT}

        # Qt Vulkan Configure, Build and Install
        COMMAND ${CMAKE_COMMAND} -E env ${SHELL} -c
        "QT_VERSION=${QT_VERSION} QT_VERSION_PATCH=${QT_VERSION_PATCH} . ${QT_BUILD_SCRIPT}"
    )
    set(QT_BUILD_COMPLETE TRUE CACHE BOOL "Qt Build completed?" FORCE)
endif()
