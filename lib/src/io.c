#include "io.h"
#include "transactions.h"
#include "shared.h"
#include "util.h"
#include "fs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#define SIZE_BLOCK_HASH 32

#define DIR_BLOCKS "/blocks/"
#define DIR_TXINDEX "/txindex/"

int intern_find_hash(uint8_t *hash, char *path)
{
    FILE *f;
    uint8_t *hash_buffer = malloc(SIZE_SHA256);

    f = fopen(path, "rb");
    if (!f)
        fatal("Failed to open file in intern_find_hash()");
    
    // Size
    fseek(f, 0, SEEK_END);
    int hash_count = ftell(f);
    rewind(f);
    
    int retval = -1;
    for (int i=0; i<hash_count; i++)
    {
        fread(hash_buffer, SIZE_SHA256, 1, f);
        if (memcmp(hash, hash_buffer, SIZE_SHA256) == 0)
        {
            retval = i;
            break;
        }
    }
    
    fclose(f);
    free(hash_buffer);
    return retval;
}

/****************
 * TRANSACTIONS *
 ****************/

void io_load_nth_tx_raw(uint8_t *block_hash, uint32_t n, uint8_t *dst)
{
    uint8_t *block_buffer = malloc(MAX_BLOCK_SIZE);
    io_load_block_raw(block_hash, block_buffer);
    
    // Check for sanity
    if (n >= btoui(&block_buffer[POS_BLOCK_HEADER_TX_COUNT]))
        fatal("Attempted to use io_load_nth_tx_raw() with n >= tx_count");
    
    uint64_t ins_size,outs_size;
    int cursor = SIZE_BLOCK_HEADER;
    int tx_cursor = 0;
    while (tx_cursor < n)
        cursor += tx_raw_compute_size(&block_buffer[cursor]);
    
    memcpy(dst, &block_buffer[cursor], tx_raw_compute_size(&block_buffer[cursor]));
    
    free(block_buffer);
}

void io_load_tx_raw(uint8_t *tx_hash_src, uint8_t *dst)
{
    uint8_t *block_hash = malloc(SIZE_BLOCK_HASH);
    int pos = io_block_of_tx(tx_hash_src, block_hash);
    if (pos < 0)
        fatal("Attempted to load tx without first checking existence with io_block_of_tx()");

    io_load_nth_tx_raw(block_hash, pos, dst);
    free(block_hash);
}

// Util


int io_block_of_tx(uint8_t *src_tx_hash, uint8_t *dst_block_hash)
{
    DIR *d;
    struct dirent *dir;
    int pos = -1;
    d = opendir(DIR_TXINDEX);
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            char *path = m_strconcat(7, getenv("HOME"),"/",CONFIG_DIR_FREECOIN,"/",DIR_TXINDEX,"/",dir->d_name);
            
            pos = intern_find_hash(src_tx_hash, path);
            if (pos >= 0)
            {
                memcpy(dst_block_hash, dir->d_name, SIZE_BLOCK_HASH); // TODO does this work correctly?
                break;
            }
            
            free(path);
        }
        closedir(d);
    }
    return pos;
}

/**********
 * BLOCKS *
 **********/

void io_load_block_raw(uint8_t *block_hash, uint8_t *dst)
{
    char block_hash_ascii[SIZE_BLOCK_HASH*2+1];
    bytes_to_hexstr(block_hash_ascii, block_hash, SIZE_BLOCK_HASH);
    char *block_file_name = m_strconcat(7, getenv("HOME"),"/",CONFIG_DIR_FREECOIN,"/",DIR_BLOCKS,"/",block_hash_ascii);
    
    FILE *f;
    long file_len;

    f = fopen(block_file_name, "rb");
    if (!f)
        fatal("Attempted to open nonexistent block without first checking existence with io_height_of_block()");
    fseek(f, 0, SEEK_END);
    file_len = ftell(f);
    rewind(f);

    fread(dst, file_len, 1, f);
    fclose(f);
    free(block_file_name);
}

void io_save_block(block_t *src)
{
    fatal("Unimplemented: io_save_block");
}
void io_save_block_raw(uint8_t *src)
{
    uint8_t *block_hash = malloc(SIZE_BLOCK_HASH);
    block_raw_compute_hash(src, block_hash);
    
    char block_hash_ascii[SIZE_BLOCK_HASH*2+1];
    bytes_to_hexstr(block_hash_ascii, block_hash, SIZE_BLOCK_HASH);
    
    printf("\nAscii hash: %s\n", block_hash_ascii);////debug
    
    uint64_t block_size = block_raw_compute_size(src);
    
    char *block_file_name = m_strconcat(7, getenv("HOME"),"/",CONFIG_DIR_FREECOIN,"/",DIR_BLOCKS,"/",block_hash_ascii);
    char *txindex_file_name = m_strconcat(7, getenv("HOME"),"/",CONFIG_DIR_FREECOIN,"/",DIR_TXINDEX,"/",block_hash_ascii);
    
    FILE *f;
    f = fopen(block_file_name, "wb");
    
    fwrite(src, block_size, 1, f);
    
    fclose(f);
    
    // Txindex
    uint8_t *hash_buffer = malloc(SIZE_SHA256);
    f = fopen(txindex_file_name, "wb");
    
    uint32_t tx_count = btoui(&src[POS_BLOCK_HEADER_TX_COUNT]);
    int cursor = SIZE_BLOCK_HEADER;
    for (int i=0; i<tx_count; i++)
    {
        tx_raw_compute_hash(&src[cursor], hash_buffer);
        fwrite(hash_buffer, SIZE_SHA256, 1, f);
        
        cursor += tx_raw_compute_size(&src[cursor]);
    }
    fclose(f);
    free(hash_buffer);
    free(block_file_name);
    free(txindex_file_name);
    free(block_hash);
}

// Util

void io_block_at_height(uint32_t pos, uint8_t *dst_hash);
int io_height_of_block(uint8_t *block_hash);