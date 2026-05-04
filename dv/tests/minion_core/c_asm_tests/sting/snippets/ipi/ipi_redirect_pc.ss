
import "$STING_ROOT/src/snippets/rv/rv_param_blocks.pb"
import "$MINION_DIAGS/sting/snippets/common.pb"
import "$MINION_DIAGS/sting/snippets/ipi/ipi.pb"

resource 
gnum_t first_ipi_redirect_pc[BOARD_CONF.NUM_CPUS]

unum_t cpuid = snp_cpu_id()

; IPI Redirect PC
unum_t is_set_redirect_pc = snp_is_set_redirect_pc(cpuid)
unum_t esr_addr_redirect_pc = snp_get_ipi_redirect_pc_addr(cpuid)
gmemory_t ipi_redirect_pc[64]@64

unum_t sm_mode = snp_urandrange(0, 2)

axreg_t rx1, rx2


snip_setup 
    ; Empty
    ; For current Sting version, this is a placeholder


snip_init 
@cpu: *
if (first_ipi_redirect_pc[cpuid] == 0):
   ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
   ;;; Write into the redirected PC a 'jalr' instruction
   ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
   ; Load into rx1, the encoding for 'jalr x0, x12, 0'
   li rx1, 0x60067 ;jalr x0, x12, 0
   ; Load the address where IPI is going to redirect
   la rx2, ipi_redirect_pc
   ; Store the 'jalr' instruction into the IPI redirect PC
   sd rx1, 0(rx2)
   li rx1, 0x400000000000000
   or rx2, rx1, rx2
   csrrw x0, evict_va, rx2

   ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
   ;;; Set IPI Redirect PC
   ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
if (is_set_redirect_pc == 1):
   ;; This snippet only runs in User mode
if (sm_mode == 1):
   ;; But if we want to test the S/M Mode case
if (IPI_REDIRECT.DELEGATE_TO_S == 1):
   li x10, 2
   ecall
else:
   et_jump_to_m_mode()
endif
else:
   ;; User Mode case
   set_ipi_redirect_pc(mem_redirect_pc = ipi_redirect_pc, ipi_esr_addr = esr_addr_redirect_pc, regx1 = rx1, regx2 = rx2)
endif
endif

   first_ipi_redirect_pc[cpuid] = snp_add(first_ipi_redirect_pc[cpuid], 1)
endif


snip_run
    ; Empty


snip_check 
   ; Empty


snip_handler
shandler@cpu: *
   set_ipi_redirect_pc(mem_redirect_pc = ipi_redirect_pc, ipi_esr_addr = esr_addr_redirect_pc, regx1 = rx1, regx2 = rx2)

   ; Skip instruction that trapped
   csrr rx1, sepc
   addi rx1, rx1, 4
   csrw sepc, rx1

mhandler@cpu: *
   set_ipi_redirect_pc(mem_redirect_pc = ipi_redirect_pc, ipi_esr_addr = esr_addr_redirect_pc, regx1 = rx1, regx2 = rx2)

   ; Skip instruction that trapped
   csrr rx1, mepc
   addi rx1, rx1, 4
   csrw mepc, rx1
