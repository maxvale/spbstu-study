#include <stdio.h>
#include <stdbool.h>
#include <omp.h>
#include <x86intrin.h>

#pragma GCC target("sse")
#pragma GCC optimize("O3")

void compareArraysVecOmp(int* a, int* b, int* c, int len)
{
  int i, j;
  int check = 1;
  int *res = (int *) malloc(len * sizeof(int));
  __m128i mask = _mm_set_epi32(-1, -1, -1, -1);
  __m128i res_b;
  __m128i res_a;
  __m128i res_;
  omp_set_num_threads(4);


#pragma omp parallel for private(i, j) shared(a, b, c) reduction(*: check)
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
  else printf ("This arrays are not in superposition %d\n");
}
