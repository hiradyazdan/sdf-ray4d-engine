###########################
# SANITIZERS
###########################

# NOTE:
#
# This script is not available as an independent cmake script
#

if(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    if(MSVC)
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
                #            -fno-omit-frame-pointer
                #            -g -02 # TODO: figure out why not known even with RelWithDebInfo build
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

        target_link_directories (${TARGET_NAME} PRIVATE "$ENV{ProgramFiles\(x86\)}/$ENV{CLANG_CL_LIB_DIR}")
        target_link_libraries   (${TARGET_NAME} PRIVATE $ENV{CLANG_RT_ASAN_DYN} $ENV{CLANG_RT_ASAN_DYN_THUNK})
        target_link_options     (${TARGET_NAME} PRIVATE /wholearchive:$ENV{CLANG_RT_ASAN_DYN_THUNK}.lib)
    else()
        # NOTE
        #
        # - Sanitizers do not work with gcc/clang via MinGW and Cygwin on any of their ports
        # - Vulkan is not yet supported (officially) via WSL/WSL2 either for runtime, so WSL is also not available to use
        #
        # https://github.com/msys2/MINGW-packages/issues/3163
        # https://github.com/microsoft/wslg/issues/40
        # https://docs.microsoft.com/en-us/windows/wsl/tutorials/gui-apps
        # https://docs.nvidia.com/cuda/wsl-user-guide/index.html
        # https://github.com/gnsmrky/wsl-vulkan-mesa
        #
        target_compile_options(
                ${TARGET_NAME} PRIVATE
                -fsanitize=address
                -fsanitize-recover=address
                -fsanitize-address-use-after-scope
                -fno-omit-frame-pointer
                -g
        )

        target_link_libraries(${TARGET_NAME} PRIVATE -fsanitize=address)
    endif()
endif()
