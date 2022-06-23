#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
  int pid, count;

  if (argc != 2) {
    printf("Us: %s <pid>\n", argv[0]);
    exit(1);
  }

  pid = atoi(argv[1]);
  printf("Enviare senyals al proces amb pid %d\n", pid);

  count = 0;

  while (count < 4) {
    sleep(1);
    printf("Envio el SIGUSR1\n");
    kill(pid, SIGUSR1);
    sleep(1);
    printf("Envio el SIGUSR2\n");
    kill(pid, SIGUSR2);
    count++;
  }

  printf("Envio el SIGTERM\n");
  kill(pid, SIGTERM);

  return 0;
}
