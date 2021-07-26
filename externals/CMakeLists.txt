cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

project(${TARGET_NAME}_EXTERNALS LANGUAGES C CXX)

# glslang
option(ENABLE_SPVREMAPPER OFF)
option(ENABLE_GLSLANG_BINARIES OFF)
option(ENABLE_HLSL OFF)
option(ENABLE_OPT OFF)
option(BUILD_TESTING OFF)
option(BUILD_EXTERNAL OFF)

add_subdirectory(glslang)

if (NOT TARGET glslang-default-resource-limits)
    add_library(
        glslang-default-resource-limits
        glslang/StandAlone/ResourceLimits.cpp
    )
    set_property(TARGET glslang-default-resource-limits PROPERTY FOLDER "externals")
    target_include_directories(
        glslang-default-resource-limits PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/glslang/StandAlone
    )
endif()

if(NOT MSVC)
    target_compile_options(
        glslang PRIVATE
        "-Wno-logical-op-parentheses"
        "-Wno-unused-parameter"
    )
    target_compile_options(
        SPIRV PRIVATE
        "-Wno-logical-op-parentheses"
        "-Wno-unused-parameter"
    )
endif()

#set_property(TARGET glslang     PROPERTY FOLDER "externals")
#set_property(TARGET OGLCompiler PROPERTY FOLDER "externals")
#set_property(TARGET OSDependent PROPERTY FOLDER "externals")
#set_property(TARGET SPIRV       PROPERTY FOLDER "externals")
