option(BUILD_DOC "Build documentation") # default: OFF
#if (CMAKE_BUILD_TYPE MATCHES "^[Rr]elease" OR CMAKE_BUILD_TYPE MATCHES "RelWithDebInfo")
#    set(BUILD_DOC ON)
#endif()

# check if Doxygen is installed
find_package(Doxygen)
if (DOXYGEN_FOUND AND BUILD_DOC)
    # set input and output files
    set(DOXYGEN_IN ${PROJECT_SOURCE_DIR}/docs/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    # request to configure the file
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    message("Doxygen build started")

    # ALL allows to build the docs together with the library
    add_custom_target(yloc-doxygen ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM )

else ()
  message("Doxygen need to be installed to generate the doxygen documentation")
endif ()

# doxygen_add_docs(
#     doxygen ALL
#     ${PROJECT_SOURCE_DIR}
# )
