
import "$MINION_DIAGS/sting/snippets/common.pb"
import "$STING_ROOT/src/snippets/rv/rv_param_blocks.pb"

resource

unum_t cpuid = snp_cpu_id()

;unum_t m_base_csr     = 0xB00
;unum_t m_counter_csr  = snp_add(m_base_csr, PMU_ENV.CNTR_NUM)
;unum_t su_base_csr    = 0xC00
;unum_t su_counter_csr = snp_add(su_base_csr, PMU_ENV.CNTR_NUM)

axreg_t rx1


snip_setup
    ; Empty


snip_init
   ; Empty


snip_run
@cpu: 0,2 .. BOARD_CONF.NUM_CPUS
   if (cpu_id == 0) :
      if (TEST_PROPS.TEST_MODE == RV_PARAMS.RV_MODE.MACHINE):
         csrr rx1, mhpmcounter3 
         csrr rx1, mhpmcounter4 
         csrr rx1, mhpmcounter5 
         csrr rx1, mhpmcounter6 
         csrr rx1, mhpmcounter7 
         csrr rx1, mhpmcounter8 
      else :
         csrr rx1, hpmcounter3 
         csrr rx1, hpmcounter4 
         csrr rx1, hpmcounter5 
         csrr rx1, hpmcounter6 
         csrr rx1, hpmcounter7 
         csrr rx1, hpmcounter8 
      endif
   endif

snip_check
   ; Empty


snip_handler
mhandler@cpu: 0,2 .. BOARD_CONF.NUM_CPUS

   ; We should never trap in Machine Mode and
   ; SNP_pmu_setup_cntr should have enabled read access from U/S Mode
   test_fail(regz = rx1)

   csrr rx1, mepc
   addi rx1, rx1, 4
   csrw mepc, rx1
