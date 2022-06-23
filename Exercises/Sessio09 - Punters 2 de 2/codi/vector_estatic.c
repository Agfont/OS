#include <stdio.h>

int main(void)
{
  int a[10];

  printf("Faig assignacions\n");

  a[100] = 1234;
  printf("Direcció de a: %x\n", &a);
  printf("a[100] = %d\n", a[100]);
  printf("Direcció de a[100]: %x\n", &a[100]);

  a[1000] = 4321;
  printf("a[1000] = %d\n", a[1000]);
  printf("Direcció de a[1000]: %x\n", &a[1000]);

  printf("Surto del main\n");

  return  0;
}
