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
    int ret, parent_pid, child_pid, temp;
   
    pipe(fd);

    ret = fork();

    if (ret == 0) { // child
      
        /* Inserir codi aqui per gestionar senyals */
	signal(SIGUSR2, sigusr2);
	parent_pid = getppid();
       
        /* Esperar a rebre senyal del pare */
	wait(NULL); 

        /* Llegir el nombre de valors que ens envia el pare */
	read(fd[0], &num_values, sizeof(int));

        i = 0;
        sum = 0;
	temp = 0; 
        while (i < num_values) {
            /* Llegir valors de la canonada i fer la suma */ 
	    read(fd[0], &temp, sizeof(int));
	    sum += temp; 
            i++;
        }
    
        printf("El fill escriu el resultat: %d\n", sum);
        /* Escriure el valor de la suma a la canonada */
	write(fd[1], &sum, sizeof(int));
        
        /* Avisar al pare que s'han escrit els valors */
	kill(parent_pid, SIGUSR1); 

        exit(0);

    } else { // parent

        /* Inserir codi aqui per gestionar senyals */
	signal(SIGUSR1, sigusr1);
	child_pid = ret;

        /* Random seed */
        srand(time(NULL));

        i = 0;
       
        /* Escriure aquest valor a la canonada */
        num_values = 1000;
        write(fd[1], &num_values, sizeof(int));

        while (i < num_values) {  
            /* Generar valor aleatori i inserir a la canonada */
            value = rand() % MAXVALUE + 1;
	    write(fd[1], &value, sizeof(int));
            i++;
        }

	//el pare envia senyal al fill que ja ha escrit els N valors i el fill ja pot calcular la suma
	kill(child_pid, SIGUSR2);

        printf("El pare espera la suma...\n");

        /* Inserir codi aqui per esperar senyal del fill */
	wait(NULL);

        /* Llegir el resultat */
	read(fd[0], &sum, sizeof(int));

        printf("El fill em diu que la suma es: %d\n", sum);

        wait(NULL);
    }

    return 0;
}
