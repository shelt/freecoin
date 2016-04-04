### NOTICE
> Remember to disable switching back to big endian after sha256. It's a wasteful operation, and we already want it in little endian because that's what our compare function uses. Alternatively, modify the compare function.

# Task burndown

## Routine
* Unix philosophy: Do one thing and do it well
* Dynamic programming principles
* Commits should coorespond to checks off the burndown
* Find memory leaks by grepping for "m_" and "malloc"
* Check:
  * for failure of fopen() and *alloc()
  * that conversions to primitive types are in host byte order

## Burndown
* ~~Tx structs~~
* ~~Tx (de)serializing~~
* ~~Block structs~~
* ~~Block (de)serializing~~
* IO
  * ~~Tx storage~~
  * ~~Block storage~~
  * ~~Functions to add and remove from chain index~~
  * ~~Function to check if a block is in the chain (ie or in limbo)~~
* ~~Safe wrapper for IO and such~~
* ui.c; smart interaction via command line
* Admin binary, other binaries
* Validation: Txs and Blocks
  * Target validation: second byte is `<32`
* UI
  * Command line options
  * fprint_tx()
  * fprint_block()


### Far future
* Config file
* Networking
* Mining
