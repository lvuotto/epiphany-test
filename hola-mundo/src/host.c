
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <e-hal.h>

#include "shared.h"


#define BUF_OFFSET 0x01000000


static inline void nano_wait (uint32_t s, uint32_t ns) {
  struct timespec ts;
  ts.tv_sec = s;
  ts.tv_nsec = ns;
  nanosleep(&ts, NULL);
}


int main () {
  
  unsigned int row, col, core;
  e_platform_t eplat;
  e_epiphany_t edev;
  e_mem_t emem;
  static msg_block_t msg;
  
  memset(&msg, 0, sizeof(msg));
  
  srand(0);
  for (row = 0; row < E_ROWS; row++) {
    for (col = 0; col < E_COLS; col++) {
      core = row*E_COLS + col;
      msg.shared_msg[core].seed = core;
      printf("A (%d,%d) le toco %d\n", row, col, msg.shared_msg[core].seed);
    }
  }
  printf("\n---\n\n");
  
  e_init(NULL);
  e_reset_system();
  e_get_platform_info(&eplat);
  e_alloc(&emem, BUF_OFFSET, sizeof(msg_block_t));
  /* Cómo sé que ^ pone el buffer en 0x8f00000000? */
  /* Esta definido en el hdf por default.          */
  
  e_open(&edev, 0, 0, E_ROWS, E_COLS);
  e_write(&emem, 0, 0, 0, &msg, sizeof(msg));
  e_reset_group(&edev);
  e_load_group("epiphany.srec", &edev, 0, 0, E_ROWS, E_COLS, E_TRUE);
  
  nano_wait(0, 10000000);  /* Necesario para sincronizar? */
  
  while (E_TRUE) {
    for (row = 0; row < E_ROWS; row++) {
      for (col = 0; col < E_COLS; col++) {
        core = row*E_COLS + col;
        while (E_TRUE) {  /* espero hasta que cambie algo */
          e_read(&emem,
                 0,
                 0,
                 (off_t) ((char *)&msg.shared_msg[core] - (char *)&msg),
                 &msg.shared_msg[core],
                 sizeof(msg_info_t));
          if (msg.shared_msg[core].coreid > 0 &&
              msg.shared_msg[core].msg > 0 &&
              msg.shared_msg[core].external > 0)
          {
            printf("Termino %d\n", core);
            break;
          }
          printf(".");
          nano_wait(0, 1000000);
        }
      }
    }
    break;
  }
  
  for (row = 0; row < E_ROWS; row++) {
    for (col = 0; col < E_COLS; col++) {
      core = row*E_COLS + col;
      printf("Hola, soy %u (%d, %-2d)! Tengo el mensaje %u, "
             "recibi el mensaje %u, y tarde %u ticks en procesar todo. "
             "seed ahora vale %d.\n",
             msg.shared_msg[core].coreid,
             msg.shared_msg[core].coreid >> 6,
             msg.shared_msg[core].coreid & 0x3f,
             msg.shared_msg[core].msg,
             msg.shared_msg[core].external,
             msg.shared_msg[core].timer,
             msg.shared_msg[core].seed);
    }
  }
  
  e_close(&edev);
  e_free(&emem);
  e_finalize();
  
  return 0;
  
}
