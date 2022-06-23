#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int global;

int main(void)
{
  int mida;
  char str[10];

  int local, *vector;

  global = 0;
  local  = 0;

  printf("Funcio main: %lu\n", (unsigned long) &main);
  printf("Variable global: %lu\n", (unsigned long) &global);
  printf("Variable local: %lu\n", (unsigned long) &local);
  printf("Variable vector: %lu\n", (unsigned long) &vector);

  vector = malloc(1000);
  printf("Vector apunta a: %lu\n", (unsigned long) vector);

  return 0;
}
