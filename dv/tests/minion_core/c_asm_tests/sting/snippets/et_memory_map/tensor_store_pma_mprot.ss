
import "$MINION_DIAGS/sting/snippets/tensor/tensor_store.pb"
import "$STING_ROOT/src/snippets/rv/rv_param_blocks.pb"

; This is a resource block where different hardware resources used
; by each snippet can be reserved. In this snippet, we are reserving
; a general purpose register. More complex resource notations will be
; presented in the next set of snippets
resource ;## STING PHASE
; Control thread
unum_t cpu_rand  = snp_urandrange_glbl(0, BOARD_CONF.NUM_CPUS)
; Always chose an even CPU
unum_t cpu_x     = snp_and(cpu_rand, 0xfffffffffffffffe)

unum_t bias      = 1
unum_t rand_op   = snp_urandrange(0, 2)
;unum_t cpu_x     = snp_get_hartid_biased_from_rand(BOARD_CONF.NUM_CPUS, bias, cpu_rand, rand_op)

unum_t hid_mask  = snp_gen_harts_mask(BOARD_CONF.NUM_CPUS)
;unum_t odd_mask  = snp_get_odd_bits(hid_mask)
;unum_t even_mask = snp_get_even_bits(hid_mask)

unum_t odd_mask  = 0xffffffff
unum_t even_mask = 0xffffffff

unum_t cpuid     = snp_cpu_id()

unum_t  rand_init       = snp_urandrange(0, 32)

; Tensor Store
unum_t  ts_csr_val_rand = snp_randnum()
unum_t  ts_csr_val_base = snp_and(ts_csr_val_rand, 0xfff800000000000f)
unum_t  ts_csr_val      = snp_cts_set_valid_config(ts_csr_val_base, rand_init)
unum_t  x31_rand        = snp_urandrange(1,9)
unum_t  x31_val         = snp_get_ts_stride(ts_csr_val, x31_rand)
unum_t  rval            = snp_get_tensor_rand_mprot()

unum_t  mprot_val    = snp_and(rval, 0x0000000000000008)

axreg_t csr_reg ; Unpredictable GPR (0002_reg_resvn_schemes.ss)
axreg_t rx1     ; Unpredictable GPR (0002_reg_resvn_schemes.ss)
axreg_t rx2     ; Unpredictable GPR (0002_reg_resvn_schemes.ss)
axreg_t rx3     ; Unpredictable GPR (0002_reg_resvn_schemes.ss)
axreg_t rx4     ; Unpredictable GPR (0002_reg_resvn_schemes.ss)
axreg_t rx5     ; Unpredictable GPR (0002_reg_resvn_schemes.ss)

; Max number of lines we can load is 16 (16 lines * 64 bytes per line * 8(MAX_STRIDE) = 8192)
uc_memory_t  mem1[8192]@64:ET_OS_REGION

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



;
; The 'snip_run' section gets rendered inside the random code section depending
; on the bias specified for this particular snippet.
;
snip_run ;## STING PHASE

;@cpu: 0, 2 .. BOARD_CONF.NUM_CPUS
;   if (cpuid != cpu_x):
;      wait_fcc(cnt = 0)
;   endif
;
;@cpu: 1, 3 .. BOARD_CONF.NUM_CPUS
;   if (cpuid != cpu_x):
;      wait_fcc(cnt = 2)
;   endif
;
;@cpu: cpu_x
@cpu: *

   if (cpu_id == cpu_x):

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
      
         tensor_store_snip_run(num_ts_csr_val = ts_csr_val, num_x31_val = x31_val, memx = mem1, addr_off = 0, csr_regx = csr_reg, regx1 = rx1, regx2 = rx2, regx3 = rx3, is_coop = 0, ten_wait = 0)
      
         tensor_store_wait()
      
         ; Read Tensor Error CSR
         csrrs rx1, tensor_error, x0
         ; Load the mask to consider the bits we are interested in only
         li   rx2, 0x80
         and  rx2, rx1, rx2
      
      if (mprot_val == 0):
      test_no_error:
         ; Check Mem access fault
         beq  rx2, x0, pass_tensor_store_error
         test_fail(regz = rx2)
      else :
       test_error:
         ; Check Mem access fault
         bne  rx2, x0, pass_tensor_store_error
         test_fail(regz = rx2)
      endif
      
      pass_tensor_store_error:
         reset_tensor_error()
      
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
      
         if (cpu_id%2) == 0:
           wait_fcc(cnt = 0)
         else:
           wait_fcc(cnt = 2)
         endif

   else:

     li rx4, cpu_x
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

mhandler@cpu: cpu_x

if (TEST_PROPS.TEST_MODE == RV_PARAMS.RV_MODE.MACHINE):
   test_fail(regz = rx1)
else:
   csrrs rx1, mcause, x0
   la rx1, mem_mhandler[cpuid]
   ld rx3, 0(rx1)
   addi rx4, x0, 0x004; load esr @mprot
   beq  x0, rx3, reset_mprot
   beq  rx3, rx4, mprot_backup

set_mprot:
   set_random_mprot(rega = rx1, regb = rx4, rand_val = rval)
   j mprot_done
mprot_backup:
   backup_mprot(rega = rx1, regb = rx4, regc = rx5, memory = mem_mprot[cpuid])
   j jmp_next_pc
reset_mprot:
   restore_mprot(rega = rx1, regb = rx4, regc = rx5, memory = mem_mprot[cpuid])

mprot_done:
endif

jmp_next_pc:
   csrr rx1, mepc
   addi rx1, rx1, 4
   csrw mepc, rx1
