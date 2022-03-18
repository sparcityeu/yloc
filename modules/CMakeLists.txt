
set(YLOC_MODULES "")

message(STATUS "Searching for available modules for yloc")
file(GLOB modules RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *)

foreach(module ${modules})
    # Check if the found module can be configured
    if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${module} AND
       EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${module}/CMakeLists.txt)
        string(TOUPPER "${module}" MOD_VAR)
        set(ENABLE_${MOD_VAR} ON CACHE BOOL "Enable module yloc-${module}")

        # Check variable from cache to allow disabling of unwanted modules
        if(${ENABLE_${MOD_VAR}})
            # Add directory and library target within
            set(MOD_SOURCES "")
            set(MOD_DEPENDS "")
            set(MOD_AVAIL ON)
            add_subdirectory(${module})
            list(TRANSFORM MOD_SOURCES PREPEND "${module}/")

            if(MOD_AVAIL)
                add_library(yloc-${module} STATIC
                            ${MOD_SOURCES})
                target_link_libraries(yloc-${module}
                            ${MOD_DEPENDS})
                message(STATUS "Prepared module yloc-${module} for building")

                list(APPEND YLOC_MODULES "yloc-${module}")
            else()
                message(STATUS "Module yloc-${module} is not available")
            endif()
        else()
            message(STATUS "Not building module yloc-${module} (ENABLE_${MOD_VAR} is ${ENABLE_${MOD_VAR}})")
        endif()
    endif()
endforeach()

# Return found modules to the main CMake-file
set(YLOC_MODULES ${YLOC_MODULES} PARENT_SCOPE)
