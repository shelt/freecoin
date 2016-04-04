#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <transactions.h>
#include <util.h>
#include <shared.h>
#include <blocks.h>
#include <secrets.h>

void cmd_newaddr()
{
    printf("Generating new address\n");
    crypt_keygen();
}

int main(int argc, char **argv)
{
    init();
    if (argc == 2)
    {
        if (strcmp(argv[1], "newaddr") == 0)
            cmd_newaddr();
    }
    return 0;
}