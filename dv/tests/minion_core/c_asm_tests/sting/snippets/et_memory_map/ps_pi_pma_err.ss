import "$STING_ROOT/src/snippets/rv/rv_param_blocks.pb"
import "$MINION_DIAGS/sting/conf/utils/memory_test_param_blocks.pb"
import "$MINION_DIAGS/sting/snippets/common.pb"

resource
cpu_t x
;; Check the reg.conf SNIP_ALL_CPU_TARGETS and SNIP_PER_CPU_TARGETS for the maximum
;; value of the available registers. On April 15, 2019, this number was 8.
;; For now, it is not interesting to know if the register is from the predictable
;; or unpredictable pool of registers.
;; For whatever reason the following did not work.
;; xreg_t       reg0, reg1, reg2, reg3, reg4, reg5, reg6, reg7

;; Integer Registers
axreg_t      reg0, reg1, reg2, reg3
pxreg_t      reg4, reg5, reg6, reg7

;; Single-precision FP Registers
pfreg_t      rf1, rf2, rf3, rf4, rf5

uc_memory_t  et_pspi_mem_err[64]@64:ET_PS_PI_MEM_ACCESS_ERR

;; Declared here but will be used in the rv_param_blocks.pb file.
unum_t cntr = 0

snip_setup
    ; Empty

snip_init

snip_run
@cpu: x

    SELECT 1 {
        1 : mova.m.x reg0
        1 : mova.m.x reg1
        1 : mova.m.x reg2
        1 : mova.m.x reg3
        1 : mova.m.x reg4
        1 : mova.m.x reg5
        1 : mova.m.x reg6
        1 : mova.m.x reg7
    }

    ;; The following workaround is needed/necessary
    ;; See VERIF-851/RTLMIN-3932 for further details
    mov.m.x m0, x0, 0xff

    la reg1, et_pspi_mem_err
    et_ps_st_inst_grp(freg = rf1, offset = 0, base_reg = reg1)
    et_ps_ld_inst_grp(freg = rf1, offset = 0, base_reg = reg1)

snip_check
    ; Empty

snip_handler

mhandler@cpu: x

  csrrs reg0, mcause, x0
  li reg2, 0x7  ; Store/amo access fault
  beq reg0, reg2, continue
  li reg2, 0x5  ; load access fault
  beq reg0, reg2, continue
  test_fail(regz = reg1)

 continue:
   csrr reg2, mepc
   addi reg2, reg2, 4
   csrw mepc, reg2
