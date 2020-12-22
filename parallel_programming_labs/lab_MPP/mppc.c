#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include "x86intrin.h"
#include <omp.h>

#pragma GCC target("sse")
#pragma GCC optimize("unroll-loops")

void createArray(int *a, int len)
{
  srand(time(NULL));
  for (int i = 0; i < len; i++)
  {
    a[i] = rand() % len;
  }
}

void compareArrays(int* a, int* b, int* c, int len)
{
  int i, j;
  bool check = false;
  bool alcheck = true;

  for (i = 0; i < len; i++)
  {
    check = false;

    for (j = 0; j < len; j++)
      if (c[i] == a[j] || c[i] == b[j])
        check = true;
    alcheck &= check;
  }
  if (alcheck) printf("This arrays are in superposition \n");
  else printf ("This arrays are not in superposition \n");
}

void compareArraysVec(int* a, int* b, int* c, int len)
{
  int i, j;
  int check = 1;
  int *res = (int *) malloc(len * sizeof(int));
  __m128i mask = _mm_set_epi32(-1, -1, -1, -1);
  __m128i res_b;
  __m128i res_a;
  __m128i res_;
#pragma omp parallel for private(i, j) shared(a, b, c)
  for (i = 0; i < len; i++)
  {
    __m128i c_ = _mm_loadu_si64((__m128i *) &c[i]);
    for (j = 0; j < len; j++)
    {
      __m128i a_ = _mm_loadu_si64((__m128i *) &a[j]);
      __m128i b_ = _mm_loadu_si64((__m128i *) &b[j]);
      res_a = _mm_cmpeq_epi32(a_, c_);
      res_b = _mm_cmpeq_epi32(b_, c_);
    }
    res_ = _mm_or_si128(res_a, res_b);
    _mm_storeu_si64((__m128i *) &res[i], res_);
    check *= res[i];
  }
  if (check == 0) printf("This arrays are in superposition \n");
  else printf ("This arrays are not in superposition %d\n", check);
}

void compareArraysMulti(int* a, int* b, int* c, int len)
{
  int i, j;
  bool check = false;
  bool alcheck = true;

#pragma omp parallel for private(i, j, check) shared(a, b, c) reduction(&: alcheck)
  for (i = 0; i < len; i++)
  {
    check = false;
    for (j = 0; j < len; j++)
        if (c[i] == a[j] || c[i] == b[j])
         check = true;
    alcheck &= check;
  }
  if (alcheck) printf("This arrays are in superposition \n");
  else printf ("This arrays are not in superposition \n");
}


int main ()
{
  int num = 1000;
  int a[1000];
  createArray(a, num);
  int b[1000];
  createArray(b, num);
  int c[1000];
  createArray(c, num);
  int t_start = 0;
  omp_set_num_threads(4);

  compareArrays(a, b, c, num);

  printf("Time:%d \n", (clock()));
  return 0;
}