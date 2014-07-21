
#ifndef __SHARED_H__

#define __SHARED_H__


#include <stdint.h>


#define MAX_HITS (2u << 27u)


typedef struct shared_msg_s shared_msg_t;

struct shared_msg_s {
  uint32_t seed;
  uint32_t hits;
  uint32_t finished;
  uint32_t timer;
};


#endif  /* __SHARED_H__ */

