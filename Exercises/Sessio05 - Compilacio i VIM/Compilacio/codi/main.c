

#include<stdio.h>
#include "calculator.h"
#include "counter.h"


#define X1 2
#define Y1 2
#define X2 1
#define Y2 1

#define SENTENCE "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua."
#define CHARACTER 'a'


int main()
{
 printf("-------------------------");
 printf(" \n euclidean distance: %f", euclideanDistance(X1,Y1,X2,Y2));
 printf(" \n manhattan distance: %f", manhattanDistance(X1,Y1,X2,Y2));
 printf(" \n number of words: %d", countWords(SENTENCE));
 printf(" \n number of occurrences: %d", countOccurrences(SENTENCE,CHARACTER));
 printf("\n--------------------------\n");

 return 0;
}
