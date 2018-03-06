#include <mpi.h>
#include <stdio.h>
int main(int argc, char** argv)
{
	MPI_Init(&argc, &argv);
	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	int wrank;
	MPI_Comm_rank(MPI_COMM_WORLD, &wrank);

	int passes = 0;
	int i = 0;
	static int passesTotal;

	for (i = 0; i < atoi(argv[1]) && passesTotal < atoi(argv[2]); ++i)
	{
		if (wrank == 0)
		{
			MPI_Ssend(&passes, 1, MPI_INT, wrank + 1, 28, MPI_COMM_WORLD);
			MPI_Recv(&passes, 1, MPI_INT, world_size - 1, 28, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			++passes;
			passesTotal += passes;

			printf("Je suis %d. J'ai recu de %d. Nombre de passes : %d.\n\n", wrank, world_size - 1, passes);
		}
		else
		{
			MPI_Recv(&passes, 1, MPI_INT, wrank - 1, 28, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			++passes;
			passesTotal += passes;

			MPI_Ssend(&passes, 1, MPI_INT, (wrank + 1) % world_size, 28, MPI_COMM_WORLD);

			printf("Je suis %d. J'ai recu de %d. Nombre de passes : %d.\n\n", wrank, wrank - 1, passes);
		}

		printf("passes %d\n\n", passes);
		printf("argv2 %d\n\n", atoi(argv[2]));
	}

	MPI_Finalize();
	return 0;
}
