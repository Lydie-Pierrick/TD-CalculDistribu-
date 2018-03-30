#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

int main(int argc, char** argv)
{
	if (argc < 2){
		printf("Erreur arg\n");
		return 0;
	}

	MPI_Init(&argc, &argv);

	int wrank;
	MPI_Comm_rank(MPI_COMM_WORLD, &wrank);
  int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	srand(time(NULL) + wrank);
  int n = 0;
	int R = world_size;
	int N = atoi(argv[1]);

	for (n = 0; n < N; n++)
	{
			int r = n % R;
			
			if(wrank == r)
			{
				FILE *f;
				f = fopen("out.txt", "a");
				fprintf(f, "%d (rang %d)\n", n, r);
				fclose(f);
			}

			MPI_Barrier(MPI_COMM_WORLD);
	}

	MPI_Finalize();

	return 0;
}
