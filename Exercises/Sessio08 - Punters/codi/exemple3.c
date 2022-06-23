#include <stdio.h>
#include <stdlib.h>

int main () {

   int  i, *var;
   
   var = malloc(1000 * sizeof(int));
   
   for ( i = 0; i < 1000; i++) {
       var[i] = 2 * i + 1;
   }
   
   free(var);
        
   return 0;
}
