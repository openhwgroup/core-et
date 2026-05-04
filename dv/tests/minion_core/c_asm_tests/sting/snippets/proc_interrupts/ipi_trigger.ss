import "$MINION_DIAGS/sting/snippets/common.pb"
import "$STING_ROOT/src/snippets/rv/rv_param_blocks.pb"

; This is a resource block where different hardware resources used
; by each snippet can be reserved. In this snippet, we are reserving
; a general purpose register. More complex resource notations will be
; presented in the next set of snippets
resource ;## STING PHASE
cpu_t x, y

; Minion-related variables
unum_t  sid_x = 0
unum_t  sid_y = 0
unum_t  mid_y = 0
unum_t  tid_x = 0
unum_t  tid_y = 0
unum_t  mmask_y = 0
unum_t  smask_y = 0
unum_t  ipi_trigger_shire = 0
unum_t  ipi_trigger_mask = 0

; Initial
unum_t  credinc0_esr    = 0x803400C0
unum_t  credinc1_esr    = 0x803400C8
unum_t  wait_event = snp_urandrange_glbl(0, 3)
unum_t  intr_bit = snp_urandrange_glbl(0, 2)
unum_t  cpuid = snp_cpu_id()

axreg_t rx1
axreg_t rx2
gmemory_t save_mtrap[BOARD_CONF.NUM_CPUS][64]@64:ET_DRAM_REGION

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
@cpu: x
   ; Stalls execution, expects to receive IPI, self-check cause in trap handler
   ; ## Pre-Processing ##
   sid_y   = snp_rshift(y, 6)
   mid_y   = snp_rshift(y, 1)
   tid_y   = snp_and(y, 0x0000000000000001)
   mmask_y =  snp_lshift(1, mid_y)
   smask_y =  snp_lshift(sid_y, 22)

   ; Jump into the helper function to register an interrupt handler
   ;rv_save_abi_xregs()
   ;la   rx1, ipi_trigger_handler
   ;jalr rx1
   ;rv_restore_abi_xregs()

   ; Read machine interrupt-enable register, keep result in mem
   la rx1, save_mtrap[cpuid]
   csrr rx2, mtvec
   sd rx2, 0(rx1)

   ; Set machine interrupt-enable register
   la rx1, handle_interrupt_mac
   csrw mtvec, rx1

   ; Enable software interrupts in machine mode
   li  rx1, 0x8
   csrs mie, rx1
   csrs mstatus, rx1

    
   ; Send credits to Y
   li  rx1, credinc0_esr
   ori rx1, rx1, smask_y
   li  rx2, mmask_y
   if (tid_y % 2) == 0:
    sd rx2, 0 (rx1) ; even thread
   else :
    sd rx2, 16 (rx1) ; odd thread
   endif

   if (wait_event == 0):
       ; csr write stall 
       csrwi stall, 0x1
   endif
   if (wait_event == 1):
       ; wfi stall
       wfi
   endif
   if (wait_event == 2):
      ; infinite loop
infinite_loop:
      ;addi rx1, x0, 1
      ;beq  x0, x0, infinite_loop   
      j infinite_loop
  endif

  ; Nop
  li  rx1, 100
  li  rx1, 100

  ; Restore machine interrupt-enable register, kept in save_mtrap gmem_t
  la rx1, save_mtrap[cpuid]
  ld rx2, 0(rx1)
  csrw mtvec, rx2

;  ; Send credits to Y  
;  li  rx1, credinc0_esr
;  ori rx1, rx1, smask_y
;  li  rx2, mmask_y
; 
;  if (tid_y % 2) == 0:
;   sd rx2, 0 (rx1) ; even thread
;  else :
;   sd rx2, 16 (rx1) ; odd thread
;  endif
;

  ; Send credits to All shire for counter0 of th0 th1
  li rx1, credinc0_esr
  ori rx1, rx1, smask_y   ; assumming x y executing in the same shire
  li rx2, 0xFFFFFFFF ; credits for all minus x 

  sd rx2,  0 (rx1)    ; counter 0 th0  iorw, iorw
  fence iorw, iorw
  sd rx2, 16 (rx1)    ; counter 0 th1
  fence iorw, iorw
  ; wait for credits from X (using counter 0)
  csrwi fcc, 0

@cpu: y
   ; ## Pre-Processing ##
   ; Y sends IPI to X
   ; Y needs to be in machine-level mode
   ; X can be in any mode
   ; Y should be setting IPI trigger of X, read IPI trigger, and then clean state

   ; prepare messaging to X
   sid_x = snp_rshift(x, 6)
   tid_x = snp_and(x, 0x0000000000000001)
   ipi_trigger_mask = snp_lshift(1, x)
   ipi_trigger_shire = snp_rshift(sid_x, 22)

   ; wait for credits from X (using counter 0)
   csrwi fcc, 0

   ; set ipi mask based on thread id
   li rx1, ipi_trigger_mask
   li rx2, ipi_trigger_shire
   or rx1, rx2, rx1
   li rx2, 0x80F40090
   sd rx1, 0(rx2)

   ; wait for credits from X (using counter 0)
   csrwi fcc, 0

@cpu: *

   if ((cpuid != x) && (cpuid != y)):
     ; wait for credits from X(using counter 0)
     csrwi fcc, 0 
   endif


; Similarly the 'snip_check' section is invoked at the end of the test after all
; the hardware threads are done with their random code section. It can be used to
; carry out any test specific checks like verifying the contents of a memory buffer
; against the expected values.
;
snip_check
   ; Empty

snip_handler

mhandler@cpu: *
    csrr rx1, mepc
    addi rx1, rx1, 4
    csrw mepc, rx1


