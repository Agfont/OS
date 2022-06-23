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
     
        // Inserir codi aqui per gestionar senyals //
       signal(SIGUSR2, sigusr2);//
       parent_pid = getppid();//
        // Esperar a rebre senyal del pare //

       // Llegir el nombre de valors que ens envia el pare /
read(fd[1], &num_values, 1);//revisar esto

        i = 0;
        sum = 0;
        while (i < num_values) {
            // Llegir valors de la canonada i fer la suma /
   sum = sum + read(fd[1], &value, 1);//
            i++;
        }
   
        printf("El fill escriu el resultat: %d\n", sum);
        // Escriure el valor de la suma a la canonada /
        write(fd[1], &sum, 1);//

        // Avisar al pare que s'han escrit els valors /
kill(parent_pid, SIGUSR1);//

        exit(0);
    } else { // parent

        // Inserir codi aqui per gestionar senyals /
signal(SIGUSR1, sigusr1);//
        child_pid = ret; //
       
// Random seed /
        srand(time(NULL));

        i = 0;
       
        // Escriure aquest valor a la canonada /
        num_values = 1000;
       
        while (i < num_values) {  
            // Generar valor aleatori i inserir a la canonada /
            value = rand() % MAXVALUE + 1;
   write(fd[0], &value, 1);//
            i++;
        }
printf("Envio SIGUSR2 al fill %d\n", child_pid); //extra
kill(child_pid, SIGUSR2);//envia la senyal al hijo
        pause();//
        printf("El pare espera la suma...\n");

        // Inserir codi aqui per esperar senyal del pare /

        // Llegir el resultat /
read(fd[0], &sum, 1);//

        printf("El fill em diu que la suma es: %d\n", sum);

        wait(NULL);
    }

    return 0;
}


