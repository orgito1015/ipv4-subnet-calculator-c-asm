#ifndef SUBNETTING_H
#define SUBNETTING_H

#include <stdint.h>

void task_a(uint32_t ip, uint32_t mask, int prefix);
void task_b(uint32_t network, uint32_t mask, int prefix);
void task_c(uint32_t network, uint32_t mask, int prefix, uint32_t subnets);

#endif
