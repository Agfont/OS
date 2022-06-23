#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
  int a;
  pid_t pid;

  a = 1;

  printf("Direccio d'a abans fork: %lu\n", (unsigned long) &a);

  pid = fork();

  if (pid == 0) { // fill
    a = 2;
    printf("Fill valor d'a: %d\n", a);
    printf("Fill direccio d'a: %lu\n", (unsigned long) &a);
  }
  else { // pare
    sleep(1);
    printf("Pare valor d'a: %d\n", a);
    printf("Pare direccio d'a: %lu\n", (unsigned long) &a);
  }

  return 0;
}
