#include "ipv4_utils.h"
#include "subnetting.h"

#include <stdio.h>
#include <string.h>

static void read_line(const char *prompt, char *buf, size_t n)
{
    printf("%s", prompt);
    fflush(stdout);
    if (!fgets(buf, (int)n, stdin))
    {
        buf[0] = '\0';
        return;
    }
    // trim newline
    size_t len = strlen(buf);
    if (len && (buf[len - 1] == '\n' || buf[len - 1] == '\r'))
        buf[len - 1] = '\0';
    len = strlen(buf);
    if (len && (buf[len - 1] == '\r'))
        buf[len - 1] = '\0';
}

static int menu(void)
{
    printf("\nIPv4 Subnet Calculator (C + Assembly)\n");
    printf("====================================\n");
    printf("1) Task A: IP + SubnetMask -> Network_ID, Broadcast_ID\n");
    printf("2) Task B: Network_ID + SubnetMask -> First/Last/Hosts\n");
    printf("3) Task C: Network_ID + SubnetMask + #Subnets -> Generate\n");
    printf("4) Extra: IPv4 to Binary\n");
    printf("5) Extra: IPv4 Type\n");
    printf("0) Exit\n");
    printf("------------------------------------\n");
    printf("Choose: ");

    int c = -1;
    if (scanf("%d", &c) != 1)
        return -1;

    // consume leftover newline
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF)
        ;

    return c;
}

static bool read_ip_and_mask(uint32_t *ip_out, uint32_t *mask_out, int *prefix_out, const char *ip_label)
{
    char ip_s[64];
    char mask_s[64];

    read_line(ip_label, ip_s, sizeof(ip_s));
    read_line("Enter Subnet Mask (e.g. 255.255.255.0 or /24): ", mask_s, sizeof(mask_s));

    uint32_t ip = 0;
    uint32_t mask = 0;
    int prefix = 0;

    if (!ipv4_parse(ip_s, &ip))
    {
        printf("Invalid IPv4: %s\n", ip_s);
        return false;
    }

    if (!mask_parse(mask_s, &mask, &prefix))
    {
        printf("Invalid subnet mask: %s\n", mask_s);
        return false;
    }

    *ip_out = ip;
    *mask_out = mask;
    *prefix_out = prefix;
    return true;
}

int main(void)
{
    while (1)
    {
        int choice = menu();
        if (choice < 0)
        {
            printf("Input error. Exiting.\n");
            return 1;
        }

        if (choice == 0)
            break;

        if (choice == 1)
        {
            uint32_t ip, mask;
            int prefix;
            if (read_ip_and_mask(&ip, &mask, &prefix, "Enter IP address: "))
            {
                printf("\n--- Task A ---\n");
                task_a(ip, mask, prefix);
            }
        }
        else if (choice == 2)
        {
            uint32_t net, mask;
            int prefix;
            if (read_ip_and_mask(&net, &mask, &prefix, "Enter Network_ID: "))
            {
                printf("\n--- Task B ---\n");
                task_b(net, mask, prefix);
            }
        }
        else if (choice == 3)
        {
            uint32_t net, mask;
            int prefix;
            if (read_ip_and_mask(&net, &mask, &prefix, "Enter Network_ID: "))
            {
                char sn[64];
                read_line("Enter number of subnets to generate: ", sn, sizeof(sn));
                unsigned long v = 0;
                if (sscanf(sn, "%lu", &v) != 1 || v == 0 || v > 1000000)
                {
                    printf("Invalid number of subnets.\n");
                }
                else
                {
                    printf("\n--- Task C ---\n");
                    task_c(net, mask, prefix, (uint32_t)v);
                }
            }
        }
        else if (choice == 4)
        {
            char ip_s[64];
            uint32_t ip;
            read_line("Enter IP address: ", ip_s, sizeof(ip_s));
            if (!ipv4_parse(ip_s, &ip))
            {
                printf("Invalid IPv4.\n");
            }
            else
            {
                printf("Binary: ");
                ipv4_print_bin(ip);
                printf("\n");
            }
        }
        else if (choice == 5)
        {
            char ip_s[64];
            uint32_t ip;
            read_line("Enter IP address: ", ip_s, sizeof(ip_s));
            if (!ipv4_parse(ip_s, &ip))
            {
                printf("Invalid IPv4.\n");
            }
            else
            {
                printf("Type: %s\n", ipv4_type(ip));
            }
        }
        else
        {
            printf("Unknown option.\n");
        }
    }

    printf("Bye!\n");
    return 0;
}
