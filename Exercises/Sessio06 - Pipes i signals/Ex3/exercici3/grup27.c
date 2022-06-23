#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void sigusr1(int signo)
{
  printf("El pare ha rebut el SIGUSR1\n");
}

void sigusr2(int signo)
{
  printf("El fill ha rebut el SIGUSR2\n");
}

int main(void)
{
  int ret, parent_pid, child_pid, fd[2], N, Tot, i,sumTot;

  pipe(fd);

  ret = fork();

  if (ret == 0) {

    signal(SIGUSR2, sigusr2);
    parent_pid = getppid();

    /* Aquí esperem a que el pare ens doni l'ordre per començar a sumar */
    pause();

    //Variable contador
    i = 0;

    //Variable que usa el fill per acumular els nombres
    int sum = 0;

    /* Llegim el primer element del pipe i el guardem a la variable Tot*/
    read(fd[0], &Tot, 4);

    /* Sumem el primer element a la suma total i imprimim aquest primer element */
    sum = sum + Tot;
    printf("El nombre llegit %i es: %i\n", i, Tot);

    /* Mentre el contador sigui més petit que el primer element menys 1, anem llegint tots els elements del pipe i sumant-los fent ara servir 
     * la variable N per treure els nombres del Pipe per a que no canvii la variable Tot i ens descuadri el while*/
    while (i<(Tot-1)) {
      i++;
      read(fd[0], &N, 4);
      printf("El nombre llegit %i es: %i\n", i, N);
      sum = sum + N;
    }

    /* A continuacio imprimim la suma total, l'escrivim al pipe i cridem al pare amb la funcio kill */
    printf("La suma total es: %i\n", sum);
    write(fd[1],&sum,4);
    printf("Envio SIGUSR1 al pare %d\n", parent_pid);
    kill(parent_pid, SIGUSR1);
    exit(0);
  } 
  else {

    signal(SIGUSR1, sigusr1);
    child_pid = ret;

    /* Primer de tot plantem la seed dels randoms i calculem el primer random a la variable Tot */
    printf("Estem al pare, ara calculem el random\n");
    srand(time(NULL));
    Tot = rand() % 20;

    /* Escribim el primer nombre a la pipe i imprimim quin nombre es */
    write(fd[1], &Tot, 4);
    printf("El nombre escrit 0 es: %i\n",Tot);

    /* Posem el contador a 1 donat que ja hem fet el primer nombre */
    i = 1;

    /* Mentre el contador sigui menor que el primer nombre random, anem generant nous randoms guardant-los 
     * a la variable N i posteriorment a la Pipe (també els imprimim per pantalla per mostrar quins son) */
    while (i<Tot) {
    /* Fem servir modul 1000 per a que no es generin randoms molt grans */
       N = rand() % 1000;
       write(fd[1], &N, 4);
       printf("El nombre escrit %i es: %i\n", i, N);
       i++;
          }
    /* Cridem al fill i pausem al pare */
    printf("Envio SIGUSR2 al fill %d\n", child_pid);
    kill(child_pid, SIGUSR2);
    pause();

    /* Llegim el valor que ens ha enviat el fill i l'escrivim per pantalla */
    read(fd[0],&sumTot,4);
    printf("El fill em diu que la suma es: %i\n", sumTot);

    wait(NULL);
  }

  return 0;
}
