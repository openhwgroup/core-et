import "$MINION_DIAGS/sting/snippets/common.pb"
import "$STING_ROOT/src/snippets/rv/rv_param_blocks.pb"
import "$MINION_DIAGS/sting/conf/utils/memory_test_param_blocks.pb"

resource
cpu_t        x

;; Check the reg.conf SNIP_ALL_CPU_TARGETS and SNIP_PER_CPU_TARGETS for the maximum
;; value of the available registers. On April 15, 2019, this number was 8.
;; For now, it is not interesting to know if the register is from the predictable
;; or unpredictable pool of registers.
;; For whatever reason the following did not work.
;; xreg_t       reg0, reg1, reg2, reg3, reg4, reg5, reg6, reg7

;; Integer Registers
axreg_t      reg0, reg1

pxreg_t      rx1

uc_memory_t  et_invalid_mem[64]@64:ET_TRAP_OVER_TRAP_IFETCH_MEM

snip_setup
    ; Empty

snip_init
 ;Empty

snip_run
@cpu: x
    li reg0, 0xdeaddead
    la reg0, et_invalid_mem

    ; Exception should be generated
    SELECT 1 {
         1 : jalr reg1,reg0,0
         1 : jalr reg1,reg0,56

         1 : jalr reg1,reg0,12
         1 : jalr reg1,reg0,52

         1 : jalr reg1,reg0,18
         1 : jalr reg1,reg0,46

         1 : jalr reg1,reg0,25
         1 : jalr reg1,reg0,39
    }

    ; We should never get here
    test_fail(regz = x0)
  
    li reg0, 0xdeadcaac

snip_check

;@cpu: x
;default_check_excp(regx = rx1, regy = rx2, regb = rb1, tm = TEST_PROPS.TEST_MODE,
;                   excp_delegatable_m = RV_PARAMS.CSR_PROPS.MEDELEG.IS_LOAD_ACCESS_FAULT_TRAP_DELEGATABLE,
;                   excp_delegatable_s = RV_PARAMS.CSR_PROPS.SEDELEG.IS_LOAD_ACCESS_FAULT_TRAP_DELEGATABLE,
;                   excp_deleg_priv_lvl = DELEG_SETTINGS_CONF.LOAD_ACCESS_FAULT_TRAP_DELEGATED,
;                   cmode = 0)

snip_handler

mhandler@cpu: x (et_invalid_mem:et_invalid_mem+64)
;    csrr rx1, mepc
    li rx1, 0xdeadbeef 
    addi rx1, reg1, 16
    csrw mepc, rx1

