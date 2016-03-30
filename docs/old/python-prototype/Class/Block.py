from binascii import unhexlify
import rsa

from module import crypto
from module import util
from Class.Exception import *

class Block(object):
    '''
        Abstract superclass of shared block functions.
    '''
    ############################
    ## INTEGRITY VERIFICATION ##
    ############################
    def verify_block_integrity(self,blockhash,block):
        block_gen_valid(block)
        # Transactions
        for tHash,transaction in block['transactions'].items():
            verify_tx_integrity(tHash,transaction)
            tx_amounts_valid(transaction)
        return True
                    
        # Block
        if util.gen_block_hash(block) != blockhash:
            raise IntegrityException('False block: ' + blockhash)
        return True
    
    def verify_tx_integrity(self,tHash,transaction):
        if util.gen_tx_hash(transaction) != tHash:
            raise IntegrityException('False transaction: ' + tHash)
        return True
    
    ######################
    # BLOCK VERIFICATION #
    ######################
    def block_gen_valid(self, block):
        found = False
        for tHash,transaction in block['transactions'].items():
            if found == True:
                raise IntegrityException('Invalid coinbase tx in block ' + blockhash)
            if len(transaction['ins']) == 0:
                if len(transaction['outs']) != 1:
                    raise IntegrityException('Invalid coinbase tx in block ' + blockhash)
                if int(transaction['outs'][0]['amount']) != REWARD_AMOUNT:
                    raise IntegrityException('Invalid coinbase reward in block ' + blockhash)
                found = True
        return True
        
    ############################
    # TRANSACTION VERIFICATION #
    ############################
    
    ''' Verify that a transaction is signed. Uses child functions get_tx_h. '''
    def tx_inputs_signed(self,tx):
        for input in tx['ins']:
            msg = tx['time'] + input['tx'] + input['tx_i'] + input['out_i']
            _output = tx['outs'][int(input['out_i'])] 
            msg += _output['address'] + util.padint(_output['amount'])
            try:
                input_trans = self.get_tx_h(input['tx'])
                pubkey = input_trans['outs'][int(input['tx_i'])]['address']
            except TypeError:
                raise NoSuchInputException('Input nonexistant: ' + input['tx'] + "-" + input['tx_i'])
            try:
                rsa.verify(unhexlify(msg), unhexlify(input['sig']), rsa.PublicKey(int(pubkey,16), crypto.PUBLIC_EXPONENT))
            except rsa.pkcs1.VerificationError:
                raise SignatureVerificationException("False signature: " + input['sig'])
        return True
        
    ''' Verify that a transaction input has not been used previously. Uses child functions output_used. '''
    def tx_inputs_usable(self,tx):
        for input in tx['ins']:
            if self.output_used(input['tx'],input['tx_i']):
                raise InvalidTransactionException('Output already used: ' + input['tx'] + '-' + input['tx_i'])
        return True
    
    ''' Get whether or not an output has been used within a block. Called from within child overrides.
        This function searches a specific block, whereas overrides call this one repeatedly.
    '''
    def output_used(self,block,hash,i):
        for tHash,transaction in block['transactions'].items():
            for input in transaction['ins']:
                if input['tx'] == hash and input['tx_i'] == i and (util.gen_tx_hash(transaction) != tHash):
                    return (tHash,i)
        return False
        
    def tx_amounts_valid(self,tx):
        change = {}
        
        if (len(tx['ins']) == 0) and (len(tx['outs']) == 1):
            # Coin generation
            return -1
        elif len(tx['outs']) > len(tx['ins']):
            raise InvalidTransactionException('More outputs than inputs')
        
        for o_index,output in enumerate(tx['outs']):
            if int(output['amount']) == 0:
                raise InvalidTransactionException('Zero-value output at output ' + str(o_index))
            in_total = 0
            for i_index,input in enumerate(tx['ins']):
                if int(input['out_i']) == o_index:
                    in_tx = self.get_tx_h(input['tx'])
                    in_amount  = int(in_tx['outs'][int(input['tx_i'])]['amount'])
                    in_total += in_amount
                    
                    # Change?
                    if int(output['amount']) < in_amount:
                        in_address = in_tx['outs'][int(input['tx_i'])]['address']
                        change[in_address] = in_amount - int(output['amount'])
            if int(output['amount']) > in_total:
                raise InvalidTransactionException('Overspending at output ' + str(o_index))
        return change