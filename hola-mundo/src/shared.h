
#ifndef __SHARED_H__

#define __SHARED_H__


#include <stdint.h>


#define E_ROWS   4
#define E_COLS   4
#define E_CORES 16


typedef struct msg_info_s  msg_info_t;
typedef struct msg_block_s msg_block_t;


struct msg_info_s {
  uint32_t coreid;
  uint32_t msg;
  uint32_t external;
  uint32_t timer;
  uint32_t seed;
} __attribute__((packed, aligned(4)));

struct msg_block_s {
  msg_info_t shared_msg[E_CORES];
} __attribute__((packed, aligned(4)));


#endif  /* __SHARED_H__ */
