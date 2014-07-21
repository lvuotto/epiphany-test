
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <e-hal.h>

#include "shared.h"


#ifndef SEED
# define SEED 50
#endif


#define BUF_OFFSET 0x01000000


static inline void nano_wait (uint32_t s, uint32_t ns) {
  struct timespec ts;
  ts.tv_sec = s;
  ts.tv_nsec = ns;
  nanosleep(&ts, NULL);
}


int main () {
  
  unsigned int row, col, core, t;
  e_platform_t platform;
  e_epiphany_t device;
  e_mem_t mem;
  static msg_block_t msg;
  
  memset(&msg, 0, sizeof(msg));
  
  e_init(NULL);
  e_reset_system();
  e_get_platform_info(&platform);
  e_alloc(&mem, BUF_OFFSET, sizeof(msg_block_t));
  /* Cómo sé que ^ pone el buffer en 0x8f00000000? */
  /* Esta definido en el hdf por default.          */
  
  srand(SEED);
  for (row = 0; row < platform.rows; row++) {
    for (col = 0; col < platform.cols; col++) {
      core = row*platform.cols + col;
      msg.shared_msg[core].seed = SEED + core;
      printf("A (%d,%d) le toco %d\n", row, col, msg.shared_msg[core].seed);
    }
  }
  printf("\n---\n\n");
  
  e_open(&device, 0, 0, platform.rows, platform.cols);
  e_write(&mem, 0, 0, 0, &msg, sizeof(msg));
  e_reset_group(&device);
  e_load_group("epiphany.srec", &device, 0, 0, platform.rows, platform.cols, E_TRUE);
  
  nano_wait(0, 10000000);  /* Necesario para sincronizar? */
  
  for (row = 0; row < platform.rows; row++) {
    for (col = 0; col < platform.cols; col++) {
      core = row*platform.cols + col;
      t = 0;
      while (E_TRUE) {  /* espero hasta que cambie algo */
        e_read(&mem,
               0,
               0,
               (off_t) ((char *)&msg.shared_msg[core] - (char *)&msg),
               &msg.shared_msg[core],
               sizeof(msg_info_t));
        if (msg.shared_msg[core].coreid != 0) {
          printf("Termino %d\n", core);
          break;
        }
        printf(".");
        nano_wait(0, 1000000);
        if (t++ == 10) {
          printf("Colgo %d\n", core);
          break;
        }
      }
    }
  }

  /* Ya hice todo lo que tenia que hacer, falta updatear. */
  nano_wait(0, 1000000);
  for (row = 0; row < platform.rows; row++) {
    for (col = 0; col < platform.cols; col++) {
      core = row*platform.cols + col;
      e_read(&mem,
             0,
             0,
             (off_t) ((char *)&msg.shared_msg[core] - (char *)&msg),
             &msg.shared_msg[core],
             sizeof(msg_info_t));
    }
  }
  
  for (row = 0; row < platform.rows; row++) {
    for (col = 0; col < platform.cols; col++) {
      core = row*platform.cols + col;
      printf("Hola, soy %#03x [%u] (%-2d, %-2d)! Tengo el mensaje %#03x, "
             "recibi el mensaje %u, y tarde %u ticks en procesar todo. "
             "seed ahora vale %d.\n",
             msg.shared_msg[core].coreid,
             msg.shared_msg[core].coreid,
             msg.shared_msg[core].coreid >> 6,
             msg.shared_msg[core].coreid & 0x3f,
             msg.shared_msg[core].msg,
             msg.shared_msg[core].external,
             msg.shared_msg[core].timer,
             msg.shared_msg[core].seed);
    }
  }
  
  e_close(&device);
  e_free(&mem);
  e_finalize();
  
  return 0;
  
}
