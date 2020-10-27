#include <iostream>
#include <ctime>
#include <omp.h>
#include <vector>

const int NUM = 10000;

std::vector<int> createVector(int label, int len)
{
  std::vector<int> vector;
  for (int i = 0; i < len; i++)
  {
    vector.push_back(rand() % len);
  }
  return vector;
}

void printVector(std::vector<int> vector)
{
  for (int i = 0; i < vector.size(); i++)
  {
    std::cout << vector.at(i) << " ";
  }
  std::cout << std::endl;
}

int main(int argc, char **argv)
{
  omp_set_num_threads(4);
  std::vector<int> vector_a = createVector(0, NUM);
  std::vector<int> vector_b = createVector(0, NUM);
  std::vector<int> vector_c = createVector(0, NUM);
  int i, j = 0;
  bool check = false;
  bool alcheck = false;
  int t_start = 0;

  //std::cout << "Vectors:" << std::endl;
  //printVector(vector_a);
  //printVector(vector_b);
  //printVector(vector_c);

  t_start = clock();
  for (i = 0; i < vector_c.size(); i++)
  {
    for (j = 0; j < vector_b.size(); j++)
    {
      check = false;
      if (vector_c[i] == vector_a[j] || vector_c[i] == vector_b[j])
      {
        check += true;
        break;
      }
    }
    alcheck *= check;
  }
  std::cout << "Calculation time: " << clock() - t_start << std::endl;
  std::cout << "Superposition: " << std::boolalpha << alcheck << std::endl;

  check = true;
  t_start = clock();

#pragma omp parallel for private(i, j) shared(vector_a, vector_b, vector_c) reduction(*: check)
  for (i = 0; i < vector_c.size(); i++)
  {
    check = false;
    for (j = 0; j < vector_b.size(); j++)
    {
      if (vector_c[i] == vector_a[j] || vector_c[i] == vector_b[j])
      {
        check = true;
        break;
      }
    }
  }
  std::cout << "Calculation time: " << clock() - t_start << std::endl;
  std::cout << "Superposition: " << std::boolalpha << check << std::endl;
  return 0;
}