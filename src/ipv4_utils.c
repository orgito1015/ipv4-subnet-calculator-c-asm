#include "ipv4_utils.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static bool parse_u8(const char *s, int *consumed, uint8_t *out)
{
    // Parse 0..255 (no leading +/-)
    int i = 0;
    if (!isdigit((unsigned char)s[i]))
        return false;

    int val = 0;
    int digits = 0;
    while (isdigit((unsigned char)s[i]) && digits < 3)
    {
        val = val * 10 + (s[i] - '0');
        i++;
        digits++;
    }
    if (digits == 0 || val > 255)
        return false;

    *consumed = i;
    *out = (uint8_t)val;
    return true;
}

bool ipv4_parse(const char *s, uint32_t *out)
{
    if (!s || !out)
        return false;

    uint8_t a, b, c, d;
    int n = 0;

    if (!parse_u8(s, &n, &a))
        return false;
    s += n;
    if (*s++ != '.')
        return false;

    if (!parse_u8(s, &n, &b))
        return false;
    s += n;
    if (*s++ != '.')
        return false;

    if (!parse_u8(s, &n, &c))
        return false;
    s += n;
    if (*s++ != '.')
        return false;

    if (!parse_u8(s, &n, &d))
        return false;
    s += n;
    if (*s != '\0')
        return false;

    *out = ((uint32_t)a << 24) | ((uint32_t)b << 16) | ((uint32_t)c << 8) | (uint32_t)d;
    return true;
}

uint32_t mask_from_prefix(int prefix)
{
    if (prefix <= 0)
        return 0u;
    if (prefix >= 32)
        return 0xFFFFFFFFu;

    return 0xFFFFFFFFu << (32 - prefix);
}

bool mask_validate(uint32_t mask, int *prefix_out)
{
    // mask must be contiguous: 111..1100..00
    // A valid mask has the property: mask & (mask + 1) == 0? Not exactly.
    // Better: invert mask => host bits, must be of form 00..0011..11
    uint32_t inv = ~mask;

    // inv must be contiguous ones at LSB: inv & (inv + 1) == 0
    if ((inv & (inv + 1)) != 0)
        return false;

    // prefix = number of 1 bits in mask
    int prefix = 0;
    for (int i = 31; i >= 0; i--)
    {
        if (mask & (1u << i))
            prefix++;
        else
            break;
    }

    if (prefix_out)
        *prefix_out = prefix;
    return true;
}

static bool parse_cidr(const char *s, int *prefix_out)
{
    // Accept "24" or "/24"
    if (!s || !prefix_out)
        return false;

    if (*s == '/')
        s++;

    if (!isdigit((unsigned char)*s))
        return false;

    char *end = NULL;
    long v = strtol(s, &end, 10);
    if (!end || *end != '\0')
        return false;
    if (v < 0 || v > 32)
        return false;

    *prefix_out = (int)v;
    return true;
}

bool mask_parse(const char *s, uint32_t *mask_out, int *prefix_out)
{
    if (!s || !mask_out || !prefix_out)
        return false;

    // Try CIDR first
    int p = 0;
    if (parse_cidr(s, &p))
    {
        *prefix_out = p;
        *mask_out = mask_from_prefix(p);
        return true;
    }

    // Otherwise dotted mask
    uint32_t mask = 0;
    if (!ipv4_parse(s, &mask))
        return false;

    if (!mask_validate(mask, &p))
        return false;

    *mask_out = mask;
    *prefix_out = p;
    return true;
}

void ipv4_to_str(uint32_t ip, char out[16])
{
    uint8_t a = (ip >> 24) & 0xFF;
    uint8_t b = (ip >> 16) & 0xFF;
    uint8_t c = (ip >> 8) & 0xFF;
    uint8_t d = ip & 0xFF;
    snprintf(out, 16, "%u.%u.%u.%u", a, b, c, d);
}

void ipv4_print(uint32_t ip)
{
    char buf[16];
    ipv4_to_str(ip, buf);
    printf("%s", buf);
}

void ipv4_print_bin(uint32_t ip)
{
    for (int oct = 0; oct < 4; oct++)
    {
        uint8_t v = (ip >> (24 - 8 * oct)) & 0xFF;
        for (int i = 7; i >= 0; i--)
            putchar((v & (1u << i)) ? '1' : '0');
        if (oct != 3)
            putchar('.');
    }
}

const char *ipv4_type(uint32_t ip)
{
    uint8_t a = (ip >> 24) & 0xFF;
    uint8_t b = (ip >> 16) & 0xFF;

    // Special ranges
    if (a == 10)
        return "Private (10.0.0.0/8)";
    if (a == 172 && (b >= 16 && b <= 31))
        return "Private (172.16.0.0/12)";
    if (a == 192 && b == 168)
        return "Private (192.168.0.0/16)";
    if (a == 127)
        return "Loopback (127.0.0.0/8)";
    if (a >= 224 && a <= 239)
        return "Multicast (224.0.0.0/4)";
    if (ip == 0xFFFFFFFFu)
        return "Limited Broadcast (255.255.255.255)";
    if (a == 0)
        return "Reserved (0.0.0.0/8)";
    if (a >= 240)
        return "Reserved (240.0.0.0/4)";

    return "Public";
}
