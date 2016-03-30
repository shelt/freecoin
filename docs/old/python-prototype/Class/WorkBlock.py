from module import util
from time import time
from Class.Block import Block


#todo
REWARD_AMOUNT = 50

class WorkBlock(Block):
    def __init__(self,blockchain,prev_hash,miner_address):
        '''
            Class for the current work block.
            Functions such as "output_used" search both this AND the passed blockchain object.
        '''
        self.BLOCKCHAIN = blockchain.blockchain
        self.hash = ""
        self.block = {} #TODO get from IRC
        self.block['nonce'] = ""
        self.block['prev'] = prev_hash
        self.block['start_time'] = str(int(time()))
        self.block['solve_time'] = ""
        self.block['transactions'] = {"0000":{ # Temp value
                "time":str(int(time())),
                'ins':[],
                'outs':[{
                "address":miner_address,
                "amount":util.padint(REWARD_AMOUNT)
                }]
            }
        }
        # Set reward transaction hash
        self.block['transactions'][util.gen_tx_hash(self.block['transactions']["0000"])] = self.block['transactions']["0000"]
        del self.block['transactions']["0000"]
    
    ''' Get a transaction by hash. '''
    def get_tx_h(self,hash):
        # SEARCH BLOCKCHAIN
        for _,block in self.BLOCKCHAIN.items():
            try:
                return block['transactions'][hash]
            except KeyError:
                pass
        # SEARCH WORKBLOCK
        try:
            return self.block['transactions'][hash]
        except KeyError:
            pass
    
    ''' Get whether or not an output has been used within the new block. '''
    def output_used(self,hash,i):
        # SEARCH BLOCKCHAIN
        for _,block in self.BLOCKCHAIN.items():
            r = super(WorkBlock,self).output_used(block,hash,i)
            if r:
                return r
        # SEARCH WORKBLOCK
        return super(WorkBlock,self).output_used(self.block,hash,i)
        
        
    def add_tx(self,tx):
        hash = util.gen_tx_hash(tx)
        if self.get_tx_h(hash):
            raise TransactionExistsException("TransactionExistsException")
        self.tx_inputs_usable(tx)
        self.tx_inputs_signed(tx)
        change = self.tx_amounts_valid(tx)
        assert change != -1 # not a coinbase transaction
        for address,amount in change.items():
            tx['outs'].append({
                                "address":address,
                                "amount":amount
                              })
        print("DEBUG")
        print(tx)
        #self.verify_tx_integrity() need not be called, because it only verifies the hash at the moment.
        
#TODO create a function get_valid_inputs_for_address(address)
        
            
    def win(self,nonce):
        self.block['nonce'] = nonce
        self.block['solve_time'] = str(int(time()))
        self.hash = util.gen_block_hash(self.block)
        
        #TODO broadcast new block to IRC