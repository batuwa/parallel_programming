#include <stdio.h>

#define Length 8
typedef int intV8 __attribute__ ((vector_size (Length * sizeof(int)))); 
    
int main() {
  intV8 v1 = {1, 2, 3, 4, 5, 6, 7, 8}; 
  intV8 v2 = {8, 7, 6, 5, 4, 3, 2, 1}; 
  
  intV8 add = v1 + v2; 
  intV8 mul = v1 * v2; 

  int i;
  for (i = 0; i < Length; i++) printf("%i ", add[i]); 
  putchar('\n');
  for (i = 0; i < Length; i++) printf("%i ", mul[i]); 
  putchar('\n');
  return 0;
}
