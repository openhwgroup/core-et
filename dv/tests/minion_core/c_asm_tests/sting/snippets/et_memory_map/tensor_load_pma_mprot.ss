
import "$MINION_DIAGS/sting/snippets/tensor/tensor.pb"
import "$MINION_DIAGS/sting/snippets/tensor/tensor_load.pb"
import "$STING_ROOT/src/snippets/rv/rv_param_blocks.pb"

; This is a resource block where different hardware resources used
; by each snippet can be reserved. In this snippet, we are reserving
; a general purpose register. More complex resource notations will be
; presented in the next set of snippets
resource ;## STING PHASE

; Control thread
unum_t rand_cpu = snp_urandrange_glbl(0, BOARD_CONF.NUM_CPUS)
; Always chose an even CPU
unum_t x_cpu = snp_and(rand_cpu, 0xfffffffffffffffe)
unum_t hid_mask = snp_gen_harts_mask(BOARD_CONF.NUM_CPUS)
;unum_t odd_mask  = snp_get_odd_bits(hid_mask)
;unum_t even_mask = snp_get_even_bits(hid_mask)

unum_t odd_mask  = 0xffffffff
unum_t even_mask = 0xffffffff

unum_t cntr = 0

; Tensor Mask
unum_t  tl_mask_val   = snp_randnum()

; Tensor Load
unum_t  tl_csr_val_rand = snp_randnum()
unum_t  tl_csr_val      = snp_and(tl_csr_val_rand, 0x8fef00000000003f)
unum_t  x31_stride      = snp_urandrange(0, 3)
unum_t  x31_val         = snp_lshift(x31_stride, 6)
unum_t  rval            = snp_get_tensor_rand_mprot()
unum_t  cpuid           = snp_cpu_id()

axreg_t csr_reg ; Unpredictable GPR (0002_reg_resvn_schemes.ss)
axreg_t rx1     ; Unpredictable GPR (0002_reg_resvn_schemes.ss)
axreg_t rx2     ; Unpredictable GPR (0002_reg_resvn_schemes.ss)
axreg_t rx3     ; Unpredictable GPR (0002_reg_resvn_schemes.ss)
axreg_t rx4     ; Unpredictable GPR (0002_reg_resvn_schemes.ss)
axreg_t rx5     ; Unpredictable GPR (0002_reg_resvn_schemes.ss)

; Max number of lines we can load is 16 (16 lines * 64 bytes per line * 2(MAX_STRIDE) = 2048)
uc_memory_t  mem1[2048]@64:ET_OS_REGION

uc_gmemory_t mem_mprot[BOARD_CONF.NUM_CPUS][64]@64:ET_DRAM_REGION
uc_gmemory_t mem_mhandler[BOARD_CONF.NUM_CPUS][64]@64:ET_DRAM_REGION

;
; The 'snip_setup' section is invoked only once in the entire duration of the
; image execution during the kernel setup. Essential setup tasks are carried
; out in this stage. For example, device driver snippets can make use of this
; stage to carry out device initialization
;
snip_setup ;## STING PHASE
    ; Empty

;
; The 'snip_init' section is invoked at the starting of the test and is used to
; carry out test specific initializations like setting up the initial values
; in the memory buffers which will be accessed in the test.
;
snip_init ;## STING PHASE
   ; Empty
@cpu: *
 
 if ((cpu_id%2) == 0):
   la rx2, mem1
   write_val_to_memblk(regx = rx1, regb = rx2, val = 0x3C, size = 2048)
 endif

;
; The 'snip_run' section gets rendered inside the random code section depending
; on the bias specified for this particular snippet.
;
snip_run ;## STING PHASE
@cpu: *

   if ((cpu_id == x_cpu) && ((cpu_id%2)==0)): 
        
     reset_tensor_error()        
        
     if ((tl_csr_val & 0x8000000000000000) != 0):
       ; [Setup] For Tensor Loads, if bit 63 is set, we are using Tensor Mask (PRM-10)
       li   csr_reg, tl_mask_val
       csrw tensor_mask, csr_reg
     endif

     if (TEST_PROPS.TEST_MODE == RV_PARAMS.RV_MODE.MACHINE):
       addi rx4, x0, 0x004; load esr @mprot
       backup_mprot(rega = rx1, regb = rx4, regc = rx5, memory = mem_mprot[cpuid])
     else: ; Call mhandler
       li rx3, 0x004
       la rx1, mem_mhandler[cpuid]
       sd rx3, 0(rx1)
       et_jump_to_m_mode()
     endif

     if (TEST_PROPS.TEST_MODE == RV_PARAMS.RV_MODE.MACHINE):
       addi rx4, x0, 0x004; load esr @mprot
       set_random_mprot(rega = rx1, regb = rx4, rand_val = rval)
     else: ; Call mhandler
       li rx3, 1
       la rx1, mem_mhandler[cpuid]
       sd rx3, 0(rx1)
       et_jump_to_m_mode()
     endif
             
     ; Remove x31[47:6], which is used for stride access
     ; Save x31
     addi rx3, x31, 0
     li  x31, x31_val
                       
     ; [CSR] Set the register to random values and keep the ones we need 
     li  csr_reg, tl_csr_val
                        
     ; Tests the OS mem region
     ; [CSR] Mask the VA and merge it with the CSR value
     la  rx1, mem1
     li  rx2, 0x0000ffffffffffc0
     and rx1, rx2, rx1
     or  csr_reg, csr_reg, rx1
                        
     ; Write to the CSR to trigger the Tensor Load Operation 
     csrrw rx3, tensor_load, csr_reg

     beq    x0, rx3, check_mprot3
     test_fail(regz = rx3)

     check_mprot3:
       ; Check mprot[3] bit set
       li rx4, rval
       andi rx2, rx4, 0x8
       bne  x0, rx2, test_error
        
     test_no_error:
       ; Need tensor wait before cheking error
       tensor_load_wait(x31_reg_value = x31_val)
        
       ; mem1 should not be in shire memory, then we should see tensor error
       ; Read Tensor Error CSR
       csrrs rx1, tensor_error, x0
        
       ; Check Mem access fault
       ; Load the mask to consider the bits we are interested in only
       li   rx2, 0x80
       and  rx2, rx1, rx2
       beq  rx2, x0, pass_tensor_load_error
       j fail_tensor_load
        
     test_error:
       ; Need tensor wait before cheking error
       tensor_load_wait(x31_reg_value = x31_val)
        
       ; mem1 should not be in shire memory, then we should see tensor error
       ; Read Tensor Error CSR
       csrrs rx1, tensor_error, x0
        
       ; Check Mem access fault
       ; Load the mask to consider the bits we are interested in only
       li   rx2, 0x80
       and  rx2, rx1, rx2
       bne  rx2, x0, pass_tensor_load_error
        
     fail_tensor_load:
       test_fail(regz = rx2)
        
     pass_tensor_load_error:
       reset_tensor_error()
       addi x31, rx3, 0

       ;set back mprot to original value to don't get Trap 0x7 
       ; when sting writes memory because PMA still working in minion
       if (TEST_PROPS.TEST_MODE == RV_PARAMS.RV_MODE.MACHINE):
          addi rx4, x0, 0x004 ; load esr @mprot
          restore_mprot(rega = rx1, regb = rx4, regc = rx5, memory = mem_mprot[cpuid])
       else:
          li rx3, 0
          la rx1, mem_mhandler[cpuid]
          sd rx3, 0(rx1)
          et_jump_to_m_mode()
       endif
  
       writeFCC(credinc = 0, regtemp = rx1, regtemp2 = rx2, valhartmask = even_mask)
       writeFCC(credinc = 2, regtemp = rx1, regtemp2 = rx2, valhartmask = odd_mask)

       if ((cpu_id%2) == 0):
         wait_fcc(cnt = 0)
       else:
          wait_fcc(cnt = 2)
       endif

   else:

     if ((cpu_id%2) == 0):
       wait_fcc(cnt = 0)
     else:
        wait_fcc(cnt = 2)
     endif

   endif
        
;
; Similarly the 'snip_check' section is invoked at the end of the test after all
; the hardware threads are done with their random code section. It can be used to
; carry out any test specific checks like verifying the contents of a memory buffer
; against the expected values.
;
snip_check
   ; Empty



; The notation given below allows user to register handlers which will
; either run in machine mode (mhandler), or in supervisor mode (shandler)
; when trap delegation is enabled. In both the cases the exception program
; counter is read and incremented by 4 so as to allow the execution to proceed
; from the program counter following the instruction which caused the exception.
;
snip_handler

mhandler@cpu: *
  csrrs rx1, mcause, x0

  if (TEST_PROPS.TEST_MODE == RV_PARAMS.RV_MODE.MACHINE):
    test_fail(regz = rx1)
    j jmp_next_pc  
  else:    
    li rx2, 2              ;force by et_jump_to_m_mode to entry in mhandler
    beq rx1, rx2, env_call
    li rx2, 8 ; call from U-mode
    beq rx1, rx2, env_call
    li rx2, 9 ; call from S-mode
    beq rx1, rx2, env_call
  endif

  test_fail(regz = rx1)

env_call:  
   addi rx4, x0, 0x004; load esr @mprot
   la rx1, mem_mhandler[cpuid]
   ld rx3, 0(rx1)
   beq x0, rx3, reset_mprot
   beq rx3, rx4, mprot_backup
 set_mprot:
   set_random_mprot(rega = rx1, regb = rx4, rand_val = rval)
   j jmp_next_pc
 mprot_backup:
   backup_mprot(rega = rx1, regb = rx4, regc = rx5, memory = mem_mprot[cpuid])
   j jmp_next_pc
 reset_mprot:
   restore_mprot(rega = rx1, regb = rx4, regc = rx5, memory = mem_mprot[cpuid])

jmp_next_pc:
  csrr rx1, mepc
  addi rx1, rx1, 4
  csrw mepc, rx1
