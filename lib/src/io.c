#include "io.h"
#include "transactions.h"
#include "shared.h"
#include "util.h"
#include "fs.h"
#include "secrets.h"
#include "sha256.h"
#include "chain.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#define SIZE_SHA256 32

static char *ROOT_FREECOIN;
static char *DIR_BLOCKS;
static char *DIR_HEADS;
static char *DIR_TXINDEX;
static char *DIR_KEYS;

void io_init()
{
    ROOT_FREECOIN   = m_strconcat(2, getenv("HOME"),"/.freecoin/");
    DIR_BLOCKS      = m_strconcat(2, ROOT_FREECOIN, "/blocks/");
    DIR_HEADS       = m_strconcat(2, ROOT_FREECOIN, "/heads/");
    DIR_TXINDEX     = m_strconcat(2, ROOT_FREECOIN, "/txindex/");
    DIR_KEYS        = m_strconcat(2, ROOT_FREECOIN, "/keys/");
}

/*********
 * HEADS *
 *********/

uint32_t io_head_count();

void io_head_load_all(head_t *dst);

void io_head_load(uint8_t *hash, head_t *dst);

void io_head_create(uint8_t *hash);

void io_head_delete(uint8_t *hash);

void io_head_update(head_t *head);


/* HASHLISTS */

int64_t intern_hashlist_getindex(uint8_t *hash, char *path)
{
    FILE *f;
    uint8_t *hash_buffer = malloc(SIZE_SHA256);

    f = fopen(path, "rb");
    if (!f)
        fatal("Failed to open file in intern_find_hash()");
    
    // Size
    fseek(f, 0, SEEK_END);
    int hash_count = ftell(f)/SIZE_SHA256;
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

BOOL intern_hashlist_get(uint64_t height, uint8_t *dst, char *path)
{
    FILE *f;

    f = fopen(path, "rb");
    if (!f)
        return FALSE;
    
    fseek(f, height*SIZE_SHA256, SEEK_SET);
    
    fread(dst,SIZE_SHA256,1,f);
    
    fclose(f);
    return TRUE;
}

BOOL intern_hashlist_add(uint8_t *hash, char *path)
{
    FILE *f = fopen(path, "ab");
    if (!f)
        return FALSE;
    fwrite(hash,SIZE_SHA256,1,f);
    
    fclose(f);
    return TRUE;
}

/****************
 * TRANSACTIONS *
 ****************/

tx_t *m_io_load_tx(uint8_t *hash)
{
    tx_t *tx = NULL;
    uint8_t *buffer = malloc(MAX_TX_SIZE);
    if (io_load_tx_raw(hash, buffer))
        tx = m_tx_deserialize(buffer);
    
    free(buffer);
    return tx;
}

BOOL io_load_tx_raw(uint8_t *tx_hash_src, uint8_t *dst)
{
    uint8_t block_hash[SIZE_SHA256];
    int pos = io_block_of_tx(tx_hash_src, block_hash);
    if (pos < 0)
        return FALSE;

    io_load_nth_tx_raw(block_hash, pos, dst);
    return TRUE;
}

// Util

BOOL io_load_nth_tx_raw(uint8_t *block_hash, uint32_t n, uint8_t *dst)
{
    uint8_t block_buffer[MAX_BLOCK_SIZE];
    if (!io_load_block_raw(block_hash, block_buffer))
        return FALSE;
    
    // Check for sanity
    if (n >= btoui(&block_buffer[POS_BLOCK_HEADER_TX_COUNT]))
        fatal("Attempted to use io_load_nth_tx_raw() with n >= tx_count");
    
    uint64_t ins_size,outs_size;
    int cursor = SIZE_BLOCK_HEADER;
    int tx_cursor = 0;
    while (tx_cursor < n)
        cursor += tx_raw_compute_size(&block_buffer[cursor]);
    
    memcpy(dst, &block_buffer[cursor], tx_raw_compute_size(&block_buffer[cursor]));
    
    return TRUE;
}

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
            char *path = m_strconcat(3, DIR_TXINDEX,"/",dir->d_name);
            
            pos = intern_hashlist_getindex(src_tx_hash, path);
            if (pos >= 0)
            {
                memcpy(dst_block_hash, dir->d_name, SIZE_SHA256); // TODO does this work correctly?
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

BOOL io_load_block_header(uint8_t *hash, block_header_t *dst)
{
    BOOL retval = FALSE;
    uint8_t *buffer = malloc(MAX_BLOCK_SIZE);
    if (io_load_block_raw(hash, buffer))
    {
        block_header_deserialize(buffer, dst);
        retval = TRUE;
    }
    free(buffer);
    return retval;
}

block_t *m_io_load_block(uint8_t *hash)
{
    block_t *block = NULL;
    uint8_t *buffer = malloc(MAX_BLOCK_SIZE);
    if (io_load_block_raw(hash, buffer))
        block = m_block_deserialize(buffer);
    
    free(buffer);
    return block;
}

BOOL io_load_block_raw(uint8_t *block_hash, uint8_t *dst)
{
    BOOL retval = FALSE;
    char block_hash_ascii[SIZE_SHA256*2+1];
    btoascii(block_hash_ascii, block_hash, SIZE_SHA256);
    char *block_file_name = m_strconcat(3, DIR_BLOCKS,"/",block_hash_ascii);
    
    FILE *f;
    long file_len;

    f = fopen(block_file_name, "rb");
    if (f)
    {
        fseek(f, 0, SEEK_END);
        file_len = ftell(f);
        rewind(f);
        
        fread(dst, file_len, 1, f);
        fclose(f);
        retval = TRUE;
    }
    free(block_file_name);
    return retval;
}

void io_save_block(block_t *src)
{
    fatal("Unimplemented: io_save_block");
}
void io_save_block_raw(uint8_t *src)
{
    uint8_t *block_hash = malloc(SIZE_SHA256);
    block_raw_compute_hash(src, block_hash);
    
    char block_hash_ascii[SIZE_SHA256*2+1];
    btoascii(block_hash_ascii, block_hash, SIZE_SHA256);
    
    uint64_t block_size = block_raw_compute_size(src);
    
    char *block_file_name = m_strconcat(3, DIR_BLOCKS,"/",block_hash_ascii);
    char *txindex_file_name = m_strconcat(3, DIR_TXINDEX,"/",block_hash_ascii);
    
    FILE *f;
    f = fopen(block_file_name, "wb");
    if (!f)
        fatal("Failed to open file in io_save_block_raw()");
    
    fwrite(src, block_size, 1, f);
    
    fclose(f);
    
    /* Txindex */
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
    
    block_t *block = m_block_deserialize(src);
    enchain_block(block);
    
    m_free_block(block);
    free(hash_buffer);
    free(block_file_name);
    free(txindex_file_name);
    free(block_hash);
}

/**********
 *  KEYS  *
 **********/

void io_store_key(uint8_t *pub, uint8_t *priv)
{
    uint8_t addr[SIZE_SHA256] = {0};
    crypt_addr(pub, addr);
    char addr_hexascii[SIZE_SHA256*2+1];
    char addr_64ascii[base64_encoded_size(SIZE_SHA256)+1];
    base64_encode(addr, addr_64ascii, SIZE_SHA256);
    btoascii(addr_hexascii, addr, SIZE_SHA256);
    
    printf("New key: %s\n", addr_64ascii);
    char *addr_dir = m_strconcat(4, DIR_KEYS,"/",addr_hexascii,"/");
    
    char *private_file_name = m_strconcat(2, addr_dir,"/private");
    char *public_file_name  = m_strconcat(2, addr_dir,"/public");
    
    struct stat st = {0};
    if (stat("/some/directory", &st) != -1)
        fatal("Attempted to store existing key");
    mkdir(addr_dir, 0700);
    
    FILE *f;
    f = fopen(private_file_name, "wb");
    fwrite(priv, SIZE_ECDSA_PRIVATE, 1, f);
    fclose(f);
    
    f = fopen(public_file_name, "wb");
    fwrite(pub, SIZE_ECDSA_PUBLIC, 1, f);
    fclose(f);
    
    free(addr_dir);
    free(private_file_name);
    free(public_file_name);
}