#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <freecoin/io.h>
#include <freecoin/transactions.h>
#include <freecoin/util.h>
#include <freecoin/shared.h>
#include <freecoin/blocks.h>
#include <freecoin/sha256.h>
#include <freecoin/secrets.h>



int main(int argc, char **argv)
{
    if (argc != 2 || strlen(argv[1]) % 4 != 0)
        fatal("Usage: %s <base64 miner address>", argv[0]);
    
    init();
    
        
    
    return 0;
}