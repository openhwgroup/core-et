#include "macros.h"

#define STATUS_MPP_CLEAN_M 0xFFE7FF
#define STATUS_MPP_S       0x000800
#define STATUS_MPP_S_M     (STATUS_MPP_S | STATUS_MPP_CLEAN_M)
#define SATP_MODE_OFFSET   60
#define SATP_MODE_SV48     9
	//#define PPN_M              0x3FFFFF
#define PPN_M              0xFFFFFFFFFFF
#define PAGE_SZ            4096
#define PAGE_SZ_LOG2       12
#define PTE_SZ             8
#define PTE_SZ_LOG2        3
#define PTE_IDX_SZ         (PAGE_SZ_LOG2-PTE_SZ_LOG2)
#define PTE_IDX_M          ((1<<PTE_IDX_SZ)-1)
#define PTE_PPN_OFFSET     10
#define PTE_D              0x80
#define PTE_A              0x40
#define PTE_X              0x08
#define PTE_W              0x04
#define PTE_R              0x02
#define PTE_V              0x01


#define CONST_A     0xaaaaaaaaaaaaaaaa
#define CONST_B     0xbbbbbbbbbbbbbbbb
#define CONST_C     0xcccccccccccccccc
#define CONST_D     0xdddddddddddddddd
#define CONST_X     0xdeadbeefdeadbeef



.text
.global _start

// MACHINE MODE CODE
// -----------------
_start:
    // VM disabled
    // -----------
    // Read data with physical address
    la t0, _data_novm_phys
    ld t3, 0(t0) // Should read 0xaaaaaaaa...
    ld t4, 8(t0) // Should read 0xbbbbbbbb...

    // Fill page table
    // ---------------
    // Pointer to supervisor code
    la a0, _scode_vm_virt
    la a1, _scode_vm_phys
    li a2, PTE_A|PTE_X // Set access and execution permission bit
    call fill_pt

    // Pointer to data read by supervisor code
    la a0, _data_r_vm_virt
    la a1, _data_r_vm_phys
    li a2, PTE_A|PTE_R // Set access and read permission bit
    call fill_pt

    // Pointer to data written by supervisor code
    la a0, _data_w_vm_virt
    la a1, _data_w_vm_phys
    li a2, PTE_A|PTE_D|PTE_W|PTE_R // Set access, dirty, and write/read permission bits
    call fill_pt

    // Configure vm
    // ------------
    li t0, SATP_MODE_SV48 // Mode Sv48
    slli t0, t0, SATP_MODE_OFFSET
    la t1, root_pt // PPN
    srli t1, t1, PAGE_SZ_LOG2
    // Mask for PPN
    li t2, (PPN_M >> 10)
    slli t2, t2, 10
    addi t2, t2, (PPN_M & 0x3FF)
    and t2, t1, t2 // Apply mask
    or t2, t0, t2 // Add mode
    // Load into SATP
    csrrw zero, satp, t2

    // Set mpp to supervisor mode in mstatus
    li t0, (STATUS_MPP_S_M >> 4)
    slli t0, t0, 4
    ori t0, t0, (STATUS_MPP_S_M & 0xF)
    csrr t1, mstatus
    and  t1, t0, t1
    // FUTURE: Emmanuel Marie Play with these bits in mstatus when exceptions are correctly handled:
    // - mxr (Make eXecutable Readable)
    // - sum (permit Supervisor User Memory access)
    csrrw zero, mstatus, t1
    // Jump to supervisor code
    la t2, _scode_vm_virt
    csrrw zero, mepc, t2
    mret

    // Fill page table
    // ---------------
    // a0 -> Virtual address
    // a1 -> Physical address
    // a2 -> Permission bits
fill_pt:
    // Save return address to stack
    addi sp, sp, -32
    sd ra, 0(sp)

    // Root page table
    la a3, root_pt
    srli a4, a0, (PAGE_SZ_LOG2+PTE_IDX_SZ*3) // Remove page offset and VPN[2:0]
    andi a4, a4, PTE_IDX_M // Take VPN[3]
    la a5, l2_pt
    li a6, PTE_V // Set valid bit
    call fill_pte

    // Level 2 page table
    la a3, l2_pt
    srli a4, a0, (PAGE_SZ_LOG2+PTE_IDX_SZ*2) // Remove page offset and VPN[1:0]
    andi a4, a4, PTE_IDX_M // Take VPN[2]
    la a5, l3_pt
    li a6, PTE_V // Set valid bit
    call fill_pte

    // Level 3 page table
    la a3, l3_pt
    srli a4, a0, (PAGE_SZ_LOG2+PTE_IDX_SZ) // Remove page offset and VPN[0]
    andi a4, a4, PTE_IDX_M // Take VPN[1]
    la a5, l4_pt
    li a6, PTE_V // Set valid bit
    call fill_pte

    // Level 4 page table (leaf)
    la a3, l4_pt
    srli a4, a0, PAGE_SZ_LOG2 // Remove page offset
    andi a4, a4, PTE_IDX_M // Take VPN[0]
    mv a5, a1
    ori a6, a2, PTE_V // Copy access and permission bits and set valid bit
    call fill_pte

    // Recover return address from stack
    ld ra, 0(sp)
    addi sp, sp, 32
    ret

    // Fill page table entry
    // ---------------------
    // a3 -> Pointer to page table
    // a4 -> VPN[i]
    // a5 -> Physical address
    // a6 -> PTE configuration bits
fill_pte:
    // Save return address to stack
    addi sp, sp, -32
    sd ra, 0(sp)

    // Generate PTE and save in s0
    srli s0, a5, PAGE_SZ_LOG2 // Remove LSBs -> PPN
    slli s0, s0, PTE_PPN_OFFSET // Shift to PTE position
    // FUTURE: Emmanuel Marie Play with configuration bits when exceptions are properly handled
    or s0, s0, a6 // Copy configuration bits
    // Obtain VPN[i]*PTE_SZ
    slli s1, a4, PTE_SZ_LOG2 // Multiply by PTE_SZ
    add s1, a3, s1 // Pointer to VPN in page table
    sd s0, 0(s1) // Write to index VPN[0]
    // Evict
    fence
    li s2, 0x0400000000000000
    andi t0, s1, ~(0x3F)
    or s2, s2, t0
    csrw 0x89F, s2
    fence

    // Recover return address from stack
    ld ra, 0(sp)
    addi sp, sp, 32
    ret

    // Fill with blanks
    .skip 10000


// SUPERVISOR MODE CODE
// --------------------
// Virtual address where supervisor code will be executed
_scode_vm_virt:
    // Fill with blanks (must be a multiple of PAGE_SZ)
    .skip (PAGE_SZ*2)

// Real physical address where supervisor code is located
_scode_vm_phys:
    // VM enabled
    // ----------
    la t1, _scode_vm_virt
    la t2, _scode_vm_phys
    sub t1, t2, t1

    // Read data with virtual address
    la t0, _data_r_vm_virt
    add t0, t0, t1
    ld t5, 0(t0) // Should read 0xcccccccc...
    ld t6, 8(t0) // Should read 0xdddddddd...
    // self-checking: here we can use "t2": 
    li t2, CONST_C
    bne t5, t2, _test_fail  
    li t2, CONST_D
    bne t6, t2, _test_fail  

    // Write data with virtual address
    la t0, _data_w_vm_virt
    add t0, t0, t1
    // first: read initial value:
    ld t5, 0(t0) // Should read 0xdeadbeef...
    ld t6, 8(t0) // Should read 0xdeadbeef...
    // self-checking:
    li t2, CONST_X
    bne t5, t2, _test_fail  
    bne t6, t2, _test_fail  
    // write new value:
    sd t3, 0(t0) // Should write 0xaaaaaaaa...
    sd t4, 8(t0) // Should write 0xbbbbbbbb...
    // read them back:
    ld t5, 0(t0) // Should read 0xaaaaaaaa...
    ld t6, 8(t0) // Should read 0xbbbbbbbb...
    // self-checking:
    li t2, CONST_A
    bne t5, t2, _test_fail  
    li t2, CONST_B
    bne t6, t2, _test_fail  

    ASM_TEST_PASS   //  end test
_test_fail:
    ASM_TEST_FAIL
    

.data
// Data read using physical address
// --------------------------------
.global _data_novm_phys
_data_novm_phys:
    .fill 8, 1, 0xaa
    .fill 8, 1, 0xbb
    .skip (4096-16)

// Data read using virtual address
// -------------------------------
.global _data_r_vm_virt
.global _data_r_vm_phys
// Virtual address where supervisor code will look for data
_data_r_vm_virt:
    // Fill with blanks (must be a multiple of PAGE_SZ)
    .skip (PAGE_SZ*5)

// Real physical address where supervisor code will find data
_data_r_vm_phys:
    .fill 8, 1, 0xcc
    .fill 8, 1, 0xdd
    .skip (4096-16)

// Data written using virtual address
// ----------------------------------
.global _data_w_vm_virt
.global _data_w_vm_phys
// Virtual address where supervisor code will look for data
_data_w_vm_virt:
    // Fill with blanks (must be a multiple of PAGE_SZ)
    .skip (PAGE_SZ*3)

// Real physical address where supervisor code will find data
_data_w_vm_phys:
    .fill 8, 4, 0xdeadbeef
    .fill 8, 4, 0xdeadbeef
    .skip (4096-16)

// Page table
// ----------
.global root_pt
.global l2_pt
.global l3_pt
.global l4_pt

// Root page table
.p2align 12
root_pt:
    .skip 4096

// Level 2 page table
.p2align 12
l2_pt:
    .skip 4096

// Level 3 page table
.p2align 12
l3_pt:
    .skip 4096

// Level 4 page table (leaf)
.p2align 12
l4_pt:
    .skip 4096

