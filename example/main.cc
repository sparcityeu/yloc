

//#include <mpi.h>
#include <yloc.h>

int main(int arc, char *argv[]) {
    // MPI_Init(&argc, &argv);
    yloc_init(YLOC_FULL | YLOC_ONGOING);

    //todo

    yloc_finalize();
    // MPI_Finalize();
}
