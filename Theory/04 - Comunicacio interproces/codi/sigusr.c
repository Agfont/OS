#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void sigusr(int signo)
{
  if (signo == SIGUSR1)
    printf("He rebut SIGURS1.\n");
  if (signo == SIGUSR2)
    printf("He rebut SIGURS2.\n");
}

int main(void)
{
  signal(SIGUSR1, sigusr);
  signal(SIGUSR2, sigusr);

  while (1)
  {
    printf("Espero senyal!\n");
    pause(); 
  }

  return 0;
}
