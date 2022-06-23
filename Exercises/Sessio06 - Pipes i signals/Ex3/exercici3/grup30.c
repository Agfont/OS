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
    
    /*Codi per gestionar les senyals*/
    //Quan el fill rep la senyal SIGUSR2, s'executa la funció sigusr2
    signal(SIGUSR2, sigusr2);
    signal(SIGUSR1, sigusr1);
    

    pipe(fd);

    ret = fork();

    if (ret == 0) { // child  
        parent_pid = getppid();
        
        /* Esperar a rebre senyal del pare */
        pause();

        /* Llegir el nombre de valors que ens envia el pare */
        //siempre es fd[0] para leer, y fd[1] para escribir
        read(fd[0], &num_values, sizeof(int));

        i = 0;
        sum = 0;
        while (i < num_values) {
            read(fd[0], &value, sizeof(int));
            sum += value;
            /* Llegir valors de la canonada i fer la suma */ 
            i++;
        }
    
        printf("El fill escriu el resultat: %d\n", sum);
        /* Escriure el valor de la suma a la canonada */
        write(fd[1], &sum, sizeof(int));
        
        /* Avisar al pare que s'han escrit els valors */ 
        kill(parent_pid, SIGUSR1);

        exit(0);
    } else { // parent
        
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

        kill(child_pid, SIGUSR2);
        printf("El pare espera la suma...\n");

        /* Inserir codi aqui per esperar senyal del fill */
        pause();

        /* Llegir el resultat */
        read(fd[0], &sum, sizeof(int));

        printf("El fill em diu que la suma es: %d\n", sum);

        wait(NULL);
    }

    return 0;
}
