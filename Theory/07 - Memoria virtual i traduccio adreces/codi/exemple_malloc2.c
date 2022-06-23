#include <stdio.h>
#include <time.h>
#include <stdlib.h>

void main(void)
{
  size_t i, mida, nelems, meitat, darrer;
  int *a;

  mida   = 8589934592;
  nelems = mida / sizeof(int); 

  printf("mida = %lu   nelems = %lu\n", mida,nelems);
  
  a = malloc(mida);

  if (!a)
  {
    printf("No he pogut ubicar la memoria.\n");
    exit(1);
  }

  darrer = nelems - 1;
  meitat = nelems / 2;

  for(i = 0; i < darrer; i++) {
    a[i] = 10;
    if (i % 1000000 == 0)
       printf("%lu\n", i);
  }
  
  free(a);
}


