###########################
# DOXYGEN - API DOC GENERATION
###########################

# NOTE:
#
# This script is not available as an independent cmake script
#

if(CMAKE_BUILD_TYPE MATCHES "^[Rr]elease")
    find_package(Doxygen REQUIRED dot OPTIONAL_COMPONENTS mscgen dia)

    if(DOXYGEN_FOUND)
        set(DOXYGEN_PROJECT_NAME "SDF Ray4D Engine")
        set(DOXYGEN_IMAGE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/docs/design)
        set(DOXYGEN_USE_MDFILE_AS_MAINPAGE README.md)
        set(DOXYGEN_OUTPUT_DIRECTORY docs)

        set(DOXYGEN_RECURSIVE               YES)

        set(DOXYGEN_HIDE_UNDOC_RELATIONS    NO)
        set(DOXYGEN_TEMPLATE_RELATIONS      YES)

        set(DOXYGEN_BUILTIN_STL_SUPPORT     YES)

        set(DOXYGEN_EXTRACT_ALL             YES)
        set(DOXYGEN_EXTRACT_PRIVATE         YES)
        set(DOXYGEN_EXTRACT_PACKAGE         YES)
        set(DOXYGEN_EXTRACT_STATIC          YES)
        set(DOXYGEN_EXTRACT_LOCALMETHODS    YES)

        set(DOXYGEN_UML_LOOK                YES)
        set(DOXYGEN_UML_LIMIT_NUM_FIELDS    50)

        set(DOXYGEN_CLASS_DIAGRAMS          YES)

        set(DOXYGEN_CLASS_GRAPH             YES)
        set(DOXYGEN_CALL_GRAPH              YES)
        set(DOXYGEN_CALLER_GRAPH            YES)
        set(DOXYGEN_COLLABORATION_GRAPH     YES)

        set(DOXYGEN_HAVE_DOT                YES)
        set(DOXYGEN_MAX_DOT_GRAPH_DEPTH     0)
        set(DOXYGEN_DOT_GRAPH_MAX_NODES     100)
        set(DOXYGEN_DOT_TRANSPARENT         YES)

        set(DOXYGEN_GENERATE_HTML           YES)
        set(DOXYGEN_GENERATE_LATEX          YES)

        doxygen_add_docs(
            ${TARGET_NAME}_DOXYGEN
            ${${TARGET_NAME}_SOURCE}
            ${CMAKE_CURRENT_SOURCE_DIR}/README.md

            COMMENT "Generating API Documentation"
        )
    else()
        message(FATAL_ERROR "Doxygen and Graphviz need to be installed")
    endif()
endif()
