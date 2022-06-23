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
    
    //Creem el fill
    ret = fork();
    
    if (ret == 0) { // child
        signal(SIGUSR2,sigusr2);
        //Esperem que el pare hagi ficat tots els valors 
        pause();
        
        //Primer mirem quants números hem de llegir
        read(fd[0],&num_values,4);
        i = 0;
        sum = 0;
        while (i < num_values) {
            read(fd[0],&value,4);
            sum+=value;
            i++;
        }
    
        printf("El fill escriu el resultat: %d\n", sum);
        /* Escriure el valor de la suma a la canonada */
        write(fd[1],&sum,4);
        parent_pid=getppid();
    
        
        /* Avisar al pare que s'han escrit els valors */ 
        kill(parent_pid,SIGUSR1);
        
        exit(0);
    } else { // parent
        signal(SIGUSR1,sigusr1);
        srand(time(NULL));
        
        i=0;
        num_values = 1000;
        //Primer fiquem a la canonada la mida.
        write(fd[1], &num_values, 4);

        while (i < num_values) {  
            /* Generar valor aleatori i inserir a la canonada */
            value = rand() % MAXVALUE + 1;
            write(fd[1], &value, 4);
            i++;
        }
        
        /* Inserir codi aqui per esperar senyal del fill */
        printf("El pare espera la suma...\n");    
        kill(ret,SIGUSR2);
        pause();
        
        /* Llegir el resultat */
        read(fd[0], &sum, 4);
        printf("El fill em diu que la suma es: %d\n", sum);
        
        wait(NULL);
    }

    return 0;
}
