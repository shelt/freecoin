#ifndef SHARED_H
#define SHARED_H

#include <stdint.h>

#define member_size(type, member) sizeof(((type *)0)->member)

// Maxmimums (not protocol. used during buffer allocation)

// 1024*1024*1024
#define MAX_BLOCK_SIZE 1073741824

// 1024*1024; 
#define MAX_TX_SIZE 1048576



#endif /* SHARED_H */

