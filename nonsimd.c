#include <stdio.h>

#define Length 8

int main() {
  int array1[ ] = {1, 2, 3, 4, 5, 6, 7, 8}; 
  int array2[ ] = {8, 7, 6, 5, 4, 3, 2, 1};
  int sum[Length];
  int mul[Length];

  int i;
  for (i = 0; i < Length; i++) sum[i] = array1[i] + array2[i]; /** serial computation **/
  for (i = 0; i < Length; i++) mul[i] = array1[i] * array2[i]; /** serial computation **/
  
  for (i = 0; i < Length; i++) printf("%i ", sum[i]); 
  putchar('\n');
  for (i = 0; i < Length; i++) printf("%i ", mul[i]); 
  putchar('\n');
  return 0;
}

