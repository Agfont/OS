#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/wait.h>

int main(void)
{
  size_t size;
  int ret, *a;

  // Mapat compartit!!!!!
  size = 10737418240; // 10 GB
  a = mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0); 

  a[1000] = 1;
 
  ret = fork();
  
  if (ret == 0) {  // fill
     sleep(1);
     printf("Soc el fill i el meu id es %d\n", getpid());
     printf("Fill a[1000] = %d\n", a[1000]); // s'imprimeix a = 2 també!
     return 0;
  } else { // pare
     a[1000] = 2;
     printf("Soc el pare del proces %d\n", ret);
     printf("Pare a [1000] = %d\n", a[1000]); // s'imprimeix a = 2!
     wait(NULL);
     return 0;
  }
}


