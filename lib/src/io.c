#include "io.h"
#include "transactions.h"
#include "shared.h"
#include <stdio.h>
#include <dirent.h>

#define SIZE_BLOCK_HASH 32

#define DIR_BLOCKS CONFIG_DIR_DATA ## "/blocks/"
#define DIR_TXINDEX CONFIG_DIR_DATA ## "/txindex/"

#define SIZE_TXINDEX_PATH SIZE_TX_HASH + sizeof(DIR_TXINDEX) + 1
#define SIZE_BLOCKS_PATH SIZE_BLOCK_HASH + sizeof(DIR_BLOCKS) + 1

void intern_get_txindex_fname(uint8_t *hash, uint8_t *dst)
{
    strcpy(dst, DIR_TXINDEX);
    memcpy(&dst[strlen(DIR_TXINDEX)], hash, SIZE_BLOCK_HASH);
    dst[SIZE_TXINDEX_PATH - 1] = "\n";
}

void intern_get_block_fname(uint8_t *block_hash, uint8_t *dst)
{
    strcpy(dst, DIR_BLOCKS);
    memcpy(&dst[strlen(DIR_BLOCKS)], block_hash, SIZE_BLOCK_HASH);
    dst[SIZE_BLOCKS_PATH - 1] = "\n";
}

intern_fread_at(); //TODO

/****************
 * TRANSACTIONS *
 ****************/

void io_load_nth_tx_raw(uint8_t *block_hash, uint32_t n, tx_t *dst)
{
    uint8_t *block_buffer = malloc(MAX_BLOCK_SIZE);
    io_load_block_raw(block_hash);
    
    // Check for sanity
    if (n >= btoui(&block_buffer[POS_BLOCK_TX_COUNT]))
        die("Attempted to use io_load_nth_tx_raw() with n >= tx_count");
    
    uint64_t ins_size,outs_size;
    int cursor = sizeof(block_header_t);
    int tx_cursor = 0;
    while (tx_cursor < n)
    {
        ins_size = sizeof(tx_input_t) * btous(&block_buffer[cursor+POS_TX_HEADER_IN_COUNT]);
        outs_size = sizeof(tx_output_t) * btous(&block_buffer[cursor+POS_TX_HEADER_OUT_COUNT]);
        cursor += sizeof(tx_header_t) + ins_size + outs_size;
    }
    tx_deserialize(cursor, dst);
}

void io_load_tx_raw(uint8_t *dst)
{
    uint8_t block_hash = malloc(SIZE_BLOCK_HASH);
    int pos = io_block_at_height(tx_hash, block_hash);
    if (pos < 0)
        die("Attempted to load tx without first checking existence with io_block_of_tx()");

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
        while ((dir = readdir(d)) != NULL)
        {
            pos = intern_find_hash(dir->d_name);
            if (pos >= 0)
            {
                memcpy(dst_block_hash, dir->d_name, SIZE_BLOCK_HASH);
                break;
            }

        }
        closedir(d);
    }
    fclose(f);
    return pos;
}

/**********
 * BLOCKS *
 **********/

void io_load_block_raw(uint8_t *hash, uint8_t *dst)
{
    char block_file_name[SIZE_BLOCKS_PATH];
    intern_get_block_fname(block_hash, &block_file_name);

    FILE *f;
    long file_len;

    f = fopen(block_file_name, "rb");
    if (!f)
        die("Attempted to open nonexistent block without first checking existence with io_height_of_block()");
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