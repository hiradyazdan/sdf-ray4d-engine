###########################
# Static Assets Pipeline
###########################

cmake_minimum_required(VERSION 3.19 FATAL_ERROR)

if(CMAKE_ARGC LESS 6)
    message(FATAL_ERROR "Usage: cmake -P cmake/static_assets_pipeline.cmake <BUILD_ENV_FILENAME> <SOURCE_DIR_PATH> <BIN_DIR_PATH>")
endif()

set(BUILD_ENV_FILE              "${CMAKE_ARGV3}")
set(SOURCE_DIR                  "${CMAKE_ARGV4}")
set(BIN_DIR                     "${CMAKE_ARGV5}")

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
            set(ENV{${ENV_KEY}} "${ENV_VALUE}")
        else()
            message(FATAL_ERROR "Malformed dotenv entry:\n${entry}")
        endif()
    endif()
endforeach()

if(NOT DEFINED ENV{ASSETS_PATH})
    set(ASSETS_PATH             assets)
else()
    set(ASSETS_PATH             $ENV{ASSETS_PATH})
endif()

set(SHADERS_PATH                ${ASSETS_PATH}/shaders)
set(SDFR_SHADERS_PATH           ${SHADERS_PATH}/Raymarch)
set(DEPTH_SHADERS_PATH          ${SHADERS_PATH}/Depth)
set(ACTOR_SHADERS_PATH          ${SHADERS_PATH}/Actor)
set(STATIC_SHADERS_PATH         ${SDFR_SHADERS_PATH}/static)
set(PARTS_SHADERS_PATH          ${SDFR_SHADERS_PATH}/_partials)

set(SPV_COMPILE_SCRIPT          ./spirv_static_compile.sh)
set(SPV_PARTS_COMPILE_SCRIPT    ./spirv_partials_compile.sh)

find_program(SHELL bash HINTS /bin)

execute_process(
    # Copy SPIRV Static Compile Script
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${SOURCE_DIR}/${SPV_COMPILE_SCRIPT}
    ${BIN_DIR}

    # Copy SPIRV Partial Static Compile Script
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${SOURCE_DIR}/${SPV_PARTS_COMPILE_SCRIPT}
    ${BIN_DIR}

    # Copy All Assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${SOURCE_DIR}/${ASSETS_PATH}
    ${BIN_DIR}/${ASSETS_PATH}

    COMMAND_ERROR_IS_FATAL ANY
)
execute_process(
    # Compile & Cleanup: Depth Pass Shaders
    COMMAND ${CMAKE_COMMAND} -E env ${SHELL} -c
    "STATIC_SHADERS_PATH=${DEPTH_SHADERS_PATH} . ${SPV_COMPILE_SCRIPT}"

    WORKING_DIRECTORY ${BIN_DIR}
    COMMAND_ERROR_IS_FATAL ANY
)
execute_process(
    # Compile & Cleanup: Rasterization Pass Shaders
    COMMAND ${CMAKE_COMMAND} -E env ${SHELL} -c
    "STATIC_SHADERS_PATH=${ACTOR_SHADERS_PATH} . ${SPV_COMPILE_SCRIPT}"

    WORKING_DIRECTORY ${BIN_DIR}
    COMMAND_ERROR_IS_FATAL ANY
)
execute_process(
    # Compile & Cleanup: SDFR (Raymarch) Main Shaders
    COMMAND ${CMAKE_COMMAND} -E env ${SHELL} -c
    "STATIC_SHADERS_PATH=${STATIC_SHADERS_PATH} . ${SPV_COMPILE_SCRIPT}"

    WORKING_DIRECTORY ${BIN_DIR}
    COMMAND_ERROR_IS_FATAL ANY
)
execute_process(
    # Compile & Cleanup: SDFR (Raymarch) Partial Shaders
    COMMAND ${CMAKE_COMMAND} -E env ${SHELL} -c
    "PARTS_SHADERS_PATH=${PARTS_SHADERS_PATH} . ${SPV_PARTS_COMPILE_SCRIPT}"

    WORKING_DIRECTORY ${BIN_DIR}
    COMMAND_ERROR_IS_FATAL ANY
)
