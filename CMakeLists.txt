cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE AND DEFINED ENV{CMAKE_TOOLCHAIN_FILE})
    set(CMAKE_TOOLCHAIN_FILE $ENV{CMAKE_TOOLCHAIN_FILE})
endif()

# if vcpkg versions feature flag is not enabled by default
#set(VCPKG_FEATURE_FLAGS "versions")

set(TARGET_NAME Ray4D)
project(
    ${TARGET_NAME}_build
    VERSION 1.0.0
    DESCRIPTION "Ray4D Engine: 4D Raymarching Engine"
    HOMEPAGE_URL ""
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

set(CMAKE_AUTOMOC ON) # Qt's extensions handler - Meta Object Compiler (moc)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON) # UIC tools
set(CMAKE_AUTOUIC_SEARCH_PATHS ${PROJECT_SOURCE_DIR}/ui)

include_directories(include)
include_directories(externals)

#include_directories(./externals/nodeeditor/include)
#include_directories(./externals/nodeeditor/include/nodes/internal)

#find_package(PkgConfig REQUIRED)
#find_package(glslang CONFIG REQUIRED) # uses vcpkg cmake config, but pulls in the Vulkan SDK's ports
find_package(Vulkan REQUIRED)
find_package(glm REQUIRED)
find_package(Qt5 COMPONENTS Widgets OpenGL)

#qt5_add_resources(RESOURCES ${PROJECT_SOURCE_DIR}/externals/nodeeditor/resources/resources.qrc)

add_executable(${TARGET_NAME} ${RESOURCES})

file(
    GLOB_RECURSE ${TARGET_NAME}_SOURCE
    ${PROJECT_SOURCE_DIR}/include/*.hpp
    ${PROJECT_SOURCE_DIR}/src/*.cpp

#    ${PROJECT_SOURCE_DIR}/externals/glslang/*

#    ${PROJECT_SOURCE_DIR}/externals/nodeeditor/include/*
#    ${PROJECT_SOURCE_DIR}/externals/nodeeditor/src/*
)

#target_compile_definitions(${TARGET_NAME} PRIVATE NODE_EDITOR_SHARED NODE_EDITOR_EXPORTS)

target_sources(${TARGET_NAME} PRIVATE ${${TARGET_NAME}_SOURCE})

#target_link_libraries(${TARGET_NAME} PRIVATE glslang::HLSL glslang::SPIRV glslang::glslang glslang::OGLCompiler)
target_link_libraries(${TARGET_NAME} PRIVATE Vulkan::Vulkan)
target_link_libraries(${TARGET_NAME} PRIVATE glm::glm)
target_link_libraries(${TARGET_NAME} PRIVATE Qt5::Widgets Qt5::OpenGL)

# Temporary precompile shaders to SPV files till figuring out how to
# integrate with glslang/shaderc apis
file(
    GLOB_RECURSE ${TARGET_NAME}_SHADERS_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/*.vert
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/*.frag
    ${CMAKE_CURRENT_SOURCE_DIR}/spirv_compile.sh
)
set(
    ${TARGET_NAME}_SHADERS_BIN
    ${CMAKE_CURRENT_BINARY_DIR}/assets/*.vert
    ${CMAKE_CURRENT_BINARY_DIR}/assets/*.frag
)
set(
    ${TARGET_NAME}_SHADERS_SPV
    ${CMAKE_CURRENT_BINARY_DIR}/assets/*.spv
)
add_custom_command(
    OUTPUT ${${TARGET_NAME}_SHADERS_BIN}
    COMMAND ${CMAKE_COMMAND} -E copy
    ${${TARGET_NAME}_SHADERS_SRC} .

    COMMENT "Copying across shader source files."
    DEPENDS ${${TARGET_NAME}_SHADERS_SRC}
)
add_custom_command(
    OUTPUT ${${TARGET_NAME}_SHADERS_SPV}
    COMMAND ./spirv_compile.sh
    COMMENT "Compiling shader files and removing redundant files."
    DEPENDS ${${TARGET_NAME}_SHADERS_BIN}
)
add_custom_target(
    ${TARGET_NAME}CopyAssets ALL
    DEPENDS ${${TARGET_NAME}_SHADERS_SPV}
)

#add_custom_target(
#    ${TARGET_NAME}CopyAssets ALL
#
#    COMMAND ${CMAKE_COMMAND} -E copy_directory
#    ${CMAKE_CURRENT_SOURCE_DIR}/assets
#    $<TARGET_FILE_DIR:${TARGET_NAME}>/assets
#
#    COMMENT "Copying assets to build directory"
#)

# TODO: fix nodeeditor cmake to work with vcpkg
#add_subdirectory(${PROJECT_SOURCE_DIR}/externals/nodeeditor)
#add_subdirectory(${PROJECT_SOURCE_DIR}/externals/glslang)
add_subdirectory(${PROJECT_SOURCE_DIR}/tests)
