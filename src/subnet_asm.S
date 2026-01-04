/* x86-64 System V ABI (Linux/macOS on Intel)
 * GAS via GCC. We use Intel syntax for readability.
 */

.intel_syntax noprefix
.text

.global calc_network
.global calc_broadcast
.global calc_first_host
.global calc_last_host
.global calc_usable_hosts

/* uint32_t calc_network(uint32_t ip, uint32_t mask)
 * args: edi=ip, esi=mask
 */
calc_network:
    mov eax, edi
    and eax, esi
    ret

/* uint32_t calc_broadcast(uint32_t network, uint32_t mask)
 * args: edi=network, esi=mask
 * broadcast = network | ~mask
 */
calc_broadcast:
    mov eax, esi
    not eax
    or eax, edi
    ret

/* uint32_t calc_first_host(uint32_t network, int prefix)
 * args: edi=network, esi=prefix
 * if prefix >= 31, return network (no usable host range)
 * else return network + 1
 */
calc_first_host:
    cmp esi, 31
    jge .fh_nohost
    mov eax, edi
    add eax, 1
    ret
.fh_nohost:
    mov eax, edi
    ret

/* uint32_t calc_last_host(uint32_t broadcast, int prefix)
 * args: edi=broadcast, esi=prefix
 * if prefix >= 31, return broadcast
 * else return broadcast - 1
 */
calc_last_host:
    cmp esi, 31
    jge .lh_nohost
    mov eax, edi
    sub eax, 1
    ret
.lh_nohost:
    mov eax, edi
    ret

/* uint32_t calc_usable_hosts(int prefix)
 * args: edi=prefix
 * usable = 2^(32-prefix) - 2, but for /31 and /32 => 0
 */
calc_usable_hosts:
    cmp edi, 31
    jge .hosts_zero
    mov ecx, 32
    sub ecx, edi          /* host_bits = 32 - prefix */

    mov eax, 1
    shl eax, cl           /* 2^host_bits */
    sub eax, 2
    ret
.hosts_zero:
    xor eax, eax
    ret

.section .note.GNU-stack,"",@progbits
