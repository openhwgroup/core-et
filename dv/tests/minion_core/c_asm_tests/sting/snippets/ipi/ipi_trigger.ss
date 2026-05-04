
import "$STING_ROOT/src/snippets/rv/rv_param_blocks.pb"
import "$MINION_DIAGS/sting/snippets/common.pb"
import "$MINION_DIAGS/sting/snippets/ipi/ipi.pb"

resource 

unum_t rand_num = snp_randnum_glbl()
unum_t cpuid = snp_cpu_id()
unum_t is_reset_cpu = snp_is_reset_cpu(cpuid)

; Using a random barrier to sync all the HARTs
unum_t barrier = glb_snp_get_ipi_flb_num(ET_TEST_ENV.STING_RAND, cpuid, EOT_ENV.FLB_NUM)
; snp_get_flb_addr() will return barrier's FLB address with the correct Shire bits set
unum_t flb_addr = snp_get_flb_addr(barrier)
; flb_match_val corresponds to the total number of HARTS in the same Neigh as 'cpuid'
unum_t flb_match_val = snp_get_ipi_flb_match_val(ET_TEST_ENV.STING_RAND, cpuid)
; snp_get_flb_csr_value_cov() returns the value to write into the ESR address considering that we are using the random barrier chosen above
unum_t flb_csr_val = snp_get_flb_csr_value_cov(barrier, flb_match_val)

; IPI Trigger
unum_t esr_addr_ipi_trigger = snp_get_trigger_addr(cpuid)
unum_t ipi_trigger_value = snp_get_ipi_trigger_value(ET_TEST_ENV.STING_RAND, cpuid)

axreg_t rx1, rx2, rx3

gmemory_t save_mtrap[BOARD_CONF.NUM_CPUS][64]@64:ET_DRAM_REGION
gmemory_t save_mie[BOARD_CONF.NUM_CPUS][64]@64:ET_DRAM_REGION


snip_setup 
    ; Empty
    ; For current Sting version, this is a placeholder


snip_init 

@cpu: *
   ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
   ;;; Reset FLB Barrier
   ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
if (is_reset_cpu == 1):
    li rx1, flb_addr
    sd x0, 0(rx1)
    fence iorw, iorw
endif



snip_run 
@cpu: *
   ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
   ;;; WATERMARK
   ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
   snippet_start(random_num = rand_num, rega = rx1, regb = rx2)

   ; rv_save_abi_xregs() and rv_restore_abi_xregs() are utility parameter blocks
   ; defined in the file rv_param_blocks.pb to save and restore the registers used
   ; by the test This allows the test context to not get destroyed by the C/C++ code
   rv_save_abi_xregs()

   backup_tvec(bak_mem = save_mtrap[cpuid], regx1 = rx1, regx2 = rx2)
   backup_sw_interrupt(bak_mem = save_mie[cpuid], regx1 = rx1, regx2 = rx2)

   ; 'handle_interrupt_mac' is a label defined in
   ; the $MINION_DIAGS/sting/snippets/ipi/ih_override.cpp file
   ;set_tvec(handle_interrupt = handle_interrupt_mac, regx1 = rx1)

   ; Enable Software Interrupts
   enable_sw_interrupt(regx1 = rx1)

   ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
   ;;; FLB Synchronization
   ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
   ; First make sure there is no pending IPREFETCHES that could delay
   ; the fetching of the instructions after the barrier
   wait_iprefetch(pb_rx1 = rx1, pb_rx2 = rx2)

   ; Check the Barrier
   li rx1, flb_csr_val
   csrrw rx1, flb0, rx1

   ; If I am not the last one, stall
   ; otherwise, IPI trigger
   bne x0, rx1, trigger_ipi

wait_ipi:
   SELECT 1 {
      1 : ipi_stall()
      1 : ipi_wfi()
      1 : j wait_ipi ; Infinite loop
   }

   j done_ipi

trigger_ipi:
if ET_TEST_ENV.SILICON_MODE == 0:
   RANDINST 500:650
else:
   li rx3, 650
ipi_trigger_nops:
   add x0, x0, x0
   addi rx3, rx3, -1
   bne rx3, x0, ipi_trigger_nops
endif
   set_ipi_trigger(trigger_val = ipi_trigger_value, ipi_esr_addr = esr_addr_ipi_trigger, regx1 = rx1, regx2 = rx2)


done_ipi:
   restore_tvec(bak_mem = save_mtrap[cpuid], regx1 = rx1, regx2 = rx2)
   restore_sw_interrupt(bak_mem = save_mie[cpuid], regx1 = rx1, regx2 = rx2)

   rv_restore_abi_xregs()

   ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
   ;;; WATERMARK
   ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
   snippet_end(random_num = rand_num, rega = rx1, regb = rx2)



snip_check 
   ; Empty

snip_handler
mhandler@cpu: *

    ; Skip instruction that trapped
    csrr rx1, mepc
    addi rx1, rx1, 4
    csrw mepc, rx1
