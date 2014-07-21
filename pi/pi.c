
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <e-hal.h>

#include "shared.h"
#include "nano_wait.h"


#ifndef SEED
# define SEED time(NULL)
#endif


#define BUF_ADDRESS 0x1000000


int main () {
  
  unsigned int row, col, core, hits, t;
  float pi;
  e_platform_t platform;
  e_epiphany_t device;
  e_mem_t mem;
  static shared_msg_t shared[16];
  
  e_init(NULL);
  e_reset_system();
  e_get_platform_info(&platform);
  e_alloc(&mem, BUF_ADDRESS, 16*sizeof(shared_msg_t));
  
  srand(SEED);
  for (row = 0; row < platform.rows; row++) {
    for (col = 0; col < platform.cols; col++) {
      core = row*platform.cols + row;
      shared[core].seed = rand();
      shared[core].hits = 0;
      shared[core].finished = E_FALSE;
      shared[core].timer = 0;
    }
  }

  e_open(&device, 0, 0, platform.rows, platform.cols);
  e_write(&mem, 0, 0, 0, shared, 16*sizeof(shared_msg_t));
  e_reset_group(&device);
  e_load_group("e_pi.srec", &device, 0, 0, platform.rows, platform.cols, E_TRUE);

  nano_wait(0, 10000000);

  for (row = 0; row < platform.rows; row++) {
    for (col = 0; col < platform.cols; col++) {
      core = row*platform.cols + col;
      t = 0;
      while (E_TRUE) {
        e_read(&mem,
               0,
               0,
               (off_t) ((char *)&shared[core] - (char *)shared),
               &shared[core],
               sizeof(shared_msg_t));
        if (shared[core].finished != E_FALSE) {
          printf("Termino %u\n", core);
          break;
        }
        nano_wait(0, 1000000);
        if (t++ == 0xffffffff) {
          printf("Colgo %u\n", core);
          break;
        }
      }
    }
  }
  
  hits = 0;
  for (row = 0; row < platform.rows; row++) {
    for (col = 0; col < platform.cols; col++) {
      core = row*platform.cols + col;
      hits += shared[core].hits;
      printf("El core #%u [%#03x] tardo %u ticks.\n", core, shared[core].finished, shared[core].timer);
    }
  }

  e_close(&device);
  e_free(&mem);
  e_finalize();
  
  pi = 4.0*(hits / (16.0*MAX_HITS));
  printf("pi ~ %.10f\n", pi);

  return 0;

}

