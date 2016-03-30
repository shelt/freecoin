class BlockException(Exception): # abstract superclass
    pass

class NoSuchInputException(BlockException):
    def __init__(self,text):
        self.text = text
    def __str__(self):
        return repr(self.text)
        
class SignatureVerificationException(BlockException):
    def __init__(self,text):
        self.text = text
    def __str__(self):
        return repr(self.text)

class InvalidTransactionException(BlockException):
    def __init__(self,text):
        self.text = text
    def __str__(self):
        return repr(self.text)


class IntegrityException(BlockException):
    def __init__(self,text):
        self.text = text
    def __str__(self):
        return repr(self.text)
    
class TransactionExistsException(BlockException):
    def __init__(self,text):
        self.text = text
    def __str__(self):
        return repr(self.text)