#ifndef SUBNET_ASM_H
#define SUBNET_ASM_H

#include <stdint.h>

// Implemented in x86-64 assembly (GAS via GCC)
uint32_t calc_network(uint32_t ip, uint32_t mask);
uint32_t calc_broadcast(uint32_t network, uint32_t mask);
uint32_t calc_first_host(uint32_t network, int prefix);
uint32_t calc_last_host(uint32_t broadcast, int prefix);
uint32_t calc_usable_hosts(int prefix);

#endif
