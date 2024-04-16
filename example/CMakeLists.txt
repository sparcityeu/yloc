# Basic example (filtering / distances)
add_executable(example "example.cc")
target_link_libraries(example yloc)
target_include_directories(example PRIVATE)

# GPU example
add_executable(example-gpu "example-gpu.cc")
target_link_libraries(example-gpu yloc)
target_include_directories(example-gpu PRIVATE)

# MPI example
find_package(MPI)
if(${MPI_FOUND})
    add_executable(example-mpi "example-mpi.cc")    
    target_link_libraries(example-mpi yloc)
    target_include_directories(example-mpi PRIVATE ${MPI_CXX_INCLUDE_PATH})
    target_link_libraries(example-mpi ${MPI_CXX_LIBRARIES})
endif()

# CLI
add_executable(yloc-cli "cli.cc" "cli.h")
target_link_libraries(yloc-cli yloc)
target_include_directories(yloc-cli PRIVATE)
