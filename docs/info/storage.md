# Data storage

The data/ folder:

* data/blocks: raw block data
* data/heads : blockchain head refs
* data/txindex: block transaction indexes

## HEADS
Heads are references to top-level blocks in the chain. In other words, all blocks which are not referenced by a successive block are referenced by a head. Because blocks only keep backward references, this allows the client to keep track of where the chain starts and potential places for new blocks to go. It also allows the client to easily identify stale forks.

#### Fully chained head
Head which can be traced all the way back to the genesis block
#### Fast forwarding of a head
When an existing head is modified to reference a successive block
#### Stale head
Head with a distance of 6 or more from the highest head. Represents a "stale fork" which should be deleted.
#### Dead head
Head which is no longer on top.

### Enchaining a block
* Check that the block isn't already pointed to by some head.
* No? Check if the block can be appended to any current head (and that head fast forwarded). 
* No? Create a new head that points to the new block.
* Perform a "chain clean":
  * Check for "dead heads"; for each head `H`, move backwards from `H` and delete any heads *directly* pointing to blocks in the range `height(H)` down to `height(lowest_head_height)`.  b
  * Check for "stale heads"; delete each head with a distance of 6 or more from the highest head.
  * Delete all blocks not referenced by heads.
  * For each non-fully-chained head `H`, check if it is now fully chained and update it accordingly.
  * For each fully chained head `H`, ensure each block all the way down is **chain valid**.
    * If one isn't, store it's `prev_hash` block as `P`, remove it and all blocks and heads above it. Continue checking each block starting with `P`.
    * If we find another invalid block, repeat the last step on it, g enerating a new `P`. If we don't, and `P` isn't referenced by another full chain, create a new head pointing to `P`.
