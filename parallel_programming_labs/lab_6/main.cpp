#include <iostream>
#include <ctime>
#include <omp.h>
#include <vector>

#define THREADS_NUM 4
#define C_LEN 1000000

char random[26] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
                   'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

std::vector<char> createVector(int label, int len) //label - 1 creates palindrome, label - 0 creates random str
{
  std::vector<char> result;
  if (label == 1)
  {
    if (len % 2 != 0) result.push_back(random[rand() % 8]);
    for (int i = 0; i < len / 2; i++)
    {
      char letter = random[rand() % 8];
      result.push_back(letter);
      result.insert(result.begin(), letter);
    }
  }
  else
  {
    for (int i = 0; i < len; i++)
    {
      char letter = random[rand() % 8];
      result.push_back(letter);
    }
  }
  return result;
}

void printVector(std::vector<char> vector)
{
  for (int i = 0; i < vector.size(); i++)
  {
    std::cout << vector.at(i) << " ";
  }
}

int main(int argc, char** argv)
{
  int step = 0;
  std::cout << "Print number of chars for thread:";
  std::cin >> step;

  std::vector<char> array_c;
  unsigned int t_start = 0;
  bool check = true;
  int i, j = 0;

  omp_set_num_threads(THREADS_NUM);
  array_c = createVector(1, C_LEN);

  t_start = clock();
  for (i = 0; i < array_c.size() / 2; i += step)
  {
    for (j = 0; j < step; j++)
    {
      if (array_c.at(i + j) != array_c.at((array_c.size() - 1) - (i + j)))
        check = false;
    }
  }
  /*for (i = 0; i < array_c.size() / 2; i++)
  {
    if (array_c.at(i) != array_c.at(array_c.size() - 1 - i))
      check = false;
  }*/
  std::cout << "Calculation time: " << clock() - t_start << std::endl;
  std::cout << std::boolalpha;
  std::cout << "Palindrome: " << check << std::endl << std::endl;


  t_start = clock();
  #pragma omp parallel for private(i, j) shared(array_c) reduction(+:check)
    for (i = 0; i < array_c.size() / 2; i += step)
    {
      for (j = 0; j < step; j++)
      {
        if (array_c.at(i + j) != array_c.at((array_c.size() - 1) - (i + j)))
          check = false;
      }
    }
    std::cout << "Calculation time: " << clock() - t_start << std::endl;
    std::cout << std::boolalpha;
    std::cout << "Palindrome: " << check << std::endl;

  system("pause");
  return 0;
}
