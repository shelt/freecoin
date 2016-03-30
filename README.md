# freecoin

**freecoin** is a fully-functional cryptocurrency I'm making for fun. Unlike most  cryptocurrencies which followed Bitcoin (litecoin, dogecoin), freecoin uses no Bitcoin source code.

## Background

### What is a cryptocurrency?

A cryptocurrency is a currency that is functionally possible because of cryptography. Mathematics and cryptographic principles allow a user to prove they were the recipient of a past transaction.

### What does it mean to be "decentralized"?

With banks that deal with regular currencies, all regulation happens in one place. Storing the amount of money each person has along with the processing of transactions all happens on a few servers. Though they probably take backups and security very seriously, banks (and the governments that regulate them) represent a central authority over a vastly powerful resource. With decentralized currencies, transactions are processed and balances are stored by the people. Everyone agrees on how things should work, so it works.

## Protocol specification
#####Revision 4 - 9/28/15

> Freecoin uses a session-layer protocol that enables peer-to-peer communication. It is intended to be used specifically for freecoin. 
>
> The freecoin protocol requires a reliable underlying transport layer protocol. This is due to the nature of the exchanged data; info must be able to permeate the network quickly.

### Message content types
> Messages can be of fixed or variable size. If they are the latter, some fixed part of their content specifies the size of the following content. All messages have an additional 3 bytes at the beginning specifying their client version and their content type.

| Name | reject |
| ---- | ---- |
| Value    | 0  |
| Size     | `1B + 1B + [<=255B] (ERRORTYPE + info_size + info)`  |
| Purpose  | Tell a peer that a block/tx/time/alert/version is invalid. Sent in response to various messages.  |
| Content  | ERRORTYPE byte and info about what specifically is invalid.    |

| Name | getblocks |
| ---- | ---- |
| Value    | 1  |
| Size     | `32B + 1B (start_block + block_count)`  |
| Purpose  | Request an inv containing start_block and (block_count-1) blocks following it (up to 255). Used to update blockchain from a block onward. Sent unsolicited.  |
| Content  | A block hash and a block count.  |

| Name | mempool |
| ---- | ---- |
| Value    | 2  |
| Size     | `0B`  |
| Purpose  | Request an inv containing peer's mempool txs. Alternatively named `gettxs` to fit in with `getnodes` and `getblocks`. Sent unsolicited.  |
| Content  | N/A  |

| Name | inv |
| ---- | ---- |
| Value    | 3  |
| Size     | `1B + 1B + ?*32B (DATATYPE + data_ids_count + data_ids[])`  |
| Purpose  | Tell a peer about blocks or txs that you have. Sent unsolicited or in response to getblocks or mempool.  |
| Content  | List of tx or block hashes that you have in your mempool or blockchain, respectively; or list of peers you are connected to.  |

| Name | getdata |
| ---- | ---- |
| Value    | 4  |
| Size     | `1B + 1B + ?*32B (DATATYPE + data_ids_count + data_ids[])`  |
| Purpose  | Request full block(s)/tx(s) or all of a peer's peers. Sent unsolicited, often in response to an inv (for the former cases).  |
| Content  | DATATYPE byte and identifying hash(es).  |

| Name | block |
| ---- | ---- |
| Value    | 5  |
| Size     | `4B + [varies] (size + block)`  |
| Purpose  | Send a full block to a peer. Solicited by getdata.  |
| Content  | A full block.  |

| Name | tx |
| ---- | ---- |
| Value    | 6  |
| Size     | `4B + [varies] (size + tx)`  |
| Purpose  | Send a full transaction to a peer. Solicited by getdata.  |
| Content  | A full transaction.  |

| Name | peer |
| ---- | ---- |
| Value    | 7  |
| Size     | `2B + 2B + [varies] (port + addrlen + addr)`  |
| Purpose  | Send a full transaction to a peer. Solicited by getdata.  |
| Content  | A full transaction.  |

| Name | alert |
| ---- | ---- |
| Value    | 8  |
| Size     | `1B + 1B + 4B + 128B + 1B + [<=255B] (ALERTTYPE + command + time + sig{ALERTTYPE+time+msg}) +  msg_len + msg`  |
| Purpose  | Notify entire network about network emergency. Only valid if signed by key at key.shelt.ca.  int |
| Content  | Information about the network issue. Depending on ALERTTYPE, the implementation may need to take action (such as not allowing mining).  |

| Name | ping |
| ---- | ---- |
| Value    | 9  |
| Size     | `0B`  |
| Purpose  | Verify connectivity of peer. Sent unsolicited.  |
| Content  | N/A  |

| Name | pong |
| ---- | ---- |
| Value    | `10 ` |
| Size     | 0B  |
| Purpose  | Verify connectivity of self to peer. Solicited by ping. |
| Content  | N/A  |

### Errors

See the `<reject>` message specification for full size of error messages.

#### Block-related

| Name | `BLOCK_CONFLICT` |
| ---- | ---- |
| Value    | 4 |
| Size     | `32B` |
| Meaning  | Block's `prev_hash` is already in another block.  |
| Content  | Offending block's hash.  |

| Name | `BLOCK_BAD_TIME` |
| ---- | ---- |
| Value    | 5 |
| Size     | `32B` |
| Meaning  | Block's time is before last block.  |
| Content  | Offending block's hash.  |

| Name | `BLOCK_BAD_ROOT` |
| ---- | ---- |
| Value    | 6 |
| Size     | `32B` |
| Meaning  | Block's `merkle_root` does not match body.  |
| Content  | Offending block's hash.  |

| Name | `BLOCK_BAD_TARGET` |
| ---- | ---- |
| Value    | 7 |
| Size     | `32B` |
| Meaning  | Block's target is not correct.  |
| Content  | Offending block's hash.  |

| Name | `BLOCK_BAD_HASH` |
| ---- | ---- |
| Value    | 8 |
| Size     | `32B` |
| Meaning  | Block's hash isn't valid as per the target.  |
| Content  | Offending block's hash.  |

| Name | `BLOCK_BAD_BODY` |
| ---- | ---- |
| Value    | 9 |
| Size     | `32B` |
| Meaning  | Transaction(s) in body are invalid. No tx-specific reject messages are needed because the offender shouldn't have send such a block anyway.  |
| Content  | Offending block's hash.  |

| Name | `BLOCK_OVERFLOW` |
| ---- | ---- |
| Value    | 10 |
| Size     | `32B` |
| Meaning  | Block is larger than 1 MB.  |
| Content  | Offending block's hash.  |

| Name | `BLOCK_EXISTS` |
| ---- | ---- |
| Value    | 11 |
| Size     | `32B` |
| Meaning  | Block already exists in the blockchain.  |
| Content  | Offending block's hash.  |

#### Transaction-related

| Name | unused |
| ---- | ---- |
| Value    | 12 |
| Size     |  |
| Meaning  |  |
| Content  |  |

| Name | `TX_BAD_SIG` |
| ---- | ---- |
| Value    | 13 |
| Size     | `32B` |
| Meaning  | Transaction input signature is invalid.  |
| Content  | Offending transaction's hash.  |

| Name | `TX_BAD_REF` |
| ---- | ---- |
| Value    | 14 |
| Size     | `32B` |
| Meaning  | A transaction input is already used or nonexistant. |
| Content  | Offending transaction's hash.  |

| Name | `TX_REF_OVERSPEND` |
| ---- | ---- |
| Value    | 15 |
| Size     | `32B` |
| Meaning  | A transaction's inputs are smaller than outputs.  |
| Content  | Offending transaction's hash  |

#### Generic

| Name | `BAD_VERSION` |
| ---- | ---- |
| Value    | 1 |
| Size     | `0B` |
| Meaning  | Incompatible version.  |
| Content  | *N/A* |

| Name | `MESSAGE_MALFORMED` |
| ---- | ---- |
| Value    | 2 |
| Size     | `0B` |
| Meaning  | Data within the message was not parsed according to specification. |
| Content  | *N/A* |

| Name | `UNKNOWN_CTYPE` |
| ---- | ---- |
| Value    | 3 |
| Size     | `0B` |
| Meaning  | First byte represents an unknown protocol CTYPE.  |
| Content  | *N/A*  |


### DATATYPES
> (used for inv and getdata)

    0 blocks
    1 transactions
    2 peers



### Network procedures

The following are standards for accomplishing network tasks. This is how they are implemented in the *freecoin* source. Some things to note:

* Networking is designed in such a way that all sending and Receiving can happen concurrently and independently of one another. For instance, a function which sends a `<getblocks>` doesn't wait for some `<block>` messages; it simply returns to its caller which in turn waits for some blocks to show up.

* **mempool**: A place where un-added transactions are stored
* **limbo**: A place where non-blockchain (Received) blocks are stored until they can be added

ANY TIME BLOCKS ARE ADDED TO CHAIN: remove any transactions that are in them from the mempool. 

The underlying goal of each client that keeps the network working is: we always want to have the highest block height possible. Therefore, we also want to know about every block we can.

#### Joining the network (theoretical)
- Try connecting to known nodes until one works. This is peer<sub>0</sub> .

For `(0 <= n < 8)`:

- Ask peer<sub>n</sub> what peers they are connected to. Select a node randomly from this pool (excluding ourself,  peer<sub>[0,n-1]</sub> and any nodes we've failed to connect to). This is peer<sub>n+1</sub>.
- If peer<sub>n</sub> for `n>0` has less than 8 peers, send them a list of all our peers.
- If peer<sub>n</sub> for `n>0` has no nodes that are valid to us (according to above exclusions), select another node from the same pool we selected peer<sub>n</sub> from.

#### Joining the network (programatically):
- Send a connected server a `<getnodes>`. We will Receive an `<inv>` of peers (handled seperately, see **Receiving an `<inv>` (peers)**)
- Wait 15 seconds and for peercount to reach 4 (normally) or 1 (testnet).

#### Receiving a `<reject>`
- Display details of error to user.

#### Receiving a `<getnodes>`
TODO noexist

#### Receiving a `<getblocks>`
- If we have the specified `start_block` in our blockchain
  - Reply with as many `<block>`s as we can between (1-255) and (1-`block_count`) (not inclusive of `start_block`)

#### Receiving a `<mempool>`
- If we have at least 1 transaction in our mempool:
  - reply with as many `<tx>`s as we can (1-255).

#### Receiving an `<inv>` (blocks)
- For each block hash:
  - If we don't have the block hash in our blockchain or limbo:
    - reply with a `<getdata>` for the block.

#### Receiving an `<inv>` (transactions)
- For each transaction hash:
  - If we don't have the transaction hash in our mempool:
    - Reply with a `<getdata>` for the transaction.

#### Receiving a `<getdata>` (blocks)
- For each requested block
  - If we have the block in limbo or our blockchain:
    - Reply with the correct `<block>`

#### Receiving a `<getdata>` (txs)
- For each requested tx:
  - If we have the tx in our mempool:
    - Reply with the correct `<tx>`

#### Receiving a `<getdata>` (peers)
- If we are connected to at least 1 peer:
  - Reply with a `<peer>` of all connected peers

#### Receiving a `<block>`
- If the block height compared to our blockchain height is:
  - **Less than or equal to, or 2 or more greater than:**
    - Save the block to limbo.
  - **1 greater than:**
    - If the `prev_blockhash` is the different from the latest block in our chain:
      - Save the block to limbo.
    - Otherwise:
        -check if this block is valid. If it isn't:
          - Drop and block the peer that sent it.
        - Otherwise:
          - Add this block to our blockchain.
          - Tell our peers about this block in an `<inv>`
- For each block in limbo with a height greater than our current blockchain height, in order from greatest difference to smallest:
  - If that block can be traced back (through `prev_hash`s) to a block in our blockchain:
    - If each block in that limbo chain is valid:
      - Swap the limbo chain with our current blockchain at the point of similarity (this is called a **revert**). The unused blocks should now be in limbo.
      - Send all our peers an `<inv>` containing the newly added chain.
      - *[break loop]*
    - Otherwise:
      - drop and block the peer that sent it. Optionally, send a error-specific `<reject>` message.
  - Otherwise:
    - Send a `<getblock>` for the referenced yet missing block.
    - *[continue loop]*

#### Receiving a `<tx>`
- If we don't have the transaction hash in our mempool:
  - If the transaction is double-spending from the mempool or the blockchain
  - If the transaction is valid

#### Receiving a `<peer>`
- Save the peers (excluding ourselves and ones we already have) to file
- If we are connected to less than 8 peers:
  - TODO

#### Receiving an `<alert>`
- TODO

#### Receiving a `<ping>`
- Reply with `<pong>`

#### Receiving a `<pong>`
- Inform user





### Misc procedures

#### Do periodically:
- Send an `<inv>` of your latest block.
- Send an `<inv>` of your peers.

#### Validating a block
- Applying the SHA256 hash function to the block's header (first `SIZE_BLOCK_HEADER` bytes) must result in a value less than or equal to the expanded form of `target`.
- `version` must match that of the current client.
- `time` must be after that of the block corresponding to `prev_hash`.
- `height` must be one more than that of the block corresponding to `prev_hash`.
- `prev_hash` must correspond to a valid block (for blocks of `height` > 0).
- `merkle_root` must be the merkle root of the transactions in the body, in order.
- `target` must be the correct target in terms of the block corresponding to `prev_hash` and the block 2016 blocks ago.
- The body of the block must contain `tx_count` transactions, all of which must be valid, with the following exception:
  - The first transaction is allowed to be a coinbase transaction.

#### Validating a coinbase transaction
- `in_count` must be 0.
- The **combined** output amount must be equal to or less than (MINING_REWARD + the **combined** output surplus from other transactions in the transaction's block).

#### Validating a transaction (non-coinbase)
- `in_count` must be greater than 1.
- `version` must match that of the current client.
- The the sum of the `amount`s corresponding to each input must be equal to or greater than the sum of this transaction's outputs' **combined** `amount`.
- If the transaction is larger than 1000 bytes, this transaction's outputs' **combined** output surplus must be equal to or greater than `[transaction_size] * TX_FEE_PER_1K`.
- The body of the transaction must consist of `in_count` inputs followed by `out_count` outputs, all of which must be valid.

#### Validating a transaction input
- `ref_tx` must be in our blockchain. If not, this procedure should return `ERR_TX_BAD_REF`, which should be passed to the source of this transaction.
- The `out_index`th output of the `ref_tx` must not be spent elsewhere in the chain.
- The `out_index`th output of the `ref_tx` must have an `out_address` which is a valid address of `pubkey`.
- `sig` must be a valid RSA signature of (`ref_tx` .. `out_index`).
- The current time (or the time of the block) must be equal to or greater than `lock_time`.
#### Validating a transaction output
*An output can contain any data because it is limited by the fee and amount rules of transaction validation.*

#### Submitting a transaction
- Send all peers an `<inv>` containing new transaction's hash

#### Upon finding a valid block
TODO





## Source information

### Data storage
Freecoin currently uses Berkeley DB for data storage.
* `blockhash:blockdata`
  *`txhash`:`blockdata` (secondary)
* `node`:TODO



## Source guidelines
* `size_t` should be used when dealing with array sizes UNLESS it is measuring lengths of serialized data (where such a number would need to fit in the specified range).
* Memory allocation (i.e., blocks) for a function's output is always done via implementation. Pointers to the pre-allocated memory block are passed into the function (typically as the last parameter).
* Only include things in the header file when the header file itself needs them.
* Functions which use/modify a type or form of data (block, tx) should be prefixed as such: `block_init()` or `tx_get_size()`
* The following are "reserved" prefixes used in identifiers:
| Prefix | Meaning |
| ---- | ---- |
| `ffind_`   | Store information into provided memory which is stored in file(s) |
| `{type}_`  | Performs some `{type}`-specific operation, where `{type}` is a struct or structured block of memory.  |


## Todo

### Questions
> Blockchain transactions need to be indexed to validate blocks and transactions. Should transactions in blocks in limbo also be indexed?

**A: ** No. We only need to index transactions to determine if they should be added to our workblock. If our workblock is included in the blockchain, the limbo blocks are not relevant. Similarily, if the limbo blocks end up being used in the blockchain, our workblock is not relevant.

### Roadmap
* How do we store blocks on the filesystem?


### Misc
* Make networking ipv6-friendly
* Clean up header files
* The first 64B chunk of every hash need not be calculated on each iteration.
* make all functions that copy data to a ptr also return the ptr
