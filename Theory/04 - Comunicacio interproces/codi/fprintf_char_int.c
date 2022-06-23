#include <stdio.h>

void main(void)
{
  FILE *fp;
  int i;
  
  char *a = "lluis";
  
  fp = fopen("log_fopen.data", "w");
  
  for(i = 0; i < 100; i++)
  {
    fprintf(fp, "%s\n", a);
    fprintf(fp, "%d\n", i);
  }
  
  fclose(fp);
}
