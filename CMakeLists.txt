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

# If UNITY BUILD causes compile or runtime issues (e.g. ODRs) turn this off.
# setting this to ON may mess up the IDE syntax/error detections intermittently
if(NOT ENV{CMAKE_UNITY_BUILD})
    set(CMAKE_UNITY_BUILD ON)
else()
    set(CMAKE_UNITY_BUILD $ENV{CMAKE_UNITY_BUILD})
endif()

if(UNIX AND NOT APPLE)
    # NOTE
    #
    # This builds qt from source to enable vulkan instance and functions at configure time.
    # It may take up to an hour or less depending on the system memory.
    if(NOT QT_BUILD_COMPLETE)
        execute_process(
            COMMAND ${CMAKE_COMMAND} -P
            ${CMAKE_CURRENT_SOURCE_DIR}/cmake/qt_source_build.cmake
            ${BUILD_ENV_FILE}
            ${CMAKE_CURRENT_SOURCE_DIR}
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

# Address Sanitizers
#############################################################################

if(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    if(WIN32)
        add_compile_options(
            -EHa # should explicitly enable exceptions with clang-cl (https://github.com/catchorg/Catch2/issues/1113)
            -Wno-c++98-compat
            -Wno-c++98-compat-pedantic
            -Wno-deprecated-declarations
            -Wno-shadow
            -Wno-switch-enum
            -Wno-double-promotion
            -Wno-float-conversion
            -Wno-sign-conversion
            -Wno-enum-enum-conversion
            -Wno-unreachable-code-break
            -Wno-suggest-destructor-override
            -Wno-inconsistent-missing-destructor-override
            -Wno-zero-as-null-pointer-constant
            -Wno-shorten-64-to-32
            -Wno-extra-semi-stmt
            -Wno-implicit-float-conversion
            -Wno-global-constructors
            -Wno-redundant-parens
            -Wno-exit-time-destructors
            -Wno-documentation
        )
        target_compile_options(
            ${TARGET_NAME} PRIVATE
            /Zi
            -EHa # should explicitly enable exceptions with clang-cl (https://github.com/catchorg/Catch2/issues/1113)
            -fsanitize=address
            -fsanitize-recover=address
            -fsanitize-address-use-after-scope
        )

        if(
            NOT DEFINED ENV{CLANG_CL_LIB_DIR} OR
            NOT DEFINED ENV{CLANG_CL_DBG_DIR} OR
            NOT DEFINED ENV{CLANG_RT_ASAN_DYN} OR
            NOT DEFINED ENV{CLANG_RT_ASAN_DYN_THUNK}
        )
            message(FATAL_ERROR "You should set all required sanitizers ENV variables")
        endif()

        # NOTE:
        #
        # On Windows, for non-release builds need to manually copy over
        # the CLANG_RT_ASAN_DYN dbg (debug) file, otherwise it cannot find
        # the dll as it doesn't exist in the CLANG_CL_LIB_DIR
        #
        # e.g. Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\14.29.30133\bin\Hostx86\x64\clang_rt.asan_dbg_dynamic-x86_64
        #
        # https://developercommunity.visualstudio.com/t/clang-rtasan-dbg-dynamic-x86-64dll-was-not-found-w/1140325
        # https://stackoverflow.com/a/66532115/5056311
        # https://devblogs.microsoft.com/cppblog/addresssanitizer-asan-for-windows-with-msvc/#compiling-with-asan-from-the-console
        #
        execute_process(
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $ENV{ProgramFiles\(x86\)}/$ENV{CLANG_CL_DBG_DIR}/$ENV{CLANG_RT_ASAN_DYN}.dll
            ${CMAKE_CURRENT_BINARY_DIR}

            COMMAND_ERROR_IS_FATAL ANY
        )

        target_link_directories (${TARGET_NAME} PRIVATE "${ProgramFiles\(x86\)}/$ENV{CLANG_CL_LIB_DIR}")
        target_link_libraries   (${TARGET_NAME} PRIVATE $ENV{CLANG_RT_ASAN_DYN} $ENV{CLANG_RT_ASAN_DYN_THUNK})
        target_link_options     (${TARGET_NAME} PRIVATE /wholearchive:$ENV{CLANG_RT_ASAN_DYN_THUNK}.lib)
    elseif(UNIX)
        # NOTE
        #
        # There seems to be gcc version incompatibility issues with sanitizers in CLion
        # and earlier than gcc 9 or 10 don't work correctly
        #
        # https://intellij-support.jetbrains.com/hc/en-us/community/posts/360003031060-Using-sanitizers-with-visual-representation
        # https://youtrack.jetbrains.com/issue/CPP-13813?_ga=2.196238828.1991408756.1633127548-1736470599.1604156095
        #
        target_compile_options(
            ${TARGET_NAME} PRIVATE
            -fsanitize=address
            -fsanitize-recover=address
            -fsanitize-address-use-after-scope
        )

        target_link_libraries(${TARGET_NAME} PRIVATE -fsanitize=address)
    endif()
endif()

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

#add_subdirectory(${PROJECT_SOURCE_DIR}/tests)
add_subdirectory(${PROJECT_SOURCE_DIR}/externals)
