
import "$STING_ROOT/src/snippets/rv/rv_param_blocks.pb"
import "$MINION_DIAGS/sting/snippets/common.pb"
import "$MINION_DIAGS/sting/conf/utils/memory_test_param_blocks.pb"

resource

cpu_t x
unum_t cntr = 0

;Integer Registers.
axreg_t reg0, reg1, reg2, r1, r2, r3

;Single-precision FP register
pfreg_t rf1

;Skip ESR memmory area.
memory_t rw_mem1[64]@64:ET_REGULAR_LDST_OK_MEM_SET

snip_setup
;Empty

snip_init
; Empty

snip_run
@cpu: x

     ;
     ;Avoid Assertions on write scratchpad with previous value readed from it without initiliazed.
     ;
;     li reg0, 0x00C0000000
;     csrw lock_va, reg0
;     wait_cacheops()
;     csrw unlock_va, reg0
;     wait_cacheops()
;     ;

     li reg1, 0x0
     li reg0, 0x0
     li reg2, 0x4
     
     ;reg2 contains mprot value on exit from set_random_prot pb block
     add reg1, x0, reg2
     la reg1, rw_mem1
   
     is_in_region(reg_addr = reg1, addr_base = 0x000010000000, addr_top = 0x000020000000, reg_area_val = r3, val_area = 0, jumpOnfind = end_finding_address)
     is_in_region(reg_addr = reg1, addr_base = 0x0000C0000000, addr_top = 0x0000C5000000, reg_area_val = r3, val_area = 1, jumpOnfind = end_finding_address)
     is_in_region(reg_addr = reg1, addr_base = 0x004000000000, addr_top = 0x008000000000, reg_area_val = r3, val_area = 2, jumpOnfind = end_finding_address)
     is_in_region(reg_addr = reg1, addr_base = 0x008000000000, addr_top = 0x008000200000, reg_area_val = r3, val_area = 3, jumpOnfind = end_finding_address)
     is_in_region(reg_addr = reg1, addr_base = 0x008000200000, addr_top = 0x008100000000, reg_area_val = r3, val_area = 4, jumpOnfind = end_finding_address)
     is_in_region(reg_addr = reg1, addr_base = 0x008200000000, addr_top = 0x00C000000000, reg_area_val = r3, val_area = 5, jumpOnfind = end_finding_address)
     is_in_region(reg_addr = reg1, addr_base = 0x00C000000000, addr_top = 0x00C000200000, reg_area_val = r3, val_area = 6, jumpOnfind = end_finding_address)
     is_in_region(reg_addr = reg1, addr_base = 0x00C000200000, addr_top = 0x00C100000000, reg_area_val = r3, val_area = 7, jumpOnfind = end_finding_address)
     is_in_region(reg_addr = reg1, addr_base = 0x00C100000000, addr_top = 0x01C100000000, reg_area_val = r3, val_area = 8, jumpOnfind = end_finding_address)

   
    end_finding_address:   
   
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
;Empty
;
; in IO  --> AMOs, TensorOps, and CacheOps not permitted
; in SCP -->  Local AMOs not permitted;
; ESR --> Not tested but The access is permitted only when all of the following conditions are met:
;         The hart?s execution mode is equal or higher to the ESR privilege mode.
;         The address maps to an existing ESR register (this implies it is 64-bit aligned).
;         The read or write access is from a 64-wide load or store.
;         The instruction generating the access is not an AMO, or a TensorOp, or a CacheOp.
;         If the ESR privilege mode is 2, the access is only permitted if issued by the SvcProc.
; Cacheable Mcode     --> The access is generated from M-mode
; Non-cacheable Mcode --> The access is generated from M-mode
;
;

snip_handler

mhandler@cpu: x

  csrrs reg0, mcause, x0

  li reg1, 0x5  ; Load access fault
  beq reg0, reg1, test_pass_check_m
  li reg1, 0x7  ; Store/amo access fault
  beq reg0, reg1, end_test_m

  if ((TEST_PROPS.TEST_MODE == RV_PARAMS.RV_MODE.USER) ||
      (TEST_PROPS.TEST_MODE == RV_PARAMS.RV_MODE.SUPERVISOR)):
    li reg1, 0xd  ; page fault from sv/U-mode
    beq reg0, reg1, end_test_m
    li reg1, 8
    beq reg0, reg1, end_test_m
    li reg1, 9
    beq reg0, reg1, end_test_m
  endif
  test_fail(regz = reg1)
  j end_test_m


  test_pass_check_m:
    li r2, 0
    beq r3, r2, IO_region
    li r2, 1
    beq r3, r2, SCP_region
    li r2, 2
    beq r3, r2, PCIE_region
    li r2, 3
    beq r3, r2, MCODE_CACH_region
    li r2, 4
    beq r3, r2, OS_CACH_region
    li r2, 6
    beq r3, r2, MCODE_NOCACH_region
    li r2, 7
    beq r3, r2, OS_NOCACH_region
    test_fail(regz = reg1)
    j end_test_m   

  IO_region:
    andi reg1, reg2, 0x1 
    bne  reg1, x0, end_test_m
    test_fail(regz = reg1)
     j end_test_m   

  SCP_region:
    ;AMO not permitted     not expected in test this fail
    test_fail(regz = reg1)
    j end_test_m

  PCIE_region:
    andi reg1, reg2, 0x2 
    bne  reg1, x0, end_test_m
    test_fail(regz = reg1)
    j end_test_m

  MCODE_CACH_region:
  MCODE_NOCACH_region:
   ;The access is generated from M-mode otherwise can't access Mcode Subregion
   if ((TEST_PROPS.TEST_MODE == RV_PARAMS.RV_MODE.USER) ||
       (TEST_PROPS.TEST_MODE == RV_PARAMS.RV_MODE.SUPERVISOR)):
     j end_test_m   
   endif

  OS_CACH_region:
  OS_NOCACH_region:
    addi reg1, reg2, 0x4
    bne  reg1, x0, end_test_m
    test_fail(regz = reg1)
    j end_test_m   

end_test_m:
   csrr reg1, mepc
   addi reg1, reg1, 4
   csrw mepc, reg1

