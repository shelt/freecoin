#ifndef TRANSACTIONS_H
#define TRANSACTIONS_H

#include <string.h>
#include "shared.h"

/**********
 * HEADER *
 **********/

// Positions
#define POS_TX_HEADER_VERSION 0
#define POS_TX_HEADER_IN_COUNT 2
#define POS_TX_HEADER_OUT_COUNT 4
#define POS_TX_HEADER_LOCK_TIME 6


typedef struct
{
    uint16_t version;
    uint16_t in_count;
    uint16_t out_count;
    uint32_t lock_time;
} tx_header_t;

void tx_header_serialize(tx_header_t *src, uint8_t *dst);


void tx_header_deserialize(uint8_t *src, tx_header_t *dst);


/*********
 * INPUT *
 *********/

// Positions
#define POS_TX_INPUT_OUT_INDEX 0
#define POS_TX_INPUT_REF_TX 2
#define POS_TX_INPUT_PUBKEY 34
#define POS_TX_INPUT_SIG 162

typedef struct
{
    uint16_t out_index;
    uint8_t ref_tx[32];
    uint8_t pubkey[128];
    uint8_t sig[128];
} tx_input_t;

void tx_input_serialize(tx_input_t *src, uint8_t *dst);

void tx_input_deserialize(uint8_t *src, tx_input_t *dst);

/**********
 * OUTPUT *
 **********/

// Positions
#define POS_TX_OUTPUT_OUT_ADDR 0
#define POS_TX_OUTPUT_AMOUNT 32

typedef struct
{
    uint8_t out_addr[32];
    uint32_t amount;
} tx_output_t;


void tx_output_serialize(tx_output_t *src, uint8_t *dst);

void tx_output_deserialize(uint8_t *src, tx_output_t *dst);

#endif /* TRANSACTIONS_H */

