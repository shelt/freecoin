#include "transactions.h"
#include "util.h"

/**********
 * HEADER *
 **********/

void tx_header_serialize(tx_header_t *src, uint8_t *dst)
{
    ustob(src->version, &dst[POS_TX_HEADER_VERSION]);
    ustob(src->in_count, &dst[POS_TX_HEADER_IN_COUNT]);
    ustob(src->out_count, &dst[POS_TX_HEADER_OUT_COUNT]);
    uitob(src->lock_time, &dst[POS_TX_HEADER_LOCK_TIME]);
}
void tx_header_deserialize(uint8_t *src, tx_header_t *dst)
{
    dst->version   = btous(&src[POS_TX_HEADER_VERSION]);
    dst->in_count  = btous(&src[POS_TX_HEADER_IN_COUNT]);
    dst->out_count = btous(&src[POS_TX_HEADER_OUT_COUNT]);
    dst->lock_time = btoui(&src[POS_TX_HEADER_LOCK_TIME]);
}

/*********
 * INPUT *
 *********/

void tx_input_serialize(tx_input_t *src, uint8_t *dst)
{
    ustob(src->out_index, &dst[POS_TX_INPUT_OUT_INDEX]);
    memcpy(&dst[POS_TX_INPUT_REF_TX], src->ref_tx, sizeof(src->ref_tx));
    memcpy(&dst[POS_TX_INPUT_PUBKEY], src->pubkey, sizeof(src->pubkey));
    memcpy(&dst[POS_TX_INPUT_SIG], src->sig, sizeof(src->sig));
}
void tx_input_deserialize(uint8_t *src, tx_input_t *dst)
{
    dst->out_index = btous(&src[POS_TX_INPUT_OUT_INDEX]);
    memcpy(dst->ref_tx, &src[POS_TX_INPUT_REF_TX], sizeof(dst->ref_tx));
    memcpy(dst->pubkey, &src[POS_TX_INPUT_PUBKEY], sizeof(dst->pubkey));
    memcpy(dst->sig, &src[POS_TX_INPUT_SIG], sizeof(dst->sig));
}

/**********
 * OUTPUT *
 **********/

void tx_output_serialize(tx_output_t *src, uint8_t *dst)
{
    memcpy(&dst[POS_TX_OUTPUT_OUT_ADDR], src->out_addr, sizeof(src->out_addr));
    uitob(src->amount, &dst[POS_TX_OUTPUT_AMOUNT]);
}

void tx_output_deserialize(uint8_t *src, tx_output_t *dst)
{
    memcpy(dst->out_addr, &src[POS_TX_OUTPUT_OUT_ADDR], sizeof(dst->out_addr));
    dst->amount = btoui(&src[POS_TX_OUTPUT_AMOUNT]);
}