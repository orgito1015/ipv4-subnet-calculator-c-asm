# IPv4 Subnet Calculator (C + Assembly)

A practical IPv4 subnet calculator for TCP/IP v4 addressing exercises.

This project uses a **hybrid approach**:

- **C**: parsing IPv4 / subnet masks, validation, menu UI, printing.
- **x86-64 Assembly (GAS via GCC)**: core bitwise math for:
  - Network ID
  - Broadcast ID
  - First/Last usable host
  - Usable host count

## Features (Assignment Coverage)

### A) Given **IP + Subnet Mask**
Prints:
- `Network_ID`
- `Broadcast_ID`

### B) Given **Network_ID + Subnet Mask**
Prints:
- `IP_first` (first usable)
- `IP_last` (last usable)
- `Nr_Hostesh` (usable hosts)

### C) Given **Network_ID + Subnet Mask + number of subnets**
Generates subnets and prints for each subnet:
- `Subnet_ID`
- `Broadcast_ID`
- `IP_first`
- `IP_last`
- `Nr_Hostesh`

### D) Extras
- IPv4 → binary conversion
- IPv4 address type (private/public/loopback/multicast/broadcast/reserved)

---

## Build

### Linux (recommended)
```bash
make
./ipv4_calc
```

> Requirements: `gcc` and standard build tools (no NASM needed).

### Clean
```bash
make clean
```

---

## Usage
The program is interactive. You can input:
- IPv4 address: `192.168.1.10`
- Subnet mask: `255.255.255.0` **or** CIDR: `/24`

---

## Medium article (by Orgito)
  - https://medium.com/@tech_with_orgito_/ipv4-calculator-program-written-in-c-885b363328db

---

## License
MIT (feel free to change).
