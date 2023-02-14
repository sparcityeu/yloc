
set(YLOC_MODULES "")

message(STATUS "Searching for available modules for yloc")
file(GLOB modules RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *)

include_directories("$<TARGET_PROPERTY:yloc,INTERFACE_INCLUDE_DIRECTORIES>")

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

# Return found modules to the main CMake-file
set(YLOC_MODULES ${YLOC_MODULES} PARENT_SCOPE)
