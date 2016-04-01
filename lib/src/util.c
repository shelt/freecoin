#include "util.h"
#include <arpa/inet.h>

void ustob(uint16_t src, uint8_t *dst)
{
    dst[0] = src >> 8 & 0xff;
    dst[1] = src & 0xff;
}

void uitob(uint32_t src, uint8_t *dst)
{
    dst[0] = src >> 24 & 0xff;
    dst[1] = src >> 16 & 0xff;
    dst[2] = src >> 8 & 0xff;
    dst[3] = src & 0xff;
}

uint16_t btous(uint8_t *src)
{
    return (uint16_t) ((src[0] << 8) | src[1]);
}

uint32_t btoui(uint8_t *src)
{
    return (uint32_t) ((src[0] << 24 |
                 src[1] << 16 |
                 src[2] << 8 |
                 src[3]));
}

uint8_t hexc_to_nibble(char c)
{
    if (c >= '0' && c <= '9')
        return c - '0';
    if (c >= 'a' && c <= 'f')
        return c - 'a' + 10;
    if (c >= 'A' && c <= 'F')
        return c - 'A' + 10;
    else return 255;
}

static char byte_map[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
char nibble_to_hexc(uint8_t n)
{
    if (n < sizeof (byte_map))
        return byte_map[n];
    return '?';
}

// bytearr must have size bytes/2 and (bytes mod 2) of 0
void hexstr_to_bytes(uint8_t *bytearr, char *string, size_t bytes)
{
    int byte_pos, str_pos;
    for (byte_pos = 0, str_pos = 0; byte_pos < bytes; byte_pos++, str_pos += 2)
        bytearr[byte_pos] = (hexc_to_nibble(string[str_pos]) << 4) | (hexc_to_nibble(string[str_pos + 1]));
}

// string must have size 2*bytes+1
void bytes_to_hexstr(char *string, uint8_t *bytes, size_t bytes_count)
{
    int i;
    for (i = 0; i < bytes_count; i++)
    {
        string[i * 2] = nibble_to_hexc(bytes[i] >> 4);
        string[i * 2 + 1] = nibble_to_hexc(bytes[i] & 0x0f);
    }
    string[bytes_count*2] = '\0';
}