#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <ctime>
#include <iterator>
#include "mpi.h"

const int N = 1000;
const double EPS = 0.001;

double generateData (std::vector<std::vector<double>> &A, std::vector<double> &B, std::vector<double> &X) {
  std::srand(std::time(nullptr));

  std::generate(X.begin(), X.end(), []() { return (double) std::rand() / RAND_MAX * 20 - 10; });
  std::generate(A.begin(), A.end(), [i = 0]() mutable {
    std::vector<double> temp(N);
    std::generate(temp.begin(), temp.end(), [i, j = 0]() mutable {
      if (i != j++) {
        return (double) std::rand() / RAND_MAX * 20 - 10;
      } else {
        return (double) std::rand() / RAND_MAX * 20 + 10 * N;
      }
    });
    ++i;
    return temp;
  });

  std::generate(B.begin(), B.end(), [&A, &X, i = 0]() mutable {
    double temp = 0.0;
    for (int j = 0; j < X.size(); j++) {
      temp += A[i][j] * X[j];
    }
    ++i;
    return temp;
  });
  return 0.0;
}

void printVector(std::vector<double> v)
{
  std::cout << "Vector X: ";
  std::for_each(v.begin(), v.end(), [](double vi)
  {
    std::cout << " " << vi << " ";
  });
  std::cout << std::endl;
}

int main(int argc, char** argv) {
  std::vector<std::vector<double>> A(N, std::vector<double>(N));
  std::vector<double> X(N), B(N), X_(N), X0(N);
  double norm_ = 0;
  double start_time = 0;
  double end_time = 0;

  int num_proc, my_rank;

  std::fill(X0.begin(),X0.end(),0);

  generateData(A, B, X0);
  X = X0;
  start_time = std::clock();
  do {
    for (int i = 0; i < N; i++)
    {
      X_[i] = B[i];
      for (int j = 0; j < N; j++)
      {
        if (i != j)
          X_[i] -= A[i][j] * X[j];
      }
      X_[i] / A[i][i];
    }
    norm_ = fabs(X[0] - X_[0]);
    for (int i = 0; i < N; i++)
    {
      if (fabs(X[i]) - X_[i] > norm_)
        norm_ = fabs(X[i] - X_[i]);
      X[i] = X_[i];
    }
  } while (norm_ > EPS);
  end_time = clock();

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &num_proc);


  if (my_rank == 0) {
    std::cout << "Size = " << N << " Eps = " << EPS << "\n";
  }

  MPI_Barrier(MPI_COMM_WORLD);
  start_time = MPI_Wtime();

  MPI_Bcast(B.data(), B.size(), MPI_DOUBLE, 0, MPI_COMM_WORLD);
  std::for_each(A.begin(), A.end(), [](std::vector<double> &v) {
    MPI_Bcast(v.data(), v.size(), MPI_DOUBLE, 0, MPI_COMM_WORLD);
  });

  int num_rank = N / num_proc;
  int rank_i = (N / num_proc) * my_rank;
  double max_norm;
  std::vector<double> xn(num_rank);
  std::vector<double> norm(num_proc);

  do {
    norm[my_rank] = 0.0;

    std::generate(xn.begin(), xn.end(), [&A, &B, &X0, i = rank_i]() mutable {
      double tmpX = B[i];

      std::for_each(X0.cbegin(), X0.cend(), [&A, &tmpX, i, j = 0](double xi) mutable {
        if (i != j) {
          tmpX -= A[i][j] * xi;
        }
        j++;
      });
      return tmpX / A[i][i++];
    });

    std::for_each(xn.cbegin(), xn.cend(), [&X, &norm, my_rank, i = rank_i](double xni) mutable {
      double diff = std::fabs(X[i++] - xni);
      if (diff > norm[my_rank]) {
        norm[my_rank] = diff;
      }
    });

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Allgather(xn.data(), N / num_proc, MPI_DOUBLE, X.data(), N / num_proc, MPI_DOUBLE, MPI_COMM_WORLD);
    MPI_Allgather(&norm[my_rank], 1, MPI_DOUBLE, norm.data(), 1, MPI_DOUBLE, MPI_COMM_WORLD);

    for (int i = (N / num_proc) * (num_proc); i < N; i++) {
      if (my_rank == num_proc - 1) {
        X0[i] = xn[i - ((N / num_proc) * (my_rank))];
      }
      MPI_Bcast(&X0[i], 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    }

    max_norm = *std::max_element(norm.cbegin(), norm.cend());
  } while (max_norm > EPS);

  MPI_Barrier(MPI_COMM_WORLD);
  end_time = MPI_Wtime();

  if (my_rank == 0) std::cout << "Execution time = " << end_time - start_time << "\n";

  MPI_Finalize();
  return 0;
}