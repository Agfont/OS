#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int done = 0;

void finalitzar(int signo)
{
  printf("He capturat el senyal.\n");
  done = 1;
}

int main(void)
{
  int comptador = 0;

  signal(SIGTERM, finalitzar);

  while (!done)
  {
    sleep(1);
    comptador++;
    printf("He esperat %d segons\n", comptador);
  }

  printf("S'ha acabat el bucle. Finalitzo normalment\n");

  return 0;
}
