
## BLOCK VALIDITY

### PSEUDO-VALID
* sha256(block_header) <= `target` field
* `version` field matches that of the current client
* `merkle_root` must be merkle root of transactions in body, in order
* The body must contain `tx_count` transactions, all of which must be **pseudo-valid**, with the exception of the first one which can also be  **coinbase-valid**.

### CHAIN-VALID
* if `height` == 0:
  * Block hash must be equal to `ONE_TRUE_ROOT`
* elif `height` > 0:
  * `time` field > `time` field of previous block
  * `height`height` field == 1 + `height` of previous block
  * If `height` < `CHAIN_RECALC_INTERVAL`:
    * `target` must be equal to the previous block's `target`
  * else:
    * `target` must be correct in terms of the previous block and the block `CHAIN_RECALC_INTERVAL` blocks prior to it
* The body's transactions must all be **chain-valid**.

## TRANSACTION VALIDITY

### PSEUDO-VALID
* If [transaction_size] > 1000 bytes, this transaction's outputs' combined output surplus must be >= [transaction_size] * `TX_FEE_PER_1K`
* The body must contain `in_count` `tx_input`s and `out_count` `tx_outputs`. All inputs must be **pseudo-valid**.

### CHAIN-VALID
* If this is the first transaction in the block and (`in_count` == 0) (coinbase transaction):
  * The combined output amount <= (`MINING_REWARD` + the combined `tx_output` surplus from other transactions in the transaction's block).
* Else:
  * `in_count` > 0
  * The the sum of the `amount`s corresponding to each `tx_input` must be >= than the sum of the `amount`s corresponding to each `tx_output`.
  * The body's inputs must all be **chain-valid**.

## TRANSACTION INPUT VALIDITY

### PSEUDO-VALID
* The `sig` field must contain a signature (corresponding to the `pubkey`) of the sha256 of the input's transaction, minus the parts where the signatures would go.

### CHAIN-VALID
* The `out_index`th output of the `ref_tx` must not have been spent in this block or one prior (within the same chain).
