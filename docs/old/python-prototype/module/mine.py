import hashlib
import struct
from binascii import hexlify,unhexlify
import sys
import math

def mine(target):
    # Test values
    ver = 2
    prev_block = "000000000000000117c80378b8da0e33559b5997f2ad55e2f7d18ec1975b9717"
    mrkl_root = "871714dcbae6c8193a2bb9b2a69fe1c0440399f38d94b3a0f1b447275a29978a"
    time_ = 0x53058b35
    target = 0x5a8e89d75252446eb5d5d5b1cc5edf20a1a059e10c9ffffffffffffa76d03
    

    

    nonce = 0
    while nonce < 0x100000000:
        header =  struct.pack("<L", ver)
        header += unhexlify(prev_block)[::-1]
        header += unhexlify(mrkl_root)[::-1]
        header += struct.pack("<LL", time_, nonce)
        hash = hashlib.sha256(hashlib.sha256(header).digest()).digest()
        print nonce, hexlify(hash)
        if int(hexlify(hash),16) < target:
            return nonce
        nonce += 1
        
mine()
