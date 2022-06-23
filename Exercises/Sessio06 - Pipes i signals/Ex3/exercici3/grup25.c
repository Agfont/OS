#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
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
      
        signal(SIGUSR2, sigusr2);
        parent_pid = getppid();
       
        pause();
        
        read(fd[0], &num_values, sizeof(int));

        i = 0;
        sum = 0;
        while (i < num_values) {
            read(fd[0], &value, sizeof(int));
            sum += value;
            i++;
        }
    
        printf("El fill escriu el resultat: %d\n", sum);
        write(fd[1], &sum, sizeof(int));
        
        kill(parent_pid, SIGUSR1);

        exit(0);
    } else { // parent

        signal(SIGUSR1, sigusr1);
        child_pid = ret;

        srand(time(NULL));

        i = 0;
       
        num_values = 1000;
        write(fd[1], &num_values, sizeof(int));
        
        while (i < num_values) {
            value = rand() % MAXVALUE + 1;
            i++;
            write(fd[1], &value, sizeof(int));
        }

        printf("El pare espera la suma...\n");
        kill(child_pid, SIGUSR2);
        pause();

        read(fd[0], &sum, sizeof(int));

        printf("El fill em diu que la suma es: %d\n", sum);

        wait(NULL);
    }

    return 0;
}
