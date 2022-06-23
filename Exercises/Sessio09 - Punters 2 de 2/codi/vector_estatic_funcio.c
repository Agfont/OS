#include <stdio.h>

void funcio()
{
  int a[1000];
  int b;

  b = 5;

  printf("Direccio de b: %x\n", &b);
  printf("Valor de b: %d\n", b);

  printf("Direcció de a[0]: %x\n", &a[0]);
  printf("Direccio de a[-1]: %x\n", &a[-1]);

  a[-1] = 10;

  printf("Valor de b: %d\n", b);
}

int main(void)
{
  funcio();
}
