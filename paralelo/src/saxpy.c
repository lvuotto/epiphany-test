
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <omp.h>
#include "tiempo.h"


#define SIZE 100000


#include "arreglos.h"


int main () {
  
  unsigned long long int i;
  double a[SIZE];
  
  #pragma omp parallel for
  for (i = 0; i < SIZE; i++) {
    a[i] = sqrt(b[i]*c[i]);
  }
  
  printf("%p\n", (void *) &a);
  
  return 0;
  
}
