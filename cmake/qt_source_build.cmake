###########################
# QT SOURCE BUILD
###########################

cmake_minimum_required(VERSION 3.19 FATAL_ERROR)

if(CMAKE_ARGC LESS 5)
    message(FATAL_ERROR "Usage: cmake -P cmake/qt_source_build.cmake <BUILD_ENV_FILENAME> <SOURCE_DIR_PATH>")
endif()

set(BUILD_ENV_FILE  "${CMAKE_ARGV3}")
set(SOURCE_DIR      "${CMAKE_ARGV4}")

if(NOT EXISTS "${BUILD_ENV_FILE}")
    message(FATAL_ERROR "Dot-env file not found: ${BUILD_ENV_FILE}")
endif()

file(STRINGS "${BUILD_ENV_FILE}" entries)
foreach(entry IN LISTS entries)
    if(NOT entry MATCHES "^$|[ ]$|^\#")
        if(entry MATCHES "^([^=]+)=(.*)$")
            set(ENV_KEY ${CMAKE_MATCH_1})
            string(REGEX MATCHALL [^\"] ENV_VALUE "${CMAKE_MATCH_2}")
            list(JOIN ENV_VALUE "" ENV_VALUE)

            if(NOT ENV_VALUE MATCHES "^\#")
                set(ENV{${ENV_KEY}} "${ENV_VALUE}")
            endif()
        else()
            message(FATAL_ERROR "Malformed dotenv entry:\n${entry}")
        endif()
    endif()
endforeach()

set(QT_BUILD_SCRIPT ./qt_vulkan_source_build.sh)

find_program(SHELL bash HINTS /bin)

execute_process(
    # Qt Vulkan Configure, Build and Install
    COMMAND ${CMAKE_COMMAND} -E env ${SHELL} -c ". ${QT_BUILD_SCRIPT}"

    WORKING_DIRECTORY ${SOURCE_DIR}
    COMMAND_ERROR_IS_FATAL ANY
)
