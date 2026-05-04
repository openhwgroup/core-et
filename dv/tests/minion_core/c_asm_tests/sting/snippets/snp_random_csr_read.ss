
import "$MINION_DIAGS/sting/snippets/common.pb"
import "$STING_ROOT/src/snippets/rv/rv_param_blocks.pb"

resource

cpu_t x

axreg_t rx1

snip_setup
    ; Empty
    ; For current Sting version, this is a placeholder

snip_init
   ; Empty

snip_run
@cpu: x

   csrrwi rx1, validation3, 9
   SELECT 1 {
    10:  csrr rx1, fflags
    5:   csrr rx1, frm
    5:   csrr rx1, fcsr
if TEST_PROPS.TEST_MODE != RV_PARAMS.RV_MODE.USER:
    10:  csrr rx1, sstatus
    2:   csrr rx1, sie
    1:   csrr rx1, stvec
    1:   csrr rx1, sscratch
    2:   csrr rx1, sepc
    2:   csrr rx1, scause
    2:   csrr rx1, stval
    2:   csrr rx1, sip
    2:   csrr rx1, satp
if TEST_PROPS.TEST_MODE == RV_PARAMS.RV_MODE.MACHINE:
    10:  csrr rx1, mstatus
    1:   csrr rx1, misa
    3:   csrr rx1, medeleg
    3:   csrr rx1, mideleg
    3:   csrr rx1, mie
    1:   csrr rx1, mtvec
    1:   csrr rx1, mscratch
    3:   csrr rx1, mepc
    3:   csrr rx1, mcause
    3:   csrr rx1, mtval
    3:   csrr rx1, mip
endif
endif
   }

snip_check
   ; Empty


