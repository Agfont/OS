#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAXVALUE 1000

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
    int i, num_values, value, sum, fd[2];
    int ret, parent_pid, child_pid;
    

    pipe(fd);

    ret = fork();

    if (ret == 0) { // child
      
        /* Inserir codi aqui per gestionar senyals */
       
        /* Esperar a rebre senyal del pare */
	signal(SIGUSR2, sigusr2);

        /* Llegir el nombre de valors que ens envia el pare */
        i = 0;
        sum = 0;
	read(fd[0], &num_values, sizeof(num_values)); 
        while (i < num_values) {
            /* Llegir valors de la canonada i fer la suma */
	    read(fd[0], &value, sizeof(value));
	    sum = sum + value;
            i++;
        }
    
        printf("El fill escriu el resultat: %d\n", sum);
        /* Escriure el valor de la suma a la canonada */
        write(fd[1], &sum, sizeof(sum));
        /* Avisar al pare que s'han escrit els valors */ 
	kill(getpid(), SIGUSR2);
        exit(0);

    } else { // parent

        /* Inserir codi aqui per gestionar senyals */
	signal(SIGUSR1, sigusr1);	
        /* Random seed */
        srand(time(NULL));

        i = 0;
       
        /* Escriure aquest valor a la canonada */
        num_values = 1000;
        write(fd[1], &num_values, sizeof(num_values));
        while (i < num_values) {  
            /* Generar valor aleatori i inserir a la canonada */
            value = rand() % MAXVALUE + 1;
	    write(fd[1], &value, sizeof(value));
            i++;
        }
	

        printf("El pare espera la suma...\n");

        /* Inserir codi aqui per esperar senyal del pare */
	signal(SIGUSR1, sigusr1);
        /* Llegir el resultat */
	read(fd[0], &sum, sizeof(sum));
        printf("El fill em diu que la suma es: %d\n", sum);

        wait(NULL);
	kill(ret, SIGUSR1);
    }
    return 0;
}
