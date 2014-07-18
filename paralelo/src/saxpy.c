
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "tiempo.h"


#define SIZE 1000000


int main () {
  
  unsigned long long int a[SIZE], i;
  
  srand(0);
  
  /*#pragma omp parallel for*/
  /*for (i = 0; i < SIZE; i++) {
    b[i] = rand() % 1024;
    c[i] = rand() % 1024;
  }*/
  
  #pragma omp parallel for num_threads(40)
  for (i = 0; i < SIZE; i++) {
    a[i] = i*i;
  }
  
  printf("%p\n", (void *) &a);
  
  return 0;
  
}
