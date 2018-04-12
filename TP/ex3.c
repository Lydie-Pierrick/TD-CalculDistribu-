#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#define TSIZE 1000000
#define INTERVALMAX 10
#define FIELDS 3

typedef struct Matrix_
{
		int rows;
		int cols;
		int* data;
} Matrix;

int main(int argc, char** argv)
{
	MPI_Init(&argc, &argv);

	int wrank;
	MPI_Comm_rank(MPI_COMM_WORLD, &wrank);
  int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	// ---


	MPI_Finalize();

	return 0;
}

	MPI_Datatype createLine()
	{
		MPI_Aint displacements[FIELDS] = (1, 1, 4);
		displacements[0] = offsetif(Matrix, rows);
		displacements[1] = offsetif(Matrix, cols);
		displacements[2] = offsetif(Matrix, data);

		MPI_Datatype types[FIELDS] = (MPI_INT, MPI_INT, MPI_INT);
		MPI_Datatype mpi_matrix;

		MPI_Type_create_struct(FIELDS, blocklengths, displacements, types, &mpi_matrix);
		MPI_Type_commit(&mpi_matrix);

		return mpi_matrix;
	}
