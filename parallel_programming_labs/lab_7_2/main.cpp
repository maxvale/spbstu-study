#include <iostream>
#include <ctime>
#include <omp.h>
#include <vector>

const int NUM = 1000000;

std::vector<int> createVector(int len)
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
    std::cout << vector.at(i) << " ";
  std::cout << std::endl;
}

int main(int argc, char **argv)
{
  omp_set_num_threads(4);
  std::vector<int> vector_a = {1, 1};//createVector(NUM);
  std::vector<int> vector_b = {0, 0};//createVector(NUM);
  std::vector<int> vector_c = {1, 1};//createVector(NUM);
  int i, j, k = 0;
  bool check = false;
  bool alcheck = true;
  int t_start = 0;

  //std::cout << "Vectors:" << std::endl;
  //printVector(vector_a);
  //printVector(vector_b);
  //printVector(vector_c);

  t_start = clock();
  for (i = 0; i < vector_c.size(); i++)
  {
    check = false;
    for (j = 0; j < vector_b.size(); j++)
      if (vector_c[i] == vector_a[j] || vector_c[i] == vector_b[j]) { check = true; }
    alcheck &= check;
  }
  std::cout << "Calculation time: " << clock() - t_start << std::endl;
  std::cout << "Superposition: " << std::boolalpha << alcheck << std::endl;
  std::cout << "------------------" << std::endl;

  t_start = clock();
  for (i = 0; i < vector_c.size(); i++)
  {
    check = false;
    for (j = 0; j < vector_a.size(); j++)
      for (k = 0; k < vector_b.size(); k++)
        if (vector_c[i] == vector_a[j] && vector_c[i] == vector_b[k]) { check = true; }
    alcheck &= check;
  }
  std::cout << "Calculation time: " << clock() - t_start << std::endl;
  std::cout << "Intersection: " << std::boolalpha << alcheck << std::endl;
  std::cout << "------------------" << std::endl;


  check = true;
  alcheck = true;
  t_start = clock();
  for (i = 0; i < vector_c.size(); i++)
  {
    check = false;
    for (j = 0; j < vector_a.size(); j++)
      if (vector_c[i] == vector_a[j])
      {
        check = true;
        for (k = 0; k < vector_b.size(); k++)
        {
          if (vector_c[i] == vector_b[k])
          {
            check = false;
          }
        }
      }
    alcheck &= check;
  }
  std::cout << "Calculation time: " << clock() - t_start << std::endl;
  std::cout << "Relative complement of B in A: " << std::boolalpha << alcheck << std::endl;
  std::cout << "------------------" << std::endl;

  check = true;
  alcheck = true;
  t_start = clock();
  for (i = 0; i < vector_c.size(); i++)
  {
    check = false;
    for (j = 0; j < vector_b.size(); j++)
      if (vector_c[i] == vector_b[j])
      {
        check = true;
        for (k = 0; k < vector_a.size(); k++)
        {
          if (vector_c[i] == vector_a[k])
          {
            check = false;
          }
        }
      }
    alcheck &= check;
  }
  std::cout << "Calculation time: " << clock() - t_start << std::endl;
  std::cout << "Relative complement of A in B: " << std::boolalpha << alcheck << std::endl;
  std::cout << "------------------" << std::endl;
  std::cout << "Using OMP" << std::endl;
  std::cout << "------------------" << std::endl;

  t_start = clock();
  alcheck = true;
  check = true;
#pragma omp parallel for private(i, j, check) shared(vector_a, vector_b, vector_c) reduction(&: alcheck)
  for (i = 0; i < vector_c.size(); i++)
  {
    check = false;
    for (j = 0; j < vector_b.size(); j++)
      if ((vector_c[i] == vector_a[j]) || (vector_c[i] == vector_b[j])) { check = true; }
    alcheck &= check;
  }
  std::cout << "Calculation time: " << clock() - t_start << std::endl;
  std::cout << "Superposition: " << std::boolalpha << alcheck << std::endl;
  std::cout << "------------------" << std::endl;

  t_start = clock();
  check = true;
  alcheck = true;
#pragma omp parallel for private(i, j, k, check) shared(vector_a, vector_b, vector_c) reduction(&: alcheck)
  for (i = 0; i < vector_c.size(); i++)
  {
    check = false;
    for (j = 0; j < vector_a.size(); j++)
      for (k = 0; k < vector_b.size(); k++)
        if (vector_c[i] == vector_a[j] && vector_c[i] == vector_b[k]) { check = true; }
    alcheck &= check;
  }
  std::cout << "Calculation time: " << clock() - t_start << std::endl;
  std::cout << "Intersection: " << std::boolalpha << alcheck << std::endl;
  std::cout << "------------------" << std::endl;

  t_start = clock();
  check = true;
  alcheck = true;
#pragma omp parallel for private(i, j, k, check) shared(vector_a, vector_b, vector_c) reduction(&: alcheck)
  for (i = 0; i < vector_c.size(); i++)
  {
    check = false;
    for (j = 0; j < vector_a.size(); j++)
      if (vector_c[i] == vector_a[j])
      {
        check = true;
        for (k = 0; k < vector_b.size(); k++)
          if (vector_c[i] == vector_b[k]) {check = false;}
      }
    alcheck &= check;
  }
  std::cout << "Calculation time: " << clock() - t_start << std::endl;
  std::cout << "Relative complement of B in A: " << std::boolalpha << alcheck << std::endl;
  std::cout << "------------------" << std::endl;

  t_start = clock();
  check = true;
  alcheck = true;
#pragma omp parallel for private(i, j, k, check) shared(vector_a, vector_b, vector_c) reduction(&: alcheck)
  for (i = 0; i < vector_c.size(); i++)
  {
    check = false;
    for (j = 0; j < vector_b.size(); j++)
      if (vector_c[i] == vector_b[j])
      {
        check = true;
        for (k = 0; k < vector_a.size(); k++)
          if (vector_c[i] == vector_a[k]) {check = false;}
      }
    alcheck &= check;
  }
  std::cout << "Calculation time: " << clock() - t_start << std::endl;
  std::cout << "Relative complement of A in B: " << std::boolalpha << alcheck << std::endl;
  std::cout << "------------------" << std::endl;
  return 0;
}