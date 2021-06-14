cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

if(DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "")
endif()

project(${externals} LANGUAGES C CXX)

