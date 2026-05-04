
import "$MINION_DIAGS/sting/snippets/common.pb"
import "$STING_ROOT/src/snippets/rv/rv_param_blocks.pb"

resource

unum_t cpuid = snp_cpu_id()
;gnum_t event_id = snp_get_pmu_event(PMU_ENV.CNTR_NUM)

unum_t event_3 = snp_get_pmu_event(3)
unum_t event_4 = snp_get_pmu_event(4)
unum_t event_5 = snp_get_pmu_event(5)
unum_t event_6 = snp_get_pmu_event(6)
unum_t event_7 = snp_get_pmu_event(7)
unum_t event_8 = snp_get_pmu_event(8)


;unum_t m_base_csr     = 0xB00
;unum_t m_counter_csr  = snp_add(m_base_csr, PMU_ENV.CNTR_NUM)
;unum_t su_base_csr    = 0xC00
;unum_t su_counter_csr = snp_add(su_base_csr, PMU_ENV.CNTR_NUM)

;unum_t base_event_csr = 0x320
;unum_t event_csr      = snp_add(base_event_csr, PMU_ENV.CNTR_NUM)

axreg_t rx1


snip_setup
    ; Empty


snip_init
   ; Empty


snip_run
@cpu: 0,2 .. BOARD_CONF.NUM_CPUS

   if (cpu_id == 0) :

      if (TEST_PROPS.TEST_MODE != RV_PARAMS.RV_MODE.MACHINE):
         et_jump_to_m_mode()
      else:
         ;csrw event_csr, x0 ; set no_count
         ;; reset the counter
         ;csrw m_counter_csr, x0 ; reset the counter to 0

         csrw mhpmevent3, x0 ; set no_count
         csrw mhpmevent4, x0 ; set no_count
         csrw mhpmevent5, x0 ; set no_count
         csrw mhpmevent6, x0 ; set no_count
         csrw mhpmevent7, x0 ; set no_count
         csrw mhpmevent8, x0 ; set no_count

         csrw mhpmcounter3, x0 ; reset the counter to 0
         csrw mhpmcounter4, x0 ; reset the counter to 0
         csrw mhpmcounter5, x0 ; reset the counter to 0
         csrw mhpmcounter6, x0 ; reset the counter to 0
         csrw mhpmcounter7, x0 ; reset the counter to 0
         csrw mhpmcounter8, x0 ; reset the counter to 0

         ; set the counters with the events
         li rx1, event_3
         csrw mhpmevent3, rx1
         li rx1, event_4
         csrw mhpmevent4, rx1
         li rx1, event_5
         csrw mhpmevent5, rx1
         li rx1, event_6
         csrw mhpmevent6, rx1
         li rx1, event_7
         csrw mhpmevent7, rx1
         li rx1, event_8
         csrw mhpmevent8, rx1

         ;; set the counter with the event
         ;li rx1, event_id
         ;csrw event_csr, rx1
      endif
   endif


snip_check
   ; Empty


snip_handler
mhandler@cpu: 0,2 .. BOARD_CONF.NUM_CPUS

   if (TEST_PROPS.TEST_MODE == RV_PARAMS.RV_MODE.MACHINE):
      ; We should never trap in Machine mode
      test_fail(regz = rx1)
   else:
      ; Enable reading counters from User/Supervisor Mode
      li rx1, 0xFFFFFFF8
      csrrs x0, mcounteren, rx1
      if (TEST_PROPS.TEST_MODE == RV_PARAMS.RV_MODE.USER):
         csrrs x0, scounteren, rx1
      endif

      ;csrw event_csr, x0 ; set no_count
      ;; reset the counter
      ;csrw m_counter_csr, x0 ; reset the counter to 0

      csrw mhpmevent3, x0 ; set no_count
      csrw mhpmevent4, x0 ; set no_count
      csrw mhpmevent5, x0 ; set no_count
      csrw mhpmevent6, x0 ; set no_count
      csrw mhpmevent7, x0 ; set no_count
      csrw mhpmevent8, x0 ; set no_count

      csrw mhpmcounter3, x0 ; reset the counter to 0
      csrw mhpmcounter4, x0 ; reset the counter to 0
      csrw mhpmcounter5, x0 ; reset the counter to 0
      csrw mhpmcounter6, x0 ; reset the counter to 0
      csrw mhpmcounter7, x0 ; reset the counter to 0
      csrw mhpmcounter8, x0 ; reset the counter to 0

      ; set the counters with the events
      li rx1, event_3
      csrw mhpmevent3, rx1
      li rx1, event_4
      csrw mhpmevent4, rx1
      li rx1, event_5
      csrw mhpmevent5, rx1
      li rx1, event_6
      csrw mhpmevent6, rx1
      li rx1, event_7
      csrw mhpmevent7, rx1
      li rx1, event_8
      csrw mhpmevent8, rx1

      ;; set the counter with the event
      ;li rx1, event_id
      ;csrw event_csr, rx1
   endif

   csrr rx1, mepc
   addi rx1, rx1, 4
   csrw mepc, rx1
