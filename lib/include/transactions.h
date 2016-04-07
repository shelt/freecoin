#ifndef TRANSACTIONS_H
#define TRANSACTIONS_H
#include "util.h"
#include <stdint.h>

/*
 * A Note about structs in this project:
 *  tx_input_t an tx_output_t are fixed-size and therefore their struct
 *  implementations are simple.
 *  The tx_t and block_t structs have fixed-size headers and variable-sized
 *  bodies and are therefore not fixed-sized. Because of this, the deserialization
 *  function is self-allocating, as indicated by "m_" prefix.
 * Another special difference between the tx_t and block_t structs:
 *  block_t visualized: block_t[[header] + body[tx[[ins]+[outs]]]]
 *  As you can see, tx_t is variable length because it contains a variable number
 *  of ins and outs. block_t is variable length because it contains a variable number
 *  of transactions. However, because block_t is "quadratically" variable length
 *  (a variable number of things which contain a variable number of things), it stores
 *  its tx_t's as pointers to pointers (txs **) as opposed to in a single malloc'd
 *  block. If you can't understand why, just know that this is for a good reason.
 */


/**********
 * HEADER *
 **********/

// Positions
#define POS_TX_HEADER_VERSION 0
#define POS_TX_HEADER_IN_COUNT 2
#define POS_TX_HEADER_OUT_COUNT 4
#define POS_TX_HEADER_LOCK_TIME 6

// Sizes
#define SIZE_TX_HEADER 10
#define SIZE_TX_HEADER_VERSION 2
#define SIZE_TX_HEADER_IN_COUNT 2
#define SIZE_TX_HEADER_OUT_COUNT 2
#define SIZE_TX_HEADER_LOCK_TIME 4

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

// Sizes
#define SIZE_TX_INPUT 290
#define SIZE_TX_INPUT_OUT_INDEX 2
#define SIZE_TX_INPUT_REF_TX 32
#define SIZE_TX_INPUT_PUBKEY 128
#define SIZE_TX_INPUT_SIG 128

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

// Sizes
#define SIZE_TX_OUTPUT 36
#define SIZE_TX_OUTPUT_OUT_ADDR 32
#define SIZE_TX_OUTPUT_AMOUNT 4

typedef struct
{
    uint8_t out_addr[32];
    uint32_t amount;
} tx_output_t;


void tx_output_serialize(tx_output_t *src, uint8_t *dst);

void tx_output_deserialize(uint8_t *src, tx_output_t *dst);

/********
 * FULL *
 ********/

// Positions
#define POS_TX_HEADER 0
#define POS_TX_BODY SIZE_TX_HEADER

// Serialized size is variable

typedef struct
{
    tx_header_t header;
    tx_input_t *ins;
    tx_output_t *outs;
} tx_t;



void tx_serialize(tx_t *src, uint8_t *dst);
tx_t *m_tx_deserialize(uint8_t *src);
void m_free_tx(tx_t *tx);

/*********
 * OTHER *
 *********/

uint32_t tx_compute_size(tx_t *tx);
uint32_t tx_raw_compute_size(uint8_t *tx);

void tx_compute_hash(tx_t *tx, uint8_t *dst);
void tx_raw_compute_hash(uint8_t *tx, uint8_t *dst);

//TODO implement these
uint32_t tx_compute_surplus(tx_t *tx);
uint32_t tx_compute_required_surplus(tx_t *tx);

uint32_t tx_compute_size_signable_hash(tx_t *tx);
void tx_generate_signable_hash(tx_t *tx, uint8_t *dst);

void compute_merkle_root(tx_t **txs, uint32_t tx_count, uint8_t *out);


#endif /* TRANSACTIONS_H */

