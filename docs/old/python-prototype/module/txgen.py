import rsa
from module import crypto
from module import util
from binascii import hexlify,unhexlify


def sign_input(tx,input_i,privkey):
    assert len(privkey) == 3
    input = tx['ins'][int(input_i)]
    msg = tx['time'] + input['tx'] + input['tx_i'] + input['out_i']
    _output = tx['outs'][int(input['out_i'])] 
    msg += _output['address'] + util.padint(_output['amount'])
    p = privkey[0]
    q = privkey[1]
    d = privkey[2]
    n = p * q
    e = crypto.PUBLIC_EXPONENT
    privkey = rsa.PrivateKey(n,e,d,p,q)
    return str(hexlify(rsa.sign(unhexlify(msg), privkey, crypto.SIG_HASH_METHOD)))[2:crypto.SIG_HASH_METHOD_LEN+2]