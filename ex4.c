#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

short * generateT(int N)
{
	short * T = calloc(N, sizeof(short));
  int i = 0;
  for(i = 0; i < N; ++i)
    T[i] = rand()%10;
  return T;
}

int * getTable(short * T, int N)
{
  int i, j, index;
  int * t = calloc(10, sizeof(int));
  for(i = 0; i < 10; i++)
    t[i] = 0;

  for(j = 0; j < N; j++)
  {
    index = T[j];
    t[index] = t[index] + 1;
  }

  return t;
}

int main(int argc, char** argv)
{
	MPI_Init(&argc, &argv);

	int wrank;
	MPI_Comm_rank(MPI_COMM_WORLD, &wrank);
  int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	srand(time(NULL) + wrank);
  int i = 0;
  int N;

  int tag1 = 1;
	int tag2 = 5;

  short * T = calloc(N, sizeof(short));
  int * t;

    if(wrank%2 == 0) //pair
    {
    	N = rand()%100 + 100;
    	T = generateT(N);

    if(wrank + 1 < world_size) // if 2k+1 exists
    {
      MPI_Ssend(&N, 1, MPI_INT,wrank+1, tag1, MPI_COMM_WORLD);
      MPI_Ssend(T, N, MPI_INT, wrank+1, tag2, MPI_COMM_WORLD);
      //printf("P%d envoie a P%d : ", wrank, wrank+1);
      //for(i = 0; i < N; ++i) {
      	//printf("%d ", T[i]);
	//}
      
	//printf("%s\n","");
    }
    else // if 2k+1 doesn't exist
    { 
	printf("Rank%d  : ", wrank);
       t = getTable(T, N);
       for(i = 0; i < 10; ++i)
	   printf("%d ", t[i]);
       printf("%s\n","");
    }
  }
  else // impair
  {
    MPI_Recv(&N, 1, MPI_INT, wrank-1, tag1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Recv(T, N, MPI_INT, wrank-1, tag2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printf("Rank%d  : ", wrank);
    // for(i = 0; i < N; ++i)
    //     printf("%d", T[i]);
    // printf("%s\n","");
    //
    t = getTable(T, N);
    for(i = 0; i < 10; ++i)
        printf("%d ", t[i]);
    printf("%s\n","");
  }


	MPI_Finalize();

	return 0;
}
