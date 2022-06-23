#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>

void main(void)
{
  int i, fd, vector[1000]; 
   
  fd = open("file.data", O_RDONLY);

  read(fd, vector, sizeof(int) * 1000);

  close(fd);

  for(i = 0; i < 1000; i++)
     printf("%d\n", vector[i]);
}
