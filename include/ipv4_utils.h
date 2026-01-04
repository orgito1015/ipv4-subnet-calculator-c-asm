#ifndef IPV4_UTILS_H
#define IPV4_UTILS_H

#include <stdbool.h>
#include <stdint.h>

// Parses dotted-decimal IPv4 (e.g. 192.168.1.1) into a 32-bit integer (host order).
bool ipv4_parse(const char *s, uint32_t *out);

// Parses subnet mask input:
// - dotted mask (255.255.255.0)
// - or CIDR (/24 or 24)
// Returns mask (32-bit) and prefix length.
bool mask_parse(const char *s, uint32_t *mask_out, int *prefix_out);

// Converts prefix length to 32-bit mask.
uint32_t mask_from_prefix(int prefix);

// Validates that a 32-bit mask is contiguous (ones then zeros) and returns prefix.
bool mask_validate(uint32_t mask, int *prefix_out);

// Pretty printing
void ipv4_to_str(uint32_t ip, char out[16]);
void ipv4_print(uint32_t ip);
void ipv4_print_bin(uint32_t ip);

// Address classification
const char *ipv4_type(uint32_t ip);

#endif
