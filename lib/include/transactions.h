#ifndef TRANSACTIONS_H
#define TRANSACTIONS_H

#include "shared.h"

typedef struct
{
    uint16_t version;
    uint16_t in_count;
    uint16_t out_count;
    uint32_t lock_time;
    
} tx_header_t;

typedef struct
{
    uint16_t out_index;
    uint8_t ref_tx[32];
    uint8_t pubkey[128];
    uint8_t sig[128];
} tx_input_t;

typedef struct
{
    uint8_t out_addr[32];
    uint32_t amount;
} tx_output_t;

#endif /* TRANSACTIONS_H */

