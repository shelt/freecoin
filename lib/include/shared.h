#ifndef SHARED_H
#define SHARED_H
#include <stdint.h>

#define member_size(type, member) sizeof(((type *)0)->member)

/*************
 * CONSTANTS *
 *************/

#define __VERSION 2

// Maxmimums (not protocol. used during buffer allocation)
// 1024*1024*1024
#define MAX_BLOCK_SIZE 1073741824
// 1024*1024; 
#define MAX_TX_SIZE 1048576

// TODO replace all SIZE_BLOCK_HASH and SIZE_TX_HASH with this
#define SIZE_SHA256 32
#define SIZE_RSA1024 128

// CONFIG OPTIONS
// TODO move to config file
#define CONFIG_DIR_FREECOIN ".freecoin/"




void fatal(const char *string, ...);



#endif /* SHARED_H */

