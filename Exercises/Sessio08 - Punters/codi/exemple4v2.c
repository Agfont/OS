#include <stdio.h>
#include <stdlib.h>

int main () {
   int i;
   void  *ptr, *p_ptr;
   int   *p_int;
   double *p_double;
   
   int var_int[1000];
   double var_double[1000];

   // Inicialitzacio
   
   for(i = 0; i < 1000; i++) {
       var_int[i] = 2 * i + 1;
       var_double[i] = 2.5 * i + 6.7;
   }
   
   // Reservem memoria amb un punter generic
   
   ptr = malloc(1000 * sizeof(int) + 1000 * sizeof(double));
   
   // Reorganitzem les dades

   p_ptr = ptr;
   for(i = 0; i < 1000; i++) {
       p_int = (int *) p_ptr;
       *p_int = var_int[i];

       p_ptr += sizeof(int);

       p_double  = (double *) p_ptr;
       *p_double = var_double[i];

       p_ptr += sizeof(double);
   }

   free(ptr);

   return 0;
} 
