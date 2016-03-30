import json
from collections import OrderedDict
from Class.Block import Block

class Blockchain(Block):
    '''
        Class for the file-specified blockchain.
        Functions suffixed with _h mean "by hash"
    '''
    def __init__(self,file):
        data = open(file).read()
        self.blockchain = json.loads(data, object_pairs_hook=OrderedDict)
        
    # TODO
    #def compute_target():
    #    blocklength = len(self.blockchain.items())
    #    sum = 0
    #    self.blockchain.items()[blocklength-4]['time']
    #    for i in range(blocklength-3, blocklength):
    #        sum += self.blockchain.items()[i][
    #        target = target * (10/prev_time_diff)
    
    ''' Get a transaction by hash.
            It is inadvisable to use this function when working on a new block
        because the workblock is not searched. Use the WorkBlock functions
        instead (which search both).'''
    def get_tx_h(self,hash):
        for _,block in self.blockchain.items():
            try:
                return block['transactions'][hash]
            except KeyError:
                pass
    
    ''' Get whether or not an output has been used within the blockchain. 
            It is inadvisable to use this function when working on a new block
        because the workblock is not searched. Use the WorkBlock functions
        instead (which search both).
    '''
    def output_used(self,hash,i):
        for _,block in self.blockchain.items():
            r = super(Blockchain,self).output_used(block,hash,i)
            if r:
                return r
        return False