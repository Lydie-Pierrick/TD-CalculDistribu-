#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

int main(int argc, char** argv)
{
	MPI_Init(&argc, &argv);

	int wrank;
	MPI_Comm_rank(MPI_COMM_WORLD, &wrank);
  int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	srand(time(NULL) + wrank);

	// ---
	int randValue;
	int result;
	int * receive = (int*)calloc(world_size, sizeof(int));
	int i = 0;
	int isOK = 0;

	while(1)
	{
		randValue = 0;
		result = 0;

		randValue = rand() % 6 + 1;

		MPI_Allgather(&randValue, 1, MPI_INT, receive, 1, MPI_INT,MPI_COMM_WORLD);

		for(i = 0; i < world_size; i++)
		{
			if(receive[i] == 6)
			{
				isOK = 1;
			}
		}

		printf("I'm p%d and my random value is %d\n", wrank, randValue);

		MPI_Reduce(&randValue, &result, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

		if(wrank == 0)
		{
			printf("I'm wrank 0, my sum is %d\n", result);
		}

		if(isOK == 1)
		{
			break;
		}
	}

	MPI_Finalize();

	return 0;
}
