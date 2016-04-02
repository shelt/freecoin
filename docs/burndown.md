# Task burndown

## Routine
* Unix philosophy: Do one thing and do it well
* Dynamic programming principles
* Commits should coorespond to checks off the burndown
* Periodically find memory leaks by grepping for "m_" and "malloc"
* Check for failure of:
  * fopen()
  * 

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
* admin.c; adding genesis block and whatnot
* Validation: Txs and Blocks
* UI
  * Command line options
  * fprint_tx()
  * fprint_block()


### Far future
* Config file
* Networking
* Mining
