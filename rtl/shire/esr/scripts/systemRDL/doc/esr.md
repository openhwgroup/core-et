<!---
Markdown description for SystemRDL register map.

Don't override. Generated from: Erbium_ESR
  - esr.rdl
-->

## Erbium_ESR address map

- Absolute Address: 0x0
- Base Offset: 0x0
- Size: 0xF40318

| Offset |  Identifier  |   Name   |
|--------|--------------|----------|
|0x100000|  User_neigh  |  U_neigh |
|0x340000|   User_cpu   |   U_cpu  |
|0x740000|Supervisor_cpu|   S_cpu  |
|0x800000|  D_hart_esr  |D_hart_esr|
|0x900000|    D_neigh   |  D_neigh |
|0xB5F000|     D_cpu    |   D_cpu  |
|0xD00000| Machine_neigh|  M_neigh |
|0xF40000|  Machine_cpu |   M_cpu  |

## User_neigh address map

- Absolute Address: 0x100000
- Base Offset: 0x100000
- Size: 0x48

|Offset|   Identifier  |      Name     |
|------|---------------|---------------|
| 0x40 |ipi_redirect_pc|ipi_redirect_pc|

### ipi_redirect_pc register

- Absolute Address: 0x100040
- Base Offset: 0x40
- Size: 0x8

|Bits|   Identifier  |Access|Reset|Name|
|----|---------------|------|-----|----|
|47:0|ipi_redirect_pc|  rw  | 0x0 |  — |

#### ipi_redirect_pc field

<p>In bits 47:0 of this register, user-level software writes the 48 least significant bits of a target virtual PC of a redirect IPI for all the 
harts in a given neighborhood.Also, bit 0 of the PC is always 0</p>

## User_cpu address map

- Absolute Address: 0x340000
- Base Offset: 0x340000
- Size: 0x300

|Offset|     Identifier     |        Name        |
|------|--------------------|--------------------|
| 0x080|ipi_redirect_trigger|ipi_redirect_trigger|
| 0x0C0|      CREDINC0      |      CREDINC0      |
| 0x0C8|      CREDINC1      |      CREDINC1      |
| 0x0D0|      CREDINC2      |      CREDINC2      |
| 0x0D8|      CREDINC3      |      CREDINC3      |
| 0x100| fast_local_barrier0| fast_local_barrier0|
| 0x108| fast_local_barrier1| fast_local_barrier1|
| 0x110| fast_local_barrier2| fast_local_barrier2|
| 0x118| fast_local_barrier3| fast_local_barrier3|
| 0x120| fast_local_barrier4| fast_local_barrier4|
| 0x128| fast_local_barrier5| fast_local_barrier5|
| 0x130| fast_local_barrier6| fast_local_barrier6|
| 0x138| fast_local_barrier7| fast_local_barrier7|
| 0x140| fast_local_barrier8| fast_local_barrier8|
| 0x148| fast_local_barrier9| fast_local_barrier9|
| 0x150|fast_local_barrier10|fast_local_barrier10|
| 0x158|fast_local_barrier11|fast_local_barrier11|
| 0x160|fast_local_barrier12|fast_local_barrier12|
| 0x168|fast_local_barrier13|fast_local_barrier13|
| 0x170|fast_local_barrier14|fast_local_barrier14|
| 0x178|fast_local_barrier15|fast_local_barrier15|
| 0x180|fast_local_barrier16|fast_local_barrier16|
| 0x188|fast_local_barrier17|fast_local_barrier17|
| 0x190|fast_local_barrier18|fast_local_barrier18|
| 0x198|fast_local_barrier19|fast_local_barrier19|
| 0x1A0|fast_local_barrier20|fast_local_barrier20|
| 0x1A8|fast_local_barrier21|fast_local_barrier21|
| 0x1B0|fast_local_barrier22|fast_local_barrier22|
| 0x1B8|fast_local_barrier23|fast_local_barrier23|
| 0x1C0|fast_local_barrier24|fast_local_barrier24|
| 0x1C8|fast_local_barrier25|fast_local_barrier25|
| 0x1D0|fast_local_barrier26|fast_local_barrier26|
| 0x1D8|fast_local_barrier27|fast_local_barrier27|
| 0x1E0|fast_local_barrier28|fast_local_barrier28|
| 0x1E8|fast_local_barrier29|fast_local_barrier29|
| 0x1F0|fast_local_barrier30|fast_local_barrier30|
| 0x1F8|fast_local_barrier31|fast_local_barrier31|
| 0x2F8|  icache_uprefetch  |  icache_uprefetch  |

### ipi_redirect_trigger register

- Absolute Address: 0x340080
- Base Offset: 0x80
- Size: 0x8

|Bits|     Identifier     |Access|Reset|Name|
|----|--------------------|------|-----|----|
|15:0|ipi_redirect_trigger|  rw  |  —  |  — |

#### ipi_redirect_trigger field

<p>User-level software writes a bitmask to this register to request an IPI redirect to all harts indicated by the mask. Before delivery, the bitmask is unconditionally ANDed with the value in IPI_REDIRECT_FILTER. The register is encoded as follows:
Bit 0: Minion 0, hart 0
Bit 1: Minion 0, hart 1
Bit 2: Minion 1, hart 0
Bit 3: Minion 1, hart 1
…
Bit 12: Minion 6, hart 0
Bit 13: Minion 6, hart 1
Bit 14: Minion 7, hart 0
Bit 15: Minion 7, hart 1
When a redirect IPI is delivered to a hart, that hart must satisfy two conditions, It must be executing in user mode. It must be capable of blocking on a stall CSR read instruction.If either condition is violated, 
the hart must take a trap into M-mode with, mcause set to Bad IPI redirect interrupt (5'h10), mtval set to the redirection PC.
Reading this register returns a 0x0 value</p>

### CREDINC0 register

- Absolute Address: 0x3400C0
- Base Offset: 0xC0
- Size: 0x8

|Bits|Identifier|Access|Reset|Name|
|----|----------|------|-----|----|
| 7:0|   mask0  |   r  |  —  |  — |

#### mask0 field

<p>Writing this register increments the COUNTER0 credit counter for the first hart of all the cores selected in MASK0. Reading this register returns a value of 0.</p>

### CREDINC1 register

- Absolute Address: 0x3400C8
- Base Offset: 0xC8
- Size: 0x8

|Bits|Identifier|Access|Reset|Name|
|----|----------|------|-----|----|
| 7:0|   mask1  |   r  |  —  |  — |

#### mask1 field

<p>Writing this register increments the COUNTER1 credit counter for the first hart of all the cores selected in MASK1.</p>

### CREDINC2 register

- Absolute Address: 0x3400D0
- Base Offset: 0xD0
- Size: 0x8

|Bits|Identifier|Access|Reset|Name|
|----|----------|------|-----|----|
| 7:0|   mask2  |   r  |  —  |  — |

#### mask2 field

<p>Writing this register increments the COUNTER2 credit counter for the first hart of all the cores selected in MASK2. Reading this register returns a value of 0.</p>

### CREDINC3 register

- Absolute Address: 0x3400D8
- Base Offset: 0xD8
- Size: 0x8

|Bits|Identifier|Access|Reset|Name|
|----|----------|------|-----|----|
| 7:0|   mask3  |   r  |  —  |  — |

#### mask3 field

<p>Writing this register increments the COUNTER3 credit counter for the first hart of all the cores selected in MASK3. Reading this register returns a value of 0.</p>

### fast_local_barrier0 register

- Absolute Address: 0x340100
- Base Offset: 0x100
- Size: 0x8

|Bits|     Identifier    |Access|Reset|Name|
|----|-------------------|------|-----|----|
| 7:0|fast_local_barrier0|  rw  | 0x0 |  — |

#### fast_local_barrier0 field

<p>The Esperanto Fast Local Barrier (FLB) extension registers provide fast barrier capabilities across the Minions within a Shire. Multiple barrier counters allow a subset of threads in a Shire to atomically modify a barrier counter and determine whether all participating threads have reached the barrier.<br />
The FLB extension provides 32 barrier counters, each 8 bits wide, supporting an all-thread (64 threads) barrier in the Shire. Barrier counter i should be initialized by software before starting the barrier operation using a regular RISC-V store instruction to address (address + i * 8). Software typically initializes barrier counters to zero, 
but any value from 0 to 255 is legal.  For maximum performance, FLB counters are directly accessible by threads in the Shire using CSRRW instructions. Threads participating in a barrier use CSRRW to atomically increment the barrier counter. If a thread is the last to join the barrier,
the CSRRW returns 0x1; otherwise, it returns 0x0.</p>

### fast_local_barrier1 register

- Absolute Address: 0x340108
- Base Offset: 0x108
- Size: 0x8

|Bits|     Identifier    |Access|Reset|Name|
|----|-------------------|------|-----|----|
| 7:0|fast_local_barrier1|  rw  | 0x0 |  — |

#### fast_local_barrier1 field

<p>see description for fast_local_barrier_0</p>

### fast_local_barrier2 register

- Absolute Address: 0x340110
- Base Offset: 0x110
- Size: 0x8

|Bits|     Identifier    |Access|Reset|Name|
|----|-------------------|------|-----|----|
| 7:0|fast_local_barrier2|  rw  | 0x0 |  — |

#### fast_local_barrier2 field

<p>see description for fast_local_barrier_0</p>

### fast_local_barrier3 register

- Absolute Address: 0x340118
- Base Offset: 0x118
- Size: 0x8

|Bits|     Identifier    |Access|Reset|Name|
|----|-------------------|------|-----|----|
| 7:0|fast_local_barrier3|  rw  | 0x0 |  — |

#### fast_local_barrier3 field

<p>see description for fast_local_barrier_0</p>

### fast_local_barrier4 register

- Absolute Address: 0x340120
- Base Offset: 0x120
- Size: 0x8

|Bits|     Identifier    |Access|Reset|Name|
|----|-------------------|------|-----|----|
| 7:0|fast_local_barrier4|  rw  | 0x0 |  — |

#### fast_local_barrier4 field

<p>see description for fast_local_barrier_0</p>

### fast_local_barrier5 register

- Absolute Address: 0x340128
- Base Offset: 0x128
- Size: 0x8

|Bits|     Identifier    |Access|Reset|Name|
|----|-------------------|------|-----|----|
| 7:0|fast_local_barrier5|  rw  | 0x0 |  — |

#### fast_local_barrier5 field

<p>see description for fast_local_barrier_0</p>

### fast_local_barrier6 register

- Absolute Address: 0x340130
- Base Offset: 0x130
- Size: 0x8

|Bits|     Identifier    |Access|Reset|Name|
|----|-------------------|------|-----|----|
| 7:0|fast_local_barrier6|  rw  | 0x0 |  — |

#### fast_local_barrier6 field

<p>see description for fast_local_barrier_0</p>

### fast_local_barrier7 register

- Absolute Address: 0x340138
- Base Offset: 0x138
- Size: 0x8

|Bits|     Identifier    |Access|Reset|Name|
|----|-------------------|------|-----|----|
| 7:0|fast_local_barrier7|  rw  | 0x0 |  — |

#### fast_local_barrier7 field

<p>see description for fast_local_barrier_0</p>

### fast_local_barrier8 register

- Absolute Address: 0x340140
- Base Offset: 0x140
- Size: 0x8

|Bits|     Identifier    |Access|Reset|Name|
|----|-------------------|------|-----|----|
| 7:0|fast_local_barrier8|  rw  | 0x0 |  — |

#### fast_local_barrier8 field

<p>see description for fast_local_barrier_0</p>

### fast_local_barrier9 register

- Absolute Address: 0x340148
- Base Offset: 0x148
- Size: 0x8

|Bits|     Identifier    |Access|Reset|Name|
|----|-------------------|------|-----|----|
| 7:0|fast_local_barrier9|  rw  | 0x0 |  — |

#### fast_local_barrier9 field

<p>see description for fast_local_barrier_0</p>

### fast_local_barrier10 register

- Absolute Address: 0x340150
- Base Offset: 0x150
- Size: 0x8

|Bits|     Identifier     |Access|Reset|Name|
|----|--------------------|------|-----|----|
| 7:0|fast_local_barrier10|  rw  | 0x0 |  — |

#### fast_local_barrier10 field

<p>see description for fast_local_barrier_0</p>

### fast_local_barrier11 register

- Absolute Address: 0x340158
- Base Offset: 0x158
- Size: 0x8

|Bits|     Identifier     |Access|Reset|Name|
|----|--------------------|------|-----|----|
| 7:0|fast_local_barrier11|  rw  | 0x0 |  — |

#### fast_local_barrier11 field

<p>see description for fast_local_barrier_0</p>

### fast_local_barrier12 register

- Absolute Address: 0x340160
- Base Offset: 0x160
- Size: 0x8

|Bits|     Identifier     |Access|Reset|Name|
|----|--------------------|------|-----|----|
| 7:0|fast_local_barrier12|  rw  | 0x0 |  — |

#### fast_local_barrier12 field

<p>see description for fast_local_barrier_0</p>

### fast_local_barrier13 register

- Absolute Address: 0x340168
- Base Offset: 0x168
- Size: 0x8

|Bits|     Identifier     |Access|Reset|Name|
|----|--------------------|------|-----|----|
| 7:0|fast_local_barrier13|  rw  | 0x0 |  — |

#### fast_local_barrier13 field

<p>see description for fast_local_barrier_0</p>

### fast_local_barrier14 register

- Absolute Address: 0x340170
- Base Offset: 0x170
- Size: 0x8

|Bits|     Identifier     |Access|Reset|Name|
|----|--------------------|------|-----|----|
| 7:0|fast_local_barrier14|  rw  | 0x0 |  — |

#### fast_local_barrier14 field

<p>see description for fast_local_barrier_0</p>

### fast_local_barrier15 register

- Absolute Address: 0x340178
- Base Offset: 0x178
- Size: 0x8

|Bits|     Identifier     |Access|Reset|Name|
|----|--------------------|------|-----|----|
| 7:0|fast_local_barrier15|  rw  | 0x0 |  — |

#### fast_local_barrier15 field

<p>see description for fast_local_barrier_0</p>

### fast_local_barrier16 register

- Absolute Address: 0x340180
- Base Offset: 0x180
- Size: 0x8

|Bits|     Identifier     |Access|Reset|Name|
|----|--------------------|------|-----|----|
| 7:0|fast_local_barrier16|  rw  | 0x0 |  — |

#### fast_local_barrier16 field

<p>see description for fast_local_barrier_0</p>

### fast_local_barrier17 register

- Absolute Address: 0x340188
- Base Offset: 0x188
- Size: 0x8

|Bits|     Identifier     |Access|Reset|Name|
|----|--------------------|------|-----|----|
| 7:0|fast_local_barrier17|  rw  | 0x0 |  — |

#### fast_local_barrier17 field

<p>see description for fast_local_barrier_0</p>

### fast_local_barrier18 register

- Absolute Address: 0x340190
- Base Offset: 0x190
- Size: 0x8

|Bits|     Identifier     |Access|Reset|Name|
|----|--------------------|------|-----|----|
| 7:0|fast_local_barrier18|  rw  | 0x0 |  — |

#### fast_local_barrier18 field

<p>see description for fast_local_barrier_0</p>

### fast_local_barrier19 register

- Absolute Address: 0x340198
- Base Offset: 0x198
- Size: 0x8

|Bits|     Identifier     |Access|Reset|Name|
|----|--------------------|------|-----|----|
| 7:0|fast_local_barrier19|  rw  | 0x0 |  — |

#### fast_local_barrier19 field

<p>see description for fast_local_barrier_0</p>

### fast_local_barrier20 register

- Absolute Address: 0x3401A0
- Base Offset: 0x1A0
- Size: 0x8

|Bits|     Identifier     |Access|Reset|Name|
|----|--------------------|------|-----|----|
| 7:0|fast_local_barrier20|  rw  | 0x0 |  — |

#### fast_local_barrier20 field

<p>see description for fast_local_barrier_0</p>

### fast_local_barrier21 register

- Absolute Address: 0x3401A8
- Base Offset: 0x1A8
- Size: 0x8

|Bits|     Identifier     |Access|Reset|Name|
|----|--------------------|------|-----|----|
| 7:0|fast_local_barrier21|  rw  | 0x0 |  — |

#### fast_local_barrier21 field

<p>see description for fast_local_barrier_0</p>

### fast_local_barrier22 register

- Absolute Address: 0x3401B0
- Base Offset: 0x1B0
- Size: 0x8

|Bits|     Identifier     |Access|Reset|Name|
|----|--------------------|------|-----|----|
| 7:0|fast_local_barrier22|  rw  | 0x0 |  — |

#### fast_local_barrier22 field

<p>see description for fast_local_barrier_0</p>

### fast_local_barrier23 register

- Absolute Address: 0x3401B8
- Base Offset: 0x1B8
- Size: 0x8

|Bits|     Identifier     |Access|Reset|Name|
|----|--------------------|------|-----|----|
| 7:0|fast_local_barrier23|  rw  | 0x0 |  — |

#### fast_local_barrier23 field

<p>see description for fast_local_barrier_0</p>

### fast_local_barrier24 register

- Absolute Address: 0x3401C0
- Base Offset: 0x1C0
- Size: 0x8

|Bits|     Identifier     |Access|Reset|Name|
|----|--------------------|------|-----|----|
| 7:0|fast_local_barrier24|  rw  | 0x0 |  — |

#### fast_local_barrier24 field

<p>see description for fast_local_barrier_0</p>

### fast_local_barrier25 register

- Absolute Address: 0x3401C8
- Base Offset: 0x1C8
- Size: 0x8

|Bits|     Identifier     |Access|Reset|Name|
|----|--------------------|------|-----|----|
| 7:0|fast_local_barrier25|  rw  | 0x0 |  — |

#### fast_local_barrier25 field

<p>see description for fast_local_barrier_0</p>

### fast_local_barrier26 register

- Absolute Address: 0x3401D0
- Base Offset: 0x1D0
- Size: 0x8

|Bits|     Identifier     |Access|Reset|Name|
|----|--------------------|------|-----|----|
| 7:0|fast_local_barrier26|  rw  | 0x0 |  — |

#### fast_local_barrier26 field

<p>see description for fast_local_barrier_0</p>

### fast_local_barrier27 register

- Absolute Address: 0x3401D8
- Base Offset: 0x1D8
- Size: 0x8

|Bits|     Identifier     |Access|Reset|Name|
|----|--------------------|------|-----|----|
| 7:0|fast_local_barrier27|  rw  | 0x0 |  — |

#### fast_local_barrier27 field

<p>see description for fast_local_barrier_0</p>

### fast_local_barrier28 register

- Absolute Address: 0x3401E0
- Base Offset: 0x1E0
- Size: 0x8

|Bits|     Identifier     |Access|Reset|Name|
|----|--------------------|------|-----|----|
| 7:0|fast_local_barrier28|  rw  | 0x0 |  — |

#### fast_local_barrier28 field

<p>see description for fast_local_barrier_0</p>

### fast_local_barrier29 register

- Absolute Address: 0x3401E8
- Base Offset: 0x1E8
- Size: 0x8

|Bits|     Identifier     |Access|Reset|Name|
|----|--------------------|------|-----|----|
| 7:0|fast_local_barrier29|  rw  | 0x0 |  — |

#### fast_local_barrier29 field

<p>see description for fast_local_barrier_0</p>

### fast_local_barrier30 register

- Absolute Address: 0x3401F0
- Base Offset: 0x1F0
- Size: 0x8

|Bits|     Identifier     |Access|Reset|Name|
|----|--------------------|------|-----|----|
| 7:0|fast_local_barrier30|  rw  | 0x0 |  — |

#### fast_local_barrier30 field

<p>see description for fast_local_barrier_0</p>

### fast_local_barrier31 register

- Absolute Address: 0x3401F8
- Base Offset: 0x1F8
- Size: 0x8

|Bits|     Identifier     |Access|Reset|Name|
|----|--------------------|------|-----|----|
| 7:0|fast_local_barrier31|  rw  | 0x0 |  — |

#### fast_local_barrier31 field

<p>see description for fast_local_barrier_0</p>

### icache_uprefetch register

- Absolute Address: 0x3402F8
- Base Offset: 0x2F8
- Size: 0x8

<p>If no prefetch with a higher privilege mode associated with it is in progress, a write in this ESR prefetches Count+1 consecutive lines starting at address Address to the instruction cache.
                The privilege mode associated with this operation is User-mode, and all memory accesses are performed with User-mode privileges.
                If any of the memory requests generated by the prefetch engine generates an exception, the line is not prefetched and the operation continues with the next line.
                Reading this register returns the idle status of the prefetch engine.</p>

|Bits|Identifier|Access|Reset|Name|
|----|----------|------|-----|----|
| 5:0|   count  |  rw  | 0x0 |  — |
|47:6|  address |  rw  |  —  |  — |

#### count field

<p>Number of lines to be prefetched (minus 1) starting at the given Address.</p>

#### address field

<p>Bits [47:6] of the address where prefetching should start from (aligned to a cacheline).</p>

## Supervisor_cpu address map

- Absolute Address: 0x740000
- Base Offset: 0x740000
- Size: 0x308

|Offset|   Identifier   |      Name      |
|------|----------------|----------------|
| 0x290| shire_coop_mode| shire_coop_mode|
| 0x300|icache_sprefetch|icache_sprefetch|

### shire_coop_mode register

- Absolute Address: 0x740290
- Base Offset: 0x290
- Size: 0x8

|Bits|Identifier|Access|Reset|Name|
|----|----------|------|-----|----|
|  0 | coop_mode|  rw  | 0x0 |  — |

#### coop_mode field

<p>Software can set this bit to enable cooperative prefetches, loads, and stores.</p>

### icache_sprefetch register

- Absolute Address: 0x740300
- Base Offset: 0x300
- Size: 0x8

<p>If no prefetch with a higher privilege mode associated with it is in progress, a write in this ESR prefetches Count+1 consecutive lines starting at address Address to the instruction caches.
                The privilege mode associated with this operation is Supervisor-mode, and all memory accesses are performed with Supervisor-mode privileges.
                If any of the memory requests generated by the prefetch engine generates an exception, the line is not prefetched and the operation continues with the next line.
                Reading this register returns the idle status of the prefetch engine.</p>

|Bits|Identifier|Access|Reset|Name|
|----|----------|------|-----|----|
| 5:0|   count  |  rw  | 0x0 |  — |
|47:6|  address |  rw  |  —  |  — |

#### count field

<p>Number of lines to be prefetched (minus 1) starting at the given Address.</p>

#### address field

<p>Bits [47:6] of the address where prefetching should start from (aligned to a cacheline).</p>

## D_hart_esr address map

- Absolute Address: 0x800000
- Base Offset: 0x800000
- Size: 0x10000

<p>These registers are per hart.([15:12] bits) in the address indicates hart_id. bits[15:13] minion id and bit [12] is thread id.</p>

|Offset| Identifier |Name|
|------|------------|----|
|0x0000| dbg_hart[0]|  — |
|0x1000| dbg_hart[1]|  — |
|0x2000| dbg_hart[2]|  — |
|0x3000| dbg_hart[3]|  — |
|0x4000| dbg_hart[4]|  — |
|0x5000| dbg_hart[5]|  — |
|0x6000| dbg_hart[6]|  — |
|0x7000| dbg_hart[7]|  — |
|0x8000| dbg_hart[8]|  — |
|0x9000| dbg_hart[9]|  — |
|0xA000|dbg_hart[10]|  — |
|0xB000|dbg_hart[11]|  — |
|0xC000|dbg_hart[12]|  — |
|0xD000|dbg_hart[13]|  — |
|0xE000|dbg_hart[14]|  — |
|0xF000|dbg_hart[15]|  — |

## dbg_hart register file

- Absolute Address: 0x800000
- Base Offset: 0x0
- Size: 0x7C8
- Array Dimensions: [16]
- Array Stride: 0x1000
- Total Size: 0x10000

|Offset| Identifier|    Name   |
|------|-----------|-----------|
| 0x780|  NXDATA0  |  NXDATA0  |
| 0x788|  NXDATA1  |  NXDATA1  |
| 0x790|  AXDATA0  |  AXDATA0  |
| 0x798|  AXDATA1  |  AXDATA1  |
| 0x7A0|AXPROGBUFF0|AXPROGBUFF0|
| 0x7A8|AXPROGBUFF1|AXPROGBUFF1|
| 0x7B0|NXPROGBUFF0|NXPROGBUFF0|
| 0x7B8|NXPROGBUFF1|NXPROGBUFF1|
| 0x7C0|   ABSCMD  |   ABSCMD  |

### NXDATA0 register

- Absolute Address: 0x800780
- Base Offset: 0x780
- Size: 0x8

| Bits|Identifier|Access|Reset|Name|
|-----|----------|------|-----|----|
| 31:0|NXDATA0_f0|  rw  |  —  |  — |
|63:32|NXDATA0_f1|  rw  |  —  |  — |

#### NXDATA0_f0 field

<p>The low (nxdata0)  of ddata0</p>

#### NXDATA0_f1 field

<p>WIRI</p>

### NXDATA1 register

- Absolute Address: 0x800788
- Base Offset: 0x788
- Size: 0x8

| Bits|Identifier|Access|Reset|Name|
|-----|----------|------|-----|----|
| 31:0|NXDATA1_f0|  rw  |  —  |  — |
|63:32|NXDATA1_f1|  rw  |  —  |  — |

#### NXDATA1_f0 field

<p>The high (nxdata0)  of ddata0</p>

#### NXDATA1_f1 field

<p>WIRI</p>

### AXDATA0 register

- Absolute Address: 0x800790
- Base Offset: 0x790
- Size: 0x8

| Bits|Identifier|Access|Reset|Name|
|-----|----------|------|-----|----|
| 31:0|AXDATA0_f0|  rw  |  —  |  — |
|63:32|AXDATA0_f1|  rw  |  —  |  — |

#### AXDATA0_f0 field

<p>This register is shadow, and have the same layout as the nxdata0 register. A write to this register "wakes up" a halted hart, after ddata0 is written, and starts execution from the beginning of the instruction buffer</p>

### AXDATA1 register

- Absolute Address: 0x800798
- Base Offset: 0x798
- Size: 0x8

| Bits|Identifier|Access|Reset|Name|
|-----|----------|------|-----|----|
| 31:0|AXDATA1_f0|  rw  |  —  |  — |
|63:32|AXDATA1_f1|  rw  |  —  |  — |

#### AXDATA1_f0 field

<p>This register is shadow, and have the same layout as the nxdata1 register. A write to this register "wakes up" a halted hart, after ddata0 is written, and starts execution from the beginning of the instruction buffer.</p>

### AXPROGBUFF0 register

- Absolute Address: 0x8007A0
- Base Offset: 0x7A0
- Size: 0x8

|Bits| Identifier|Access|Reset|Name|
|----|-----------|------|-----|----|
|31:0|AXPROGBUFF0|  rw  |  —  |  — |

#### AXPROGBUFF0 field

<p>This register shadows the nxprogbuf0 register. A write to this register wakes up a halted hart, after the instruction buffer is written, and starts execution from the beginning of the instruction buffer, until either an ebreak instruction is executed or the end of the instruction buffer is reached, at which point the hart goes back to being halted.</p>

### AXPROGBUFF1 register

- Absolute Address: 0x8007A8
- Base Offset: 0x7A8
- Size: 0x8

|Bits| Identifier|Access|Reset|Name|
|----|-----------|------|-----|----|
|31:0|AXPROGBUFF1|  rw  |  —  |  — |

#### AXPROGBUFF1 field

<p>This register shadows the nxprogbuf1 register. A write to this register wakes up a halted hart, after the instruction buffer is written, and starts execution from the beginning of the instruction buffer, until either an ebreak instruction is executed or the end of the instruction buffer is reached, at which point the hart goes back to being halted.</p>

### NXPROGBUFF0 register

- Absolute Address: 0x8007B0
- Base Offset: 0x7B0
- Size: 0x8

|Bits| Identifier|Access|Reset|Name|
|----|-----------|------|-----|----|
|31:0|NXPROGBUFF0|  rw  |  —  |  — |

#### NXPROGBUFF0 field

<p>This register holds bits 64-95 of the instruction buffer from where instructions can be executed when the hart is halted (see also abscmd), encoded in the lower half of the register.</p>

### NXPROGBUFF1 register

- Absolute Address: 0x8007B8
- Base Offset: 0x7B8
- Size: 0x8

|Bits| Identifier|Access|Reset|Name|
|----|-----------|------|-----|----|
|31:0|NXPROGBUFF1|  rw  |  —  |  — |

#### NXPROGBUFF1 field

<p>This register holds bits 96-127 of the instruction buffer from where instructions can be executed when the hart is halted (see also abscmd), encoded in the lower half of the register.</p>

### ABSCMD register

- Absolute Address: 0x8007C0
- Base Offset: 0x7C0
- Size: 0x8

<p>This register holds bits 0-63 of the instruction buffer from where instructions can be executed when the hart is halted. The first instruction in the buffer is in the lower order 16 or 32 bits of the ABSCMD register. Writing to this register wakes up a halted hart, after the instruction buffer is written, and starts execution from the beginning of the instruction buffer, until either an ebreak instruction is executed or the end of the instruction buffer is reached, at which point the hart goes back to being halted.</p>

|Bits|Identifier|Access|Reset|Name|
|----|----------|------|-----|----|
|63:0|  ABSCMD  |  rw  |  —  |  — |

## dbg_hart register file

- Absolute Address: 0x801000
- Base Offset: 0x0
- Size: 0x7C8
- Array Dimensions: [16]
- Array Stride: 0x1000
- Total Size: 0x10000

|Offset| Identifier|    Name   |
|------|-----------|-----------|
| 0x780|  NXDATA0  |  NXDATA0  |
| 0x788|  NXDATA1  |  NXDATA1  |
| 0x790|  AXDATA0  |  AXDATA0  |
| 0x798|  AXDATA1  |  AXDATA1  |
| 0x7A0|AXPROGBUFF0|AXPROGBUFF0|
| 0x7A8|AXPROGBUFF1|AXPROGBUFF1|
| 0x7B0|NXPROGBUFF0|NXPROGBUFF0|
| 0x7B8|NXPROGBUFF1|NXPROGBUFF1|
| 0x7C0|   ABSCMD  |   ABSCMD  |

### NXDATA0 register

- Absolute Address: 0x801780
- Base Offset: 0x780
- Size: 0x8

| Bits|Identifier|Access|Reset|Name|
|-----|----------|------|-----|----|
| 31:0|NXDATA0_f0|  rw  |  —  |  — |
|63:32|NXDATA0_f1|  rw  |  —  |  — |

#### NXDATA0_f0 field

<p>The low (nxdata0)  of ddata0</p>

#### NXDATA0_f1 field

<p>WIRI</p>

### NXDATA1 register

- Absolute Address: 0x801788
- Base Offset: 0x788
- Size: 0x8

| Bits|Identifier|Access|Reset|Name|
|-----|----------|------|-----|----|
| 31:0|NXDATA1_f0|  rw  |  —  |  — |
|63:32|NXDATA1_f1|  rw  |  —  |  — |

#### NXDATA1_f0 field

<p>The high (nxdata0)  of ddata0</p>

#### NXDATA1_f1 field

<p>WIRI</p>

### AXDATA0 register

- Absolute Address: 0x801790
- Base Offset: 0x790
- Size: 0x8

| Bits|Identifier|Access|Reset|Name|
|-----|----------|------|-----|----|
| 31:0|AXDATA0_f0|  rw  |  —  |  — |
|63:32|AXDATA0_f1|  rw  |  —  |  — |

#### AXDATA0_f0 field

<p>This register is shadow, and have the same layout as the nxdata0 register. A write to this register "wakes up" a halted hart, after ddata0 is written, and starts execution from the beginning of the instruction buffer</p>

### AXDATA1 register

- Absolute Address: 0x801798
- Base Offset: 0x798
- Size: 0x8

| Bits|Identifier|Access|Reset|Name|
|-----|----------|------|-----|----|
| 31:0|AXDATA1_f0|  rw  |  —  |  — |
|63:32|AXDATA1_f1|  rw  |  —  |  — |

#### AXDATA1_f0 field

<p>This register is shadow, and have the same layout as the nxdata1 register. A write to this register "wakes up" a halted hart, after ddata0 is written, and starts execution from the beginning of the instruction buffer.</p>

### AXPROGBUFF0 register

- Absolute Address: 0x8017A0
- Base Offset: 0x7A0
- Size: 0x8

|Bits| Identifier|Access|Reset|Name|
|----|-----------|------|-----|----|
|31:0|AXPROGBUFF0|  rw  |  —  |  — |

#### AXPROGBUFF0 field

<p>This register shadows the nxprogbuf0 register. A write to this register wakes up a halted hart, after the instruction buffer is written, and starts execution from the beginning of the instruction buffer, until either an ebreak instruction is executed or the end of the instruction buffer is reached, at which point the hart goes back to being halted.</p>

### AXPROGBUFF1 register

- Absolute Address: 0x8017A8
- Base Offset: 0x7A8
- Size: 0x8

|Bits| Identifier|Access|Reset|Name|
|----|-----------|------|-----|----|
|31:0|AXPROGBUFF1|  rw  |  —  |  — |

#### AXPROGBUFF1 field

<p>This register shadows the nxprogbuf1 register. A write to this register wakes up a halted hart, after the instruction buffer is written, and starts execution from the beginning of the instruction buffer, until either an ebreak instruction is executed or the end of the instruction buffer is reached, at which point the hart goes back to being halted.</p>

### NXPROGBUFF0 register

- Absolute Address: 0x8017B0
- Base Offset: 0x7B0
- Size: 0x8

|Bits| Identifier|Access|Reset|Name|
|----|-----------|------|-----|----|
|31:0|NXPROGBUFF0|  rw  |  —  |  — |

#### NXPROGBUFF0 field

<p>This register holds bits 64-95 of the instruction buffer from where instructions can be executed when the hart is halted (see also abscmd), encoded in the lower half of the register.</p>

### NXPROGBUFF1 register

- Absolute Address: 0x8017B8
- Base Offset: 0x7B8
- Size: 0x8

|Bits| Identifier|Access|Reset|Name|
|----|-----------|------|-----|----|
|31:0|NXPROGBUFF1|  rw  |  —  |  — |

#### NXPROGBUFF1 field

<p>This register holds bits 96-127 of the instruction buffer from where instructions can be executed when the hart is halted (see also abscmd), encoded in the lower half of the register.</p>

### ABSCMD register

- Absolute Address: 0x8017C0
- Base Offset: 0x7C0
- Size: 0x8

<p>This register holds bits 0-63 of the instruction buffer from where instructions can be executed when the hart is halted. The first instruction in the buffer is in the lower order 16 or 32 bits of the ABSCMD register. Writing to this register wakes up a halted hart, after the instruction buffer is written, and starts execution from the beginning of the instruction buffer, until either an ebreak instruction is executed or the end of the instruction buffer is reached, at which point the hart goes back to being halted.</p>

|Bits|Identifier|Access|Reset|Name|
|----|----------|------|-----|----|
|63:0|  ABSCMD  |  rw  |  —  |  — |

## dbg_hart register file

- Absolute Address: 0x802000
- Base Offset: 0x0
- Size: 0x7C8
- Array Dimensions: [16]
- Array Stride: 0x1000
- Total Size: 0x10000

|Offset| Identifier|    Name   |
|------|-----------|-----------|
| 0x780|  NXDATA0  |  NXDATA0  |
| 0x788|  NXDATA1  |  NXDATA1  |
| 0x790|  AXDATA0  |  AXDATA0  |
| 0x798|  AXDATA1  |  AXDATA1  |
| 0x7A0|AXPROGBUFF0|AXPROGBUFF0|
| 0x7A8|AXPROGBUFF1|AXPROGBUFF1|
| 0x7B0|NXPROGBUFF0|NXPROGBUFF0|
| 0x7B8|NXPROGBUFF1|NXPROGBUFF1|
| 0x7C0|   ABSCMD  |   ABSCMD  |

### NXDATA0 register

- Absolute Address: 0x802780
- Base Offset: 0x780
- Size: 0x8

| Bits|Identifier|Access|Reset|Name|
|-----|----------|------|-----|----|
| 31:0|NXDATA0_f0|  rw  |  —  |  — |
|63:32|NXDATA0_f1|  rw  |  —  |  — |

#### NXDATA0_f0 field

<p>The low (nxdata0)  of ddata0</p>

#### NXDATA0_f1 field

<p>WIRI</p>

### NXDATA1 register

- Absolute Address: 0x802788
- Base Offset: 0x788
- Size: 0x8

| Bits|Identifier|Access|Reset|Name|
|-----|----------|------|-----|----|
| 31:0|NXDATA1_f0|  rw  |  —  |  — |
|63:32|NXDATA1_f1|  rw  |  —  |  — |

#### NXDATA1_f0 field

<p>The high (nxdata0)  of ddata0</p>

#### NXDATA1_f1 field

<p>WIRI</p>

### AXDATA0 register

- Absolute Address: 0x802790
- Base Offset: 0x790
- Size: 0x8

| Bits|Identifier|Access|Reset|Name|
|-----|----------|------|-----|----|
| 31:0|AXDATA0_f0|  rw  |  —  |  — |
|63:32|AXDATA0_f1|  rw  |  —  |  — |

#### AXDATA0_f0 field

<p>This register is shadow, and have the same layout as the nxdata0 register. A write to this register "wakes up" a halted hart, after ddata0 is written, and starts execution from the beginning of the instruction buffer</p>

### AXDATA1 register

- Absolute Address: 0x802798
- Base Offset: 0x798
- Size: 0x8

| Bits|Identifier|Access|Reset|Name|
|-----|----------|------|-----|----|
| 31:0|AXDATA1_f0|  rw  |  —  |  — |
|63:32|AXDATA1_f1|  rw  |  —  |  — |

#### AXDATA1_f0 field

<p>This register is shadow, and have the same layout as the nxdata1 register. A write to this register "wakes up" a halted hart, after ddata0 is written, and starts execution from the beginning of the instruction buffer.</p>

### AXPROGBUFF0 register

- Absolute Address: 0x8027A0
- Base Offset: 0x7A0
- Size: 0x8

|Bits| Identifier|Access|Reset|Name|
|----|-----------|------|-----|----|
|31:0|AXPROGBUFF0|  rw  |  —  |  — |

#### AXPROGBUFF0 field

<p>This register shadows the nxprogbuf0 register. A write to this register wakes up a halted hart, after the instruction buffer is written, and starts execution from the beginning of the instruction buffer, until either an ebreak instruction is executed or the end of the instruction buffer is reached, at which point the hart goes back to being halted.</p>

### AXPROGBUFF1 register

- Absolute Address: 0x8027A8
- Base Offset: 0x7A8
- Size: 0x8

|Bits| Identifier|Access|Reset|Name|
|----|-----------|------|-----|----|
|31:0|AXPROGBUFF1|  rw  |  —  |  — |

#### AXPROGBUFF1 field

<p>This register shadows the nxprogbuf1 register. A write to this register wakes up a halted hart, after the instruction buffer is written, and starts execution from the beginning of the instruction buffer, until either an ebreak instruction is executed or the end of the instruction buffer is reached, at which point the hart goes back to being halted.</p>

### NXPROGBUFF0 register

- Absolute Address: 0x8027B0
- Base Offset: 0x7B0
- Size: 0x8

|Bits| Identifier|Access|Reset|Name|
|----|-----------|------|-----|----|
|31:0|NXPROGBUFF0|  rw  |  —  |  — |

#### NXPROGBUFF0 field

<p>This register holds bits 64-95 of the instruction buffer from where instructions can be executed when the hart is halted (see also abscmd), encoded in the lower half of the register.</p>

### NXPROGBUFF1 register

- Absolute Address: 0x8027B8
- Base Offset: 0x7B8
- Size: 0x8

|Bits| Identifier|Access|Reset|Name|
|----|-----------|------|-----|----|
|31:0|NXPROGBUFF1|  rw  |  —  |  — |

#### NXPROGBUFF1 field

<p>This register holds bits 96-127 of the instruction buffer from where instructions can be executed when the hart is halted (see also abscmd), encoded in the lower half of the register.</p>

### ABSCMD register

- Absolute Address: 0x8027C0
- Base Offset: 0x7C0
- Size: 0x8

<p>This register holds bits 0-63 of the instruction buffer from where instructions can be executed when the hart is halted. The first instruction in the buffer is in the lower order 16 or 32 bits of the ABSCMD register. Writing to this register wakes up a halted hart, after the instruction buffer is written, and starts execution from the beginning of the instruction buffer, until either an ebreak instruction is executed or the end of the instruction buffer is reached, at which point the hart goes back to being halted.</p>

|Bits|Identifier|Access|Reset|Name|
|----|----------|------|-----|----|
|63:0|  ABSCMD  |  rw  |  —  |  — |

## dbg_hart register file

- Absolute Address: 0x803000
- Base Offset: 0x0
- Size: 0x7C8
- Array Dimensions: [16]
- Array Stride: 0x1000
- Total Size: 0x10000

|Offset| Identifier|    Name   |
|------|-----------|-----------|
| 0x780|  NXDATA0  |  NXDATA0  |
| 0x788|  NXDATA1  |  NXDATA1  |
| 0x790|  AXDATA0  |  AXDATA0  |
| 0x798|  AXDATA1  |  AXDATA1  |
| 0x7A0|AXPROGBUFF0|AXPROGBUFF0|
| 0x7A8|AXPROGBUFF1|AXPROGBUFF1|
| 0x7B0|NXPROGBUFF0|NXPROGBUFF0|
| 0x7B8|NXPROGBUFF1|NXPROGBUFF1|
| 0x7C0|   ABSCMD  |   ABSCMD  |

### NXDATA0 register

- Absolute Address: 0x803780
- Base Offset: 0x780
- Size: 0x8

| Bits|Identifier|Access|Reset|Name|
|-----|----------|------|-----|----|
| 31:0|NXDATA0_f0|  rw  |  —  |  — |
|63:32|NXDATA0_f1|  rw  |  —  |  — |

#### NXDATA0_f0 field

<p>The low (nxdata0)  of ddata0</p>

#### NXDATA0_f1 field

<p>WIRI</p>

### NXDATA1 register

- Absolute Address: 0x803788
- Base Offset: 0x788
- Size: 0x8

| Bits|Identifier|Access|Reset|Name|
|-----|----------|------|-----|----|
| 31:0|NXDATA1_f0|  rw  |  —  |  — |
|63:32|NXDATA1_f1|  rw  |  —  |  — |

#### NXDATA1_f0 field

<p>The high (nxdata0)  of ddata0</p>

#### NXDATA1_f1 field

<p>WIRI</p>

### AXDATA0 register

- Absolute Address: 0x803790
- Base Offset: 0x790
- Size: 0x8

| Bits|Identifier|Access|Reset|Name|
|-----|----------|------|-----|----|
| 31:0|AXDATA0_f0|  rw  |  —  |  — |
|63:32|AXDATA0_f1|  rw  |  —  |  — |

#### AXDATA0_f0 field

<p>This register is shadow, and have the same layout as the nxdata0 register. A write to this register "wakes up" a halted hart, after ddata0 is written, and starts execution from the beginning of the instruction buffer</p>

### AXDATA1 register

- Absolute Address: 0x803798
- Base Offset: 0x798
- Size: 0x8

| Bits|Identifier|Access|Reset|Name|
|-----|----------|------|-----|----|
| 31:0|AXDATA1_f0|  rw  |  —  |  — |
|63:32|AXDATA1_f1|  rw  |  —  |  — |

#### AXDATA1_f0 field

<p>This register is shadow, and have the same layout as the nxdata1 register. A write to this register "wakes up" a halted hart, after ddata0 is written, and starts execution from the beginning of the instruction buffer.</p>

### AXPROGBUFF0 register

- Absolute Address: 0x8037A0
- Base Offset: 0x7A0
- Size: 0x8

|Bits| Identifier|Access|Reset|Name|
|----|-----------|------|-----|----|
|31:0|AXPROGBUFF0|  rw  |  —  |  — |

#### AXPROGBUFF0 field

<p>This register shadows the nxprogbuf0 register. A write to this register wakes up a halted hart, after the instruction buffer is written, and starts execution from the beginning of the instruction buffer, until either an ebreak instruction is executed or the end of the instruction buffer is reached, at which point the hart goes back to being halted.</p>

### AXPROGBUFF1 register

- Absolute Address: 0x8037A8
- Base Offset: 0x7A8
- Size: 0x8

|Bits| Identifier|Access|Reset|Name|
|----|-----------|------|-----|----|
|31:0|AXPROGBUFF1|  rw  |  —  |  — |

#### AXPROGBUFF1 field

<p>This register shadows the nxprogbuf1 register. A write to this register wakes up a halted hart, after the instruction buffer is written, and starts execution from the beginning of the instruction buffer, until either an ebreak instruction is executed or the end of the instruction buffer is reached, at which point the hart goes back to being halted.</p>

### NXPROGBUFF0 register

- Absolute Address: 0x8037B0
- Base Offset: 0x7B0
- Size: 0x8

|Bits| Identifier|Access|Reset|Name|
|----|-----------|------|-----|----|
|31:0|NXPROGBUFF0|  rw  |  —  |  — |

#### NXPROGBUFF0 field

<p>This register holds bits 64-95 of the instruction buffer from where instructions can be executed when the hart is halted (see also abscmd), encoded in the lower half of the register.</p>

### NXPROGBUFF1 register

- Absolute Address: 0x8037B8
- Base Offset: 0x7B8
- Size: 0x8

|Bits| Identifier|Access|Reset|Name|
|----|-----------|------|-----|----|
|31:0|NXPROGBUFF1|  rw  |  —  |  — |

#### NXPROGBUFF1 field

<p>This register holds bits 96-127 of the instruction buffer from where instructions can be executed when the hart is halted (see also abscmd), encoded in the lower half of the register.</p>

### ABSCMD register

- Absolute Address: 0x8037C0
- Base Offset: 0x7C0
- Size: 0x8

<p>This register holds bits 0-63 of the instruction buffer from where instructions can be executed when the hart is halted. The first instruction in the buffer is in the lower order 16 or 32 bits of the ABSCMD register. Writing to this register wakes up a halted hart, after the instruction buffer is written, and starts execution from the beginning of the instruction buffer, until either an ebreak instruction is executed or the end of the instruction buffer is reached, at which point the hart goes back to being halted.</p>

|Bits|Identifier|Access|Reset|Name|
|----|----------|------|-----|----|
|63:0|  ABSCMD  |  rw  |  —  |  — |

## dbg_hart register file

- Absolute Address: 0x804000
- Base Offset: 0x0
- Size: 0x7C8
- Array Dimensions: [16]
- Array Stride: 0x1000
- Total Size: 0x10000

|Offset| Identifier|    Name   |
|------|-----------|-----------|
| 0x780|  NXDATA0  |  NXDATA0  |
| 0x788|  NXDATA1  |  NXDATA1  |
| 0x790|  AXDATA0  |  AXDATA0  |
| 0x798|  AXDATA1  |  AXDATA1  |
| 0x7A0|AXPROGBUFF0|AXPROGBUFF0|
| 0x7A8|AXPROGBUFF1|AXPROGBUFF1|
| 0x7B0|NXPROGBUFF0|NXPROGBUFF0|
| 0x7B8|NXPROGBUFF1|NXPROGBUFF1|
| 0x7C0|   ABSCMD  |   ABSCMD  |

### NXDATA0 register

- Absolute Address: 0x804780
- Base Offset: 0x780
- Size: 0x8

| Bits|Identifier|Access|Reset|Name|
|-----|----------|------|-----|----|
| 31:0|NXDATA0_f0|  rw  |  —  |  — |
|63:32|NXDATA0_f1|  rw  |  —  |  — |

#### NXDATA0_f0 field

<p>The low (nxdata0)  of ddata0</p>

#### NXDATA0_f1 field

<p>WIRI</p>

### NXDATA1 register

- Absolute Address: 0x804788
- Base Offset: 0x788
- Size: 0x8

| Bits|Identifier|Access|Reset|Name|
|-----|----------|------|-----|----|
| 31:0|NXDATA1_f0|  rw  |  —  |  — |
|63:32|NXDATA1_f1|  rw  |  —  |  — |

#### NXDATA1_f0 field

<p>The high (nxdata0)  of ddata0</p>

#### NXDATA1_f1 field

<p>WIRI</p>

### AXDATA0 register

- Absolute Address: 0x804790
- Base Offset: 0x790
- Size: 0x8

| Bits|Identifier|Access|Reset|Name|
|-----|----------|------|-----|----|
| 31:0|AXDATA0_f0|  rw  |  —  |  — |
|63:32|AXDATA0_f1|  rw  |  —  |  — |

#### AXDATA0_f0 field

<p>This register is shadow, and have the same layout as the nxdata0 register. A write to this register "wakes up" a halted hart, after ddata0 is written, and starts execution from the beginning of the instruction buffer</p>

### AXDATA1 register

- Absolute Address: 0x804798
- Base Offset: 0x798
- Size: 0x8

| Bits|Identifier|Access|Reset|Name|
|-----|----------|------|-----|----|
| 31:0|AXDATA1_f0|  rw  |  —  |  — |
|63:32|AXDATA1_f1|  rw  |  —  |  — |

#### AXDATA1_f0 field

<p>This register is shadow, and have the same layout as the nxdata1 register. A write to this register "wakes up" a halted hart, after ddata0 is written, and starts execution from the beginning of the instruction buffer.</p>

### AXPROGBUFF0 register

- Absolute Address: 0x8047A0
- Base Offset: 0x7A0
- Size: 0x8

|Bits| Identifier|Access|Reset|Name|
|----|-----------|------|-----|----|
|31:0|AXPROGBUFF0|  rw  |  —  |  — |

#### AXPROGBUFF0 field

<p>This register shadows the nxprogbuf0 register. A write to this register wakes up a halted hart, after the instruction buffer is written, and starts execution from the beginning of the instruction buffer, until either an ebreak instruction is executed or the end of the instruction buffer is reached, at which point the hart goes back to being halted.</p>

### AXPROGBUFF1 register

- Absolute Address: 0x8047A8
- Base Offset: 0x7A8
- Size: 0x8

|Bits| Identifier|Access|Reset|Name|
|----|-----------|------|-----|----|
|31:0|AXPROGBUFF1|  rw  |  —  |  — |

#### AXPROGBUFF1 field

<p>This register shadows the nxprogbuf1 register. A write to this register wakes up a halted hart, after the instruction buffer is written, and starts execution from the beginning of the instruction buffer, until either an ebreak instruction is executed or the end of the instruction buffer is reached, at which point the hart goes back to being halted.</p>

### NXPROGBUFF0 register

- Absolute Address: 0x8047B0
- Base Offset: 0x7B0
- Size: 0x8

|Bits| Identifier|Access|Reset|Name|
|----|-----------|------|-----|----|
|31:0|NXPROGBUFF0|  rw  |  —  |  — |

#### NXPROGBUFF0 field

<p>This register holds bits 64-95 of the instruction buffer from where instructions can be executed when the hart is halted (see also abscmd), encoded in the lower half of the register.</p>

### NXPROGBUFF1 register

- Absolute Address: 0x8047B8
- Base Offset: 0x7B8
- Size: 0x8

|Bits| Identifier|Access|Reset|Name|
|----|-----------|------|-----|----|
|31:0|NXPROGBUFF1|  rw  |  —  |  — |

#### NXPROGBUFF1 field

<p>This register holds bits 96-127 of the instruction buffer from where instructions can be executed when the hart is halted (see also abscmd), encoded in the lower half of the register.</p>

### ABSCMD register

- Absolute Address: 0x8047C0
- Base Offset: 0x7C0
- Size: 0x8

<p>This register holds bits 0-63 of the instruction buffer from where instructions can be executed when the hart is halted. The first instruction in the buffer is in the lower order 16 or 32 bits of the ABSCMD register. Writing to this register wakes up a halted hart, after the instruction buffer is written, and starts execution from the beginning of the instruction buffer, until either an ebreak instruction is executed or the end of the instruction buffer is reached, at which point the hart goes back to being halted.</p>

|Bits|Identifier|Access|Reset|Name|
|----|----------|------|-----|----|
|63:0|  ABSCMD  |  rw  |  —  |  — |

## dbg_hart register file

- Absolute Address: 0x805000
- Base Offset: 0x0
- Size: 0x7C8
- Array Dimensions: [16]
- Array Stride: 0x1000
- Total Size: 0x10000

|Offset| Identifier|    Name   |
|------|-----------|-----------|
| 0x780|  NXDATA0  |  NXDATA0  |
| 0x788|  NXDATA1  |  NXDATA1  |
| 0x790|  AXDATA0  |  AXDATA0  |
| 0x798|  AXDATA1  |  AXDATA1  |
| 0x7A0|AXPROGBUFF0|AXPROGBUFF0|
| 0x7A8|AXPROGBUFF1|AXPROGBUFF1|
| 0x7B0|NXPROGBUFF0|NXPROGBUFF0|
| 0x7B8|NXPROGBUFF1|NXPROGBUFF1|
| 0x7C0|   ABSCMD  |   ABSCMD  |

### NXDATA0 register

- Absolute Address: 0x805780
- Base Offset: 0x780
- Size: 0x8

| Bits|Identifier|Access|Reset|Name|
|-----|----------|------|-----|----|
| 31:0|NXDATA0_f0|  rw  |  —  |  — |
|63:32|NXDATA0_f1|  rw  |  —  |  — |

#### NXDATA0_f0 field

<p>The low (nxdata0)  of ddata0</p>

#### NXDATA0_f1 field

<p>WIRI</p>

### NXDATA1 register

- Absolute Address: 0x805788
- Base Offset: 0x788
- Size: 0x8

| Bits|Identifier|Access|Reset|Name|
|-----|----------|------|-----|----|
| 31:0|NXDATA1_f0|  rw  |  —  |  — |
|63:32|NXDATA1_f1|  rw  |  —  |  — |

#### NXDATA1_f0 field

<p>The high (nxdata0)  of ddata0</p>

#### NXDATA1_f1 field

<p>WIRI</p>

### AXDATA0 register

- Absolute Address: 0x805790
- Base Offset: 0x790
- Size: 0x8

| Bits|Identifier|Access|Reset|Name|
|-----|----------|------|-----|----|
| 31:0|AXDATA0_f0|  rw  |  —  |  — |
|63:32|AXDATA0_f1|  rw  |  —  |  — |

#### AXDATA0_f0 field

<p>This register is shadow, and have the same layout as the nxdata0 register. A write to this register "wakes up" a halted hart, after ddata0 is written, and starts execution from the beginning of the instruction buffer</p>

### AXDATA1 register

- Absolute Address: 0x805798
- Base Offset: 0x798
- Size: 0x8

| Bits|Identifier|Access|Reset|Name|
|-----|----------|------|-----|----|
| 31:0|AXDATA1_f0|  rw  |  —  |  — |
|63:32|AXDATA1_f1|  rw  |  —  |  — |

#### AXDATA1_f0 field

<p>This register is shadow, and have the same layout as the nxdata1 register. A write to this register "wakes up" a halted hart, after ddata0 is written, and starts execution from the beginning of the instruction buffer.</p>

### AXPROGBUFF0 register

- Absolute Address: 0x8057A0
- Base Offset: 0x7A0
- Size: 0x8

|Bits| Identifier|Access|Reset|Name|
|----|-----------|------|-----|----|
|31:0|AXPROGBUFF0|  rw  |  —  |  — |

#### AXPROGBUFF0 field

<p>This register shadows the nxprogbuf0 register. A write to this register wakes up a halted hart, after the instruction buffer is written, and starts execution from the beginning of the instruction buffer, until either an ebreak instruction is executed or the end of the instruction buffer is reached, at which point the hart goes back to being halted.</p>

### AXPROGBUFF1 register

- Absolute Address: 0x8057A8
- Base Offset: 0x7A8
- Size: 0x8

|Bits| Identifier|Access|Reset|Name|
|----|-----------|------|-----|----|
|31:0|AXPROGBUFF1|  rw  |  —  |  — |

#### AXPROGBUFF1 field

<p>This register shadows the nxprogbuf1 register. A write to this register wakes up a halted hart, after the instruction buffer is written, and starts execution from the beginning of the instruction buffer, until either an ebreak instruction is executed or the end of the instruction buffer is reached, at which point the hart goes back to being halted.</p>

### NXPROGBUFF0 register

- Absolute Address: 0x8057B0
- Base Offset: 0x7B0
- Size: 0x8

|Bits| Identifier|Access|Reset|Name|
|----|-----------|------|-----|----|
|31:0|NXPROGBUFF0|  rw  |  —  |  — |

#### NXPROGBUFF0 field

<p>This register holds bits 64-95 of the instruction buffer from where instructions can be executed when the hart is halted (see also abscmd), encoded in the lower half of the register.</p>

### NXPROGBUFF1 register

- Absolute Address: 0x8057B8
- Base Offset: 0x7B8
- Size: 0x8

|Bits| Identifier|Access|Reset|Name|
|----|-----------|------|-----|----|
|31:0|NXPROGBUFF1|  rw  |  —  |  — |

#### NXPROGBUFF1 field

<p>This register holds bits 96-127 of the instruction buffer from where instructions can be executed when the hart is halted (see also abscmd), encoded in the lower half of the register.</p>

### ABSCMD register

- Absolute Address: 0x8057C0
- Base Offset: 0x7C0
- Size: 0x8

<p>This register holds bits 0-63 of the instruction buffer from where instructions can be executed when the hart is halted. The first instruction in the buffer is in the lower order 16 or 32 bits of the ABSCMD register. Writing to this register wakes up a halted hart, after the instruction buffer is written, and starts execution from the beginning of the instruction buffer, until either an ebreak instruction is executed or the end of the instruction buffer is reached, at which point the hart goes back to being halted.</p>

|Bits|Identifier|Access|Reset|Name|
|----|----------|------|-----|----|
|63:0|  ABSCMD  |  rw  |  —  |  — |

## dbg_hart register file

- Absolute Address: 0x806000
- Base Offset: 0x0
- Size: 0x7C8
- Array Dimensions: [16]
- Array Stride: 0x1000
- Total Size: 0x10000

|Offset| Identifier|    Name   |
|------|-----------|-----------|
| 0x780|  NXDATA0  |  NXDATA0  |
| 0x788|  NXDATA1  |  NXDATA1  |
| 0x790|  AXDATA0  |  AXDATA0  |
| 0x798|  AXDATA1  |  AXDATA1  |
| 0x7A0|AXPROGBUFF0|AXPROGBUFF0|
| 0x7A8|AXPROGBUFF1|AXPROGBUFF1|
| 0x7B0|NXPROGBUFF0|NXPROGBUFF0|
| 0x7B8|NXPROGBUFF1|NXPROGBUFF1|
| 0x7C0|   ABSCMD  |   ABSCMD  |

### NXDATA0 register

- Absolute Address: 0x806780
- Base Offset: 0x780
- Size: 0x8

| Bits|Identifier|Access|Reset|Name|
|-----|----------|------|-----|----|
| 31:0|NXDATA0_f0|  rw  |  —  |  — |
|63:32|NXDATA0_f1|  rw  |  —  |  — |

#### NXDATA0_f0 field

<p>The low (nxdata0)  of ddata0</p>

#### NXDATA0_f1 field

<p>WIRI</p>

### NXDATA1 register

- Absolute Address: 0x806788
- Base Offset: 0x788
- Size: 0x8

| Bits|Identifier|Access|Reset|Name|
|-----|----------|------|-----|----|
| 31:0|NXDATA1_f0|  rw  |  —  |  — |
|63:32|NXDATA1_f1|  rw  |  —  |  — |

#### NXDATA1_f0 field

<p>The high (nxdata0)  of ddata0</p>

#### NXDATA1_f1 field

<p>WIRI</p>

### AXDATA0 register

- Absolute Address: 0x806790
- Base Offset: 0x790
- Size: 0x8

| Bits|Identifier|Access|Reset|Name|
|-----|----------|------|-----|----|
| 31:0|AXDATA0_f0|  rw  |  —  |  — |
|63:32|AXDATA0_f1|  rw  |  —  |  — |

#### AXDATA0_f0 field

<p>This register is shadow, and have the same layout as the nxdata0 register. A write to this register "wakes up" a halted hart, after ddata0 is written, and starts execution from the beginning of the instruction buffer</p>

### AXDATA1 register

- Absolute Address: 0x806798
- Base Offset: 0x798
- Size: 0x8

| Bits|Identifier|Access|Reset|Name|
|-----|----------|------|-----|----|
| 31:0|AXDATA1_f0|  rw  |  —  |  — |
|63:32|AXDATA1_f1|  rw  |  —  |  — |

#### AXDATA1_f0 field

<p>This register is shadow, and have the same layout as the nxdata1 register. A write to this register "wakes up" a halted hart, after ddata0 is written, and starts execution from the beginning of the instruction buffer.</p>

### AXPROGBUFF0 register

- Absolute Address: 0x8067A0
- Base Offset: 0x7A0
- Size: 0x8

|Bits| Identifier|Access|Reset|Name|
|----|-----------|------|-----|----|
|31:0|AXPROGBUFF0|  rw  |  —  |  — |

#### AXPROGBUFF0 field

<p>This register shadows the nxprogbuf0 register. A write to this register wakes up a halted hart, after the instruction buffer is written, and starts execution from the beginning of the instruction buffer, until either an ebreak instruction is executed or the end of the instruction buffer is reached, at which point the hart goes back to being halted.</p>

### AXPROGBUFF1 register

- Absolute Address: 0x8067A8
- Base Offset: 0x7A8
- Size: 0x8

|Bits| Identifier|Access|Reset|Name|
|----|-----------|------|-----|----|
|31:0|AXPROGBUFF1|  rw  |  —  |  — |

#### AXPROGBUFF1 field

<p>This register shadows the nxprogbuf1 register. A write to this register wakes up a halted hart, after the instruction buffer is written, and starts execution from the beginning of the instruction buffer, until either an ebreak instruction is executed or the end of the instruction buffer is reached, at which point the hart goes back to being halted.</p>

### NXPROGBUFF0 register

- Absolute Address: 0x8067B0
- Base Offset: 0x7B0
- Size: 0x8

|Bits| Identifier|Access|Reset|Name|
|----|-----------|------|-----|----|
|31:0|NXPROGBUFF0|  rw  |  —  |  — |

#### NXPROGBUFF0 field

<p>This register holds bits 64-95 of the instruction buffer from where instructions can be executed when the hart is halted (see also abscmd), encoded in the lower half of the register.</p>

### NXPROGBUFF1 register

- Absolute Address: 0x8067B8
- Base Offset: 0x7B8
- Size: 0x8

|Bits| Identifier|Access|Reset|Name|
|----|-----------|------|-----|----|
|31:0|NXPROGBUFF1|  rw  |  —  |  — |

#### NXPROGBUFF1 field

<p>This register holds bits 96-127 of the instruction buffer from where instructions can be executed when the hart is halted (see also abscmd), encoded in the lower half of the register.</p>

### ABSCMD register

- Absolute Address: 0x8067C0
- Base Offset: 0x7C0
- Size: 0x8

<p>This register holds bits 0-63 of the instruction buffer from where instructions can be executed when the hart is halted. The first instruction in the buffer is in the lower order 16 or 32 bits of the ABSCMD register. Writing to this register wakes up a halted hart, after the instruction buffer is written, and starts execution from the beginning of the instruction buffer, until either an ebreak instruction is executed or the end of the instruction buffer is reached, at which point the hart goes back to being halted.</p>

|Bits|Identifier|Access|Reset|Name|
|----|----------|------|-----|----|
|63:0|  ABSCMD  |  rw  |  —  |  — |

## dbg_hart register file

- Absolute Address: 0x807000
- Base Offset: 0x0
- Size: 0x7C8
- Array Dimensions: [16]
- Array Stride: 0x1000
- Total Size: 0x10000

|Offset| Identifier|    Name   |
|------|-----------|-----------|
| 0x780|  NXDATA0  |  NXDATA0  |
| 0x788|  NXDATA1  |  NXDATA1  |
| 0x790|  AXDATA0  |  AXDATA0  |
| 0x798|  AXDATA1  |  AXDATA1  |
| 0x7A0|AXPROGBUFF0|AXPROGBUFF0|
| 0x7A8|AXPROGBUFF1|AXPROGBUFF1|
| 0x7B0|NXPROGBUFF0|NXPROGBUFF0|
| 0x7B8|NXPROGBUFF1|NXPROGBUFF1|
| 0x7C0|   ABSCMD  |   ABSCMD  |

### NXDATA0 register

- Absolute Address: 0x807780
- Base Offset: 0x780
- Size: 0x8

| Bits|Identifier|Access|Reset|Name|
|-----|----------|------|-----|----|
| 31:0|NXDATA0_f0|  rw  |  —  |  — |
|63:32|NXDATA0_f1|  rw  |  —  |  — |

#### NXDATA0_f0 field

<p>The low (nxdata0)  of ddata0</p>

#### NXDATA0_f1 field

<p>WIRI</p>

### NXDATA1 register

- Absolute Address: 0x807788
- Base Offset: 0x788
- Size: 0x8

| Bits|Identifier|Access|Reset|Name|
|-----|----------|------|-----|----|
| 31:0|NXDATA1_f0|  rw  |  —  |  — |
|63:32|NXDATA1_f1|  rw  |  —  |  — |

#### NXDATA1_f0 field

<p>The high (nxdata0)  of ddata0</p>

#### NXDATA1_f1 field

<p>WIRI</p>

### AXDATA0 register

- Absolute Address: 0x807790
- Base Offset: 0x790
- Size: 0x8

| Bits|Identifier|Access|Reset|Name|
|-----|----------|------|-----|----|
| 31:0|AXDATA0_f0|  rw  |  —  |  — |
|63:32|AXDATA0_f1|  rw  |  —  |  — |

#### AXDATA0_f0 field

<p>This register is shadow, and have the same layout as the nxdata0 register. A write to this register "wakes up" a halted hart, after ddata0 is written, and starts execution from the beginning of the instruction buffer</p>

### AXDATA1 register

- Absolute Address: 0x807798
- Base Offset: 0x798
- Size: 0x8

| Bits|Identifier|Access|Reset|Name|
|-----|----------|------|-----|----|
| 31:0|AXDATA1_f0|  rw  |  —  |  — |
|63:32|AXDATA1_f1|  rw  |  —  |  — |

#### AXDATA1_f0 field

<p>This register is shadow, and have the same layout as the nxdata1 register. A write to this register "wakes up" a halted hart, after ddata0 is written, and starts execution from the beginning of the instruction buffer.</p>

### AXPROGBUFF0 register

- Absolute Address: 0x8077A0
- Base Offset: 0x7A0
- Size: 0x8

|Bits| Identifier|Access|Reset|Name|
|----|-----------|------|-----|----|
|31:0|AXPROGBUFF0|  rw  |  —  |  — |

#### AXPROGBUFF0 field

<p>This register shadows the nxprogbuf0 register. A write to this register wakes up a halted hart, after the instruction buffer is written, and starts execution from the beginning of the instruction buffer, until either an ebreak instruction is executed or the end of the instruction buffer is reached, at which point the hart goes back to being halted.</p>

### AXPROGBUFF1 register

- Absolute Address: 0x8077A8
- Base Offset: 0x7A8
- Size: 0x8

|Bits| Identifier|Access|Reset|Name|
|----|-----------|------|-----|----|
|31:0|AXPROGBUFF1|  rw  |  —  |  — |

#### AXPROGBUFF1 field

<p>This register shadows the nxprogbuf1 register. A write to this register wakes up a halted hart, after the instruction buffer is written, and starts execution from the beginning of the instruction buffer, until either an ebreak instruction is executed or the end of the instruction buffer is reached, at which point the hart goes back to being halted.</p>

### NXPROGBUFF0 register

- Absolute Address: 0x8077B0
- Base Offset: 0x7B0
- Size: 0x8

|Bits| Identifier|Access|Reset|Name|
|----|-----------|------|-----|----|
|31:0|NXPROGBUFF0|  rw  |  —  |  — |

#### NXPROGBUFF0 field

<p>This register holds bits 64-95 of the instruction buffer from where instructions can be executed when the hart is halted (see also abscmd), encoded in the lower half of the register.</p>

### NXPROGBUFF1 register

- Absolute Address: 0x8077B8
- Base Offset: 0x7B8
- Size: 0x8

|Bits| Identifier|Access|Reset|Name|
|----|-----------|------|-----|----|
|31:0|NXPROGBUFF1|  rw  |  —  |  — |

#### NXPROGBUFF1 field

<p>This register holds bits 96-127 of the instruction buffer from where instructions can be executed when the hart is halted (see also abscmd), encoded in the lower half of the register.</p>

### ABSCMD register

- Absolute Address: 0x8077C0
- Base Offset: 0x7C0
- Size: 0x8

<p>This register holds bits 0-63 of the instruction buffer from where instructions can be executed when the hart is halted. The first instruction in the buffer is in the lower order 16 or 32 bits of the ABSCMD register. Writing to this register wakes up a halted hart, after the instruction buffer is written, and starts execution from the beginning of the instruction buffer, until either an ebreak instruction is executed or the end of the instruction buffer is reached, at which point the hart goes back to being halted.</p>

|Bits|Identifier|Access|Reset|Name|
|----|----------|------|-----|----|
|63:0|  ABSCMD  |  rw  |  —  |  — |

## dbg_hart register file

- Absolute Address: 0x808000
- Base Offset: 0x0
- Size: 0x7C8
- Array Dimensions: [16]
- Array Stride: 0x1000
- Total Size: 0x10000

|Offset| Identifier|    Name   |
|------|-----------|-----------|
| 0x780|  NXDATA0  |  NXDATA0  |
| 0x788|  NXDATA1  |  NXDATA1  |
| 0x790|  AXDATA0  |  AXDATA0  |
| 0x798|  AXDATA1  |  AXDATA1  |
| 0x7A0|AXPROGBUFF0|AXPROGBUFF0|
| 0x7A8|AXPROGBUFF1|AXPROGBUFF1|
| 0x7B0|NXPROGBUFF0|NXPROGBUFF0|
| 0x7B8|NXPROGBUFF1|NXPROGBUFF1|
| 0x7C0|   ABSCMD  |   ABSCMD  |

### NXDATA0 register

- Absolute Address: 0x808780
- Base Offset: 0x780
- Size: 0x8

| Bits|Identifier|Access|Reset|Name|
|-----|----------|------|-----|----|
| 31:0|NXDATA0_f0|  rw  |  —  |  — |
|63:32|NXDATA0_f1|  rw  |  —  |  — |

#### NXDATA0_f0 field

<p>The low (nxdata0)  of ddata0</p>

#### NXDATA0_f1 field

<p>WIRI</p>

### NXDATA1 register

- Absolute Address: 0x808788
- Base Offset: 0x788
- Size: 0x8

| Bits|Identifier|Access|Reset|Name|
|-----|----------|------|-----|----|
| 31:0|NXDATA1_f0|  rw  |  —  |  — |
|63:32|NXDATA1_f1|  rw  |  —  |  — |

#### NXDATA1_f0 field

<p>The high (nxdata0)  of ddata0</p>

#### NXDATA1_f1 field

<p>WIRI</p>

### AXDATA0 register

- Absolute Address: 0x808790
- Base Offset: 0x790
- Size: 0x8

| Bits|Identifier|Access|Reset|Name|
|-----|----------|------|-----|----|
| 31:0|AXDATA0_f0|  rw  |  —  |  — |
|63:32|AXDATA0_f1|  rw  |  —  |  — |

#### AXDATA0_f0 field

<p>This register is shadow, and have the same layout as the nxdata0 register. A write to this register "wakes up" a halted hart, after ddata0 is written, and starts execution from the beginning of the instruction buffer</p>

### AXDATA1 register

- Absolute Address: 0x808798
- Base Offset: 0x798
- Size: 0x8

| Bits|Identifier|Access|Reset|Name|
|-----|----------|------|-----|----|
| 31:0|AXDATA1_f0|  rw  |  —  |  — |
|63:32|AXDATA1_f1|  rw  |  —  |  — |

#### AXDATA1_f0 field

<p>This register is shadow, and have the same layout as the nxdata1 register. A write to this register "wakes up" a halted hart, after ddata0 is written, and starts execution from the beginning of the instruction buffer.</p>

### AXPROGBUFF0 register

- Absolute Address: 0x8087A0
- Base Offset: 0x7A0
- Size: 0x8

|Bits| Identifier|Access|Reset|Name|
|----|-----------|------|-----|----|
|31:0|AXPROGBUFF0|  rw  |  —  |  — |

#### AXPROGBUFF0 field

<p>This register shadows the nxprogbuf0 register. A write to this register wakes up a halted hart, after the instruction buffer is written, and starts execution from the beginning of the instruction buffer, until either an ebreak instruction is executed or the end of the instruction buffer is reached, at which point the hart goes back to being halted.</p>

### AXPROGBUFF1 register

- Absolute Address: 0x8087A8
- Base Offset: 0x7A8
- Size: 0x8

|Bits| Identifier|Access|Reset|Name|
|----|-----------|------|-----|----|
|31:0|AXPROGBUFF1|  rw  |  —  |  — |

#### AXPROGBUFF1 field

<p>This register shadows the nxprogbuf1 register. A write to this register wakes up a halted hart, after the instruction buffer is written, and starts execution from the beginning of the instruction buffer, until either an ebreak instruction is executed or the end of the instruction buffer is reached, at which point the hart goes back to being halted.</p>

### NXPROGBUFF0 register

- Absolute Address: 0x8087B0
- Base Offset: 0x7B0
- Size: 0x8

|Bits| Identifier|Access|Reset|Name|
|----|-----------|------|-----|----|
|31:0|NXPROGBUFF0|  rw  |  —  |  — |

#### NXPROGBUFF0 field

<p>This register holds bits 64-95 of the instruction buffer from where instructions can be executed when the hart is halted (see also abscmd), encoded in the lower half of the register.</p>

### NXPROGBUFF1 register

- Absolute Address: 0x8087B8
- Base Offset: 0x7B8
- Size: 0x8

|Bits| Identifier|Access|Reset|Name|
|----|-----------|------|-----|----|
|31:0|NXPROGBUFF1|  rw  |  —  |  — |

#### NXPROGBUFF1 field

<p>This register holds bits 96-127 of the instruction buffer from where instructions can be executed when the hart is halted (see also abscmd), encoded in the lower half of the register.</p>

### ABSCMD register

- Absolute Address: 0x8087C0
- Base Offset: 0x7C0
- Size: 0x8

<p>This register holds bits 0-63 of the instruction buffer from where instructions can be executed when the hart is halted. The first instruction in the buffer is in the lower order 16 or 32 bits of the ABSCMD register. Writing to this register wakes up a halted hart, after the instruction buffer is written, and starts execution from the beginning of the instruction buffer, until either an ebreak instruction is executed or the end of the instruction buffer is reached, at which point the hart goes back to being halted.</p>

|Bits|Identifier|Access|Reset|Name|
|----|----------|------|-----|----|
|63:0|  ABSCMD  |  rw  |  —  |  — |

## dbg_hart register file

- Absolute Address: 0x809000
- Base Offset: 0x0
- Size: 0x7C8
- Array Dimensions: [16]
- Array Stride: 0x1000
- Total Size: 0x10000

|Offset| Identifier|    Name   |
|------|-----------|-----------|
| 0x780|  NXDATA0  |  NXDATA0  |
| 0x788|  NXDATA1  |  NXDATA1  |
| 0x790|  AXDATA0  |  AXDATA0  |
| 0x798|  AXDATA1  |  AXDATA1  |
| 0x7A0|AXPROGBUFF0|AXPROGBUFF0|
| 0x7A8|AXPROGBUFF1|AXPROGBUFF1|
| 0x7B0|NXPROGBUFF0|NXPROGBUFF0|
| 0x7B8|NXPROGBUFF1|NXPROGBUFF1|
| 0x7C0|   ABSCMD  |   ABSCMD  |

### NXDATA0 register

- Absolute Address: 0x809780
- Base Offset: 0x780
- Size: 0x8

| Bits|Identifier|Access|Reset|Name|
|-----|----------|------|-----|----|
| 31:0|NXDATA0_f0|  rw  |  —  |  — |
|63:32|NXDATA0_f1|  rw  |  —  |  — |

#### NXDATA0_f0 field

<p>The low (nxdata0)  of ddata0</p>

#### NXDATA0_f1 field

<p>WIRI</p>

### NXDATA1 register

- Absolute Address: 0x809788
- Base Offset: 0x788
- Size: 0x8

| Bits|Identifier|Access|Reset|Name|
|-----|----------|------|-----|----|
| 31:0|NXDATA1_f0|  rw  |  —  |  — |
|63:32|NXDATA1_f1|  rw  |  —  |  — |

#### NXDATA1_f0 field

<p>The high (nxdata0)  of ddata0</p>

#### NXDATA1_f1 field

<p>WIRI</p>

### AXDATA0 register

- Absolute Address: 0x809790
- Base Offset: 0x790
- Size: 0x8

| Bits|Identifier|Access|Reset|Name|
|-----|----------|------|-----|----|
| 31:0|AXDATA0_f0|  rw  |  —  |  — |
|63:32|AXDATA0_f1|  rw  |  —  |  — |

#### AXDATA0_f0 field

<p>This register is shadow, and have the same layout as the nxdata0 register. A write to this register "wakes up" a halted hart, after ddata0 is written, and starts execution from the beginning of the instruction buffer</p>

### AXDATA1 register

- Absolute Address: 0x809798
- Base Offset: 0x798
- Size: 0x8

| Bits|Identifier|Access|Reset|Name|
|-----|----------|------|-----|----|
| 31:0|AXDATA1_f0|  rw  |  —  |  — |
|63:32|AXDATA1_f1|  rw  |  —  |  — |

#### AXDATA1_f0 field

<p>This register is shadow, and have the same layout as the nxdata1 register. A write to this register "wakes up" a halted hart, after ddata0 is written, and starts execution from the beginning of the instruction buffer.</p>

### AXPROGBUFF0 register

- Absolute Address: 0x8097A0
- Base Offset: 0x7A0
- Size: 0x8

|Bits| Identifier|Access|Reset|Name|
|----|-----------|------|-----|----|
|31:0|AXPROGBUFF0|  rw  |  —  |  — |

#### AXPROGBUFF0 field

<p>This register shadows the nxprogbuf0 register. A write to this register wakes up a halted hart, after the instruction buffer is written, and starts execution from the beginning of the instruction buffer, until either an ebreak instruction is executed or the end of the instruction buffer is reached, at which point the hart goes back to being halted.</p>

### AXPROGBUFF1 register

- Absolute Address: 0x8097A8
- Base Offset: 0x7A8
- Size: 0x8

|Bits| Identifier|Access|Reset|Name|
|----|-----------|------|-----|----|
|31:0|AXPROGBUFF1|  rw  |  —  |  — |

#### AXPROGBUFF1 field

<p>This register shadows the nxprogbuf1 register. A write to this register wakes up a halted hart, after the instruction buffer is written, and starts execution from the beginning of the instruction buffer, until either an ebreak instruction is executed or the end of the instruction buffer is reached, at which point the hart goes back to being halted.</p>

### NXPROGBUFF0 register

- Absolute Address: 0x8097B0
- Base Offset: 0x7B0
- Size: 0x8

|Bits| Identifier|Access|Reset|Name|
|----|-----------|------|-----|----|
|31:0|NXPROGBUFF0|  rw  |  —  |  — |

#### NXPROGBUFF0 field

<p>This register holds bits 64-95 of the instruction buffer from where instructions can be executed when the hart is halted (see also abscmd), encoded in the lower half of the register.</p>

### NXPROGBUFF1 register

- Absolute Address: 0x8097B8
- Base Offset: 0x7B8
- Size: 0x8

|Bits| Identifier|Access|Reset|Name|
|----|-----------|------|-----|----|
|31:0|NXPROGBUFF1|  rw  |  —  |  — |

#### NXPROGBUFF1 field

<p>This register holds bits 96-127 of the instruction buffer from where instructions can be executed when the hart is halted (see also abscmd), encoded in the lower half of the register.</p>

### ABSCMD register

- Absolute Address: 0x8097C0
- Base Offset: 0x7C0
- Size: 0x8

<p>This register holds bits 0-63 of the instruction buffer from where instructions can be executed when the hart is halted. The first instruction in the buffer is in the lower order 16 or 32 bits of the ABSCMD register. Writing to this register wakes up a halted hart, after the instruction buffer is written, and starts execution from the beginning of the instruction buffer, until either an ebreak instruction is executed or the end of the instruction buffer is reached, at which point the hart goes back to being halted.</p>

|Bits|Identifier|Access|Reset|Name|
|----|----------|------|-----|----|
|63:0|  ABSCMD  |  rw  |  —  |  — |

## dbg_hart register file

- Absolute Address: 0x80A000
- Base Offset: 0x0
- Size: 0x7C8
- Array Dimensions: [16]
- Array Stride: 0x1000
- Total Size: 0x10000

|Offset| Identifier|    Name   |
|------|-----------|-----------|
| 0x780|  NXDATA0  |  NXDATA0  |
| 0x788|  NXDATA1  |  NXDATA1  |
| 0x790|  AXDATA0  |  AXDATA0  |
| 0x798|  AXDATA1  |  AXDATA1  |
| 0x7A0|AXPROGBUFF0|AXPROGBUFF0|
| 0x7A8|AXPROGBUFF1|AXPROGBUFF1|
| 0x7B0|NXPROGBUFF0|NXPROGBUFF0|
| 0x7B8|NXPROGBUFF1|NXPROGBUFF1|
| 0x7C0|   ABSCMD  |   ABSCMD  |

### NXDATA0 register

- Absolute Address: 0x80A780
- Base Offset: 0x780
- Size: 0x8

| Bits|Identifier|Access|Reset|Name|
|-----|----------|------|-----|----|
| 31:0|NXDATA0_f0|  rw  |  —  |  — |
|63:32|NXDATA0_f1|  rw  |  —  |  — |

#### NXDATA0_f0 field

<p>The low (nxdata0)  of ddata0</p>

#### NXDATA0_f1 field

<p>WIRI</p>

### NXDATA1 register

- Absolute Address: 0x80A788
- Base Offset: 0x788
- Size: 0x8

| Bits|Identifier|Access|Reset|Name|
|-----|----------|------|-----|----|
| 31:0|NXDATA1_f0|  rw  |  —  |  — |
|63:32|NXDATA1_f1|  rw  |  —  |  — |

#### NXDATA1_f0 field

<p>The high (nxdata0)  of ddata0</p>

#### NXDATA1_f1 field

<p>WIRI</p>

### AXDATA0 register

- Absolute Address: 0x80A790
- Base Offset: 0x790
- Size: 0x8

| Bits|Identifier|Access|Reset|Name|
|-----|----------|------|-----|----|
| 31:0|AXDATA0_f0|  rw  |  —  |  — |
|63:32|AXDATA0_f1|  rw  |  —  |  — |

#### AXDATA0_f0 field

<p>This register is shadow, and have the same layout as the nxdata0 register. A write to this register "wakes up" a halted hart, after ddata0 is written, and starts execution from the beginning of the instruction buffer</p>

### AXDATA1 register

- Absolute Address: 0x80A798
- Base Offset: 0x798
- Size: 0x8

| Bits|Identifier|Access|Reset|Name|
|-----|----------|------|-----|----|
| 31:0|AXDATA1_f0|  rw  |  —  |  — |
|63:32|AXDATA1_f1|  rw  |  —  |  — |

#### AXDATA1_f0 field

<p>This register is shadow, and have the same layout as the nxdata1 register. A write to this register "wakes up" a halted hart, after ddata0 is written, and starts execution from the beginning of the instruction buffer.</p>

### AXPROGBUFF0 register

- Absolute Address: 0x80A7A0
- Base Offset: 0x7A0
- Size: 0x8

|Bits| Identifier|Access|Reset|Name|
|----|-----------|------|-----|----|
|31:0|AXPROGBUFF0|  rw  |  —  |  — |

#### AXPROGBUFF0 field

<p>This register shadows the nxprogbuf0 register. A write to this register wakes up a halted hart, after the instruction buffer is written, and starts execution from the beginning of the instruction buffer, until either an ebreak instruction is executed or the end of the instruction buffer is reached, at which point the hart goes back to being halted.</p>

### AXPROGBUFF1 register

- Absolute Address: 0x80A7A8
- Base Offset: 0x7A8
- Size: 0x8

|Bits| Identifier|Access|Reset|Name|
|----|-----------|------|-----|----|
|31:0|AXPROGBUFF1|  rw  |  —  |  — |

#### AXPROGBUFF1 field

<p>This register shadows the nxprogbuf1 register. A write to this register wakes up a halted hart, after the instruction buffer is written, and starts execution from the beginning of the instruction buffer, until either an ebreak instruction is executed or the end of the instruction buffer is reached, at which point the hart goes back to being halted.</p>

### NXPROGBUFF0 register

- Absolute Address: 0x80A7B0
- Base Offset: 0x7B0
- Size: 0x8

|Bits| Identifier|Access|Reset|Name|
|----|-----------|------|-----|----|
|31:0|NXPROGBUFF0|  rw  |  —  |  — |

#### NXPROGBUFF0 field

<p>This register holds bits 64-95 of the instruction buffer from where instructions can be executed when the hart is halted (see also abscmd), encoded in the lower half of the register.</p>

### NXPROGBUFF1 register

- Absolute Address: 0x80A7B8
- Base Offset: 0x7B8
- Size: 0x8

|Bits| Identifier|Access|Reset|Name|
|----|-----------|------|-----|----|
|31:0|NXPROGBUFF1|  rw  |  —  |  — |

#### NXPROGBUFF1 field

<p>This register holds bits 96-127 of the instruction buffer from where instructions can be executed when the hart is halted (see also abscmd), encoded in the lower half of the register.</p>

### ABSCMD register

- Absolute Address: 0x80A7C0
- Base Offset: 0x7C0
- Size: 0x8

<p>This register holds bits 0-63 of the instruction buffer from where instructions can be executed when the hart is halted. The first instruction in the buffer is in the lower order 16 or 32 bits of the ABSCMD register. Writing to this register wakes up a halted hart, after the instruction buffer is written, and starts execution from the beginning of the instruction buffer, until either an ebreak instruction is executed or the end of the instruction buffer is reached, at which point the hart goes back to being halted.</p>

|Bits|Identifier|Access|Reset|Name|
|----|----------|------|-----|----|
|63:0|  ABSCMD  |  rw  |  —  |  — |

## dbg_hart register file

- Absolute Address: 0x80B000
- Base Offset: 0x0
- Size: 0x7C8
- Array Dimensions: [16]
- Array Stride: 0x1000
- Total Size: 0x10000

|Offset| Identifier|    Name   |
|------|-----------|-----------|
| 0x780|  NXDATA0  |  NXDATA0  |
| 0x788|  NXDATA1  |  NXDATA1  |
| 0x790|  AXDATA0  |  AXDATA0  |
| 0x798|  AXDATA1  |  AXDATA1  |
| 0x7A0|AXPROGBUFF0|AXPROGBUFF0|
| 0x7A8|AXPROGBUFF1|AXPROGBUFF1|
| 0x7B0|NXPROGBUFF0|NXPROGBUFF0|
| 0x7B8|NXPROGBUFF1|NXPROGBUFF1|
| 0x7C0|   ABSCMD  |   ABSCMD  |

### NXDATA0 register

- Absolute Address: 0x80B780
- Base Offset: 0x780
- Size: 0x8

| Bits|Identifier|Access|Reset|Name|
|-----|----------|------|-----|----|
| 31:0|NXDATA0_f0|  rw  |  —  |  — |
|63:32|NXDATA0_f1|  rw  |  —  |  — |

#### NXDATA0_f0 field

<p>The low (nxdata0)  of ddata0</p>

#### NXDATA0_f1 field

<p>WIRI</p>

### NXDATA1 register

- Absolute Address: 0x80B788
- Base Offset: 0x788
- Size: 0x8

| Bits|Identifier|Access|Reset|Name|
|-----|----------|------|-----|----|
| 31:0|NXDATA1_f0|  rw  |  —  |  — |
|63:32|NXDATA1_f1|  rw  |  —  |  — |

#### NXDATA1_f0 field

<p>The high (nxdata0)  of ddata0</p>

#### NXDATA1_f1 field

<p>WIRI</p>

### AXDATA0 register

- Absolute Address: 0x80B790
- Base Offset: 0x790
- Size: 0x8

| Bits|Identifier|Access|Reset|Name|
|-----|----------|------|-----|----|
| 31:0|AXDATA0_f0|  rw  |  —  |  — |
|63:32|AXDATA0_f1|  rw  |  —  |  — |

#### AXDATA0_f0 field

<p>This register is shadow, and have the same layout as the nxdata0 register. A write to this register "wakes up" a halted hart, after ddata0 is written, and starts execution from the beginning of the instruction buffer</p>

### AXDATA1 register

- Absolute Address: 0x80B798
- Base Offset: 0x798
- Size: 0x8

| Bits|Identifier|Access|Reset|Name|
|-----|----------|------|-----|----|
| 31:0|AXDATA1_f0|  rw  |  —  |  — |
|63:32|AXDATA1_f1|  rw  |  —  |  — |

#### AXDATA1_f0 field

<p>This register is shadow, and have the same layout as the nxdata1 register. A write to this register "wakes up" a halted hart, after ddata0 is written, and starts execution from the beginning of the instruction buffer.</p>

### AXPROGBUFF0 register

- Absolute Address: 0x80B7A0
- Base Offset: 0x7A0
- Size: 0x8

|Bits| Identifier|Access|Reset|Name|
|----|-----------|------|-----|----|
|31:0|AXPROGBUFF0|  rw  |  —  |  — |

#### AXPROGBUFF0 field

<p>This register shadows the nxprogbuf0 register. A write to this register wakes up a halted hart, after the instruction buffer is written, and starts execution from the beginning of the instruction buffer, until either an ebreak instruction is executed or the end of the instruction buffer is reached, at which point the hart goes back to being halted.</p>

### AXPROGBUFF1 register

- Absolute Address: 0x80B7A8
- Base Offset: 0x7A8
- Size: 0x8

|Bits| Identifier|Access|Reset|Name|
|----|-----------|------|-----|----|
|31:0|AXPROGBUFF1|  rw  |  —  |  — |

#### AXPROGBUFF1 field

<p>This register shadows the nxprogbuf1 register. A write to this register wakes up a halted hart, after the instruction buffer is written, and starts execution from the beginning of the instruction buffer, until either an ebreak instruction is executed or the end of the instruction buffer is reached, at which point the hart goes back to being halted.</p>

### NXPROGBUFF0 register

- Absolute Address: 0x80B7B0
- Base Offset: 0x7B0
- Size: 0x8

|Bits| Identifier|Access|Reset|Name|
|----|-----------|------|-----|----|
|31:0|NXPROGBUFF0|  rw  |  —  |  — |

#### NXPROGBUFF0 field

<p>This register holds bits 64-95 of the instruction buffer from where instructions can be executed when the hart is halted (see also abscmd), encoded in the lower half of the register.</p>

### NXPROGBUFF1 register

- Absolute Address: 0x80B7B8
- Base Offset: 0x7B8
- Size: 0x8

|Bits| Identifier|Access|Reset|Name|
|----|-----------|------|-----|----|
|31:0|NXPROGBUFF1|  rw  |  —  |  — |

#### NXPROGBUFF1 field

<p>This register holds bits 96-127 of the instruction buffer from where instructions can be executed when the hart is halted (see also abscmd), encoded in the lower half of the register.</p>

### ABSCMD register

- Absolute Address: 0x80B7C0
- Base Offset: 0x7C0
- Size: 0x8

<p>This register holds bits 0-63 of the instruction buffer from where instructions can be executed when the hart is halted. The first instruction in the buffer is in the lower order 16 or 32 bits of the ABSCMD register. Writing to this register wakes up a halted hart, after the instruction buffer is written, and starts execution from the beginning of the instruction buffer, until either an ebreak instruction is executed or the end of the instruction buffer is reached, at which point the hart goes back to being halted.</p>

|Bits|Identifier|Access|Reset|Name|
|----|----------|------|-----|----|
|63:0|  ABSCMD  |  rw  |  —  |  — |

## dbg_hart register file

- Absolute Address: 0x80C000
- Base Offset: 0x0
- Size: 0x7C8
- Array Dimensions: [16]
- Array Stride: 0x1000
- Total Size: 0x10000

|Offset| Identifier|    Name   |
|------|-----------|-----------|
| 0x780|  NXDATA0  |  NXDATA0  |
| 0x788|  NXDATA1  |  NXDATA1  |
| 0x790|  AXDATA0  |  AXDATA0  |
| 0x798|  AXDATA1  |  AXDATA1  |
| 0x7A0|AXPROGBUFF0|AXPROGBUFF0|
| 0x7A8|AXPROGBUFF1|AXPROGBUFF1|
| 0x7B0|NXPROGBUFF0|NXPROGBUFF0|
| 0x7B8|NXPROGBUFF1|NXPROGBUFF1|
| 0x7C0|   ABSCMD  |   ABSCMD  |

### NXDATA0 register

- Absolute Address: 0x80C780
- Base Offset: 0x780
- Size: 0x8

| Bits|Identifier|Access|Reset|Name|
|-----|----------|------|-----|----|
| 31:0|NXDATA0_f0|  rw  |  —  |  — |
|63:32|NXDATA0_f1|  rw  |  —  |  — |

#### NXDATA0_f0 field

<p>The low (nxdata0)  of ddata0</p>

#### NXDATA0_f1 field

<p>WIRI</p>

### NXDATA1 register

- Absolute Address: 0x80C788
- Base Offset: 0x788
- Size: 0x8

| Bits|Identifier|Access|Reset|Name|
|-----|----------|------|-----|----|
| 31:0|NXDATA1_f0|  rw  |  —  |  — |
|63:32|NXDATA1_f1|  rw  |  —  |  — |

#### NXDATA1_f0 field

<p>The high (nxdata0)  of ddata0</p>

#### NXDATA1_f1 field

<p>WIRI</p>

### AXDATA0 register

- Absolute Address: 0x80C790
- Base Offset: 0x790
- Size: 0x8

| Bits|Identifier|Access|Reset|Name|
|-----|----------|------|-----|----|
| 31:0|AXDATA0_f0|  rw  |  —  |  — |
|63:32|AXDATA0_f1|  rw  |  —  |  — |

#### AXDATA0_f0 field

<p>This register is shadow, and have the same layout as the nxdata0 register. A write to this register "wakes up" a halted hart, after ddata0 is written, and starts execution from the beginning of the instruction buffer</p>

### AXDATA1 register

- Absolute Address: 0x80C798
- Base Offset: 0x798
- Size: 0x8

| Bits|Identifier|Access|Reset|Name|
|-----|----------|------|-----|----|
| 31:0|AXDATA1_f0|  rw  |  —  |  — |
|63:32|AXDATA1_f1|  rw  |  —  |  — |

#### AXDATA1_f0 field

<p>This register is shadow, and have the same layout as the nxdata1 register. A write to this register "wakes up" a halted hart, after ddata0 is written, and starts execution from the beginning of the instruction buffer.</p>

### AXPROGBUFF0 register

- Absolute Address: 0x80C7A0
- Base Offset: 0x7A0
- Size: 0x8

|Bits| Identifier|Access|Reset|Name|
|----|-----------|------|-----|----|
|31:0|AXPROGBUFF0|  rw  |  —  |  — |

#### AXPROGBUFF0 field

<p>This register shadows the nxprogbuf0 register. A write to this register wakes up a halted hart, after the instruction buffer is written, and starts execution from the beginning of the instruction buffer, until either an ebreak instruction is executed or the end of the instruction buffer is reached, at which point the hart goes back to being halted.</p>

### AXPROGBUFF1 register

- Absolute Address: 0x80C7A8
- Base Offset: 0x7A8
- Size: 0x8

|Bits| Identifier|Access|Reset|Name|
|----|-----------|------|-----|----|
|31:0|AXPROGBUFF1|  rw  |  —  |  — |

#### AXPROGBUFF1 field

<p>This register shadows the nxprogbuf1 register. A write to this register wakes up a halted hart, after the instruction buffer is written, and starts execution from the beginning of the instruction buffer, until either an ebreak instruction is executed or the end of the instruction buffer is reached, at which point the hart goes back to being halted.</p>

### NXPROGBUFF0 register

- Absolute Address: 0x80C7B0
- Base Offset: 0x7B0
- Size: 0x8

|Bits| Identifier|Access|Reset|Name|
|----|-----------|------|-----|----|
|31:0|NXPROGBUFF0|  rw  |  —  |  — |

#### NXPROGBUFF0 field

<p>This register holds bits 64-95 of the instruction buffer from where instructions can be executed when the hart is halted (see also abscmd), encoded in the lower half of the register.</p>

### NXPROGBUFF1 register

- Absolute Address: 0x80C7B8
- Base Offset: 0x7B8
- Size: 0x8

|Bits| Identifier|Access|Reset|Name|
|----|-----------|------|-----|----|
|31:0|NXPROGBUFF1|  rw  |  —  |  — |

#### NXPROGBUFF1 field

<p>This register holds bits 96-127 of the instruction buffer from where instructions can be executed when the hart is halted (see also abscmd), encoded in the lower half of the register.</p>

### ABSCMD register

- Absolute Address: 0x80C7C0
- Base Offset: 0x7C0
- Size: 0x8

<p>This register holds bits 0-63 of the instruction buffer from where instructions can be executed when the hart is halted. The first instruction in the buffer is in the lower order 16 or 32 bits of the ABSCMD register. Writing to this register wakes up a halted hart, after the instruction buffer is written, and starts execution from the beginning of the instruction buffer, until either an ebreak instruction is executed or the end of the instruction buffer is reached, at which point the hart goes back to being halted.</p>

|Bits|Identifier|Access|Reset|Name|
|----|----------|------|-----|----|
|63:0|  ABSCMD  |  rw  |  —  |  — |

## dbg_hart register file

- Absolute Address: 0x80D000
- Base Offset: 0x0
- Size: 0x7C8
- Array Dimensions: [16]
- Array Stride: 0x1000
- Total Size: 0x10000

|Offset| Identifier|    Name   |
|------|-----------|-----------|
| 0x780|  NXDATA0  |  NXDATA0  |
| 0x788|  NXDATA1  |  NXDATA1  |
| 0x790|  AXDATA0  |  AXDATA0  |
| 0x798|  AXDATA1  |  AXDATA1  |
| 0x7A0|AXPROGBUFF0|AXPROGBUFF0|
| 0x7A8|AXPROGBUFF1|AXPROGBUFF1|
| 0x7B0|NXPROGBUFF0|NXPROGBUFF0|
| 0x7B8|NXPROGBUFF1|NXPROGBUFF1|
| 0x7C0|   ABSCMD  |   ABSCMD  |

### NXDATA0 register

- Absolute Address: 0x80D780
- Base Offset: 0x780
- Size: 0x8

| Bits|Identifier|Access|Reset|Name|
|-----|----------|------|-----|----|
| 31:0|NXDATA0_f0|  rw  |  —  |  — |
|63:32|NXDATA0_f1|  rw  |  —  |  — |

#### NXDATA0_f0 field

<p>The low (nxdata0)  of ddata0</p>

#### NXDATA0_f1 field

<p>WIRI</p>

### NXDATA1 register

- Absolute Address: 0x80D788
- Base Offset: 0x788
- Size: 0x8

| Bits|Identifier|Access|Reset|Name|
|-----|----------|------|-----|----|
| 31:0|NXDATA1_f0|  rw  |  —  |  — |
|63:32|NXDATA1_f1|  rw  |  —  |  — |

#### NXDATA1_f0 field

<p>The high (nxdata0)  of ddata0</p>

#### NXDATA1_f1 field

<p>WIRI</p>

### AXDATA0 register

- Absolute Address: 0x80D790
- Base Offset: 0x790
- Size: 0x8

| Bits|Identifier|Access|Reset|Name|
|-----|----------|------|-----|----|
| 31:0|AXDATA0_f0|  rw  |  —  |  — |
|63:32|AXDATA0_f1|  rw  |  —  |  — |

#### AXDATA0_f0 field

<p>This register is shadow, and have the same layout as the nxdata0 register. A write to this register "wakes up" a halted hart, after ddata0 is written, and starts execution from the beginning of the instruction buffer</p>

### AXDATA1 register

- Absolute Address: 0x80D798
- Base Offset: 0x798
- Size: 0x8

| Bits|Identifier|Access|Reset|Name|
|-----|----------|------|-----|----|
| 31:0|AXDATA1_f0|  rw  |  —  |  — |
|63:32|AXDATA1_f1|  rw  |  —  |  — |

#### AXDATA1_f0 field

<p>This register is shadow, and have the same layout as the nxdata1 register. A write to this register "wakes up" a halted hart, after ddata0 is written, and starts execution from the beginning of the instruction buffer.</p>

### AXPROGBUFF0 register

- Absolute Address: 0x80D7A0
- Base Offset: 0x7A0
- Size: 0x8

|Bits| Identifier|Access|Reset|Name|
|----|-----------|------|-----|----|
|31:0|AXPROGBUFF0|  rw  |  —  |  — |

#### AXPROGBUFF0 field

<p>This register shadows the nxprogbuf0 register. A write to this register wakes up a halted hart, after the instruction buffer is written, and starts execution from the beginning of the instruction buffer, until either an ebreak instruction is executed or the end of the instruction buffer is reached, at which point the hart goes back to being halted.</p>

### AXPROGBUFF1 register

- Absolute Address: 0x80D7A8
- Base Offset: 0x7A8
- Size: 0x8

|Bits| Identifier|Access|Reset|Name|
|----|-----------|------|-----|----|
|31:0|AXPROGBUFF1|  rw  |  —  |  — |

#### AXPROGBUFF1 field

<p>This register shadows the nxprogbuf1 register. A write to this register wakes up a halted hart, after the instruction buffer is written, and starts execution from the beginning of the instruction buffer, until either an ebreak instruction is executed or the end of the instruction buffer is reached, at which point the hart goes back to being halted.</p>

### NXPROGBUFF0 register

- Absolute Address: 0x80D7B0
- Base Offset: 0x7B0
- Size: 0x8

|Bits| Identifier|Access|Reset|Name|
|----|-----------|------|-----|----|
|31:0|NXPROGBUFF0|  rw  |  —  |  — |

#### NXPROGBUFF0 field

<p>This register holds bits 64-95 of the instruction buffer from where instructions can be executed when the hart is halted (see also abscmd), encoded in the lower half of the register.</p>

### NXPROGBUFF1 register

- Absolute Address: 0x80D7B8
- Base Offset: 0x7B8
- Size: 0x8

|Bits| Identifier|Access|Reset|Name|
|----|-----------|------|-----|----|
|31:0|NXPROGBUFF1|  rw  |  —  |  — |

#### NXPROGBUFF1 field

<p>This register holds bits 96-127 of the instruction buffer from where instructions can be executed when the hart is halted (see also abscmd), encoded in the lower half of the register.</p>

### ABSCMD register

- Absolute Address: 0x80D7C0
- Base Offset: 0x7C0
- Size: 0x8

<p>This register holds bits 0-63 of the instruction buffer from where instructions can be executed when the hart is halted. The first instruction in the buffer is in the lower order 16 or 32 bits of the ABSCMD register. Writing to this register wakes up a halted hart, after the instruction buffer is written, and starts execution from the beginning of the instruction buffer, until either an ebreak instruction is executed or the end of the instruction buffer is reached, at which point the hart goes back to being halted.</p>

|Bits|Identifier|Access|Reset|Name|
|----|----------|------|-----|----|
|63:0|  ABSCMD  |  rw  |  —  |  — |

## dbg_hart register file

- Absolute Address: 0x80E000
- Base Offset: 0x0
- Size: 0x7C8
- Array Dimensions: [16]
- Array Stride: 0x1000
- Total Size: 0x10000

|Offset| Identifier|    Name   |
|------|-----------|-----------|
| 0x780|  NXDATA0  |  NXDATA0  |
| 0x788|  NXDATA1  |  NXDATA1  |
| 0x790|  AXDATA0  |  AXDATA0  |
| 0x798|  AXDATA1  |  AXDATA1  |
| 0x7A0|AXPROGBUFF0|AXPROGBUFF0|
| 0x7A8|AXPROGBUFF1|AXPROGBUFF1|
| 0x7B0|NXPROGBUFF0|NXPROGBUFF0|
| 0x7B8|NXPROGBUFF1|NXPROGBUFF1|
| 0x7C0|   ABSCMD  |   ABSCMD  |

### NXDATA0 register

- Absolute Address: 0x80E780
- Base Offset: 0x780
- Size: 0x8

| Bits|Identifier|Access|Reset|Name|
|-----|----------|------|-----|----|
| 31:0|NXDATA0_f0|  rw  |  —  |  — |
|63:32|NXDATA0_f1|  rw  |  —  |  — |

#### NXDATA0_f0 field

<p>The low (nxdata0)  of ddata0</p>

#### NXDATA0_f1 field

<p>WIRI</p>

### NXDATA1 register

- Absolute Address: 0x80E788
- Base Offset: 0x788
- Size: 0x8

| Bits|Identifier|Access|Reset|Name|
|-----|----------|------|-----|----|
| 31:0|NXDATA1_f0|  rw  |  —  |  — |
|63:32|NXDATA1_f1|  rw  |  —  |  — |

#### NXDATA1_f0 field

<p>The high (nxdata0)  of ddata0</p>

#### NXDATA1_f1 field

<p>WIRI</p>

### AXDATA0 register

- Absolute Address: 0x80E790
- Base Offset: 0x790
- Size: 0x8

| Bits|Identifier|Access|Reset|Name|
|-----|----------|------|-----|----|
| 31:0|AXDATA0_f0|  rw  |  —  |  — |
|63:32|AXDATA0_f1|  rw  |  —  |  — |

#### AXDATA0_f0 field

<p>This register is shadow, and have the same layout as the nxdata0 register. A write to this register "wakes up" a halted hart, after ddata0 is written, and starts execution from the beginning of the instruction buffer</p>

### AXDATA1 register

- Absolute Address: 0x80E798
- Base Offset: 0x798
- Size: 0x8

| Bits|Identifier|Access|Reset|Name|
|-----|----------|------|-----|----|
| 31:0|AXDATA1_f0|  rw  |  —  |  — |
|63:32|AXDATA1_f1|  rw  |  —  |  — |

#### AXDATA1_f0 field

<p>This register is shadow, and have the same layout as the nxdata1 register. A write to this register "wakes up" a halted hart, after ddata0 is written, and starts execution from the beginning of the instruction buffer.</p>

### AXPROGBUFF0 register

- Absolute Address: 0x80E7A0
- Base Offset: 0x7A0
- Size: 0x8

|Bits| Identifier|Access|Reset|Name|
|----|-----------|------|-----|----|
|31:0|AXPROGBUFF0|  rw  |  —  |  — |

#### AXPROGBUFF0 field

<p>This register shadows the nxprogbuf0 register. A write to this register wakes up a halted hart, after the instruction buffer is written, and starts execution from the beginning of the instruction buffer, until either an ebreak instruction is executed or the end of the instruction buffer is reached, at which point the hart goes back to being halted.</p>

### AXPROGBUFF1 register

- Absolute Address: 0x80E7A8
- Base Offset: 0x7A8
- Size: 0x8

|Bits| Identifier|Access|Reset|Name|
|----|-----------|------|-----|----|
|31:0|AXPROGBUFF1|  rw  |  —  |  — |

#### AXPROGBUFF1 field

<p>This register shadows the nxprogbuf1 register. A write to this register wakes up a halted hart, after the instruction buffer is written, and starts execution from the beginning of the instruction buffer, until either an ebreak instruction is executed or the end of the instruction buffer is reached, at which point the hart goes back to being halted.</p>

### NXPROGBUFF0 register

- Absolute Address: 0x80E7B0
- Base Offset: 0x7B0
- Size: 0x8

|Bits| Identifier|Access|Reset|Name|
|----|-----------|------|-----|----|
|31:0|NXPROGBUFF0|  rw  |  —  |  — |

#### NXPROGBUFF0 field

<p>This register holds bits 64-95 of the instruction buffer from where instructions can be executed when the hart is halted (see also abscmd), encoded in the lower half of the register.</p>

### NXPROGBUFF1 register

- Absolute Address: 0x80E7B8
- Base Offset: 0x7B8
- Size: 0x8

|Bits| Identifier|Access|Reset|Name|
|----|-----------|------|-----|----|
|31:0|NXPROGBUFF1|  rw  |  —  |  — |

#### NXPROGBUFF1 field

<p>This register holds bits 96-127 of the instruction buffer from where instructions can be executed when the hart is halted (see also abscmd), encoded in the lower half of the register.</p>

### ABSCMD register

- Absolute Address: 0x80E7C0
- Base Offset: 0x7C0
- Size: 0x8

<p>This register holds bits 0-63 of the instruction buffer from where instructions can be executed when the hart is halted. The first instruction in the buffer is in the lower order 16 or 32 bits of the ABSCMD register. Writing to this register wakes up a halted hart, after the instruction buffer is written, and starts execution from the beginning of the instruction buffer, until either an ebreak instruction is executed or the end of the instruction buffer is reached, at which point the hart goes back to being halted.</p>

|Bits|Identifier|Access|Reset|Name|
|----|----------|------|-----|----|
|63:0|  ABSCMD  |  rw  |  —  |  — |

## dbg_hart register file

- Absolute Address: 0x80F000
- Base Offset: 0x0
- Size: 0x7C8
- Array Dimensions: [16]
- Array Stride: 0x1000
- Total Size: 0x10000

|Offset| Identifier|    Name   |
|------|-----------|-----------|
| 0x780|  NXDATA0  |  NXDATA0  |
| 0x788|  NXDATA1  |  NXDATA1  |
| 0x790|  AXDATA0  |  AXDATA0  |
| 0x798|  AXDATA1  |  AXDATA1  |
| 0x7A0|AXPROGBUFF0|AXPROGBUFF0|
| 0x7A8|AXPROGBUFF1|AXPROGBUFF1|
| 0x7B0|NXPROGBUFF0|NXPROGBUFF0|
| 0x7B8|NXPROGBUFF1|NXPROGBUFF1|
| 0x7C0|   ABSCMD  |   ABSCMD  |

### NXDATA0 register

- Absolute Address: 0x80F780
- Base Offset: 0x780
- Size: 0x8

| Bits|Identifier|Access|Reset|Name|
|-----|----------|------|-----|----|
| 31:0|NXDATA0_f0|  rw  |  —  |  — |
|63:32|NXDATA0_f1|  rw  |  —  |  — |

#### NXDATA0_f0 field

<p>The low (nxdata0)  of ddata0</p>

#### NXDATA0_f1 field

<p>WIRI</p>

### NXDATA1 register

- Absolute Address: 0x80F788
- Base Offset: 0x788
- Size: 0x8

| Bits|Identifier|Access|Reset|Name|
|-----|----------|------|-----|----|
| 31:0|NXDATA1_f0|  rw  |  —  |  — |
|63:32|NXDATA1_f1|  rw  |  —  |  — |

#### NXDATA1_f0 field

<p>The high (nxdata0)  of ddata0</p>

#### NXDATA1_f1 field

<p>WIRI</p>

### AXDATA0 register

- Absolute Address: 0x80F790
- Base Offset: 0x790
- Size: 0x8

| Bits|Identifier|Access|Reset|Name|
|-----|----------|------|-----|----|
| 31:0|AXDATA0_f0|  rw  |  —  |  — |
|63:32|AXDATA0_f1|  rw  |  —  |  — |

#### AXDATA0_f0 field

<p>This register is shadow, and have the same layout as the nxdata0 register. A write to this register "wakes up" a halted hart, after ddata0 is written, and starts execution from the beginning of the instruction buffer</p>

### AXDATA1 register

- Absolute Address: 0x80F798
- Base Offset: 0x798
- Size: 0x8

| Bits|Identifier|Access|Reset|Name|
|-----|----------|------|-----|----|
| 31:0|AXDATA1_f0|  rw  |  —  |  — |
|63:32|AXDATA1_f1|  rw  |  —  |  — |

#### AXDATA1_f0 field

<p>This register is shadow, and have the same layout as the nxdata1 register. A write to this register "wakes up" a halted hart, after ddata0 is written, and starts execution from the beginning of the instruction buffer.</p>

### AXPROGBUFF0 register

- Absolute Address: 0x80F7A0
- Base Offset: 0x7A0
- Size: 0x8

|Bits| Identifier|Access|Reset|Name|
|----|-----------|------|-----|----|
|31:0|AXPROGBUFF0|  rw  |  —  |  — |

#### AXPROGBUFF0 field

<p>This register shadows the nxprogbuf0 register. A write to this register wakes up a halted hart, after the instruction buffer is written, and starts execution from the beginning of the instruction buffer, until either an ebreak instruction is executed or the end of the instruction buffer is reached, at which point the hart goes back to being halted.</p>

### AXPROGBUFF1 register

- Absolute Address: 0x80F7A8
- Base Offset: 0x7A8
- Size: 0x8

|Bits| Identifier|Access|Reset|Name|
|----|-----------|------|-----|----|
|31:0|AXPROGBUFF1|  rw  |  —  |  — |

#### AXPROGBUFF1 field

<p>This register shadows the nxprogbuf1 register. A write to this register wakes up a halted hart, after the instruction buffer is written, and starts execution from the beginning of the instruction buffer, until either an ebreak instruction is executed or the end of the instruction buffer is reached, at which point the hart goes back to being halted.</p>

### NXPROGBUFF0 register

- Absolute Address: 0x80F7B0
- Base Offset: 0x7B0
- Size: 0x8

|Bits| Identifier|Access|Reset|Name|
|----|-----------|------|-----|----|
|31:0|NXPROGBUFF0|  rw  |  —  |  — |

#### NXPROGBUFF0 field

<p>This register holds bits 64-95 of the instruction buffer from where instructions can be executed when the hart is halted (see also abscmd), encoded in the lower half of the register.</p>

### NXPROGBUFF1 register

- Absolute Address: 0x80F7B8
- Base Offset: 0x7B8
- Size: 0x8

|Bits| Identifier|Access|Reset|Name|
|----|-----------|------|-----|----|
|31:0|NXPROGBUFF1|  rw  |  —  |  — |

#### NXPROGBUFF1 field

<p>This register holds bits 96-127 of the instruction buffer from where instructions can be executed when the hart is halted (see also abscmd), encoded in the lower half of the register.</p>

### ABSCMD register

- Absolute Address: 0x80F7C0
- Base Offset: 0x7C0
- Size: 0x8

<p>This register holds bits 0-63 of the instruction buffer from where instructions can be executed when the hart is halted. The first instruction in the buffer is in the lower order 16 or 32 bits of the ABSCMD register. Writing to this register wakes up a halted hart, after the instruction buffer is written, and starts execution from the beginning of the instruction buffer, until either an ebreak instruction is executed or the end of the instruction buffer is reached, at which point the hart goes back to being halted.</p>

|Bits|Identifier|Access|Reset|Name|
|----|----------|------|-----|----|
|63:0|  ABSCMD  |  rw  |  —  |  — |

## D_neigh address map

- Absolute Address: 0x900000
- Base Offset: 0x900000
- Size: 0xFFA0

|Offset|  Identifier  |     Name     |
|------|--------------|--------------|
|0xFF80|    hactrl    |    hactrl    |
|0xFF88|   hastatus0  |   hastatus0  |
|0xFF90|   hastatus1  |   hastatus1  |
|0xFF98|and_or_tree_IO|and_or_tree_IO|

### hactrl register

- Absolute Address: 0x90FF80
- Base Offset: 0xFF80
- Size: 0x8

| Bits|Identifier|Access|Reset|Name|
|-----|----------|------|-----|----|
| 15:0| hawindow |  rw  | 0x0 |  — |
|31:16| hart_mask|  rw  | 0x0 |  — |
|47:32| resethalt|   r  | 0x0 |  — |

#### hawindow field

<p>The n-th bit selects the n-th hart in the neighborhood, if hasel is 1. Non-selected harts ignore the reset (except ndmreset), halt and resume requests from the DM.</p>

#### hart_mask field

<p>The n-th bit selects the n-th hart in the neighborhood (independent of the value of hasel). Non-selected harts ignore the reset (except ndmreset), halt and resume requests from the DM.</p>

#### resethalt field

<p>Each bit in this field corresponds to one hart in the neighborhood. While a given resethalt bit is asserted, the corresponding hart will halt upon the next deassertion of its reset. These bits are set for all selected harts when the setresethaltreq bit in the RISC-V Debug Module Control register (DMCTRL) is written. 
This field is cleared for all selected harts when the clrresethaltreq bit in the DMCTRL register is written.</p>

### hastatus0 register

- Absolute Address: 0x90FF88
- Base Offset: 0xFF88
- Size: 0x8

| Bits|Identifier|Access|Reset|Name|
|-----|----------|------|-----|----|
| 15:0|  halted  |   r  | 0x0 |  — |
|31:16|  running |   r  | 0x0 |  — |
|47:32| resumeack|   r  | 0x0 |  — |
|63:48| havereset|   r  | 0x0 |  — |

#### halted field

<p>Each bit in this field corresponds to one of the 16 harts in the neighborhood. The sticky halted bit for a hart is asserted when the hart is in Debug Mode, and hardware clears this bit when the hart receives a reset or resume request.</p>

#### running field

<p>Each bit in this field corresponds to one of the 16 harts in the neighborhood. The sticky running bit for each hart is asserted when a hart finishes resuming execution, and hardware clears this bit when a hart receives a reset or halt request.</p>

#### resumeack field

<p>Each bit in this field corresponds to one of the 16 harts in the neighborhood. The sticky resumeack bit for a hart is asserted when a hart finishes resuming execution, and hardware clears this bit whenever the resumereq bit in the RISC-V Debug Module Control register (DMCTRL) is deasserted.</p>

#### havereset field

<p>Each bit in this field corresponds to one of the 16 harts in the neighborhood. The sticky havereset bit for a hart is asserted when a hart exits reset, and hardware clears this bit whenever the ackhavereset bit in the RISC-V Debug Module Control register (DMCTRL) is written.
Each ET-Minion core contains two harts; however, it is not possible to reset an individual hart within an ET-Minion core. Therefore, the havereset field can be implemented as eight physical bits, with bits 2m and 2m+1 corresponding to the m-th Minion hardwired to the same value.
For example, bits 0 and 1 correspond to the two harts in Minion core 0, but only one of these bits is required to reset both threads. Similarly, bits 2 and 3 correspond to ET-Minion core 1, and so on</p>

### hastatus1 register

- Absolute Address: 0x90FF90
- Base Offset: 0xFF90
- Size: 0x8

| Bits|Identifier|Access|Reset|Name|
|-----|----------|------|-----|----|
| 15:0|   busy   |   r  | 0x0 |  — |
|31:16| exception|  rw  | 0x0 |  — |
|47:32|   error  |  rw  | 0x0 |  — |

#### busy field

<p>Each bit in this field corresponds to one of the 16 harts in the neighborhood, and a bit is set to 1 while the corresponding hart executes an Abstract Command or a Program Buffer program.</p>

#### exception field

<p>Each bit in this field corresponds to one of the 16 harts in the neighborhood. A bit is set to 1 if the execution of an Abstract Command or a Program Buffer program by the corresponding hart generates an exception. These bits are sticky and must be cleared using an explicit write to the register.</p>

#### error field

<p>Each bit in this field corresponds to one of the 16 harts in the neighborhood. A bit is set to 1 if the execution of an Abstract Command or a Program Buffer program by the corresponding hart cannot complete, but no exception is generated. 
Examples of this include writes to ET-Minion core registers such as abscmd, axprogbuf0, axprogbuf1, nxprogbuf0, nxprogbuf1, and so on. These bits are sticky and must be cleared using an explicit write to the register.</p>

### and_or_tree_IO register

- Absolute Address: 0x90FF98
- Base Offset: 0xFF98
- Size: 0x8

|Bits|  Identifier  |Access|Reset|Name|
|----|--------------|------|-----|----|
|  0 |   anyhalted  |   r  |  —  |  — |
|  1 |   allhalted  |   r  |  —  |  — |
|  2 |  anyrunning  |   r  |  —  |  — |
|  3 |  allrunning  |   r  |  —  |  — |
|  4 | anyresumeack |   r  |  —  |  — |
|  5 | allresumeack |   r  |  —  |  — |
|  6 | anyhavereset |   r  |  —  |  — |
|  7 | allhavereset |   r  |  —  |  — |
|  8 |anyunavailable|   r  |  —  |  — |
|  9 |  anyselected |   r  |  —  |  — |

#### anyhalted field

<p>Hardware sets this bit whenever any of the 16 bits of the halted field in the Esperanto Neighborhood Hart Status 0 register for the selected harts is set, and the hasel bit in the RISC-V Debug Module Control register (DMCTRL) is 1.</p>

#### allhalted field

<p>Hardware sets this bit when all 16 bits of the halted field in the Esperanto Neighborhood Hart Status 0 register for the selected harts are set, and the hasel bit in the RISC-V Debug Module Control register (DMCTRL) is 1.</p>

#### anyrunning field

<p>Hardware sets this bit whenever any of the 16 bits of the running field in the Esperanto Neighborhood Hart Status 0 register for the selected harts is set, and the hasel bit in the RISC-V Debug Module Control register (DMCTRL) is 1.</p>

#### allrunning field

<p>Hardware sets this bit whenever all 16 bits of the running field in the Esperanto Neighborhood Hart Status 0 register for the selected harts are set, and the hasel bit in the RISC-V Debug Module Control register (DMCTRL) is 1.</p>

#### anyresumeack field

<p>Hardware sets this bit whenever any of the 16 bits of the resumeack field in the Esperanto Neighborhood Hart Status 0 register for the selected harts is set, and the hasel bit in the RISC-V Debug Module Control register (DMCTRL) is 1.</p>

#### allresumeack field

<p>Hardware sets this bit whenever any of the 16 bits of the resumeack field in the Esperanto Neighborhood Hart Status 0 register for the selected harts is set, and the hasel bit in the RISC-V Debug Module Control register (DMCTRL) is 1.</p>

#### anyhavereset field

<p>Hardware sets this bit whenever any 16 bits of the havereset field in the Esperanto Neighborhood Hart Status 0 register for the selected harts are set, and the hasel bit in the RISC-V Debug Module Control register (DMCTRL) is 1.</p>

#### allhavereset field

<p>Hardware sets this bit whenever all of the 16 bits of the havereset field in the Esperanto Neighborhood Hart Status 0 register for the selected harts is set, and the hasel bit in the RISC-V Debug Module Control register (DMCTRL) is 1.</p>

#### anyunavailable field

<p>Hardware sets this bit when any of the selected harts in the neighborhood are unavailable, and the hasel bit in the RISC-V Debug Module Control register (DMCTRL) is 1. A selected hart is considered unavailable if both HASTATUS0.halted[n] and HASTATUS0.running[n] are 0.</p>

#### anyselected field

<p>Hardware sets this bit when any of the harts in the neighborhood are selected, and the hasel bit in the RISC-V Debug Module Control register (DMCTRL) is 1.</p>

## D_cpu address map

- Absolute Address: 0xB5F000
- Base Offset: 0xB5F000
- Size: 0xFD8

|Offset|Identifier|   Name   |
|------|----------|----------|
| 0xF88|  dmctrl  |  dmctrl  |
| 0xF90| sm_config| sm_config|
| 0xF98|sm_trigger|sm_trigger|
| 0xFA8| sm_match | sm_match |
| 0xFB0|sm_filter0|sm_filter0|
| 0xFB8|sm_filter1|sm_filter1|
| 0xFC0|sm_filter2|sm_filter2|
| 0xFC8| sm_data0 | sm_data0 |
| 0xFD0| sm_data1 | sm_data1 |

### dmctrl register

- Absolute Address: 0xB5FF88
- Base Offset: 0xF88
- Size: 0x8

|Bits|   Identifier  |Access|Reset|Name|
|----|---------------|------|-----|----|
|  0 |    dmactive   |  rw  | 0x0 |  — |
|  1 |    ndmreset   |  rw  |  —  |  — |
|  2 |clrresethaltreq|  rw  |  —  |  — |
|  3 |setresethaltreq|  rw  |  —  |  — |
|25:4|  reserved25_4 |   r  |  —  |  — |
| 26 |     hasel     |  rw  |  —  |  — |
| 27 |   reserved27  |   r  |  —  |  — |
| 28 |  ackhavereset |  rw  |  —  |  — |
| 29 |   hartreset   |  rw  |  —  |  — |
| 30 |   resumereq   |  rw  |  —  |  — |
| 31 |    haltreq    |  rw  |  —  |  — |

#### dmactive field

<p>This bit controls the dmactive signal from the DM; it is broadcast to all Minion shires. When the signal is low it should hold at reset the debug logic of the receiving harts, and of the DM. After writing 1, the debugger should poll DMCTRL until dmactive is high. Hardware may take an arbitrarily long time to initialize and will indicate completion by setting dmactive to 1</p>

#### ndmreset field

<p>This bit controls the ndmreset signal from the DM; it is broadcast to all Minion shires. The signal should reset every part of the receiving shire, except for any logic required for debug.</p>

#### clrresethaltreq field

<p>This bit controls the clrresethaltreq signal from the DM; it is broadcast to all Minion shires. Writing 1 to this bit clears the HACTRL.resethalt bits of all currently selected harts. This bit always reads as 0.</p>

#### setresethaltreq field

<p>This bit controls the setresethaltreq signal from the DM; it is broadcast to all Minion shires. Writing 1 to this bit sets the HACTRL.resethalt bits of all currently selected harts, unless clrresethaltreq is simultaneously set to 1. This bit always reads as 0.</p>

#### reserved25_4 field

<p>WARL(0)</p>

#### hasel field

<p>This bit controls the hasel signal from the DM; it is broadcast to all Minion shires. If set to 0, then DM requests, such as halt and resume apply only to the harts that are selected by HACTRL.hartmask. If set to 1, then DM requests apply to all harts selected by the union of HACTRL.hartmask and HACTRL.hawindow.</p>

#### reserved27 field

<p>WARL(0)</p>

#### ackhavereset field

<p>This bit controls the ackhavereset signal from the DM; it is broadcast to all Minion shires. Writing 1 to this bit clears HASTATUS0.havereset for all currently selected harts. This bit always reads as 0.</p>

#### hartreset field

<p>This bit controls the hartreset signal from the DM; it is broadcast to all Minion shires.</p>

#### resumereq field

<p>This bit controls the resumereq signal from the DM; it is broadcast to all Minion shires. Writing 1 to this bit causes the currently selected harts to resume and clears the HASTATUS0.resumeack bit for those harts. This bit always reads as 0.</p>

#### haltreq field

<p>Bits [31:31]: This bit controls the haltreq signal from the DM; it is broadcast to all Minion shires. Writing 1 to this bit sets the halt request bit for all currently selected harts. Writing 0 to this bit clears the halt request bit for all currently selected harts. This may cancel outstanding halt requests for those harts. This bit always reads as 0.</p>

### sm_config register

- Absolute Address: 0xB5FF90
- Base Offset: 0xF90
- Size: 0x8

|Bits|  Identifier |Access|Reset|Name|
|----|-------------|------|-----|----|
| 6:0| sm_data_sel |  rw  | 0x0 |  — |
|10:7|sm_target_sel|  rw  |  —  |  — |
| 11 |  sm_enable  |  rw  |  —  |  — |

#### sm_data_sel field

<p>Data selector within each target</p>

#### sm_target_sel field

<p>Target selector:</p>

#### sm_enable field

<p>Status Monitor logic enable.</p>

### sm_trigger register

- Absolute Address: 0xB5FF98
- Base Offset: 0xF98
- Size: 0x8

|Bits|Identifier|Access|Reset|Name|
|----|----------|------|-----|----|
|  0 |sm_trigger|   r  |  —  |  — |

#### sm_trigger field

<p>Writing this bit triggers a simultaneous snapshot captured in all the SM ESRs</p>

### sm_match register

- Absolute Address: 0xB5FFA8
- Base Offset: 0xFA8
- Size: 0x8

|Bits|Identifier|Access|Reset|Name|
|----|----------|------|-----|----|
|63:0| sm_match |   r  |  —  |  — |

#### sm_match field

<p>Match bus</p>

### sm_filter0 register

- Absolute Address: 0xB5FFB0
- Base Offset: 0xFB0
- Size: 0x8

|Bits|Identifier|Access|Reset|Name|
|----|----------|------|-----|----|
|63:0|sm_filter0|   r  |  —  |  — |

#### sm_filter0 field

<p>Filter bus [63:0]</p>

### sm_filter1 register

- Absolute Address: 0xB5FFB8
- Base Offset: 0xFB8
- Size: 0x8

|Bits|Identifier|Access|Reset|Name|
|----|----------|------|-----|----|
|63:0|sm_filter1|   r  |  —  |  — |

#### sm_filter1 field

<p>Filter bus [127:64]</p>

### sm_filter2 register

- Absolute Address: 0xB5FFC0
- Base Offset: 0xFC0
- Size: 0x8

|Bits|Identifier|Access|Reset|Name|
|----|----------|------|-----|----|
|63:0|sm_filter2|   r  |  —  |  — |

#### sm_filter2 field

<p>Filter bus [191:128]</p>

### sm_data0 register

- Absolute Address: 0xB5FFC8
- Base Offset: 0xFC8
- Size: 0x8

|Bits|Identifier|Access|Reset|Name|
|----|----------|------|-----|----|
|63:0| sm_data0 |   r  |  —  |  — |

#### sm_data0 field

<p>Data bus [63:0]</p>

### sm_data1 register

- Absolute Address: 0xB5FFD0
- Base Offset: 0xFD0
- Size: 0x8

|Bits|Identifier|Access|Reset|Name|
|----|----------|------|-----|----|
|63:0| sm_data1 |   r  |  —  |  — |

#### sm_data1 field

<p>Data bus [127:64]</p>

## Machine_neigh address map

- Absolute Address: 0xD00000
- Base Offset: 0xD00000
- Size: 0x98

|Offset|      Identifier      |         Name         |
|------|----------------------|----------------------|
| 0x18 |      minion_boot     |      minion_boot     |
| 0x38 |         mprot        |         mprot        |
| 0x68 |       pmu_ctrl       |       pmu_ctrl       |
| 0x70 |     neigh_chicken    |     neigh_chicken    |
| 0x78 |  icache_err_log_ctl  |  icache_err_log_ctl  |
| 0x80 |  icache_err_log_info |  icache_err_log_info |
| 0x88 |icache_err_log_address|icache_err_log_address|
| 0x90 | icache_sbe_dbe_counts| icache_sbe_dbe_counts|

### minion_boot register

- Absolute Address: 0xD00018
- Base Offset: 0x18
- Size: 0x8

|Bits| Identifier|Access|  Reset  |Name|
|----|-----------|------|---------|----|
|47:0|minion_boot|  rw  |0x2008000|  — |

#### minion_boot field

<p>Each neighborhood contains a MINION_BOOT ESR that determines the initial PC of the Minions in the neighborhood after reset. This register holds the 48 least significant bits of the reset PC, bits 63:48 of the PC are ignored on writes. Software must make sure that fetching from the PC value written in this register will not generate any faults.</p>

### mprot register

- Absolute Address: 0xD00038
- Base Offset: 0x38
- Size: 0x8

|Bits|Identifier|Access|Reset|Name|
|----|----------|------|-----|----|
| 3:0|smode_size|  rw  | 0x0 |  — |
| 7:4|mmode_size|  rw  | 0x0 |  — |
|  8 | mprot_en |  rw  | 0x0 |  — |

#### smode_size field

<p>Size of the MRAM region which can be only accessed by Smode. The size is 4KB<em>(2^SMODE_SIZE) - 4KB</em>(2^MMODE_SIZE) starting at offset 4KB*(2^MMODE_SIZE) in the MRAM.</p>

#### mmode_size field

<p>Size of the MRAM region which can be only accessed by Mmode. The size is 4KB*(2^MMODE_SIZE) starting at offset 0 in the MRAM.</p>

#### mprot_en field

<p>Enable privilege based memory protection.</p>

### pmu_ctrl register

- Absolute Address: 0xD00068
- Base Offset: 0x68
- Size: 0x8

|Bits|  Identifier |Access|Reset|Name|
|----|-------------|------|-----|----|
|  0 |disable_clock|  rw  | 0x0 |  — |

#### disable_clock field

<p>Set this bit to disable the clock</p>

### neigh_chicken register

- Absolute Address: 0xD00070
- Base Offset: 0x70
- Size: 0x8

|Bits|   Identifier   |Access|Reset|Name|
|----|----------------|------|-----|----|
|  0 |  bypass_icache |  rw  | 0x0 |  — |
|  1 |  bypass_dcache |  rw  | 0x0 |  — |
|  2 | force_dest_fifo|  rw  | 0x0 |  — |
| 4:3|    dest_fifo   |  rw  | 0x0 |  — |
|  5 |force_all_agents|  rw  | 0x0 |  — |
|  6 |  agent_forced  |  rw  | 0x0 |  — |

#### bypass_icache field

<p>Setting this bit bypasses the instruction cache.</p>

#### bypass_dcache field

<p>Setting this bit bypasses the data cache.</p>

#### force_dest_fifo field

<p>If enabled, all traffic from agents specified by the force_all_agents and agent_forced fields in the neighborhood will be routed to the destination specified in the dest_fifo field.</p>

#### dest_fifo field

<p>Destination to which requests are routed when the force_dest_fifo bit of this register is set:</p>

#### force_all_agents field

<p>When the force_dest_fifo bit and this bit are set, requests from all agents in the neighborhood go to the FIFO selected by the dest_fifo field.</p>

#### agent_forced field

<p>When the force_dest_fifo bit is set and the force_all_agents bit is clear, this field selects agents from which requests will be forced to go to the FIFO selected by the dest_fifo field. 0: Requests are routed to the corresponding bank FIFO, according to the destination address.
1: Requests are routed to the UC FIFO. 2-3: Reserved</p>

### icache_err_log_ctl register

- Absolute Address: 0xD00078
- Base Offset: 0x78
- Size: 0x8

|Bits|     Identifier     |Access|Reset|Name|
|----|--------------------|------|-----|----|
| 2:0|err_interrupt_enable|  rw  | 0x6 |  — |

#### err_interrupt_enable field

<p>This 3-bit field is used to enable or prevent certain categories of logged errors from generating interrupts or sending error responses. In general, single-bit ECC errors should not generate an interrupt. Software may choose whether the ECC error counter saturation should generate an interrupt. Each bit has a specific meaning: Bit 0 enables single-bit ECC errors, Bit 1 enables double-bit ECC errors, and Bit 2 enables ECC counter saturation. Error codes that are not enabled are still logged but have lower priority than errors that generate interrupts. Masked interrupts do not prevent subsequent unmasked errors from being recorded. When an enabled interrupt error overwrites a masked error, the multiple error bit is not set, and multiple masked errors also do not cause the multiple error bit to be set. The multiple error bit is intended to indicate that a fatal error was missed.</p>

### icache_err_log_info register

- Absolute Address: 0xD00080
- Base Offset: 0x80
- Size: 0x8

|Bits|Identifier|Access|Reset|Name|
|----|----------|------|-----|----|
|  0 |   valid  |  rw  | 0x0 |  — |
|  1 |   multi  |  rw  | 0x0 |  — |
|  2 |  enabled |  rw  | 0x0 |  — |
|  3 | reserved |   r  |  —  |  — |
| 7:4| err_code |  rw  | 0x0 |  — |
|63:8|   info   |  rw  |  —  |  — |

#### valid field

<p>When set, this bit indicates an error has occurred and that the register contents are valid. This can be cleared by writing a 1 to this field and writing the matching code to err_code.</p>

#### multi field

<p>When set, this bit indicates that multiple errors have occurred. When an error is detected, the esr_icache_err_log_info register records the details of the first error encountered. Any subsequent errors set this bit to indicate that another fatal error occurred, but their details are not recorded.</p>

#### enabled field

<p>The error detected was enabled by esr_icache_err_log_ctl.err_interrupt_enable. Error codes that are not enabled are still logged, but they have lower priority than errors that generate interrupts.</p>

#### err_code field

<p>This 4-bit field indicates the type of error. This field is used to determine how to decode the info bits in positions 63:8.
              0: ECC single-bit error
              1: ECC double-bit error
              2: ECC error counter saturation</p>

#### info field

<p>Additional information about the error. The format of this field is dependent upon the type of error.
SBE_DBE format:
    - Bits 14:8: set. This field indicates the way that contained the error
    - Bits 16:15: way. This field indicates the way that contained the error
    - Bits 47:40: error_bits.  Indicates which doubleword contained the error. The error_bits field is multi-hot.
ECC_Saturation format:
    - Bit 52: double. This bit indicates the type of error counter that saturated and is encoded as follows:
        0: The single-bit error counter saturated.
        1: The double-bit error counter saturated.</p>

### icache_err_log_address register

- Absolute Address: 0xD00088
- Base Offset: 0x88
- Size: 0x8

|Bits|        Identifier       |Access|Reset|Name|
|----|-------------------------|------|-----|----|
|33:0|icache_err_log_address_f0|   r  | 0x0 |  — |

#### icache_err_log_address_f0 field

<p>This register contains the physical address (PA) associated with the error if it is available.</p>

### icache_sbe_dbe_counts register

- Absolute Address: 0xD00090
- Base Offset: 0x90
- Size: 0x8

<p>Single-bit errors (SBEs) are corrected so normal processing can continue, and no error response is sent to the requesting core because the request was processed correctly. SBEs should be logged locally, and no action should be taken.
Double-bit errors (DBEs) cannot be corrected and are considered fatal, requiring immediate reporting. SBE and DBE responses occur once per ECC protection, meaning one error bit per dword in the cache line (8 bits). 
The SBE counter is 8 bits and saturates at 255, while the DBE counter is 3 bits and saturates at 7. All fields of this register are read-only. Writing all ones to a field clears its respective counter. 
If a counter saturates, it can be error-logged as described in the icache_err_log_info register.</p>

|Bits|Identifier|Access|Reset|Name|
|----|----------|------|-----|----|
| 7:0|sbe_counts|  rw  | 0x0 |  — |
|10:8|dbe_counts|  rw  |  —  |  — |

#### sbe_counts field

<p>This field stores the data RAM single-bit error count.</p>

#### dbe_counts field

<p>This field stores the data RAM double-bit error count.</p>

## Machine_cpu address map

- Absolute Address: 0xF40000
- Base Offset: 0xF40000
- Size: 0x318

|Offset|     Identifier    |        Name       |
|------|-------------------|-------------------|
| 0x000|   minion_feature  |   minion_feature  |
| 0x010|  thread1_disable  |  thread1_disable  |
| 0x088|ipi_redirect_filter|ipi_redirect_filter|
| 0x090|    ipi_trigger    |    ipi_trigger    |
| 0x098| ipi_trigger_clear | ipi_trigger_clear |
| 0x200|       mtime       |       mtime       |
| 0x208|     mtime_cmp     |     mtime_cmp     |
| 0x210|    time_config    |    time_config    |
| 0x218| mtime_local_target| mtime_local_target|
| 0x240|  thread0_disable  |  thread0_disable  |
| 0x308|  icache_mprefetch |  icache_mprefetch |
| 0x310|   clk_gate_ctrl   |   clk_gate_ctrl   |

### minion_feature register

- Absolute Address: 0xF40000
- Base Offset: 0x0
- Size: 0x8

|Bits|      Identifier      |Access|Reset|Name|
|----|----------------------|------|-----|----|
|  0 |      trap_on_gfx     |  rw  | 0x1 |  — |
|  1 |      trap_on_ml      |  rw  | 0x0 |  — |
|  2 |  trap_on_u_cacheops  |  rw  | 0x0 |  — |
|  3 |     trap_on_u_scp    |  rw  | 0x0 |  — |
|  4 |disable_multithreading|  rw  | 0x0 |  — |
|  5 |  disable_lock_unlock |  rw  | 0x0 |  — |

#### trap_on_gfx field

<p>Set this bit to trap on GFX. When this bit is enabled, GFX instructions generate an illegal instruction exception.</p>

#### trap_on_ml field

<p>Set this bit to trap on ML. When this bit is set, tensor pseudo-instructions generate an illegal instruction exception.</p>

#### trap_on_u_cacheops field

<p>Setting this bit disables user cacheops. If set, user mode will not have access to the cache op pseudo-instructions.</p>

#### trap_on_u_scp field

<p>Setting this bit disables the user scratchpad. When set, user mode cannot enable or disable the scratchpad.</p>

#### disable_multithreading field

<p>Set this bit to disable multithreading, making only thread 0 available. The hart IDs for all thread 0s in a Shire must be contiguous.</p>

#### disable_lock_unlock field

<p>Set this bit to disable lock/unlock. When set, user mode cannot use the LockVA and UnlockVA operations.</p>

### thread1_disable register

- Absolute Address: 0xF40010
- Base Offset: 0x10
- Size: 0x8

|Bits|    Identifier    |Access|Reset|Name|
|----|------------------|------|-----|----|
| 7:0|thread1_disable_f0|  rw  | 0xFF|  — |

#### thread1_disable_f0 field

<p>Each bit of this register corresponds to thread 1 of each Minion core in the Neighborhood. When a bit is set, thread 1 of the corresponding Minion is disabled. The field is encoded as follows:
            Bit 0: Minion 0, thread 1
            Bit 1: Minion 1, thread 1
            Bit 2: Minion 2, thread 1
            …
            Bit 7: Minion 7, thread 1</p>

### ipi_redirect_filter register

- Absolute Address: 0xF40088
- Base Offset: 0x88
- Size: 0x8

|Bits|     Identifier    |Access| Reset|Name|
|----|-------------------|------|------|----|
|15:0|ipi_redirect_filter|  rw  |0xFFFF|  — |

#### ipi_redirect_filter field

<p>ipi_redirect_filter Writing a 16-bit value to this register sets the hart ID filter mask for subsequent writes to the IPI_REDIRECT_TRIGGER register. This register is used by privileged software (M-Mode) to prevent accidental user-level redirections. Its value is ANDed with the IPI_REDIRECT_TRIGGER ESR before sending any user-level redirections; setting this register to 0x0 disables all user-level redirections. The encoding of this register is as follows: 
            Bit 0: Minion 0, hart 0
            Bit 1: Minion 0, hart 1
            Bit 2: Minion 1, hart 0
            Bit 3: Minion 1, hart 1
            …
            Bit 12: Minion 6, hart 0
            Bit 13: Minion 6, hart 1
            Bit 14: Minion 7, hart 0
            Bit 15: Minion 7, hart 1</p>

### ipi_trigger register

- Absolute Address: 0xF40090
- Base Offset: 0x90
- Size: 0x8

|Bits| Identifier|Access|Reset|Name|
|----|-----------|------|-----|----|
|15:0|ipi_trigger|  rw  | 0x0 |  — |

#### ipi_trigger field

<p>Machine-level software writes a a bit-mask into this register to set the MSIP bit of one or more harts in the Neighborhood. The encoding of this register is as follows:
Bit 0: Minion 0, hart 0
Bit 1: Minion 0, hart 1
Bit 2: Minion 1, hart 0
Bit 3: Minion 1, hart 1
…
Bit 12: Minion 6, hart 0
Bit 13: Minion 6, hart 1
Bit 14: Minion 7, hart 0
Bit 15: Minion 7, hart 1
Reading this register reads the value of the MSIP bits of the harts in the Neighborhood. Writing a value with the i-th bit set to 0,
will have no effect on the value of the MSIP bit of the corresponding hart.</p>

### ipi_trigger_clear register

- Absolute Address: 0xF40098
- Base Offset: 0x98
- Size: 0x8

|Bits|    Identifier   |Access|Reset|Name|
|----|-----------------|------|-----|----|
|15:0|ipi_trigger_clear|   r  | 0x0 |  — |

#### ipi_trigger_clear field

<p>Machine-level software writes a bit-mask into this register to clear the MSIP bit of one or more harts in the Neighborhood. The encoding of this register is as follows:
            Bit 0: Minion 0, hart 0
            Bit 1: Minion 0, hart 1
            Bit 2: Minion 1, hart 0
            Bit 3: Minion 1, hart 1
            …
            Bit 12: Minion 6, hart 0
            Bit 13: Minion 6, hart 1
            Bit 14: Minion 7, hart 0
            Bit 15: Minion 7, hart 1
            Reading this register returns a 0x0 value.</p>

### mtime register

- Absolute Address: 0xF40200
- Base Offset: 0x200
- Size: 0x8

|Bits|Identifier|Access|Reset|Name|
|----|----------|------|-----|----|
|63:0|   mtime  |  rw  | 0x0 |  — |

#### mtime field

<p>This register matches the definition on the RISCV Privilege v1.1 specification. 64-bit timer counter. This timer increments in 100ns intervals. The mtimecmp register is compared to the value in this register. When the two are equal, a timer interrupt is generated.</p>

### mtime_cmp register

- Absolute Address: 0xF40208
- Base Offset: 0x208
- Size: 0x8

|Bits|Identifier|Access|Reset|Name|
|----|----------|------|-----|----|
|63:0| mtime_cmp|  rw  | 0x0 |  — |

#### mtime_cmp field

<p>This register matches the definition on the RISCV Privilege v1.1 specification. 64-bit timer counter compare. Software sets the desired timer interval using this register. The value is then compared to the value in the mtime register. When the two are equal, an timer interrupt is generated.</p>

### time_config register

- Absolute Address: 0xF40210
- Base Offset: 0x210
- Size: 0x8

|Bits|     Identifier    |Access|Reset|Name|
|----|-------------------|------|-----|----|
| 0:6|prescalar_threshold|  rw  | 0x28|  — |
|  7 |   ref_clock_mux   |  rw  | 0x0 |  — |

#### prescalar_threshold field

<p>The value in this field will be used as the timer prescalar threshold.</p>

#### ref_clock_mux field

<p>When this field holds 0 the timer is driven by the external reference clock. When this field holds 1 the timer is driven by the same clock that drives the cpu subsystem (external clock or ring oscillator)</p>

### mtime_local_target register

- Absolute Address: 0xF40218
- Base Offset: 0x218
- Size: 0x8

|Bits|    Identifier    |Access| Reset|Name|
|----|------------------|------|------|----|
|15:0|mtime_local_target|  rw  |0xFFFF|  — |

#### mtime_local_target field

<p>This register controls which harts receive a timer interrupt. If bit N in mtime_local_target register is 1, hart N would be able to observe a timer interrupt in mip CSR if the mtimecmp is greater than mtime. If bit N is 0, the interrupt will be masked and not observable on hart N mip CSR.</p>

### thread0_disable register

- Absolute Address: 0xF40240
- Base Offset: 0x240
- Size: 0x8

|Bits|   Identifier  |Access|Reset|Name|
|----|---------------|------|-----|----|
| 7:0|thread0_disable|  rw  | 0xFE|  — |

#### thread0_disable field

<p>Each bit of this register corresponds to thread 0 of each Minion core in the Neighborhood. When a bit is set, thread 0 of the corresponding Minion is disabled. The field is encoded as follows:  Bit 0: Minion 0, thread 0 Bit 1: Minion 1, thread 0 Bit 2: Minion 2, thread 0 Bit 7: Minion 7, thread 0</p>

### icache_mprefetch register

- Absolute Address: 0xF40308
- Base Offset: 0x308
- Size: 0x8

<p>A write in this ESR prefetches Count+1 consecutive lines starting at address Address to the instruction cache.
               The privilege mode associated with this operation is Machine-mode, and all memory accesses are performed with Machine-mode privileges.
               If any of the memory requests generated by the prefetch engine generates an exception, the line is not prefetched and the operation continues with the next line.
               Reading this register returns the idle status of the prefetch engine.</p>

|Bits|Identifier|Access|Reset|Name|
|----|----------|------|-----|----|
| 5:0|   count  |  rw  | 0x0 |  — |
|47:6|  address |  rw  | 0x0 |  — |

#### count field

<p>Number of lines to be prefetched (minus 1) starting at the given Address.</p>

#### address field

<p>Bits [47:6] of the address where prefetching should start from (aligned to a cacheline).</p>

### clk_gate_ctrl register

- Absolute Address: 0xF40310
- Base Offset: 0x310
- Size: 0x8

|Bits|           Identifier          |Access|Reset|Name|
|----|-------------------------------|------|-----|----|
|  0 | min_intpipe_clock_gate_disable|  rw  | 0x0 |  — |
|  1 | min_vpulane_clock_gate_disable|  rw  | 0x0 |  — |
|  2 | min_vputima_clock_gate_disable|  rw  | 0x0 |  — |
|  3 |min_vputrans_clock_gate_disable|  rw  | 0x0 |  — |
|  4 | min_dcache_clock_gate_disable |  rw  |  —  |  — |
|  5 |           reserved0           |   r  |  —  |  — |
|  6 |min_frontend_clock_gate_disable|  rw  | 0x0 |  — |
|10:7|           reserved1           |   r  |  —  |  — |

#### min_intpipe_clock_gate_disable field

<p>.Setting this bit disables integer pipeline clock gating</p>

#### min_vpulane_clock_gate_disable field

<p>Setting this bit disables VPU lane clock gating.</p>

#### min_vputima_clock_gate_disable field

<p>Setting this bit disables VPUtima clock gating</p>

#### min_vputrans_clock_gate_disable field

<p>Setting this bit disables VPUtrans clock gating</p>

#### min_dcache_clock_gate_disable field

<p>Setting this bit disables data cache clock gating</p>

#### reserved0 field

<p>Write as zero. Returns 0 on read.</p>

#### min_frontend_clock_gate_disable field

<p>Setting this bit disables front end clock gating.</p>

#### reserved1 field

<p>Write as zero. Returns 0 on read.</p>
