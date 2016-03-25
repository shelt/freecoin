#include "util.h"
#include <arpa/inet.h>

void ustob(uint16_t src, uint8_t *dst)
{
    dst[0] = src >> 8 & 0xff;
    dst[1] = src      & 0xff;
}

void uitob(uint32_t src, uint8_t *dst)
{
    dst[0] = src >> 24 & 0xff;
    dst[1] = src >> 16 & 0xff;
    dst[2] = src >> 8  & 0xff;
    dst[3] = src       & 0xff;
}

uint16_t btous(uint8_t *src)
{
    return ntohs((uint16_t)((src[0] << 8) | src[1]));
}

uint32_t btoui(uint8_t *src)
{
    return ntohl((uint32_t)((src[0] << 24 |
                             src[1] << 16 |
                             src[2] << 8  |
                             src[3])));
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
char nibble_to_hexc(uint8_t n)
{
    if (n >= 0 && n <= 9)
        return n + '0';
    else
        return n + 'a' - 10;
}

// bytearr must have size bytes/2 and (bytes mod 2) of 0
void hexstr_to_bytes(uint8_t *bytearr, char *string, size_t bytes)
{
    int byte_pos, str_pos;
    for(byte_pos=0, str_pos=0; byte_pos<bytes; byte_pos++, str_pos+=2)
        bytearr[byte_pos] = (hexc_to_nibble(string[str_pos]) << 4) | (hexc_to_nibble(string[str_pos+1]));
}
// string must have size 2*bytes+1
void bytes_to_hexstr(char *string, uint8_t *bytearr, size_t bytes)
{
    int byte_pos, str_pos;
    for(byte_pos=0, str_pos=0; byte_pos<bytes; byte_pos++, str_pos+=2)
    {
        string[str_pos  ] = nibble_to_hexc(bytearr[byte_pos] >> 4);
        string[str_pos+1] = nibble_to_hexc(bytearr[byte_pos]);
    }
}