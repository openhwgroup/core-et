import "$MINION_DIAGS/sting/snippets/common.pb"

resource
cpu_t   x

num_t   csr_val = snp_randnum()
num_t   do_unlock = snp_urandrange(0, 2)

axreg_t rx1
axreg_t rx2
axreg_t base_addr

axreg_t  rb1
unum_t   mem_offset = snp_urandrange(0, 8192)
gmemory_t mem_lock[8192]@64:ET_DRAM_REGION

snip_setup
    ; Empty

snip_init
    ; Empty

snip_run
@cpu: x

    la rb1, mem_lock
    li rx1, mem_offset
    add rb1, rb1, rx1
    ;
    ; rx1 is the register used to write to CSR
    ; rx2 is used for masking
    ; rb1 contains a PA
    ; All equally random, except PA (or just set for unlock)
    ;
    li  rx1, csr_val
if do_unlock > 0:
    li  rx2, 0xfffffffffffffc3f
else:
    li  rx2, 0xffffff000000003f
endif
    and rx1, rx1, rx2

    ; Mask Set or PA
if do_unlock > 0:
    li  rx2, 0x00000000000003c0
else:
    li  rx2, 0x000000ffffffffc0
endif
    and base_addr, rb1, rx2
    or  rx1, rx1, base_addr

if do_unlock > 0:
    csrw unlock_sw, rx1
else:
    csrw lock_sw, rx1
endif

; If not executing in machine mode, should cause an exception
; Following block makes the test fail, but should be skipped by the exception handler
if TEST_PROPS.TEST_MODE != RV_PARAMS.RV_MODE.MACHINE:
    test_fail(regz = rx2)
endif


snip_check
    ; Empty


snip_handler

mhandler@cpu: x
    csrr rx1, mepc
    addi rx1, rx1, 16
    csrw mepc, rx1

shandler@cpu: x
    csrr rx1, sepc
    addi rx1, rx1, 16
    csrw sepc, rx1

