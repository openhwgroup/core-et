
import "$STING_ROOT/src/snippets/rv/rv_param_blocks.pb"
import "$MINION_DIAGS/sting/snippets/common.pb"
import "$MINION_DIAGS/sting/conf/utils/memory_test_param_blocks.pb"

resource
unum_t cpuid = snp_cpu_id()

; ACTIONS
unum_t RESTORE_MPROT = 0
unum_t SET_MPROT = 1
unum_t BACKUP_MPROT = 4

unum_t cntr = 0
unum_t mem_area = 0
unum_t rand_mprot_val = snp_get_rand_mprot()
unum_t x_cpu = snp_urandrange_glbl(0, BOARD_CONF.NUM_CPUS)
unum_t hid_mask = snp_gen_harts_mask(BOARD_CONF.NUM_CPUS)
;unum_t odd_mask  = snp_get_odd_bits(hid_mask)
;unum_t even_mask = snp_get_even_bits(hid_mask)

unum_t odd_mask  = 0xffffffff
unum_t even_mask = 0xffffffff

;Integer Registers.
axreg_t reg0, reg1, reg2, r1, r2, r3

;Single-precision FP register
pfreg_t rf1

;Skip ESR memmory area.
uc_memory_t rw_mem1[64]@64:ET_REGULAR_LDST_OK_MEM_SET
uc_gmemory_t mem_mhandler[BOARD_CONF.NUM_CPUS][64]@64:ET_DRAM_REGION
uc_gmemory_t mem_mprot[BOARD_CONF.NUM_CPUS][64]@64:ET_DRAM_REGION

snip_setup
;Empty

snip_init
; Empty

snip_run
@cpu: *

  if (cpuid == x_cpu) :
     ;
     ;Avoid Assertions on write scratchpad with previous value readed from it without initiliazed.
     ;
     li reg0, 0x00C0000000
     csrw lock_va, reg0
     wait_cacheops()
     csrw unlock_va, reg0
     wait_cacheops()

     if (TEST_PROPS.TEST_MODE == RV_PARAMS.RV_MODE.MACHINE):
       li reg2, 0x004
       backup_mprot(rega = reg0, regb = reg2, regc = reg1, memory = mem_mprot[cpuid])
     else: ; Call mhandler
       set_handler_action(action = BACKUP_MPROT, action_mem = mem_mhandler[cpuid], regx1 = reg0, regx2 = reg2)
       et_jump_to_m_mode()
     endif

     li reg1, 0x0
     li reg0, 0x0
     li reg2, 0x4

     if (TEST_PROPS.TEST_MODE == RV_PARAMS.RV_MODE.MACHINE):
       set_random_mprot(rega = reg0, regb = reg2, rand_val = rand_mprot_val)
     else: ; Call mhandler
       set_handler_action(action = SET_MPROT, action_mem = mem_mhandler[cpuid], regx1 = reg0, regx2 = reg2)
       et_jump_to_m_mode()
     endif
   
     ;reg2 contains mprot value on exit from set_random_prot pb block
     add reg1, x0, reg2
     la reg1, rw_mem1
   
     is_in_region(reg_addr = reg1, addr_base = 0x000010000000, addr_top = 0x000020000000, reg_area_val = r3, val_area = 0, jumpOnfind = end_finding_address)
     is_in_region(reg_addr = reg1, addr_base = 0x0000C0000000, addr_top = 0x0000C5000000, reg_area_val = r3, val_area = 1, jumpOnfind = end_finding_address)
     is_in_region(reg_addr = reg1, addr_base = 0x004000000000, addr_top = 0x008000000000, reg_area_val = r3, val_area = 2, jumpOnfind = end_finding_address)
     is_in_region(reg_addr = reg1, addr_base = 0x008000000000, addr_top = 0x008000200000, reg_area_val = r3, val_area = 3, jumpOnfind = end_finding_address)
     is_in_region(reg_addr = reg1, addr_base = 0x008000200000, addr_top = 0x008100000000, reg_area_val = r3, val_area = 4, jumpOnfind = end_finding_address)
     is_in_region(reg_addr = reg1, addr_base = 0x008100000000, addr_top = 0x008200000000, reg_area_val = r3, val_area = 5, jumpOnfind = end_finding_address)
     is_in_region(reg_addr = reg1, addr_base = 0x008200000000, addr_top = 0x008400000000, reg_area_val = r3, val_area = 6, jumpOnfind = end_finding_address)
     is_in_region(reg_addr = reg1, addr_base = 0x008400000000, addr_top = 0x008600000000, reg_area_val = r3, val_area = 7, jumpOnfind = end_finding_address)
     is_in_region(reg_addr = reg1, addr_base = 0x008600000000, addr_top = 0x008800000000, reg_area_val = r3, val_area = 8, jumpOnfind = end_finding_address)
     is_in_region(reg_addr = reg1, addr_base = 0x008800000000, addr_top = 0x00C000000000, reg_area_val = r3, val_area = 9, jumpOnfind = end_finding_address)
     is_in_region(reg_addr = reg1, addr_base = 0x00C000000000, addr_top = 0x00C000200000, reg_area_val = r3, val_area = 0xa, jumpOnfind = end_finding_address)
     is_in_region(reg_addr = reg1, addr_base = 0x00C000200000, addr_top = 0x00C100000000, reg_area_val = r3, val_area = 0xb, jumpOnfind = end_finding_address)
     is_in_region(reg_addr = reg1, addr_base = 0x00C100000000, addr_top = 0x01C100000000, reg_area_val = r3, val_area = 0xc, jumpOnfind = end_finding_address)

   
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

     ; set back mprot to original value to don't get Trap 0x7 
     ; when sting writes memory because PMA still working in minion  
   
     if (TEST_PROPS.TEST_MODE == RV_PARAMS.RV_MODE.MACHINE):
        addi r2, x0, 0x004 ; load esr @mprot
        set_random_mprot(rega = r1, regb = r2, rand_val = 0x10)
     else:
        set_handler_action(action = RESTORE_MPROT, action_mem = mem_mhandler[cpuid], regx1 = r1, regx2 = r2)
        et_jump_to_m_mode()
     endif
    
     writeFCC(credinc = 0, regtemp = r1, regtemp2 = r2, valhartmask = even_mask)

     writeFCC(credinc = 2, regtemp = r1, regtemp2 = r2, valhartmask = odd_mask)

     if (cpuid%2) == 0:
       wait_fcc(cnt = 0)
     else:
       wait_fcc(cnt = 2)
     endif

         
  else:
    ;other cpu's threads only wait for fcc given by the thread working for this renderization.
    ;The next renderization will start with all cpu's ready to do an execution

    if (cpuid%2) == 0:
      wait_fcc(cnt = 0)
    else:
      wait_fcc(cnt = 2)
    endif

  endif



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

mhandler@cpu: *

  csrrs reg0, mcause, x0

  li reg1, 0x5  ; Load access fault
  beq reg0, reg1, test_pass_check_m
  li reg1, 0x7  ; Store/amo access fault
  beq reg0, reg1, end_test_m

  if ((TEST_PROPS.TEST_MODE == RV_PARAMS.RV_MODE.USER) ||
      (TEST_PROPS.TEST_MODE == RV_PARAMS.RV_MODE.SUPERVISOR)):
    li reg1, 2
    beq reg0, reg1, env_call
    li reg1, 8
    beq reg0, reg1, env_call
    li reg1, 9
    beq reg0, reg1, env_call
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
    beq r3, r2, MEM_16G_region
    li r2, 7
    beq r3, r2, MEM_24G_region
    li r2, 8
    beq r3, r2, MEM_32G_region
    li r2, 9
    beq r3, r2, MEM_REST_region
    li r2, 0xa
    beq r3, r2, MCODE_NOCACH_region
    li r2, 0xb
    beq r3, r2, OS_NOCACH_region
    test_fail(regz = reg1)
    j end_test_m   

  IO_region:
    andi reg1, reg2, 0x3
    ; MPROT 0b010 always causes an exception
    li reg2, 0x2
    beq reg1, reg2, end_test_m
  if (TEST_PROPS.TEST_MODE == RV_PARAMS.RV_MODE.USER):
    li reg2, 0x01
    ; In user mode, MPROT == 0b0001 causes an exception
    beq  reg1, reg2, end_test_m
    li reg2, 0x03
    ; In user mode, MPROT == 0b0011 causes an exception
    beq  reg1, reg2, end_test_m
  elif (TEST_PROPS.TEST_MODE == RV_PARAMS.RV_MODE.SUPERVISOR):
    li reg2, 0x03
    ; In supervisor mode, MPROT == 0b0011 causes an exception
    beq  reg1, reg2, end_test_m
  endif
    test_fail(regz = reg1)
    j end_test_m

  MEM_16G_region:
    andi reg1, reg2, 0x30
    li reg2, 0x00
    beq reg1, reg2, end_test_m
    test_fail(regz = reg1)
    j end_test_m

  MEM_24G_region:
    andi reg1, reg2, 0x30
    li reg2, 0x00
    beq reg1, reg2, end_test_m
    li reg2, 0x10
    beq reg1, reg2, end_test_m
    test_fail(regz = reg1)
    j end_test_m

  MEM_32G_region:
    andi reg1, reg2, 0x30
    li reg2, 0x00
    beq reg1, reg2, end_test_m
    li reg2, 0x10
    beq reg1, reg2, end_test_m
    li reg2, 0x20
    beq reg1, reg2, end_test_m
    test_fail(regz = reg1)
    j end_test_m

  MEM_REST_region:
    j end_test_m

  SCP_region:
    ;AMO not permitted     not expected in test this fail
    test_fail(regz = reg1)
    j end_test_m

  PCIE_region:
    andi reg1, reg2, 0x4
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
    addi reg1, reg2, 0x8
    bne  reg1, x0, end_test_m
    test_fail(regz = reg1)
    j end_test_m   

env_call:

   get_handler_action(action_mem = mem_mhandler[cpuid], regx1 = reg2, action_reg = r2)

action_reset_mprot:
   li reg2, RESTORE_MPROT
   bne r2, reg2, action_set_mprot
   li reg2, 0x004 ; load esr @mprot
   restore_mprot(rega = reg0, regb = reg2, regc =reg1, memory = mem_mprot[cpuid])
   j end_test_m

action_set_mprot:
   li reg2, SET_MPROT
   bne r2, reg2, action_backup_mprot
   set_random_mprot(rega = reg0, regb = reg2, rand_val = rand_mprot_val)
   j end_test_m

action_backup_mprot:
   li reg2, 0x004
   backup_mprot(rega = reg0, regb = reg2, regc = reg1, memory = mem_mprot[cpuid])

end_test_m:
   csrr reg1, mepc
   addi reg1, reg1, 4
   csrw mepc, reg1

