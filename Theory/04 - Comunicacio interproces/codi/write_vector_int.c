#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>

void main(void)
{
  int i, fd, vector[1000]; 
    
  fd = open("file.data", 
        O_WRONLY | O_CREAT | O_TRUNC, 
        S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
  
  for(i = 0; i < 1000; i++)
     vector[i] = 2 * i + 1; 

  write(fd, &vector, sizeof(int) * 1000); 
  
  close(fd);
}
