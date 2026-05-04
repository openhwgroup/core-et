
import "$MINION_DIAGS/sting/snippets/common.pb"
import "$MINION_DIAGS/sting/snippets/amo.pb"
 
resource ;## STING PHASE
unum_t  cpu_x = snp_urandrange(0, BOARD_CONF.NUM_CPUS)

unum_t rand_init = snp_randnum_glbl()

; GSC Mask
unum_t  gsc_mask_val    = snp_urandrange(0, 256)

; Op type
unum_t op_type = snp_urandrange(0, 4)
unum_t op_zero_rand = 0

; Base address alignment
unum_t base_alignment = snp_urandrange(4, 68)

; On trap, what to do?
; 0 : Skip instruction
; 1 : Increase GSC progress
; 2 : Modify FP register to make it not to trap
unum_t trap_action_rand = snp_urandrange(0, 2)
unum_t SKIP_INSTRUCTION = 0;
unum_t trap_action = snp_if_zero_else(GSC_ENV.USE_GSC_PROGRESS, SKIP_INSTRUCTION, trap_action_rand)

unum_t  off0 = snp_get_gsc_offset(op_type, gsc_mask_val, 0, base_alignment)
unum_t  off1 = snp_get_gsc_offset(op_type, gsc_mask_val, 1, base_alignment)
unum_t  off2 = snp_get_gsc_offset(op_type, gsc_mask_val, 2, base_alignment)
unum_t  off3 = snp_get_gsc_offset(op_type, gsc_mask_val, 3, base_alignment)
unum_t  off4 = snp_get_gsc_offset(op_type, gsc_mask_val, 4, base_alignment)
unum_t  off5 = snp_get_gsc_offset(op_type, gsc_mask_val, 5, base_alignment)
unum_t  off6 = snp_get_gsc_offset(op_type, gsc_mask_val, 6, base_alignment)
unum_t  off7 = snp_get_gsc_offset(op_type, gsc_mask_val, 7, base_alignment)

axreg_t rx1     ; Unpredictable GPR (0002_reg_resvn_schemes.ss)
axreg_t rx2     ; Unpredictable GPR (0002_reg_resvn_schemes.ss)
axreg_t rx3     ; Unpredictable GPR (0002_reg_resvn_schemes.ss)
axreg_t rx_mem  ; Unpredictable GPR (0002_reg_resvn_schemes.ss)

afreg_t fx_off
afreg_t fx1

unum_t mem_off_rand = snp_urandrange(0, 512)
unum_t mem_off      = snp_and(mem_off_rand, 0xffffffffffffffc0)
; Max number of lines we can load is 16 (16 lines * 64 bytes per line * 2(MAX_STRIDE) = 2048)
uc_gmemory_t  atomic_mem[BOARD_CONF.NUM_CPUS][2560]@64:ET_MINION_SHIRE_MEM_SET



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
@cpu: cpu_x

   snippet_start(random_num = rand_init, rega = rx1, regb = rx2)

   la rx_mem, atomic_mem[cpu_x]
   li rx1, mem_off
   add rx_mem, rx_mem, rx1

   if (op_type == 0):
      op_zero_rand = snp_urandrange(0, 2)
      ; FUTURE: consider the case that base is not aligned (to test trap)
      if (op_zero_rand == 0) :
         ; Mask has to be hardcoded to all 1s (PRM-0, Errata section)
         set_m0(value = 0xff)
         fswg.ps fx1, (rx_mem)
      else :
         set_m0(value = gsc_mask_val)
         flwg.ps fx1, (rx_mem)
      endif
   else :
      ; Load the offsets into the VPU register
      mv_imm_to_fd1(lane_num = 0, imm = off0, regx1 = rx1, fregx1 = fx_off)
      mv_imm_to_fd1(lane_num = 1, imm = off1, regx1 = rx1, fregx1 = fx_off)
      mv_imm_to_fd1(lane_num = 2, imm = off2, regx1 = rx1, fregx1 = fx_off)
      mv_imm_to_fd1(lane_num = 3, imm = off3, regx1 = rx1, fregx1 = fx_off)
      mv_imm_to_fd1(lane_num = 4, imm = off4, regx1 = rx1, fregx1 = fx_off)
      mv_imm_to_fd1(lane_num = 5, imm = off5, regx1 = rx1, fregx1 = fx_off)
      mv_imm_to_fd1(lane_num = 6, imm = off6, regx1 = rx1, fregx1 = fx_off)
      mv_imm_to_fd1(lane_num = 7, imm = off7, regx1 = rx1, fregx1 = fx_off)

      ; set a random memory offset
      li rx1, base_alignment
      add rx_mem, rx_mem, rx1

      ; Set the M0 mask (we must do it here, because 'mv_imm_to_fd1' uses m0
      set_m0(value = gsc_mask_val)

      if (op_type == 1):
         SELECT 1 {
            1 : fgbg.ps  fx1, fx_off, rx_mem
            1 : fscbg.ps fx1, fx_off, rx_mem
         }
      elif (op_type == 2):
         SELECT 1 {
            1 : fghg.ps  fx1, fx_off, rx_mem
            1 : fschg.ps fx1, fx_off, rx_mem
         }
      else :
         SELECT 1 {
            1 : fgwg.ps  fx1, fx_off, rx_mem
            1 : fscwg.ps fx1, fx_off, rx_mem
         }
      endif
   endif

   if ET_TEST_ENV.SILICON_MODE == 1:
      if (op_type == 0):
         ; Load a predictable value into the VPU register
         mv_imm_to_fd1(lane_num = 0, imm = off0, regx1 = rx1, fregx1 = fx_off)
         mv_imm_to_fd1(lane_num = 1, imm = off1, regx1 = rx1, fregx1 = fx_off)
         mv_imm_to_fd1(lane_num = 2, imm = off2, regx1 = rx1, fregx1 = fx_off)
         mv_imm_to_fd1(lane_num = 3, imm = off3, regx1 = rx1, fregx1 = fx_off)
         mv_imm_to_fd1(lane_num = 4, imm = off4, regx1 = rx1, fregx1 = fx_off)
         mv_imm_to_fd1(lane_num = 5, imm = off5, regx1 = rx1, fregx1 = fx_off)
         mv_imm_to_fd1(lane_num = 6, imm = off6, regx1 = rx1, fregx1 = fx_off)
         mv_imm_to_fd1(lane_num = 7, imm = off7, regx1 = rx1, fregx1 = fx_off)
      endif
      copy_fs1_to_fd(fd = fx1, fs1 = fx_off)
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
   csrr   rx2,  mepc

if (trap_action == 0):
   ; Skip instruction and clean GSC Progress
   csrrw  x0,  gsc_progress, x0
   addi rx2, rx2, 4
;elif (trap_action == 1):
else:
   ; Increase GSC_progress and execute GSC again
   csrrs  rx1,  gsc_progress, x0
   li rx3, 7
   beq rx1, rx3, gsc_done
   addi   rx1,  rx1, 1
   csrrw  x0,  gsc_progress, rx1
   j do_mret
gsc_done:
   addi rx2, rx2, 4
endif
   ; FUTURE: add the option to modify the FPregister in order to not make it fail
do_mret:
   csrw   mepc, rx2
