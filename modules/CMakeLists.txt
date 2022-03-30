
set(YLOC_MODULES "")

message(STATUS "Searching for available modules for yloc")
file(GLOB modules RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *)

# Todo custom module order, necessary?
# Custom order during cmake or only in YLOC_MODULES-list?

foreach(module ${modules})
    # Check if the found module can be configured
    if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${module} AND
       EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${module}/CMakeLists.txt)
        string(TOUPPER "${module}" MOD_VAR)
        set(ENABLE_${MOD_VAR} ON CACHE BOOL "Enable module yloc-${module}.")

        # Check variable from cache to allow disabling of unwanted modules
        if(${ENABLE_${MOD_VAR}})
            # Add directory and library target within
            set(MOD_TARGET "yloc-${module}")
            add_subdirectory(${module})

            if(TARGET ${MOD_TARGET})    # todo check for target type == library
                message(STATUS "Prepared module ${MOD_TARGET} for building")

                list(APPEND YLOC_MODULES "${MOD_TARGET}")
            else()
                message(STATUS "Module ${MOD_TARGET} is not available")
            endif()
        else()
            message(STATUS "Not building module ${MOD_TARGET} (ENABLE_${MOD_VAR} is ${ENABLE_${MOD_VAR}})")
        endif()
    endif()
endforeach()

set(MAIN_MODULE "yloc-hwloc" CACHE STRING "Main module that is used as default for initialising of the graph.")
set_property(CACHE MAIN_MODULE PROPERTY STRINGS ${YLOC_MODULES})
# Todo minor, cmake could be used to reduce this list down to only main modules (would prevent runtime errors (compiletime with C++20))

# Return found modules to the main CMake-file
set(YLOC_MODULES ${YLOC_MODULES} PARENT_SCOPE)
