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
    int aux,i, num_values, value, sum, fd[2];
    int ret, parent_pid, child_pid;
    
    pipe(fd);
    ret = fork();

    if (ret == 0) { // child
        /* Inserir codi aqui per gestionar senyals */
        /* Esperar a rebre senyal del pare */
	child_pid=ret;
	signal(SIGUSR2,sigusr2); 
	sleep(1);
	i = 0;
        sum = 0;
	value = 0;
	read(fd[0],&num_values,sizeof(int));
	while (i < num_values) {
            /* Llegir valors de la canonada i fer la suma */ 
	    read(fd[0], &value, sizeof(int));
	    sum += value;
            i++;
	    //printf("suma total -> %d\n",sum);
        }
        printf("El fill escriu el resultat: %d\n", sum);
        /* Escriure el valor de la suma a la canonada */
        write(fd[1], &sum, sizeof(int));
        /* Avisar al pare que s'han escrit els valors */ 
	kill(child_pid,SIGUSR1);
        exit(0);
    } else { // parent
        /* Random seed */
        srand(time(NULL));
        i = 0;

        /* Escriure aquest valor a la canonada */
        num_values = 100;
        write(fd[1],&num_values,sizeof(int));
        while (i < num_values) {  
            /* Generar valor aleatori i inserir a la canonada */
            value = rand() % MAXVALUE + 1;
	    write(fd[1], &value, sizeof(int));
	    //printf("valor padre %d\n", value);
            i++;
        }
	
        printf("El pare espera la suma...\n");

	kill(ret,SIGUSR2);
	signal(SIGUSR1,sigusr1);	
	wait(NULL);	

        /* Inserir codi aqui per esperar senyal del pare */
        /* Llegir el resultat */

	while( i < num_values ){
		read(fd[0], &value, sizeof(int));
		i++;
	}
	
	read(fd[0],&value,sizeof(int));

        printf("El fill em diu que la suma es: %d\n", value);
	close(fd[0]);
	close(fd[1]);
	wait(NULL);
    }

    return 0;
}
