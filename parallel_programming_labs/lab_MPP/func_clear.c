#include <stdio.h>
#include <stdbool.h>

void compareArraysClear(int* a, int* b, int* c, int len)
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