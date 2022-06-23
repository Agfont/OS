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
    int i, num_values, value, sum, fd[2],sumavalue;
    int ret, parent_pid, child_pid;
    

    pipe(fd);

    ret = fork();

    if (ret == 0) { // child
        printf("Fill gestiona senyals \n");
        /* Inserir codi aqui per gestionar senyals */
        signal(SIGUSR2, sigusr2);
        parent_pid = getppid();
       
        /* Esperar a rebre senyal del pare */
        pause();
        /* Llegir el nombre de valors que ens envia el pare */
        printf("Fill Surt del pause \n");
        i = 0;
        sum = 0;
        sumavalue = 0;
        num_values = 0;
        read(fd[0], &num_values, 4);
        printf("Num Values: %d\n", num_values);
        while (i < num_values) {
            /* Llegir valors de la canonada i fer la suma */ 
            read(fd[0], &sumavalue, 4);
            //printf("Pipe a fill: %d\n", sumavalue);
            sum = sum + sumavalue;
            //printf("Suma: %d\n", sumavalue);
            i++;
        }
    
        printf("El fill escriu el resultat: %d\n", sum);
        /* Escriure el valor de la suma a la canonada */
        write(fd[1], &sum, 4);
        /* Avisar al pare que s'han escrit els valors */ 
        printf("Envio SIGUSR1 al pare %d\n", parent_pid);
        kill(parent_pid, SIGUSR1);
        exit(0);
    } else { // parent
        printf("Pare gestiona senyals \n");

        /* Inserir codi aqui per gestionar senyals */
        signal(SIGUSR1, sigusr1);
        child_pid = ret;
        /* Random seed */
        srand(time(NULL));

        i = 0;
       
        /* Escriure aquest valor a la canonada */
        num_values = 1000;
        write(fd[1], &num_values, 4);
        
        while (i < num_values) {  
            /* Generar valor aleatori i inserir a la canonada */
            value = rand() % MAXVALUE + 1;
            //printf("Pare a pipe : %d\n", value);
            write(fd[1], &value, 4);
            i++;
        }
        /*enviem senyal fill*/
        printf("Envio SIGUSR2 al fill %d\n", child_pid);
        kill(child_pid, SIGUSR2);

        printf("El pare espera la suma...\n");

        /* Inserir codi aqui per esperar senyal del fill */
        pause();
        /* Llegir el resultat */
        read(fd[0], &sum, 4);

        printf("El fill em diu que la suma es: %d\n", sum);

        wait(NULL);
    }

    return 0;
}
