import "$MINION_DIAGS/sting/snippets/cacheops/cops.pb"
import "$MINION_DIAGS/sting/snippets/tensor/tensor.pb"

resource
cpu_t   x

; Allocate enough space to fit 4 addresses in the same set
gmemory_t test_mem[4096]@64:ET_DRAM_REGION
num_t noise = snp_randnum()
; Global, since we need to guarantee that 1 PA/set maps to 1 way
gnum_t init_way = snp_urandrange(0, 4)
; Select which set will be locked/unlocked
; Each thread gets assigned sets using round-robin
unum_t lock_set = snp_urandrange(0, 8)

axreg_t rx1 ; Register written to CSR
axreg_t rx2 ; Temporary register/noise
axreg_t rx3 ; Used for way/set increment
axreg_t base_addr ; Base address of test memory

snip_setup
    ; Empty

snip_init
    ; Empty

snip_run
@cpu: x
  
    ; Load address to test memory
    la base_addr, test_mem
    ; Load set base
    li rx2, lock_set
    slli rx2, rx2, 1
    ; Mask set based on hart_id
    get_hart_id(reg_hartid_o = rx1)
    andi rx1, rx1, 1
    or rx2, rx2, rx1 
    slli rx2, rx2, 6
    ; Base address = Test memory + Set offset
    add base_addr, base_addr, rx2
    li  rx2, 0x000000ffffffffc0
    and base_addr, base_addr, rx2
    
    ; Select first way to be locked
    li rx2, init_way
    slli rx2, rx2, 55
    or base_addr, base_addr, rx2

    ; Add noise
    li  rx2, 0xfe7fff000000003f
    li  rx1, noise
    and rx2, rx2, rx1

    ; Adding rx3 will
    ; - Increment to the next way (first way is random)
    ; - Move PA to the next valid address in the same set
    ;   (In the case of unlock_sw the values for set stay the same)
    li  rx3, 1
    slli rx3, rx3, 55     ; Select the next way
    addi rx3, rx3, 1024   ; Select the next address with same set

    ; Unlock all sets/ways
    or rx1, base_addr, rx2 ; Way x
    csrw unlock_sw, rx1
    add rx1, rx1, rx3      ; Way x+1
    csrw unlock_sw, rx1
    add rx1, rx1, rx3      ; Way x+2
    csrw unlock_sw, rx1
    add rx1, rx1, rx3      ; Way x+3
    csrw unlock_sw, rx1

    ; Tensor wait
    csrwi tensor_wait, 6

    ; Lock all sets/ways
    or rx1, base_addr, rx2 ; Way x
    csrw lock_sw, rx1
    add rx1, rx1, rx3      ; Way x+1
    csrw lock_sw, rx1
    add rx1, rx1, rx3      ; Way x+2
    csrw lock_sw, rx1
    add rx1, rx1, rx3      ; Way x+3
    csrw lock_sw, rx1      ; This should fail

    ; Tensor wait
    csrwi tensor_wait, 6

    ; Unlock again before checking for errors
    or rx1, base_addr, rx2 ; Way x
    csrw unlock_sw, rx1
    add rx1, rx1, rx3      ; Way x+1
    csrw unlock_sw, rx1
    add rx1, rx1, rx3      ; Way x+2
    csrw unlock_sw, rx1
    add rx1, rx1, rx3      ; Way x+3
    csrw unlock_sw, rx1

    ; Tensor wait
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
