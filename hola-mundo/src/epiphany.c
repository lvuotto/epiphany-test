
#include "e-lib.h"
#include "shared.h"
#include <stdlib.h>


#define BUF_ADDRESS           0x8f000000
#define EPIPHANY_BASE_ADDRESS        0x0


int main () {
  
  e_coreid_t coreid;
  unsigned int row, col, core, trow, tcol, *ec;
  volatile msg_block_t *msg = (msg_block_t *) BUF_ADDRESS;
  
  e_ctimer_set(E_CTIMER_0, 0xffffffff);
  e_ctimer_start(E_CTIMER_0, E_CTIMER_CLK);
 
  ec = (unsigned int *) 0x4000;
  *ec = 0;
  
  coreid = e_get_coreid();
  e_coords_from_coreid(coreid, &row, &col);
   
  core = row*E_COLS + col;
  
  srand(msg->shared_msg[core].seed);
  msg->shared_msg[core].seed = msg->shared_msg[core].seed + 10;
  
  do {
    trow = rand() % E_ROWS;
    tcol = rand() % E_COLS;
  } while (trow == row && tcol == col);
  
  ec = e_get_global_addres(trow, tcol, ec);
  *ec = coreid;
  msg->shared_msg[core].msg = 2000 + rand() % 1000;
  msg->shared_msg[core].external = *(unsigned int *) 0x4000;
  msg->shared_msg[core].timer = 0xffffffff - e_ctimer_get(E_CTIMER_0);
  msg->shared_msg[core].coreid = coreid;

  e_ctimer_stop(E_CTIMER_0);
  
  return 0;
  
}

