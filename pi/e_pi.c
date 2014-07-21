
#include "e-lib.h"
#include <stdlib.h>

#include "shared.h"


#define BUF_ADDRESS 0x8f000000
#define random() (2.0*(rand() / (float) RAND_MAX) - 1.0)


int main () {
  
  e_coreid_t coreid;
  unsigned int row, col, core, hits, i;
  float x, y;
  volatile shared_msg_t *msg;

  msg = (shared_msg_t *) BUF_ADDRESS;
  
  e_ctimer_set(E_CTIMER_0, 0xffffffff);
  e_ctimer_start(E_CTIMER_0, E_CTIMER_CLK);

  coreid = e_get_coreid();
  e_coords_from_coreid(coreid, &row, &col);
  core = row*e_group_config.group_cols + col;
  
  srand(msg[core].seed);
  
  hits = 0;
  for (i = 0; i < MAX_HITS; i++) {
    x = random();
    y = random();
    if (x*x + y*y <= 1)
      hits++;
  }
  msg[core].hits = hits;
  msg[core].timer = 0xffffffff - e_ctimer_get(E_CTIMER_0);
  msg[core].finished = coreid;
  
  e_ctimer_stop(E_CTIMER_0);

  return 0;

}

