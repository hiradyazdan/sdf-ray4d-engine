cmake_minimum_required(VERSION 3.19 FATAL_ERROR)

# if vcpkg versions feature flag is not enabled by default
#set(VCPKG_FEATURE_FLAGS "versions")

set(TARGET_NAME SDFRay4D)
project(
    ${TARGET_NAME}_build
    VERSION 1.0.0
    DESCRIPTION "SDF Ray4D Engine: 4D SDF Raymarching Engine"
    HOMEPAGE_URL "https://github.com/hiradyazdan/sdf-ray4d-engine"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

set(CMAKE_AUTOMOC ON) # Qt's extensions handler - Meta Object Compiler (moc)

# NOTE:
#
# Here we explicitly define and unify output directory path, to avoid
# discrepancies between command-line vs IDE build paths on Windows, as
# need to use them within shell scripts for asset pipeline. MSVC defaults
# to nested output sub-directory which is not always the case with different
# IDEs configurations.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY                  ${CMAKE_CURRENT_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG            ${CMAKE_CURRENT_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE          ${CMAKE_CURRENT_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO   ${CMAKE_CURRENT_BINARY_DIR})

set(BUILD_ENV_FILE ${CMAKE_CURRENT_SOURCE_DIR}/.env.build)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/env_var_parser.cmake)

# If UNITY BUILD causes compile or runtime issues (e.g. ODRs) turn this off.
# setting this to ON may mess up the IDE syntax/error detections intermittently
if(NOT DEFINED ENV{UNITY_BUILD_ENABLED})
    set(CMAKE_UNITY_BUILD ON)
else()
    set(CMAKE_UNITY_BUILD $ENV{UNITY_BUILD_ENABLED})
endif()

if(UNIX AND NOT APPLE)
    # NOTE
    #
    # This builds qt from source to enable vulkan instance and functions at configure time.
    # It may take up to an hour or more depending on the system memory.
    if(NOT QT_BUILD_COMPLETE)
        execute_process(
            COMMAND ${CMAKE_COMMAND} -P
            ${CMAKE_CURRENT_SOURCE_DIR}/cmake/qt_source_build.cmake
            ${BUILD_ENV_FILE}
            ${CMAKE_CURRENT_SOURCE_DIR}

            COMMAND_ERROR_IS_FATAL ANY
        )
        set(QT_BUILD_COMPLETE TRUE CACHE BOOL "Qt Build completed?" FORCE)
    endif()
endif()

include_directories(include)
include_directories(externals)

#find_package(glslang CONFIG REQUIRED) # vcpkg port is broken
find_package(Vulkan REQUIRED)
#find_package(glm REQUIRED) # not required yet
find_package(Qt5 $ENV{QT_VERSION} COMPONENTS Widgets Concurrent REQUIRED)

add_executable(${TARGET_NAME})

file(
    GLOB_RECURSE ${TARGET_NAME}_SOURCE

    ${PROJECT_SOURCE_DIR}/include/*.hpp
    ${PROJECT_SOURCE_DIR}/src/*.cpp
)

target_sources(${TARGET_NAME} PRIVATE ${${TARGET_NAME}_SOURCE})

if($ENV{PCH_ENABLED})
    target_precompile_headers(${TARGET_NAME} PRIVATE include/pch.hpp)
    set(CMAKE_PCH_INSTANTIATE_TEMPLATES ON)
endif()

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

# Doxygen
#############################################################################

if($ENV{DOC_BUILD_ENABLED})
    include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/doxygen.cmake)
endif()

#############################################################################

# Sanitizers
#############################################################################

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/sanitizers.cmake)

#############################################################################

# Static Assets Pipeline
#############################################################################

add_custom_target(
    ${TARGET_NAME}_ASSET_PIPELINE ALL

    COMMAND ${CMAKE_COMMAND} -P
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/static_assets_pipeline.cmake
    ${BUILD_ENV_FILE}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}

    COMMENT "Copying Assets, Compiling Shaders & Removing Redundant files."
    VERBATIM
)

#############################################################################

add_subdirectory(${PROJECT_SOURCE_DIR}/externals)
#add_subdirectory(${PROJECT_SOURCE_DIR}/tests)
