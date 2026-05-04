import "$MINION_DIAGS/sting/snippets/cacheops/cops.pb"
import "$MINION_DIAGS/sting/snippets/tensor/tensor.pb"

resource
cpu_t   x

num_t   csr_val = snp_randnum()
num_t   way_inc = snp_urandrange(1, 4)

axreg_t rx1
axreg_t rx2
axreg_t rx3
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
    ; rx3 is used for way increment
    ; rb1 contains a PA
    ; All equally random, except PA (or just set for unlock)
    ;
    li  rx1, csr_val
    li  rx2, 0xffffff000000003f
    and rx1, rx1, rx2

    ; Mask Set or PA
    li  rx2, 0x000000ffffffffc0
    and base_addr, rb1, rx2
    or  rx1, rx1, base_addr
    csrw lock_sw, rx1
    
    ; Add a random number [1,3] (rx3) to the position corresponding to the way
    li  rx3, way_inc
    slli rx3, rx3, 55
    add  rx1, rx1, rx3
    csrw lock_sw, rx1

    ; tensor wait
    csrwi tensor_wait, 6

    ; We expect a LockSW tensor error here
    check_tensor_error_lock_sw(rega1 = rx1,
                               should_fail = 1,
                               on_error = pass_lock_sw_error)

  fail_lock_sw_error:
    test_fail(regz = rx1)

  pass_lock_sw_error:
    reset_tensor_error()
      
snip_check
    ; Empty

