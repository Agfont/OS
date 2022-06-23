#include <stdio.h>
#include <stdlib.h>

#define SIZE_GIGAS 4

void main(int argc, char **argv)
{
  FILE *fp;
  size_t i, j, iterations;
  int value;

  fp = fopen("big_file.bin", "w");
  if (!fp) {
    printf("Could not open big_file.bin\n");
    exit(1);
  }

  iterations = (1024*1024*1024)/sizeof(int);

  fprintf(stderr, "Creating file...\n");
  for(i = 0; i < SIZE_GIGAS; i++) {
    for(j = 0; j < iterations; j++) {
      value = rand();
      fwrite(&value, sizeof(int), 1, fp);
    }
    fprintf(stderr, "Written %d of %d gigabytes\n", i+1, SIZE_GIGAS);
  }

  fclose(fp);
  fprintf(stderr, "Done!\n");
}
