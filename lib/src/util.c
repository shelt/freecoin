#include "util.h"

void fatal(const char *string, ...)
{
    va_list args;
    va_start(args, string);

    printf("Fatal: ");
    vfprintf (stdout, string, args);
    printf("\n");
    
    exit(1);
};

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