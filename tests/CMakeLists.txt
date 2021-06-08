cmake_minimum_required(VERSION 3.12)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE AND DEFINED ENV{CMAKE_TOOLCHAIN_FILE})
    set(CMAKE_TOOLCHAIN_FILE $ENV{CMAKE_TOOLCHAIN_FILE})
endif()

find_package(Catch2 CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

enable_testing()
add_executable(${TARGET_NAME}_TESTS)

file(
    GLOB_RECURSE ${TARGET_NAME}_TESTS_SOURCES

    ${PROJECT_SOURCE_DIR}/include/*.hpp
    ${PROJECT_SOURCE_DIR}/src/*.cpp

    ${PROJECT_SOURCE_DIR}/tests/include/*.hpp
    ${PROJECT_SOURCE_DIR}/tests/src/*.cpp
)

list(REMOVE_ITEM ${TARGET_NAME}_TESTS_SOURCES ${PROJECT_SOURCE_DIR}/src/main.cpp)

target_sources(${TARGET_NAME}_TESTS PRIVATE ${${TARGET_NAME}_TESTS_SOURCES})

target_link_libraries(${TARGET_NAME}_TESTS PRIVATE glfw)
target_link_libraries(${TARGET_NAME}_TESTS PRIVATE Catch2::Catch2)
