
#include "erbium_mtvec_defines.h"
#include "macros.h"

// trap vector for m mode
.global test_mtrap_vector
.align 2
test_mtrap_vector:
        // Save s0 and s1 registers
	addi sp, sp, -16
	sd s0, 0(sp)
	sd s1, 8(sp)
	// epc + 4
	csrr s0, mepc
	addi s0, s0, 4
	csrw mepc, s0
	// Notify tb trap was taken
	la s0, xcpt_taken
	li s1, 1
	sb s1, 0(s0)
	// Restore registers
	ld s0, 0(sp)
	ld s1, 8(sp)
	addi sp, sp, 16
	mret

// trap vector for s mode
.global test_mtrap_vector
.align 2
test_strap_vector:
        // Save s0 and s1 registers
	addi sp, sp, -16
	sd s0, 0(sp)
	sd s1, 8(sp)
	// epc + 4
	csrr s0, sepc
	addi s0, s0, 4
	csrw sepc, s0
	// Notify tb trap was taken
	la s0, xcpt_taken
	li s1, 1
	sb s1, 0(s0)
	// Restore registers
	ld s0, 0(sp)
	ld s1, 8(sp)
	addi sp, sp, 16
	ret

// Direct mode
.global test_mtrap_direct_table
.align 2
test_mtrap_direct_table:
	.rept NUM_DIRECT_VECTORS
	j test_mtrap_vector
	.endr

.global test_strap_direct_table
.align 2
test_strap_direct_table:
        .rept NUM_DIRECT_VECTORS
        j test_strap_vector
        .endr

// Vector mode
.global test_mtrap_vector_table
.align 7
test_mtrap_vector_table:
    .rept NUM_VECTORED_VECTORS
    j test_mtrap_vector      // 0x00: Common trap for all exceptions
    j test_mtrap_vector      // 0x01: core_xcpt_cause_SSIP - Supervisor software interrupt
    j test_fail_handler      // 0x02: Unused
    j test_mtrap_vector      // 0x03: core_xcpt_cause_MSIP - Machine software interrupt
    j test_fail_handler      // 0x04: Unused
    j test_mtrap_vector      // 0x05: core_xcpt_cause_STIP - Supervisor timer interrupt
    j test_fail_handler      // 0x06: Unused
    j test_mtrap_vector      // 0x07: core_xcpt_cause_MTIP - Machine timer interrupt
    j test_fail_handler      // 0x08: Unused
    j test_mtrap_vector      // 0x09: core_xcpt_cause_SEIP - Supervisor external interrupt
    j test_fail_handler      // 0x0A: Unused
    j test_fail_handler      // 0x0B: core_xcpt_cause_MEIP - Machine external interrupt
    j test_fail_handler      // 0x0C: Unused
    j test_fail_handler      // 0x0D: Unused
    j test_fail_handler      // 0x0E: Unused
    j test_fail_handler      // 0x0F: Unused
    j test_fail_handler      // 0x10: core_xcpt_cause_MBAD_RED - bad trigger redirect
    j test_fail_handler      // 0x11: Unused
    j test_fail_handler      // 0x12: Unused
    j test_mtrap_vector      // 0x13: core_xcpt_cause_MIECO - icache ecc error counter overflow
    j test_fail_handler      // 0x14: Unused
    j test_fail_handler      // 0x15: Unused
    j test_fail_handler      // 0x16: Unused
    j test_mtrap_vector      // 0x17: core_xcpt_cause_BUS_ERROR - dcache bus errorUnused
    j test_fail_handler      // 0x18: Unused
    j test_fail_handler      // 0x19: Unused
    j test_fail_handler      // 0x1A: Unused
    j test_fail_handler      // 0x1B: Unused
    j test_fail_handler      // 0x1C: Unused
    j test_fail_handler      // 0x1D: Unused
    j test_fail_handler      // 0x1E: Unused
    j test_fail_handler      // 0x1F: Unused
    .endr

.global test_strap_vector_table
.align 7
test_strap_vector_table:
    .rept NUM_VECTORED_VECTORS
    j test_mtrap_vector      // 0x00: Common trap for all exceptions
    j test_mtrap_vector      // 0x01: core_xcpt_cause_SSIP - Supervisor software interrupt
    j test_fail_handler      // 0x02: Unused
    j test_mtrap_vector      // 0x03: core_xcpt_cause_MSIP - Machine software interrupt
    j test_fail_handler      // 0x04: Unused
    j test_mtrap_vector      // 0x05: core_xcpt_cause_STIP - Supervisor timer interrupt
    j test_fail_handler      // 0x06: Unused
    j test_mtrap_vector      // 0x07: core_xcpt_cause_MTIP - Machine timer interrupt
    j test_fail_handler      // 0x08: Unused
    j test_mtrap_vector      // 0x09: core_xcpt_cause_SEIP - Supervisor external interrupt
    j test_fail_handler      // 0x0A: Unused
    j test_fail_handler      // 0x0B: core_xcpt_cause_MEIP - Machine external interrupt
    j test_fail_handler      // 0x0C: Unused
    j test_fail_handler      // 0x0D: Unused
    j test_fail_handler      // 0x0E: Unused
    j test_fail_handler      // 0x0F: Unused
    j test_fail_handler      // 0x10: core_xcpt_cause_MBAD_RED - bad trigger redirect
    j test_fail_handler      // 0x11: Unused
    j test_fail_handler      // 0x12: Unused
    j test_mtrap_vector      // 0x13: core_xcpt_cause_MIECO - icache ecc error counter overflow
    j test_fail_handler      // 0x14: Unused
    j test_fail_handler      // 0x15: Unused
    j test_fail_handler      // 0x16: Unused
    j test_mtrap_vector      // 0x17: core_xcpt_cause_BUS_ERROR - dcache bus errorUnused
    j test_fail_handler      // 0x18: Unused
    j test_fail_handler      // 0x19: Unused
    j test_fail_handler      // 0x1A: Unused
    j test_fail_handler      // 0x1B: Unused
    j test_fail_handler      // 0x1C: Unused
    j test_fail_handler      // 0x1D: Unused
    j test_fail_handler      // 0x1E: Unused
    j test_fail_handler      // 0x1F: Unused
    .endr


.global test_fail_handler
.align 2
test_fail_handler:
    ASM_TEST_FAIL
