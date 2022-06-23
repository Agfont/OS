#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>

#define PERM_FILE  (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

int main()
{
  int fd;

  mkfifo("myfifo", PERM_FILE);
  
  printf("Obrint pipe per escriptura\n");
  fd = open("myfifo", O_WRONLY);
  printf("Escrivint missatge\n");
  write(fd, "test", 5);
  printf("Tancant\n");
  close(fd);
}
