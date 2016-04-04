#include "bignum.h"
#include "shared.h"
#include <string.h>
#include <arpa/inet.h>

/*
 * Math is all little endian.
 * Note that source bignums are not preserved
 * during arithmetic.
 */


#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

void big_uitobig(uint32_t src, bignum_t dst)
{
    memset(dst.data, 0x00, dst.size);
    
    dst.data[0] = src & 0xff;
    dst.data[1] = src >> 8 & 0xff;
    dst.data[2] = src >> 16 & 0xff;
    dst.data[3] = src >> 24 & 0xff;
}

uint32_t big_bigtoui(bignum_t src)
{
    return        (src.data[0]       |
                   src.data[1] << 8  |
                   src.data[2] << 16 |
                   src.data[3] << 24);
}
//TODO verify this revised version is correct
uint32_t big_length(bignum_t a)
{
    int i = a.size-1;
    int z = 0;
    while (i > 0)
    {
        if (a.data[i] != 0x00)
            return a.size - z;
        z++;
        i--;
    }
    return i;
}

void big_add(bignum_t a, bignum_t b)
{
    if (big_comp(a, b) > 0)
    {
        big_add(b,a);
        memcpy(b.data,a.data,b.size);
        return;
    }
    uint32_t i;
    uint32_t t = 0;
    uint32_t alen = big_length(a);
    uint32_t blen = big_length(b);
    uint32_t count = MIN(alen, blen);
    for (i = 0; i < count; i++)
    {
        t += a.data[i] + b.data[i];
        b.data[i] = t & 0xff;
        t >>= 8;
    }
    i = count;
    while (i < blen && t != 0)
    {
        t += b.data[i];
        b.data[i] = t & 0xff;
        t >>= 8;
        i += 1;
    }
}

// Note that the len(res) must be len(a)+len(b)
void big_mult(bignum_t a, bignum_t b, bignum_t res)
{
    memset(res.data, 0x00, res.size);
    uint32_t t = 0;
    uint32_t alen = big_length(a);
    uint32_t blen = big_length(b);
    uint32_t count1 = alen + blen;
    uint32_t i1, i2, count2;
    for (int i = 0; i < count1; i++)
    {
        i2 = (i < blen - 1) ? i : blen - 1;
        i1 = i - i2;
        count2 = (alen - i1 < i2 + 1) ? alen - i1 : i2 + 1;
        for (int j = 0; j < count2; j++)
            t += a.data[i1 + j] * b.data[i2 - j];

        res.data[i] = t & 0xff;
        t >>= 8;
    }
}

void big_div(bignum_t a, uint32_t b, bignum_t res)
{
    memset(res.data, 0x00, res.size);
    uint16_t temp = 0;
    for (uint8_t x = a.size - 1;; x--)
    {
        temp <<= 8;
        temp |= a.data[x];
        res.data[x] = temp / b;
        temp -= res.data[x] * b;
        if (!x)
            break;
    }
    a.size -= res.data[a.size - 1] ? 0 : 1; // If last byte is zero, adjust length.
}


int big_comp(bignum_t a, bignum_t b)
{
    if (a.size != b.size)
        fatal("Attempted to big_comp() different sized numbers");
    for (int i=a.size-1; i>0; i--)
    {
        if (a.data[i] < b.data[i])
            return -1;
        if (a.data[i] > b.data[i])
            return 1;
    }
    return 0;
}

void target_to_btarget(uint8_t *target, bignum_t btarget)
{
    if (btarget.size != 32)
        fatal("Attempted to store btarget in a bignum of a size other than 32 bytes");
    btarget.data[target[1]-1] = target[0];
}
void btarget_to_target(bignum_t btarget, uint8_t *target)
{
    if (btarget.size != 32)
        fatal("Attempted to extract target from a bignum of a size other than 32 bytes");
    // Identical to loop in big_length()
    int i = 31;
    int z = 0;
    while (i > 0)
    {
        if (btarget.data[i] == 0x00)
        {
            target[1] = 32 - z;
            target[0] = btarget.data[target[1]-1];
            return;
        }
        i--;
        z++;
    }
}