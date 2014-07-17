
#include "e-lib.h"
#include "shared.h"


#define BUF_ADDRESS           0x8f000000
#define EPIPHANY_BASE_ADDRESS 0x80000000


int main () {
  
  e_coreid_t coreid;
  unsigned int row, col, core, trow, tcol, *ec;
  volatile msg_block_t *msg = (msg_block_t *) BUF_ADDRESS;
  /* Por qué volatile? */
  
  coreid = e_get_coreid();
  e_coords_form_coreid(coreid, &row, &col);
  
  e_ctimer_set(E_CTIMER_0, 0xffffffff);
  e_ctimer_start(E_CTIMER_0, E_CTIMER_CLK);
  
  core = row*E_COLS + col;
  
  srand(msg->shared_msg[core].seed);
  msg->shared_msg[core].seed++;
  
  do {
    trow = rand() % E_CORES;
    tcol = rand() % E_CORES;
  } while (trow == row && tcol == col);
  
  msg->shared_msg[core].coreid = coreid;
  ec = (unsigned int *) (EPIPHANY_BASE_ADDRESS + (trow << 28) + (tcol << 22));
  *(ec + 0x4000) = 1000 + rand() % 1000;
  msg->shared_msg[core].msg = 1 + rand() % 999;
  msg->shared_msg[core].external = *(unsigned int *) 0x4000;
  msg->shared_msg[core].timer = 0xffffffff - e_ctimer_get(E_CTIMER_0);
  
  e_ctimer_stop(E_CTIMER_0);
  
  return 0;
  
}
