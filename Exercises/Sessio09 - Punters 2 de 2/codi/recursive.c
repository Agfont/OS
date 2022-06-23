#include <stdio.h>

void funcio(int a)
{
  int b[1000];
  printf("%d\n", a);
  funcio(a+1);
}

int main(void)
{
  funcio(0);

  return 0;
}
