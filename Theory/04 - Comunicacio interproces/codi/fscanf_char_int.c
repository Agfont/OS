#include <stdio.h>

void main(void)
{
  FILE *fp;
  int i, k;
  char a[6];
  
  fp = fopen("log_fopen.data", "r");
  
  for(i = 0; i < 100; i++)
  {
    fscanf(fp, "%s", a);
    fscanf(fp, "%d", &k);

    printf("Llegit: %s y %d\n", a, k);
  }
  
  fclose(fp);
}
