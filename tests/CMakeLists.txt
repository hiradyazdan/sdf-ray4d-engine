cmake_minimum_required(VERSION 3.12)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE AND DEFINED ENV{CMAKE_TOOLCHAIN_FILE})
    set(CMAKE_TOOLCHAIN_FILE $ENV{CMAKE_TOOLCHAIN_FILE})
endif()

set(TESTS_TARGET_NAME ${TARGET_NAME}_TESTS)

find_package(Catch2 CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(Vulkan REQUIRED)
find_package(Qt5 COMPONENTS Widgets OpenGL Concurrent REQUIRED)

enable_testing()
add_executable(${TESTS_TARGET_NAME})

target_compile_definitions(${TESTS_TARGET_NAME} PRIVATE NODE_EDITOR_SHARED NODE_EDITOR_EXPORTS)

file(
    GLOB_RECURSE ${TESTS_TARGET_NAME}_SOURCES

    ${PROJECT_SOURCE_DIR}/include/*.hpp
    ${PROJECT_SOURCE_DIR}/src/*.cpp

    ${PROJECT_SOURCE_DIR}/externals/nodeEditor_old/*.hpp
    ${PROJECT_SOURCE_DIR}/externals/nodeEditor_old/*.cpp

    ${PROJECT_SOURCE_DIR}/tests/include/*.hpp
    ${PROJECT_SOURCE_DIR}/tests/src/*.cpp
)

list(REMOVE_ITEM ${TESTS_TARGET_NAME}_SOURCES ${PROJECT_SOURCE_DIR}/src/main.cpp)

target_sources(${TESTS_TARGET_NAME} PRIVATE ${${TESTS_TARGET_NAME}_SOURCES})

target_link_libraries(${TESTS_TARGET_NAME} PRIVATE glslang glslang-default-resource-limits SPIRV)
target_link_libraries(${TESTS_TARGET_NAME} PRIVATE Vulkan::Vulkan)
target_link_libraries(${TESTS_TARGET_NAME} PRIVATE glfw Qt5::Widgets Qt5::OpenGL Qt5::Concurrent)
target_link_libraries(${TESTS_TARGET_NAME} PRIVATE Catch2::Catch2)
