#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>

int main()
{
  char buf[20];
  int fd;

  printf("Obrint pipe per lectura\n");
  fd = open("myfifo", O_RDONLY);
  printf("Llegint missatge\n");
  read(fd, buf, 5);
  printf("Rebut %s, tancant.\n", buf);
  close(fd);
}
