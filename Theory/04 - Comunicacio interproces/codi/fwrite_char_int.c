#include <stdio.h>

void main(void)
{
  FILE *fp;
  int i;
  
  char *a = "lluis";
  
  fp = fopen("log_fopen.data", "w");
  
  for(i = 0; i < 100; i++)
  {
    fwrite(a, sizeof(char), 5, fp);
    fwrite(&i, sizeof(int), 1, fp);
  }
  
  fclose(fp);
}
