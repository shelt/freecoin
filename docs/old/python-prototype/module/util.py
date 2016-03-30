import hashlib
from binascii import unhexlify

def padint(input):
    input = str(input)
    pad = (len(input)%2) * '0'
    return pad + input
###########
# HASHING #
###########
def gen_block_hash(block):    
    ver = block['ver']
    prev = block['prev']
    time = hex(int(block['start_time']))
    nonce = padint(block['nonce'])
    
    # Get merkle root
    all_tx_hash = hashlib.sha256()
    for t_hash,_tx in block['transactions'].items():
        all_tx_hash.update(t_hash)
    mrkl_root = all_tx_hash.hexdigest()
        
    header =  struct.pack("<L", ver)
    header += unhexlify(prev)[::-1]
    header += unhexlify(mrkl_root)[::-1]
    header += struct.pack("<LLL", time, crypto.BITS, nonce)
    return hashlib.sha256(hashlib.sha256(header).digest()).digest()

def gen_tx_hash(tx):
    m = hashlib.sha256()
    text = tx['time']
    for input in tx['ins']:
        text += input['tx']
        text += input['tx_i']
        text += input['out_i']
        text += input['sig']
    for output in tx['outs']:
        text += output['address']
        text += padint(output['amount'])
        
    m.update(unhexlify(text))
    return m.hexdigest()