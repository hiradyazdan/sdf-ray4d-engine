cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE AND DEFINED ENV{CMAKE_TOOLCHAIN_FILE})
    set(CMAKE_TOOLCHAIN_FILE $ENV{CMAKE_TOOLCHAIN_FILE})
endif()

# if vcpkg versions feature flag is not enabled by default
#set(VCPKG_FEATURE_FLAGS "versions")

set(TARGET_NAME 4DRayEngine)
project(
    ${TARGET_NAME}_build
    VERSION 1.0.0
    DESCRIPTION "4D Ray Engine"
    HOMEPAGE_URL ""
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

#set(CMAKE_AUTOMOC ON) # Qt's extensions handler - Meta Object Compiler (moc)
#set(CMAKE_AUTOUIC ON) # UIC tools
#set(CMAKE_AUTOUIC_SEARCH_PATHS ${PROJECT_SOURCE_DIR}/ui)

#find_package(PkgConfig REQUIRED)
find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)
#find_package(glm REQUIRED)

add_executable(${TARGET_NAME})

file(
    GLOB_RECURSE ${TARGET_NAME}_SOURCE
    ${PROJECT_SOURCE_DIR}/include/*.hpp
    ${PROJECT_SOURCE_DIR}/src/*.cpp
)

target_sources(${TARGET_NAME} PRIVATE ${${TARGET_NAME}_SOURCE})

target_link_libraries(${TARGET_NAME} glfw Vulkan::Vulkan)

add_custom_target(
        ${TARGET_NAME}CopyAssets ALL

        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:${TARGET_NAME}>/assets

        COMMENT "Copying assets to build directory"
)

add_subdirectory(${PROJECT_SOURCE_DIR}/tests)
