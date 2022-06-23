

#include <string.h>

int countWords(char *sentence)
{
  int i;
  int count = 1;
  for (i = 0; i<=strlen(sentence); i++)
  {
      if (sentence[i] == ' ')
	  count++;    
  }
  return count;
}


int countOccurrences(char *sentence, char ch)
{
  int  i;
  int count = 0;
  for (i = 0; i<=strlen(sentence); i++)
  {
      if (sentence[i] == ch)
	  count++;    
  }
  return count;
}