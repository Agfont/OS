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

        /* Llegir el nombre de valors que ens envia el pare */

        i = 0;
        sum = 0;
        while (i < num_values) {
            /* Llegir valors de la canonada i fer la suma */ 
            i++;
        }
    
        printf("El fill escriu el resultat: %d\n", sum);
        /* Escriure el valor de la suma a la canonada */
        
        /* Avisar al pare que s'han escrit els valors */ 

        exit(0);
    } else { // parent

        /* Inserir codi aqui per gestionar senyals */

        /* Random seed */
        srand(time(NULL));

        i = 0;
       
        /* Escriure aquest valor a la canonada */
        num_values = 1000;
        
        while (i < num_values) {  
            /* Generar valor aleatori i inserir a la canonada */
            value = rand() % MAXVALUE + 1;
            i++;
        }

        printf("El pare espera la suma...\n");

        /* Inserir codi aqui per esperar senyal del pare */

        /* Llegir el resultat */

        printf("El fill em diu que la suma es: %d\n", sum);

        wait(NULL);
    }

    return 0;
}
