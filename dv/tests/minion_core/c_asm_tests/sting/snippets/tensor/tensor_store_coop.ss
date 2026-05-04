
import "$MINION_DIAGS/sting/snippets/tensor/tensor_store.pb"
 
; This is a resource block where different hardware resources used
; by each snippet can be reserved. In this snippet, we are reserving
; a general purpose register. More complex resource notations will be
; presented in the next set of snippets

assert
    (BOARD_CONF.NUM_CPUS%4) == 0
assert_end

resource ;## STING PHASE
cpu_t   x ;0003_cpu_selection_schemes.ss

unum_t  rand_init         = snp_urandrange(0, 32)
unum_t  rand_mark         = snp_randnum_glbl()

; Tensor Store
unum_t  ts_csr_val_rand   = snp_randnum_glbl()
; [CSR] Set the register to random values and keep the ones we need
unum_t  ts_csr_val_masked = snp_and(ts_csr_val_rand,  0xFFFE00000000000F)
; [CSR] Set a random cooperative way
unum_t  ts_csr_val_base   = glbl_snp_set_rand_cts_way(ts_csr_val_masked)

; Tensor Load Transformation Bias to reduce the number of wrong transformations
unum_t  ts_csr_valid    = snp_cts_set_valid_config(ts_csr_val_base, rand_init)
unum_t  ts_csr_invalid  = snp_cts_set_invalid_config(ts_csr_val_base, rand_init)
unum_t  config_bias     = snp_urandrange_glbl(0, 27)
unum_t  ts_csr_val = snp_if_zero_else(config_bias, ts_csr_invalid, ts_csr_valid)

unum_t  x31_rand        = snp_urandrange_glbl(1,  3)
unum_t  x31_val         = 0

unum_t  ten_wait_en     = 0

unum_t  coop_offset     = 0

unum_t  in_cts = snp_in_cts(ts_csr_valid)

; Tensor Store 3D memory management
unum_t  base_offset_rand = snp_urandrange_glbl(0, 4096)
unum_t  base_offset      = snp_and(base_offset_rand, 0xffffffffffffffc0)
unum_t  memory_idx       = snp_urandrange_glbl(0, BOARD_CONF.NUM_CPUS)

axreg_t csr_reg ; Unpredictable GPR (0002_reg_resvn_schemes.ss)
axreg_t rx1     ; Unpredictable GPR (0002_reg_resvn_schemes.ss)
axreg_t rx2     ; Unpredictable GPR (0002_reg_resvn_schemes.ss)
axreg_t rx3     ; Unpredictable GPR (0002_reg_resvn_schemes.ss)

; Max number of lines we can load is 16 (16 lines * 64 bytes per line * 2(MAX_STRIDE) = 2048)
gmemory_t  cts_mem2[BOARD_CONF.NUM_CPUS][8192]@64:ET_MINION_SHIRE_MEM_SET



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
@cpu: *
if (ET_TEST_ENV.STING_RAND == 1) :
   ten_wait_en = 1
else :
   ten_wait_en = snp_urandrange(0,2)
endif

if (in_cts == 1):
   x31_val = snp_get_ts_stride(ts_csr_val, x31_rand)

   snippet_start(random_num = rand_mark, rega = rx1, regb = rx2)
   coop_offset = snp_align_coop_addr(ts_csr_val, base_offset)
   tensor_store_snip_run(num_ts_csr_val = ts_csr_val, num_x31_val = x31_val, memx = cts_mem2[memory_idx], addr_off = coop_offset, csr_regx = csr_reg, regx1 = rx1, regx2 = rx2, regx3 = rx3, is_coop = 1, ten_wait = ten_wait_en)
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
   tensor_coop_mhandler(regx1 = rx1, regx2 = rx2, is_coop = 1)
   csrr rx1, mepc
   addi rx1, rx1, 16
   csrw mepc, rx1
