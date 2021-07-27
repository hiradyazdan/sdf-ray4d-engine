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

if(${CMAKE_VERSION} VERSION_LESS "3.16")
    message("Your CMake version is ${CMAKE_VERSION}. Update to at least 3.16 to enable unity_build to speed up incremental builds")
else()
    # If UNITY BUILD causes compile or runtime issues turn this off.
    # TODO: setting this messes up my IDE syntax error detections intermittently
#    set(CMAKE_UNITY_BUILD ON)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

set(CMAKE_AUTOMOC ON) # Qt's extensions handler - Meta Object Compiler (moc)
set(CMAKE_AUTORCC ON)
# uncomment below if ui layout created with Qt creator
#set(CMAKE_AUTOUIC ON) # UIC tools
#set(CMAKE_AUTOUIC_SEARCH_PATHS ${PROJECT_SOURCE_DIR}/ui)

include_directories(include)
include_directories(externals)

#include_directories(./externals/nodeeditor/include)
#include_directories(./externals/nodeeditor/include/nodes/internal)

#find_package(PkgConfig REQUIRED)
find_package(glslang CONFIG REQUIRED) # uses vcpkg cmake config, but pulls in the Vulkan SDK's ports
find_package(Vulkan REQUIRED)
#find_package(unofficial-vulkan-memory-allocator REQUIRED)
find_package(glm REQUIRED)
find_package(Qt5 COMPONENTS Widgets OpenGL Concurrent)
#find_package(Boost REQUIRED)

#qt5_add_resources(RESOURCES ${PROJECT_SOURCE_DIR}/externals/nodeeditor/resources/resources.qrc)

add_executable(${TARGET_NAME})

#add_compile_definitions(NODE_EDITOR_SHARED NODE_EDITOR_EXPORTS)
target_compile_definitions(${TARGET_NAME} PRIVATE NODE_EDITOR_SHARED NODE_EDITOR_EXPORTS)

file(
    GLOB_RECURSE ${TARGET_NAME}_SOURCE

    ${PROJECT_SOURCE_DIR}/include/*.hpp
    ${PROJECT_SOURCE_DIR}/src/*.cpp

    ${PROJECT_SOURCE_DIR}/externals/nodeEditor_old/*.hpp
    ${PROJECT_SOURCE_DIR}/externals/nodeEditor_old/*.cpp

#    ${PROJECT_SOURCE_DIR}/externals/nodeeditor/include/*
#    ${PROJECT_SOURCE_DIR}/externals/nodeeditor/src/*
)

target_sources(${TARGET_NAME} PRIVATE ${${TARGET_NAME}_SOURCE})

#target_link_libraries(${TARGET_NAME} PRIVATE glslang::HLSL glslang::SPIRV glslang::glslang glslang::OGLCompiler)
target_link_libraries(${TARGET_NAME} PRIVATE glslang glslang-default-resource-limits SPIRV)
#target_link_libraries(${TARGET_NAME} PRIVATE shaderc)
target_link_libraries(
    ${TARGET_NAME} PRIVATE
    Vulkan::Vulkan
#    unofficial::vulkan-memory-allocator::vulkan-memory-allocator
)
target_link_libraries(${TARGET_NAME} PRIVATE glm::glm)
target_link_libraries(${TARGET_NAME} PRIVATE Qt5::Widgets Qt5::OpenGL Qt5::Concurrent)

# ONLY use for static shaders to speed up the scene's initial load time.
add_custom_target(
    ${TARGET_NAME}_ASSET_PIPELINE ALL

    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/spirv_static_compile.sh
    $<TARGET_FILE_DIR:${TARGET_NAME}>

    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/assets
    $<TARGET_FILE_DIR:${TARGET_NAME}>/assets

    COMMAND ./spirv_static_compile.sh

    COMMENT "Copying Assets, Compiling Shaders & Removing Redundant files."
)

add_subdirectory(${PROJECT_SOURCE_DIR}/externals)
add_subdirectory(${PROJECT_SOURCE_DIR}/tests)
