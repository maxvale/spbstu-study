#include <stdbool.h>
#include <stdio.h>
#include <omp.h>

void compareArraysOmp(int* a, int* b, int* c, int len)
{
  int i, j;
  bool check = false;
  bool alcheck = true;
  omp_set_num_threads(4);

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
