#ifndef CHAIN_H
#define CHAIN_H

#include "blocks.h"
#include "shared.h"

#define GENESIS_PREV {0x47, 0x4e, 0x55, 0x2f, 0x48, 0x75, 0x72, 0x64, 0x20, 0x31, 0x2e, 0x30, 0x20, 0x52, 0x65, 0x6c, 0x65, 0x61, 0x73, 0x65, 0x64, 0x21, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
// TODO adjust
#define CHAIN_INITIAL_TARGET {0xff, 0x00}
// 1 days worth of blocks if blocks take 10 minutes (TODO increase to 2 weeks)
#define CHAIN_RECALC_INTERVAL 144

/* Times in seconds */
#define SECONDS_IN_20_MINUTES 1200
#define SECONDS_IN_HALF_WEEK 5040
#define SECONDS_IN_8_WEEKS 80640

block_t *m_genesis_block();

void compute_next_target(block_t *block_b, uint8_t *next_target);

#endif /* CHAIN_H */

