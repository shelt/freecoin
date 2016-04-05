#include "util.h"
#include <arpa/inet.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>


static char base64_encoding_table[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
    'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
    'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
    'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
    'w', 'x', 'y', 'z', '0', '1', '2', '3',
    '4', '5', '6', '7', '8', '9', '+', '/'};
static char *base64_decoding_table = NULL;
static int base64_mod_table[] = {0, 2, 1};

void util_init()
{
    // Base 64
    base64_decoding_table = malloc(256);
    for (int i = 0; i < 64; i++)
        base64_decoding_table[(unsigned char) base64_encoding_table[i]] = i;
}

void swap_bo(uint8_t *arr, uint32_t len)
{
    uint8_t *tmp = malloc(len);
    for (int i = 0; i < len; i++)
        tmp[len - 1 - i] = arr[i];
    memcpy(arr, tmp, len);
    free(tmp);
}

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

void asciitob(uint8_t *bytearr, char *string, size_t bytes)
{
    int byte_pos, str_pos;
    for (byte_pos = 0, str_pos = 0; byte_pos < bytes; byte_pos++, str_pos += 2)
        bytearr[byte_pos] = (hexc_to_nibble(string[str_pos]) << 4) | (hexc_to_nibble(string[str_pos + 1]));
}

// string must have size 2*bytes+1

void btoascii(char *string, uint8_t *bytes, size_t bytes_count)
{
    int i;
    for (i = 0; i < bytes_count; i++)
    {
        string[i * 2] = nibble_to_hexc(bytes[i] >> 4);
        string[i * 2 + 1] = nibble_to_hexc(bytes[i] & 0x0f);
    }
    string[bytes_count * 2] = '\0';
}

/* Base 64 */

uint32_t base64_encoded_size(uint32_t bytes)
{
    return 4 * ((bytes + 2) / 3);
}
uint32_t base64_decoded_size(uint32_t strlen)
{
    return strlen / 4 * 3;
}

void base64_encode(const uint8_t *data, char *dst, uint32_t bytes)
{
    uint32_t output_length = base64_encoded_size(bytes);

    for (int i = 0, j = 0; i < bytes;)
    {

        uint32_t octet_a = i < bytes ? (uint8_t) data[i++] : 0;
        uint32_t octet_b = i < bytes ? (uint8_t) data[i++] : 0;
        uint32_t octet_c = i < bytes ? (uint8_t) data[i++] : 0;

        uint32_t triple = (octet_a << 0x10) + (octet_b << 0x08) + octet_c;

        dst[j++] = base64_encoding_table[(triple >> 3 * 6) & 0x3F];
        dst[j++] = base64_encoding_table[(triple >> 2 * 6) & 0x3F];
        dst[j++] = base64_encoding_table[(triple >> 1 * 6) & 0x3F];
        dst[j++] = base64_encoding_table[(triple >> 0 * 6) & 0x3F];
    }

    for (int i = 0; i < base64_mod_table[bytes % 3]; i++)
        dst[output_length - 1 - i] = '=';
}

void base64_decode(const char *ascii, uint8_t *dst, uint32_t bytes)
{
    uint32_t input_length = base64_encoded_size(bytes);
    if (input_length % 4 != 0) return;

    if (ascii[input_length - 1] == '=') bytes--;
    if (ascii[input_length - 2] == '=') bytes--;
    
    for (int i = 0, j = 0; i < input_length;)
    {
        uint32_t sextet_a = ascii[i] == '=' ? 0 & i++ : base64_decoding_table[(uint8_t)ascii[i++]];
        uint32_t sextet_b = ascii[i] == '=' ? 0 & i++ : base64_decoding_table[(uint8_t)ascii[i++]];
        uint32_t sextet_c = ascii[i] == '=' ? 0 & i++ : base64_decoding_table[(uint8_t)ascii[i++]];
        uint32_t sextet_d = ascii[i] == '=' ? 0 & i++ : base64_decoding_table[(uint8_t)ascii[i++]];

        uint32_t triple = (sextet_a << 3 * 6)
                + (sextet_b << 2 * 6)
                + (sextet_c << 1 * 6)
                + (sextet_d << 0 * 6);

        if (j < bytes) dst[j++] = (triple >> 2 * 8) & 0xFF;
        if (j < bytes) dst[j++] = (triple >> 1 * 8) & 0xFF;
        if (j < bytes) dst[j++] = (triple >> 0 * 8) & 0xFF;
    }
}


//TODO this doesn't belong here.

uint32_t get_net_time()
{
    return time(NULL); //TODO make use network times from peers
}