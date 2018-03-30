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
  int i = 0;
  int nbBoucleArg = atoi(argv[1]);

  int randomValue = 0;
  int counterOne = 0;
  int counterZero= 0;

  int * tabBufferReceiv;

  // ---

  int isOK = 0;
  int try = 0;

  // Si WRANK 0 alors vérifier si arret
  tabBufferReceiv = (int*)calloc(world_size, sizeof(int));

  for(try = 0; try < nbBoucleArg; try++)
  {
      // Generation de pile ou face
      randomValue = rand()%2;
      isOK = 0;
      counterZero = 0;
      counterOne = 0;

      MPI_Allgather(&randomValue, 1, MPI_INT, tabBufferReceiv, 1, MPI_INT, MPI_COMM_WORLD);

      for(i = 0; i < world_size; i++)
      {
        if(tabBufferReceiv[i] == 0)
        {
          counterZero++;
        }
        else if(tabBufferReceiv[i] == 1)
        {
          counterOne++;
        }

        if(counterZero == world_size || counterOne == world_size)
        {
          isOK = 1;
        }
      }

      printf("Try num : %d RandomValue = %d and rank = %d", try, randomValue, wrank);
      printf("\n");

      if(isOK == 1)
      {
        break;
      }

  }


	MPI_Finalize();

	return 0;
}
