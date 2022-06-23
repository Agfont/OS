#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
 
int main(int argc, char **argv)
{
  int n, buffsize;
  
  if (argc != 2) {
      printf("%s <mida_buffer>\n", argv[0]);
      exit(1);
  }
  
  buffsize = atoi(argv[1]);
  char buf[buffsize];

  while ((n = fread(buf, sizeof(char), buffsize, stdin)) > 0)
    fwrite(buf, sizeof(char), n, stdout);

  return 0;
}


