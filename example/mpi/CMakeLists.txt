find_package(MPI)

if(NOT ${MPI_FOUND})
    return()
endif()

add_executable(example-mpi "main.cc")

target_link_libraries(example-mpi yloc)
target_include_directories(example-mpi PRIVATE ${MPI_CXX_INCLUDE_PATH})
target_link_libraries(example-mpi ${MPI_CXX_LIBRARIES})
