#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>

void main(void)
{
  int i, k, fd; 
  char a[6];
   
  fd = open("file.data", O_RDONLY);

  for(i = 0; i < 100; i++)
  {
    read(fd, a, 5); 
    read(fd, &k, sizeof(int));
    
    a[5] = 0; // Equivalent a[5] = '\0'
    printf("Llegit: %s y %d\n", a, k);
  }
  
  close(fd);
}
