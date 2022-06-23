// JOAN MARTIN MARTRUS & SALVADOR MIRA MORILLAS

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAXVALUE 1000

int done = 0; // La utilitzare per saber quan haig de deixar de enviar el SIGUSR2

void sigusr1(int signo)
{
  printf("El pare ha rebut el SIGUSR1\n");
  done = 1; // Un cop el fill envia SIGUSR2, puc estar segur que el fill ha rebut SIGUSR2 i per tant puc deixar de enviar-lo
}

void sigusr2(int signo)
{
    printf("El fill ha rebut el SIGUSR2\n");

}

int main(void)
{
    int i, num_values, value, sum, fd[2];
    int ret, parent_pid, child_pid;
    
    pipe(fd);

    ret = fork();

    if (ret == 0) { // child

        int values_child[MAXVALUE], num_values_child;

        signal(SIGUSR2,sigusr2);

        printf("EL FILL ES POSA EN PAUSA\n");
        pause();
        printf("EL FILL S'HA ACTIVAT\n");


        parent_pid = getppid();

        read(fd[0], &num_values_child, sizeof(int));


        i = 0;
        sum = 0;
        while (i < num_values_child) {
            int valor_llegit;
            read(fd[0], &valor_llegit, sizeof(int));
            printf("LLEGEIXO DEL PIPE: %d    [%d]\n", valor_llegit,i );

            sum += valor_llegit;
            i++;
        }

        //printf("El fill escriu el resultat: %d\n", sum);
        /* Escriure el valor de la suma a la canonada */

        write(fd[1], &sum, sizeof(int));

        while (1) {
            sleep(1);
            kill(parent_pid, SIGUSR1);
        }
        

        exit(0);
    } else { // parent

        signal(SIGUSR1, sigusr1);
        child_pid = ret;

        srand(time(NULL));
        i = 0;
        /* Escriure aquest valor a la canonada */
        num_values = 1000;
        write(fd[1], &num_values, sizeof(int));
        while (i < num_values) {  
            /* Generar valor aleatori i inserir a la canonada */
            value = rand() % MAXVALUE + 1;
            printf("FICO AL PIPE: %d    [%d]\n", value,i );
            write(fd[1], &value, sizeof(int));
            i++;
        }

       
        while(!done) {
            sleep(1);
            kill(child_pid, SIGUSR2);
        }

        
        printf("El pare espera la suma...\n");
        pause();

        kill(child_pid, SIGTERM); //Mato al proces fill per tal que no es quedi atrapat al while

    
        read(fd[0], &sum, sizeof(int));
        printf("El fill em diu que la suma es: %d\n", sum);
        wait(NULL);
    }

    return 0;
}
