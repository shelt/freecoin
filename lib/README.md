## The freecoin Library

Where the magic happens. Contains the core components of the freecoin specification. At the time of writing, this includes:

* Local blockchain IO
* Disk and in-memory storage of blocks and transactions
* Implementations of:
  * Big integer arithmetic, including Comba and Montgomery methods
  * SHA256
  * ECDSA, including secp160r1, secp192r1, secp224r1, secp256r1, secp256k1 (see MacKay 2014)
* Key management
* Utility functions
