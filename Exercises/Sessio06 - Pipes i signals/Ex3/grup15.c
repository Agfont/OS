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

    pipe(fd); // Info-> Creem una canonada

    ret = fork(); // Info -> Creem el procès fill

    if (ret == 0) { // child
      
        /* Inserir codi aqui per gestionar senyals */
	signal(SIGUSR2, sigusr2); // (3) Definir els senyals
    	parent_pid = getppid(); // (3) Detectar el padre
        /* Esperar a rebre senyal del pare */
	pause();
        /* Llegir el nombre de valors que ens envia el pare */
	read(fd[0], &num_values, sizeof(int)); // (4) Llegir primer valor 
        i = 0;
        sum = 0;
        while (i < num_values) {
            /* Llegir valors de la canonada i fer la suma */
 	    read(fd[0], &value, sizeof(int)); // (4) Llegir valors restants 
	    sum += value; // (4) Fer la suma
            i++;
        }
        printf("El fill escriu el resultat: %d\n", sum);
        /* Escriure el valor de la suma a la canonada */
	write(fd[1], &sum, sizeof(int)); // (5) Escriure la suma a la canonada

        /* Avisar al pare que s'han escrit els valors */
	kill(parent_pid, SIGUSR1); //(6) Enviem el senyal al pare

        exit(0);
    } else { // parent

        /* Inserir codi aqui per gestionar senyals */
	signal(SIGUSR1, sigusr1); // (7) Definir senyal del fill
    	child_pid = ret;// (7) Actualizar el fill

        /* Random seed */
        srand(time(NULL));

        i = 0;
       
        /* Escriure aquest valor a la canonada */
	printf("El pare escriu a la canonada!\n"); // (A) 
        num_values = 1000;
        write(fd[1], &num_values, sizeof(int)); //(1) Inserir valor a la canonada
        while (i < num_values) {  
            /* Generar valor aleatori i inserir a la canonada */
            value = rand() % MAXVALUE + 1;
	    write(fd[1], &value, sizeof(int)); // (1) Inserir valor a la canonada
            i++;
        }

	kill(child_pid, SIGUSR2);//(2) Avisem al procès fill amb SIGUSR

        printf("El pare espera la suma...\n");

        /* Inserir codi aqui per esperar senyal del pare */
	pause();
        /* Llegir el resultat */
	read(fd[0], &sum, sizeof(int)); // (8) Llegir suma 
	
        printf("El fill em diu que la suma es: %d\n", sum);

        wait(NULL);
    }

    return 0;
}
