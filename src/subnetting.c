#include "subnetting.h"
#include "ipv4_utils.h"
#include "subnet_asm.h"

#include <stdio.h>

void task_a(uint32_t ip, uint32_t mask, int prefix)
{
    (void)prefix;
    uint32_t network = calc_network(ip, mask);
    uint32_t broadcast = calc_broadcast(network, mask);

    printf("Network_ID   : ");
    ipv4_print(network);
    printf("\nBroadcast_ID : ");
    ipv4_print(broadcast);
    printf("\n");
}

void task_b(uint32_t network, uint32_t mask, int prefix)
{
    // Ensure it is aligned to mask
    uint32_t aligned = calc_network(network, mask);
    uint32_t broadcast = calc_broadcast(aligned, mask);

    uint32_t first = calc_first_host(aligned, prefix);
    uint32_t last = calc_last_host(broadcast, prefix);
    uint32_t hosts = calc_usable_hosts(prefix);

    printf("Network_ID   : ");
    ipv4_print(aligned);
    printf("/%d\n", prefix);

    printf("IP_first     : ");
    ipv4_print(first);
    printf("\n");

    printf("IP_last      : ");
    ipv4_print(last);
    printf("\n");

    printf("Nr_Hostesh   : %u\n", hosts);
}

static uint32_t ceil_log2_u32(uint32_t x)
{
    // smallest k such that 2^k >= x
    if (x <= 1)
        return 0;
    uint32_t k = 0;
    uint32_t p = 1;
    while (p < x)
    {
        p <<= 1;
        k++;
    }
    return k;
}

void task_c(uint32_t network, uint32_t mask, int prefix, uint32_t subnets)
{
    if (subnets == 0)
    {
        printf("Number of subnets must be > 0\n");
        return;
    }

    uint32_t aligned = calc_network(network, mask);

    uint32_t extra_bits = ceil_log2_u32(subnets);
    int new_prefix = prefix + (int)extra_bits;
    if (new_prefix > 32)
    {
        printf("Cannot create %u subnets from /%d (prefix would exceed /32).\n", subnets, prefix);
        return;
    }

    uint32_t new_mask = mask_from_prefix(new_prefix);

    // Block size = number of addresses in each subnet
    uint32_t host_bits = (new_prefix >= 32) ? 0u : (uint32_t)(32 - new_prefix);
    uint32_t block = (host_bits >= 31) ? 0u : (1u << host_bits);

    // For /0..../32, host_bits is 0..32, but shifting 1 by 32 is UB in C.
    if (new_prefix == 32)
        block = 1u;
    else if (new_prefix == 0)
        block = 0u; // not realistic for this assignment, but kept safe.

    printf("Base Network : ");
    ipv4_print(aligned);
    printf("/%d\n", prefix);

    printf("Subnets asked: %u\n", subnets);
    printf("New Prefix   : /%d\n", new_prefix);
    printf("New Mask     : ");
    ipv4_print(new_mask);
    printf("\n\n");

    // Generate exactly `subnets` subnets (even if not power of two)
    for (uint32_t i = 0; i < subnets; i++)
    {
        uint32_t subnet_id = aligned + i * block;
        uint32_t subnet_net = calc_network(subnet_id, new_mask);
        uint32_t broadcast = calc_broadcast(subnet_net, new_mask);

        uint32_t first = calc_first_host(subnet_net, new_prefix);
        uint32_t last = calc_last_host(broadcast, new_prefix);
        uint32_t hosts = calc_usable_hosts(new_prefix);

        printf("Subnet #%u\n", i + 1);
        printf("  Subnet_ID    : ");
        ipv4_print(subnet_net);
        printf("/%d\n", new_prefix);

        printf("  Broadcast_ID : ");
        ipv4_print(broadcast);
        printf("\n");

        printf("  IP_first     : ");
        ipv4_print(first);
        printf("\n");

        printf("  IP_last      : ");
        ipv4_print(last);
        printf("\n");

        printf("  Nr_Hostesh   : %u\n\n", hosts);
    }
}
