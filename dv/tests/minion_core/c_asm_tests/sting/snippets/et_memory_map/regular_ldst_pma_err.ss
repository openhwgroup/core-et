
import "$STING_ROOT/src/snippets/rv/rv_param_blocks.pb"
import "$MINION_DIAGS/sting/snippets/common.pb"
import "$MINION_DIAGS/sting/conf/utils/memory_test_param_blocks.pb"

resource

cpu_t x
unum_t cntr = 0

;Integer Registers.
axreg_t reg0, reg1, r1, r2

;Single-precision FP register
pfreg_t rf1

;Skip ESR memmory area.
uc_memory_t notrw_mem1[64]@64:ET_REGULAR_LDST_ERROR_MEM_SET

snip_setup
; Empty

snip_init
; Empty

snip_run
@cpu: x

    ;
    ;Avoid Assertions on write scratchpad with previous value readed from it without initiliazed.
    ;
    li reg0, 0x00C0000000
    csrw lock_va, reg0
    wait_cacheops()
    csrw unlock_va, reg0
    wait_cacheops()
    ;
  
    li reg1, 0x0
    li reg0, 0x0

    la reg1, notrw_mem1
  
    SELECT 1 {
        1: ld  reg0, 0(reg1)
                  
        1: lb  reg0, 0(reg1)
            
        1: lh  reg0, 0(reg1)
            
        1: lw  reg0, 0(reg1)
          
        1: lbu reg0, 0(reg1)
            
        1: lhu reg0, 0(reg1)
          
        1: sb  reg0, 0(reg1)
          
        1: sh  reg0, 0(reg1)
            
        1: sw  reg0, 0(reg1)
            
        1: sd  reg0, 0(reg1)
  
        1: flw rf1, 0(reg1)
  
        1: fsw rf1, 0(reg1)
    }


snip_check

snip_handler

mhandler@cpu: x

  csrrs reg0, mcause, x0

  li reg1, 0x5  ; Load access fault 
  beq reg0, reg1, end_test_m
  li reg1, 0x7  ; Store/amo access fault
  beq reg0, reg1, end_test_m

  if ((TEST_PROPS.TEST_MODE == RV_PARAMS.RV_MODE.USER) ||
    (TEST_PROPS.TEST_MODE == RV_PARAMS.RV_MODE.SUPERVISOR)):
    li reg1, 0xd           ; load page fault on Sv/U-mode
    beq reg0, reg1, end_test_m
    li reg1, 8
    beq reg0, reg1, end_test_m
    li reg1, 9
    beq reg0, reg1, end_test_m
  endif
  test_fail(regz = reg1)
  j end_test_m 

end_test_m:
   csrr reg1, mepc
   addi reg1, reg1, 4
   csrw mepc, reg1

