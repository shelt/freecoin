#ifndef SHARED_H
#define SHARED_H
#include <stdint.h>

#define member_size(type, member) sizeof(((type *)0)->member)

/*************
 * CONSTANTS *
 *************/

#define __VERSION 2

//TODO set this
#define ONE_TRUE_ROOT {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}
#define MINING_REWARD 20

// Maxmimums (not protocol. used during buffer allocation)
// 1024*1024*1024
#define MAX_BLOCK_SIZE 1073741824
// 1024*1024; 
#define MAX_TX_SIZE 1048576

#define SIZE_SHA256 32
#define SIZE_RSA1024 128
#define RSA_NBITS 4096


void init();

void fatal(const char *string, ...);

#endif /* SHARED_H */

