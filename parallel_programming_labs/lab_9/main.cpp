#include <mpi.h>
#include <random>
#include <ctime>
#include <cstdio>
#include <string>

const std::string FILENAME = "data.binary";

int main(int argc, char **argv)
{
  int result, results[3];
  int my_rank, num_procs;
  int other_rank, offset;
  MPI_File file;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

  if (my_rank != num_procs - 1) {
    srand((unsigned)time(nullptr) + my_rank);
    result = (rand() % 6) + 1;
    MPI_Send(&result, 1, MPI_INT, num_procs - 1, 0, MPI_COMM_WORLD);

    MPI_Recv(results, 3, MPI_INT, num_procs - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  }
  else
  {
    for (other_rank = 0; other_rank < num_procs - 1; other_rank++)
    {
      MPI_Recv(&result, 1, MPI_INT, other_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      printf("Results of %d person: %d points \n", other_rank + 1, result);
      results[other_rank] = result;
    }
    if (results[0] > results[1])
    { results[2] = 1; }
    else
    { results[2] = 2; }
    printf("Winner: %d. Congratulations! \n", results[2]);
    for (other_rank = 0; other_rank < num_procs - 1; other_rank++)
    {
      MPI_Send(results, 3, MPI_INT, other_rank, 0, MPI_COMM_WORLD);
    }
  }

  MPI_File_open(MPI_COMM_WORLD, FILENAME.data(), MPI_MODE_WRONLY | MPI_MODE_CREATE, MPI_INFO_NULL, &file);
  MPI_File_set_view(file, my_rank * 3 * sizeof(int), MPI_INT, MPI_INT, "native", MPI_INFO_NULL);
  MPI_File_write(file, results, 3 * sizeof(int), MPI_INT, MPI_STATUS_IGNORE);
  MPI_File_close(&file);
  MPI_Finalize();
  return 0;
}