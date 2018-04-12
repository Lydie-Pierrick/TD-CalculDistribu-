#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

int main(int argc, char** argv)
{
	MPI_Init(&argc, &argv);

	int argX = 0;

	if (argc < 2){
    printf("Erreur arg\n");
    return 0;
  }

	argX = atoi(argv[1]);

	int wrank;
	MPI_Comm_rank(MPI_COMM_WORLD, &wrank);
  int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	srand(time(NULL) + wrank);

	// ---
	int randValue;
	int result;
	int i = 0;
	int countX = 0;
	int maxX = 0;
	int count = 0;
	int resultAll = 0;

	while(countX < argX)
	{
		randValue = 0;
		result = 0;
		count = 0;

		while(randValue = rand() % 52 != 2)
		{
			count++;
		}

		MPI_Reduce(&count, &result, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

		if(wrank == 0)
		{
			resultAll += result;
			printf("I'm wrank 0, my sum is %d\n", resultAll);
		}

		countX++;
	}


	float divX = world_size * argX;

	if(wrank == 0)
	{
		float resultX = resultAll / divX;
		printf("I'm wrank 0, my sum is %f\n", resultX);

	}

	MPI_Finalize();

	return 0;
}
