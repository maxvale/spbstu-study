#include <mpi.h>
#include <random>
#include <ctime>
#include <cstdio>
#include <string>

const int LEN_DATA = 10;

void generateData(int *array)
{
  for (int i = 0; i < LEN_DATA; i++)
  {
    array[i] = (rand() % 100);
  }
}

void printBuffer(int *array)
{
  for (int i = 0; i < LEN_DATA; i++)
  {
    std::printf("%d ", array[i]);
  }
  std::printf("\n");
}

int main(int argc, char **argv)
{
  int my_rank, num_procs;
  int other_rank, buffer[LEN_DATA] = {0};
  int root = 0;
  int result_buf[LEN_DATA] = {0};
  double start_time = 0;
  double end_time = 0;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

  //p2p algorithm
  MPI_Barrier(MPI_COMM_WORLD);
  start_time = MPI_Wtime();
  if (my_rank != 0)
  {
    std::srand(std::clock());
    generateData(buffer);
    std::printf("This is %d proc with buffer for MyReduce: ", my_rank);
    printBuffer(buffer);
    MPI_Send(buffer, LEN_DATA, MPI_INT, 0, 0, MPI_COMM_WORLD);
  }
  MPI_Barrier(MPI_COMM_WORLD);
  if (my_rank == 0)
  {
    int result[LEN_DATA] = {0};
    for (other_rank = 1; other_rank < num_procs; other_rank++)
    {
      MPI_Recv(buffer, LEN_DATA, MPI_INT, other_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      for (int i = 0; i < LEN_DATA; i++)
      {
        result[i] += buffer[i];
      }
    }
    printf("Data in proc #0 with MyReduce: ");
    printBuffer(result);
  }
  MPI_Barrier(MPI_COMM_WORLD);
  end_time = MPI_Wtime();


  if (my_rank == 0) std::printf("Execution time of MyReduce: %f \n", end_time - start_time);

  if (my_rank != 0)
  {
    generateData(buffer);
    std::printf("This is %d proc with buffer for MPI_Reduce: ", my_rank);
    printBuffer(buffer);
  }
  MPI_Barrier(MPI_COMM_WORLD);
  start_time = MPI_Wtime();
  MPI_Reduce(buffer, result_buf, LEN_DATA, MPI_INT, MPI_SUM, root, MPI_COMM_WORLD);
  if (my_rank == 0)
  {
    std::printf("Data in proc #0 with MPI_Reduce:");
    printBuffer(result_buf);
  }
  MPI_Barrier(MPI_COMM_WORLD);
  end_time = MPI_Wtime();


  if (my_rank == 0) std::printf("Execution time of MPI_Reduce: %f \n", end_time - start_time);

  MPI_Finalize();
  return 0;
}