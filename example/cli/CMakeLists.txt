
add_executable(yloc-cli "main.cc"
        cli.h)

target_link_libraries(yloc-cli yloc)
target_include_directories(yloc-cli PRIVATE)

#add_subdirectory(generic)
#add_subdirectory(gpumonitor)
#add_subdirectory(mpi)
