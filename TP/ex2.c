#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#define TSIZE 1000000
#define INTERVALMAX 10

int main(int argc, char** argv)
{
	MPI_Init(&argc, &argv);

	int wrank;
	MPI_Comm_rank(MPI_COMM_WORLD, &wrank);
  int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	srand(time(NULL) + wrank);

	// ---
	int i = 0;
	int count = 0;

	double chronoPZero = 0.0;
	double chronoPOne = 0.0;
	double totalPZero = 0.0;
	double totalPOne = 0.0;
	double totalMoyenne = 0.0;

	double * tabDoubleOne = (double*)calloc(TSIZE,sizeof(double));
	double * tabDoubleTwo = (double*)calloc(TSIZE,sizeof(double));
	double * rcvTabDouble = (double*)calloc(TSIZE,sizeof(double));

	MPI_Request request;

	while(count < 100)
	{
		if(wrank == 0)
		{
				// Genere le tableau 1
				for(i = 0; i < TSIZE; i++)
				{
					tabDoubleOne[i] = ((double) rand() / (double) (RAND_MAX)) * INTERVALMAX;
				}

				chronoPZero = MPI_Wtime();

				// Envoi au processus 1 le tableau 1
				//MPI_Ssend(tabDoubleOne, TSIZE, MPI_DOUBLE, 1, 72, MPI_COMM_WORLD);
				MPI_Isend(tabDoubleOne, TSIZE, MPI_DOUBLE, 1, 72, MPI_COMM_WORLD, &request);

				// Filtre le tableau 1
				for(i = 0; i < TSIZE; i++)
				{
					if(i != 0 && i != TSIZE - 1)
					{
						tabDoubleTwo[i] = (tabDoubleTwo[i-1] + tabDoubleTwo[i+1]) / 2;
					}
				}

				MPI_Wait(&request, MPI_STATUS_IGNORE);

				totalPZero += MPI_Wtime() - chronoPZero;
		}

		if(wrank == 1)
		{
			// Recois le tableau envoye par P0
			//MPI_Recv(rcvTabDouble, TSIZE, MPI_DOUBLE, 0, 72, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			MPI_Irecv(rcvTabDouble, TSIZE, MPI_DOUBLE, 0, 72, MPI_COMM_WORLD, &request);

			// Genere le tableau 2
			for(i = 0; i < TSIZE; i++)
			{
				tabDoubleTwo[i] = rand()%2;
			}

			chronoPOne = MPI_Wtime();

			// Filtre le tableau 2
			for(i = 0; i < TSIZE; i++)
			{
				if(i != 0 && i != TSIZE - 1)
				{
					rcvTabDouble[i] = (rcvTabDouble[i-1] + rcvTabDouble[i+1]) / 2;
				}
			}

			MPI_Wait(&request, MPI_STATUS_IGNORE);

			// Calcul sa moyenne
			for(i = 0; i < TSIZE; i++)
			{
				totalMoyenne += rcvTabDouble[i];
			}

			totalMoyenne = totalMoyenne / TSIZE;

			totalPOne += MPI_Wtime() - chronoPOne;
		}

		count++;
	}

	if(wrank == 0)
	{
		printf("Time P0 : %f\n", totalPZero);
	}

	if(wrank == 1)
	{
		printf("Time P1 : %f\n", totalPOne);
	}

	MPI_Finalize();

	return 0;
}
