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

	// --
	int i = 0;
	float random = 0.0;
	const int sizeTab = 10000;
	float * tab = (float*)calloc(sizeTab,sizeof(float));
	const float intervalMax = 10.0;
	float sumSerie = 0.0;
	float sumParallel = 0.0;

	// Generer le tableau
	for(i = 0; i < sizeTab; i++)
	{
		random = ( (float) rand() / (float) (RAND_MAX) ) * intervalMax;
		tab[i] = random;
	}

	// Calcul en série pour le process 0
	if(wrank == 0)
	{
		for(i = 0; i < sizeTab; i++)
		{
			sumSerie+=tab[i];
		}
	}

	// Calcul en Paralel
	for(i = 0; i < sizeTab / 4; i++)
	{
			sumParallel += random;
	}

	MPI_Gather();

	printf("Sum Serie%.6f", sumSerie);
	printf("Sum Paralel%.6f", sumParallel);

	MPI_Finalize();

	return 0;
}
