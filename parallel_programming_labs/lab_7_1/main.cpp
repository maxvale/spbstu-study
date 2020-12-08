#include <iostream>
#include <omp.h>
#include <cmath>
#include <ctime>

#define THREAD_NUM 4

void printArray(double array[], int len)
{
  for (int i = 0; i < len; i++)
  {
    std::cout << array[i] << std::endl;
  }
}

int main(int argc, char** argv)
{
  omp_set_num_threads(THREAD_NUM);
  const int N = 100000;
  double a[N], b[N];
  a[0] = 1;
  b[0] = 1;
  double x;
  double result = 0;
  int i = 0;
  unsigned int t_start = 0;

  t_start = clock();
  for (int i = 1; i < N; i++)
  {
    x = sqrt(i);
    result = x * (i + x*2 + 1);
    b[i] = result;
  }
  std::cout << "Calculation time: ";
  std::cout << clock() - t_start << std::endl;
  //printArray(b, N);

  t_start = clock();
#pragma omp parallel for private(i, x) shared(a)
  for (i = 1; i < N; i++)
  {
    x = sqrt(i);
    result = x * (i + x*2 + 1);
    a[i] = result;
  }
  std::cout << "Calculation time: ";
  std::cout << clock() - t_start << std::endl;

  //printArray(a, N);
  return 0;
}