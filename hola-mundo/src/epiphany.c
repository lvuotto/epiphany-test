
#include "e-lib.h"
#include "shared.h"
#include <stdlib.h>


#define BUF_ADDRESS           0x8f000000
#define EPIPHANY_BASE_ADDRESS        0x0
#define E_GROUP_CORES (e_group_config.group_rows * e_group_config.group_cols)


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
   
  core = row*e_group_config.group_cols + col;
  
  srand(msg->shared_msg[core].seed);
  msg->shared_msg[core].seed = msg->shared_msg[core].seed + 10;
  
  trow = ((core + 1) % E_GROUP_CORES) / e_group_config.group_rows;
  tcol =  (core + 1) % e_group_config.group_cols;
  
  ec = e_get_global_address(trow, tcol, ec);
  e_write(&e_group_config, &coreid, 0, 0, ec, sizeof(e_coreid_t));
  /**ec = coreid;*/
  msg->shared_msg[core].msg = e_coreid_from_coords(trow, tcol);
  msg->shared_msg[core].external = *(unsigned int *) 0x4000;
    
  /* Sync */
  e_wait(E_CTIMER_1, 2000); 
  msg->shared_msg[core].timer = 0xffffffff - e_ctimer_get(E_CTIMER_0);
  msg->shared_msg[core].coreid = coreid;

  e_ctimer_stop(E_CTIMER_0);
  
  return 0;
  
}

