###########################
# ENV VARIABLE PARSER
###########################

if(NOT EXISTS "${BUILD_ENV_FILE}")
    message(FATAL_ERROR "Dot-env file not found: ${BUILD_ENV_FILE}")
endif()

file(STRINGS "${BUILD_ENV_FILE}" entries)
foreach(entry IN LISTS entries)
    if(NOT entry MATCHES "^$|[ ]$|^\#")
        if(entry MATCHES "^([^=]+)=(.*)$")
            set(ENV_KEY ${CMAKE_MATCH_1})

            # strip out double quotes
            string(REGEX MATCHALL [^\"] ENV_VALUE "${CMAKE_MATCH_2}")
            list(JOIN ENV_VALUE "" ENV_VALUE)

            # strip out comments
            string(REGEX REPLACE "\#.*" "" ENV_VALUE "${ENV_VALUE}")
            string(STRIP "${ENV_VALUE}" ENV_VALUE)

            set(ENV{${ENV_KEY}} "${ENV_VALUE}")
        else()
            message(FATAL_ERROR "Malformed dotenv entry:\n${entry}")
        endif()
    endif()
endforeach()
