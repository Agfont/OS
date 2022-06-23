#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>

void main(void)
{
  int i, fd; 
  char *a = "lluis";
    
  fd = open("file.data", 
        O_WRONLY | O_CREAT | O_TRUNC, 
        S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
  
  for(i = 0; i < 100; i++)
  {
    write(fd, a, 5); 
    write(fd, &i, sizeof(int));
  }
  
  close(fd);
}
