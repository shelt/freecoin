#include "io.h"
#include "transactions.h"
#include "shared.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#define SIZE_BLOCK_HASH 32

#define DIR_BLOCKS CONFIG_DIR_DATA "/blocks/"
#define DIR_TXINDEX CONFIG_DIR_DATA "/txindex/"

#define SIZE_TXINDEX_PATH SIZE_TX_HASH*2 + sizeof(DIR_TXINDEX)*2 + 1
#define SIZE_BLOCKS_PATH SIZE_BLOCK_HASH*2 + sizeof(DIR_BLOCKS)*2 + 1

/*
 * The mechanics of this file, specifically how directories
 * are looped through and how filenames are converted to hash bytearrays
 * and vice versa is very hack. This should be rectified ASAP.
 */

void intern_get_txindex_fname(uint8_t *tx_hash, char *dst)
{
    strcpy(dst, DIR_TXINDEX);
    bytes_to_hexstr(&dst[strlen(DIR_TXINDEX)], tx_hash, SIZE_BLOCK_HASH);
    dst[SIZE_TXINDEX_PATH - 1] = '\0';
}

void intern_get_block_fname(uint8_t *block_hash, char *dst)
{
    strcpy(dst, DIR_BLOCKS);
    bytes_to_hexstr(&dst[strlen(DIR_BLOCKS)], block_hash, SIZE_BLOCK_HASH);
    dst[SIZE_BLOCKS_PATH - 1] = '\0';
}

int intern_find_hash(uint8_t *hash, char *path)
{
    FILE *f;
    uint8_t *hash_buffer = malloc(SIZE_TX_HASH);

    f = fopen(path, "rb");
    if (!f)
        fatal("Attempted to open nonexistent file in intern_find_hash()");
    
    // Size
    fseek(f, 0, SEEK_END);
    int hash_count = ftell(f);
    rewind(f);
    
    int retval = -1;
    for (int i=0; i<hash_count; i++)
    {
        fread(hash_buffer, SIZE_TX_HASH, 1, f);
        if (memcmp(hash, hash_buffer, SIZE_TX_HASH) == 0)
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
    if (n >= btoui(&block_buffer[POS_BLOCK_TX_COUNT]))
        fatal("Attempted to use io_load_nth_tx_raw() with n >= tx_count");
    
    uint64_t ins_size,outs_size;
    int cursor = sizeof(block_header_t);
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

void io_save_tx(tx_t *src);
void io_save_tx_raw(uint8_t *src);

// Util


int io_block_of_tx(uint8_t *src_tx_hash, uint8_t *dst_block_hash)
{
    DIR *d;
    struct dirent *dir;
    int pos = -1;
    d = opendir(DIR_TXINDEX);
    if (d)
    {
        char *path = malloc(SIZE_TXINDEX_PATH + SIZE_BLOCK_HASH + 1);
        while ((dir = readdir(d)) != NULL)
        {
            intern_get_txindex_fname(dir->d_name, path);

            // Hack TODO
            strcpy(path, DIR_TXINDEX);
            memcpy(&path[strlen(DIR_TXINDEX)], dir->d_name, SIZE_BLOCK_HASH*2);
            path[SIZE_TXINDEX_PATH - 1] = '\0';
            
            pos = intern_find_hash(src_tx_hash, path);
            if (pos >= 0)
            {
                memcpy(dst_block_hash, dir->d_name, SIZE_BLOCK_HASH); // TODO does this work correctly?
                break;
            }
        }
        closedir(d);
        free(path);
    }
    return pos;
}

/**********
 * BLOCKS *
 **********/

void io_load_block_raw(uint8_t *block_hash, uint8_t *dst)
{
    char block_file_name[SIZE_BLOCKS_PATH];
    intern_get_block_fname(block_hash, block_file_name);

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
}

void io_save_block(block_t *src);
void io_save_block_raw(uint8_t *src);


// Util

void io_block_at_height(uint32_t pos, uint8_t *dst_hash);
int io_height_of_block(uint8_t *block_hash);