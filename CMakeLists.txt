cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE AND DEFINED ENV{CMAKE_TOOLCHAIN_FILE})
    set(CMAKE_TOOLCHAIN_FILE $ENV{CMAKE_TOOLCHAIN_FILE})
endif()

# if vcpkg versions feature flag is not enabled by default
#set(VCPKG_FEATURE_FLAGS "versions")

set(TARGET_NAME SDFRay4D)
project(
    ${TARGET_NAME}_build
    VERSION 1.0.0
    DESCRIPTION "SDF Ray4D Engine: 4D SDF Raymarching Engine"
    HOMEPAGE_URL "https://github.com/hiradyazdan/masters-project"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

# TODO: Use Sanitizers for dynamic (RTC) analysis
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -g -OX")

set(CMAKE_AUTOMOC ON) # Qt's extensions handler - Meta Object Compiler (moc)
set(CMAKE_AUTORCC ON)

# NOTE:
#
# Here we explicitly define and unify output directory path, to avoid
# discrepancies between command-line vs IDE build paths on Windows, as
# need to use them within shell scripts for asset pipeline. MSVC defaults
# to Debug output directory which is not always the case with different
# IDEs configurations.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY          ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG    ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE  ${CMAKE_BINARY_DIR})

include_directories(include)
include_directories(externals)

#find_package(glslang CONFIG REQUIRED) # vcpkg port is broken
find_package(Vulkan REQUIRED)
#find_package(glm REQUIRED) # not required yet
find_package(Qt5 COMPONENTS Widgets Concurrent)

add_executable(${TARGET_NAME})

file(
    GLOB_RECURSE ${TARGET_NAME}_SOURCE

    ${PROJECT_SOURCE_DIR}/include/*.hpp
    ${PROJECT_SOURCE_DIR}/src/*.cpp
)

target_sources(${TARGET_NAME} PRIVATE ${${TARGET_NAME}_SOURCE})

# NOTE:
#
# Currently vcpkg port of glslang does not link
# default-resource-limits.lib correctly as there is a linker error
# So had to use git-submodule to pull in the glslang repo and configure,
# build and link it as a third party library.
#
# This PR (https://github.com/microsoft/vcpkg/pull/15624) tried to address this lib
# But seems to be broken and regressed by another PR (https://github.com/microsoft/vcpkg/pull/15719)
#
#target_link_libraries(${TARGET_NAME} PRIVATE glslang::glslang glslang::SPIRV glslang::HLSL glslang::OGLCompiler)
target_link_libraries(${TARGET_NAME} PRIVATE glslang glslang-default-resource-limits SPIRV)
target_link_libraries(${TARGET_NAME} PRIVATE Vulkan::Vulkan)
#target_link_libraries(${TARGET_NAME} PRIVATE glm::glm)
target_link_libraries(${TARGET_NAME} PRIVATE Qt5::Widgets Qt5::Concurrent)
target_link_libraries(${TARGET_NAME} PRIVATE nodes)

# Static Assets Pipeline
#############################################################################

find_program(SHELL bash HINTS /bin)
set(SHADERS_PATH assets/shaders)
set(SDFR_SHADERS_PATH ${SHADERS_PATH}/Raymarch)
set(DEPTH_SHADERS_PATH ${SHADERS_PATH}/Depth)
set(ACTOR_SHADERS_PATH ${SHADERS_PATH}/Actor)
set(STATIC_SHADERS_PATH ${SDFR_SHADERS_PATH}/static)
set(PARTS_SHADERS_PATH ${SDFR_SHADERS_PATH}/_partials)
set(SPV_COMPILE_SCRIPT ./spirv_static_compile.sh)
set(SPV_PARTS_COMPILE_SCRIPT ./spirv_partials_compile.sh)

# ONLY use for static shaders to speed up the scene's initial load time.
add_custom_target(
    ${TARGET_NAME}_ASSET_PIPELINE ALL

    # Copy SPIRV Static Compile Script
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_SOURCE_DIR}/${SPV_COMPILE_SCRIPT}
    $<TARGET_FILE_DIR:${TARGET_NAME}>

    # Copy SPIRV Partial Static Compile Script
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_SOURCE_DIR}/${SPV_PARTS_COMPILE_SCRIPT}
    $<TARGET_FILE_DIR:${TARGET_NAME}>

    # Copy All Assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/assets
    $<TARGET_FILE_DIR:${TARGET_NAME}>/assets

    # Compile & Cleanup: Depth Pass Shaders
    COMMAND ${CMAKE_COMMAND} -E env ${SHELL} -c
    "STATIC_SHADERS_PATH=${DEPTH_SHADERS_PATH} ${SPV_COMPILE_SCRIPT}"

    # Compile & Cleanup: Rasterization Pass Shaders
    COMMAND ${CMAKE_COMMAND} -E env ${SHELL} -c
    "STATIC_SHADERS_PATH=${ACTOR_SHADERS_PATH} ${SPV_COMPILE_SCRIPT}"

    # Compile & Cleanup: SDFR (Raymarch) Main Shaders
    COMMAND ${CMAKE_COMMAND} -E env ${SHELL} -c
    "STATIC_SHADERS_PATH=${STATIC_SHADERS_PATH} ${SPV_COMPILE_SCRIPT}"

    # Compile & Cleanup: SDFR (Raymarch) Partial Shaders
    COMMAND ${CMAKE_COMMAND} -E env ${SHELL} -c
    "PARTS_SHADERS_PATH=${PARTS_SHADERS_PATH} ${SPV_PARTS_COMPILE_SCRIPT}"

    COMMENT "Copying Assets, Compiling Shaders & Removing Redundant files."
)

#############################################################################

#add_subdirectory(${PROJECT_SOURCE_DIR}/tests)
add_subdirectory(${PROJECT_SOURCE_DIR}/externals)

if(${CMAKE_VERSION} VERSION_LESS "3.16")
    message("Your CMake version is ${CMAKE_VERSION}. Update to at least 3.16 to enable unity_build to speed up incremental builds")
else()
    # If UNITY BUILD causes compile or runtime issues (e.g. ODRs) turn this off.
    # TODO: setting this may mess up the IDE syntax error detections intermittently
    set_target_properties(
        ${TARGET_NAME}
        glslang glslang-default-resource-limits SPIRV
        PROPERTIES UNITY_BUILD ON
    )
endif()