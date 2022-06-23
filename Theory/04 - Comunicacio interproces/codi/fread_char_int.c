#include <stdio.h>

void main(void)
{
  FILE *fp;
  int i, k;
  char a[6];
  
  fp = fopen("log_fopen.data", "r");
  
  for(i = 0; i < 100; i++)
  {
    fread(a, sizeof(char), 5, fp);
    fread(&k, sizeof(int), 1, fp);

    a[5] = 0; // Equivalent a[5] = '\0'
    printf("Llegit: %s y %d\n", a, k);
  }
  
  fclose(fp);
}
