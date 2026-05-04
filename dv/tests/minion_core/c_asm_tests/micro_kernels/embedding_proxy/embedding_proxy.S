#define STATUS_MPP_CLEAN_M 0xFFE7FF
#define STATUS_MPP_S       0x000800
#define STATUS_MPP_S_M     (STATUS_MPP_S | STATUS_MPP_CLEAN_M)
#define SATP_MODE_OFFSET   60
#define SATP_MODE_SV48     9
#define PPN_M              0xFFFFFFFFFFF                         
#define PAGE_SZ            (1<<30)
#define PAGE_SZ_LOG2       30
#define PTE_SZ             8
#define PTE_SZ_LOG2        3
#define PTE_IDX_SZ         9
#define PTE_IDX_M          ((1<<PTE_IDX_SZ)-1)
#define PTE_PPN_OFFSET     10
#define PTE_D              0x80
#define PTE_A              0x40
#define PTE_X              0x08
#define PTE_W              0x04
#define PTE_R              0x02
#define PTE_V              0x01
#define LEVELS             4

#define CODE_SECTION_START 0x8000000000
#define VA_SECTION_START   (1ULL<<33)
#define PA_SECTION_START   (CODE_SECTION_START + PAGE_SZ)
#define ESR_SECTION_START  (1ULL<<32)

	
#define TABLE_SZ_ALIGNMENT 12
#define TABLE_SZ (1<<TABLE_SZ_ALIGNMENT)

#define MAX_L2_TABLES 64
#define PAGE_OFFSET_SZ 12
        
.macro setup_page pg_nr access_bits
   // VA=VA_START + PG_NR * PAGE_SZ
   li a0, VA_SECTION_START
   li t0, \pg_nr
   slli t0, t0, PAGE_SZ_LOG2
   add a0, a0, t0

   // PA=PA_START + PG_NR * PAGE_SZ * 2 
   li a1, PA_SECTION_START
   slli t0, t0, 1
   add a1, a1, t0

   // 3rd arg are the access bits
   li a2, \access_bits

   // create page entry
   call fill_pt
.endm

.text
.global _start
	
    // MACHINE MODE CODE
    // -----------------
_start:
    // enable shadow registers to be able to read hartid in s and u privilege levels
    csrwi 0x7d2, 0x3

    // M0, S0, T0 sets page tables
    // ---------------
    csrr t0, mhartid
    bne t0, zero, wait_fcc 
	
    // Map code section to have VA the same as PA
    // ---------------
    li a0, CODE_SECTION_START
    mv a1, a0
    li a2, PTE_A|PTE_X|PTE_R // Set access and execution permission bit
    call fill_pt

    // Map ESR section to have VA the same as PA
    // ---------------
    li a0, ESR_SECTION_START
    mv a1, a0
    li a2, PTE_A|PTE_R|PTE_W|PTE_D // Set access and RW permission bit
    call fill_pt
	
	
    // Fill page table
    // ---------------
    setup_page 0, PTE_A|PTE_W|PTE_R|PTE_D
    setup_page 1, PTE_A|PTE_W|PTE_R|PTE_D
    setup_page 2, PTE_A|PTE_W|PTE_R|PTE_D
    setup_page 3, PTE_A|PTE_W|PTE_R|PTE_D
    setup_page 4, PTE_A|PTE_W|PTE_R|PTE_D
    setup_page 5, PTE_A|PTE_W|PTE_R|PTE_D
    setup_page 6, PTE_A|PTE_W|PTE_R|PTE_D
    setup_page 7, PTE_A|PTE_W|PTE_R|PTE_D
    setup_page 8, PTE_A|PTE_W|PTE_R|PTE_D
    setup_page 9, PTE_A|PTE_W|PTE_R|PTE_D
    setup_page 10, PTE_A|PTE_W|PTE_R|PTE_D
    setup_page 11, PTE_A|PTE_W|PTE_R|PTE_D
    setup_page 12, PTE_A|PTE_W|PTE_R|PTE_D
    setup_page 13, PTE_A|PTE_W|PTE_R|PTE_D
    setup_page 14, PTE_A|PTE_W|PTE_R|PTE_D
    setup_page 15, PTE_A|PTE_W|PTE_R|PTE_D
    setup_page 16, PTE_A|PTE_W|PTE_R|PTE_D
    setup_page 17, PTE_A|PTE_W|PTE_R|PTE_D
    setup_page 18, PTE_A|PTE_W|PTE_R|PTE_D
    setup_page 19, PTE_A|PTE_W|PTE_R|PTE_D
    setup_page 20, PTE_A|PTE_W|PTE_R|PTE_D
    setup_page 21, PTE_A|PTE_W|PTE_R|PTE_D
    setup_page 22, PTE_A|PTE_W|PTE_R|PTE_D
    setup_page 23, PTE_A|PTE_W|PTE_R|PTE_D
    setup_page 24, PTE_A|PTE_W|PTE_R|PTE_D
    setup_page 25, PTE_A|PTE_W|PTE_R|PTE_D
    setup_page 26, PTE_A|PTE_W|PTE_R|PTE_D
    setup_page 27, PTE_A|PTE_W|PTE_R|PTE_D
    setup_page 28, PTE_A|PTE_W|PTE_R|PTE_D
    setup_page 29, PTE_A|PTE_W|PTE_R|PTE_D
    setup_page 30, PTE_A|PTE_W|PTE_R|PTE_D
    setup_page 31, PTE_A|PTE_W|PTE_R|PTE_D

    // Configure vm
    // ------------	
write_satp:
    li t0, SATP_MODE_SV48 // Mode Sv48
    slli t0, t0, SATP_MODE_OFFSET

    la t1, root_pt // PPN
    srli t1, t1, PAGE_OFFSET_SZ
 
    li t2, PPN_M // Mask for PPN
    and t1, t1, t2 // Apply mask
    or t1, t0, t1 // Add mode

    // Load into SATP
    csrrw zero, satp, t1

    // Set mpp to supervisor mode in mstatus
    li t0, (STATUS_MPP_S_M >> 4)
    slli t0, t0, 4
    ori t0, t0, (STATUS_MPP_S_M & 0xF)
    csrr t1, mstatus
    and  t1, t0, t1
    csrrw zero, mstatus, t1

    // Jump to supervisor code
    la t2, _scode
    csrrw zero, mepc, t2
    mret



// fill_pte
//        a3 => table base
//        a4 => entry number (VPN)
//        a5 => permission bits                
//        a6 => PA (either pointer to next table base or final PA)
fill_pte:
    // Save return address to stack
    addi sp, sp, -32
    sd ra, 0(sp)

    // compute address where to store the PTE ( base + VPN*8)
    slli t0, a4, 3
    add t0, t0, a3

    // compute the entry
    srli t1, a6, PAGE_OFFSET_SZ
    slli t1, t1, PTE_PPN_OFFSET
    or t1,t1,a5

    // and store
    sd t1, 0(t0)

    // Evict to L3
    fence
    li t1, 0x0800000000000000
    andi t0, t0, ~(0x3F)
    or t1, t1, t0	
    csrw 0x89F, t1
    fence
	
    // Recover return address from stack
    ld ra, 0(sp)
    addi sp, sp, 32
    ret

// Fill page table
// ---------------
// a0 -> Virtual address
// a1 -> Physical address
// a2 -> Permission bits        
fill_pt:   
    // Save return address to stack
    addi sp, sp, -32
    sd ra, 0(sp)

    // root page table
    la a3, root_pt
    srli a4, a0, (PAGE_OFFSET_SZ + (LEVELS-1)*PTE_IDX_SZ) //vpn3
    li a5,PTE_V // valid bit
    la a6, l2_pt
    mv t0, a4
    slli t0, t0, TABLE_SZ_ALIGNMENT
    add a6, a6, t0  // next level table stored in PA= l2_pt + (vpn3<<12)
    call fill_pte

    // entry for next level
    mv a3, a6 // PA=l2_pt + vpn3<<12
    srli a4, a0, (PAGE_OFFSET_SZ + (LEVELS-2)*PTE_IDX_SZ) 
    andi a4, a4, PTE_IDX_M // Take VPN[2]
    mv a5,a1
    ori a5, a2, PTE_V // Copy access and permission bits and set valid bit
    srli a6, a1, (PAGE_OFFSET_SZ + (LEVELS-2)*PTE_IDX_SZ)
    slli a6, a6, (PAGE_OFFSET_SZ + (LEVELS-2)*PTE_IDX_SZ) //set a6=VA but with ppn0,1=0
    call  fill_pte

        
    // Recover return address from stack
    ld ra, 0(sp)
    addi sp, sp, 32
    ret

   // Supervisor code
   // ----------
_scode:	
    call main

   // Wait for minion 0 to setup page tables before enabling vm
   // ----------
wait_fcc:
    csrrw zero, 0x821, 0
    call write_satp

   // Page table
   // ----------
.global root_pt
.global l2_pt

// Root page table
.p2align TABLE_SZ_ALIGNMENT
root_pt:
    .skip TABLE_SZ

// Level 2 page table (leaf)
.p2align TABLE_SZ_ALIGNMENT
l2_pt:
    .skip TABLE_SZ*MAX_L2_TABLES
