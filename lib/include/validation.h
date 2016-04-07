#ifndef VALIDATION_H
#define VALIDATION_H

#define ERR_BLOCK_BAD_HASH 1
#define ERR_BLOCK_BAD_VERSION 2
#define ERR_BLOCK_BAD_ROOT 3
#define ERR_BLOCK_BAD_HEIGHT 4
#define ERR_BLOCK_BAD_TIME 5
#define ERR_BLOCK_BAD_TARGET 6

#define ERR_TX_BAD_SURPLUS 7
#define ERR_TX_BAD_COINBASE 8
#define ERR_TX_BAD_IN_COUNT 9
#define ERR_TX_OVERSPEND 10
#define ERR_TX_BAD_SIG 11
#define ERR_TX_DOUBLESPEND 12



uint8_t validate_block_pseudo(block_t *block);
uint8_t validate_block_chain(block_t *block);

uint8_t validate_tx_pseudo(tx_t *tx);
uint8_t validate_tx_chain(tx_t *tx, uint32_t index, block_t *parent);

uint8_t validate_tx_input_pseudo(tx_input_t *tx, tx_t *parent);
uint8_t validate_tx_input_chain(tx_input_t *input, block_t *parent);

#endif /* VALIDATION_H */

