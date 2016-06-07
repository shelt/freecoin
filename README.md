# freecoin

**freecoin** is a fully-functional cryptocurrency I'm making for fun. Unlike most  cryptocurrencies which followed Bitcoin (litecoin, dogecoin), freecoin uses no Bitcoin source code.

## Background

### What is a cryptocurrency?

A cryptocurrency is a currency that is functionally possible because of cryptography. Mathematics and cryptographic principles allow a user to prove they were the recipient of a past transaction.

### What does it mean to be "decentralized"?

With banks that deal with regular currencies, all regulation happens in one place. Storing the amount of money each person has along with the processing of transactions all happens on a few servers. Though they probably take backups and security very seriously, banks (and the governments that regulate them) represent a central authority over a vastly powerful resource. With decentralized currencies, transactions are processed and balances are stored by the people. Everyone agrees on how things should work, so it works.

## Protocol
#####Revision 5.0 - 6/4/16
All **freecoin** communication is done over TCP. Because data must be able to permeate the network quickly, most functions are designed with the purpose of data request and exchange.

### Header
All messages have the following 7B header.

    [4][2][1]
    [body_len][version][CTYPE]

### Content types (CTYPE\_\*)

    0 <reject>
    [1][1][<=255]
    [ETYPE][info_size][info]
    Sent in response to invalid data.
    
    1 <gethighest>
    []
    Requests an inv of a peer's highest chained block.
    
    2 <getchain>
    [32][1]
    [start_block][count]
    Requests an inv of the [count] blocks that proceed [start_block].
    
    3 <gettxs>
    []
    Requests an inv of all orphan transactions a peer knows about.

    4 <getpeers>
    []
    Requests all of a peer's peers.
    
    5 <inv>
    [1][1][count*32]
    [DTYPE][count][hashes..]
    Informs a peer about the existence of data.
    Should only include *orphan* transactions and *chain-valid* blocks.
    
    6 <getdata>
    [1][1][count*32..]
    [DTYPE][count][hashes..]
    Requests blocks or transactions.

    7 <block>
    [?]
    A block. Should only be sent when solicited by getdata.
    
    8 <tx>
    [?]
    A full transaction. Should only be sent when solicited by getdata.

    9 <peer>
    [1][1][addr_len]
    [port][addr_len][addr]
    Information about a peer.

    10 <alert>
    [1][48]([1][4][msg_len])
    [msg_len][sig]([ATYPE][time][msg])
    Notifies peer about network emergency. Only valid if signed by admin_key.
    
    11 <ping>
    []
    Requests that a peer respond with a <pong>.

    12 <pong>
    []
    Solicited by <ping>.

### Error types (ETYPE\_\*)
    1 bad_version
    2 message_malformed
    3 bad_ctype
    4 bad_dtype
    5 bad_atype
    
### Data types (DTYPE\_\*)
    0 blocks
    1 transactions
    2 peers
    
### Alert command types (ATYPE\_\*)
    0 warnuser
    1 forceupdate
    
### General procedures
Note: See the source for info on handling received messages.

#### General
In order for the network to remain functional, all clients must abide by the following three rules: 

* Stay as informed as possible
* Keep others as informed as you are
* Strive to have the highest possible chain.

#### Joining the network
* Try connecting to known nodes until one works. This is peer<sub>0</sub> .

* For `(0 <= n < 8)`:

  * Ask peer<sub>n</sub> what peers they are connected to. Select a node randomly from this pool (excluding ourself,  peer<sub>[0,n-1]</sub> and any nodes we've failed to connect to). This is peer<sub>n+1</sub>.
  * If peer<sub>n</sub> for `n>0` has less than 8 peers, send them a list of all our peers.
  * If peer<sub>n</sub> for `n>0` has no nodes that are valid to us (according to above exclusions), select another node from the same pool we selected peer<sub>n</sub> from.

#### Do periodically:
* Send all peers a `<gethighest>`.
* If you have less than 8 peers, send each peer a `<getpeers>` until you do.

### Validation

It's important to test the validity of data we receive. Although there are certain traits a block or transaction may have that make them blatanly invalid, there are certain types of validity that can't be tested for until we have a complete picture. To remedy this, there are two tiers of validity: **pseudo-valid** and **chain-valid**. Data can be confirmed to be **pseudo-valid** at first glance. However, it's only until we know about the entire chain of a block or the block of a transaction that we may deem data to be **chain-valid**.

#### Block validation (pseudo)
* sha256(block_header) <= `target` field
* `version` field matches that of the current client
* `merkle_root` must be merkle root of transactions in body, in order
* The body must contain `tx_count` transactions, all of which must be **pseudo-valid**.

#### Block validation (chain)
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

#### Transaction validation (pseudo)
* `version` field matches that of the current client
* If [transaction_size] > 1000 bytes, this transaction's outputs' combined output surplus must be >= [transaction_size] * `TX_FEE_PER_1K`
* The body must contain `in_count` `tx_input`s and `out_count` `tx_outputs`. All inputs must be **pseudo-valid**.

#### Transaction validation (chain)
* If this is the first transaction in the block and (`in_count` == 0) (coinbase transaction):
  * The combined output amount <= (`MINING_REWARD` + the combined `tx_output` surplus from other transactions in the transaction's block).
* Else:
  * `in_count` > 0
  * The the sum of the `amount`s corresponding to each `tx_input` must be >= than the sum of the `amount`s corresponding to each `tx_output`.
  * The body's inputs must all be **chain-valid**.

#### Transaction input validation (pseudo)
* The `sig` field must contain a signature (corresponding to the `pubkey`) of the sha256 of the input's transaction, minus the parts where the signatures would go.

#### Transaction input validation (chain)
* The `out_index`th output of the `ref_tx` must not have been spent in this block or one prior (within the same chain).


## Source information

## Source guidelines

## Todo

### Routine

### Far future

### Misc
* ipv6
* fast_mine: C API, and the first 64B chunk of every hash need not be calculated on each iteration.
