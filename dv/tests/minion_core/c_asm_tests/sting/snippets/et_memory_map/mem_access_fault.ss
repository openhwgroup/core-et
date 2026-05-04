import "$STING_ROOT/src/snippets/rv/rv_param_blocks.pb"
import "$MINION_DIAGS/sting/conf/utils/memory_test_param_blocks.pb"

resource
cpu_t        x
unum_t       cntr = 0

;; Check the reg.conf SNIP_ALL_CPU_TARGETS and SNIP_PER_CPU_TARGETS for the maximum
;; value of the available registers. On April 15, 2019, this number was 8.
;; For now, it is not interesting to know if the register is from the predictable
;; or unpredictable pool of registers.
;; For whatever reason the following did not work.
;; xreg_t       reg0, reg1, reg2, reg3, reg4, reg5, reg6, reg7

;; Integer Registers
axreg_t      reg0, reg1, reg2, reg3
;; pxreg_t      reg4, reg5, reg6, reg7
pxreg_t      rx1, rx2, rx3

;; Single-precision FP Registers
pfreg_t      rf1, rf2, rf3

base_t       rb1[TRAP_DATA_MEMBLK.SIZE]@TRAP_DATA_MEMBLK.ALIGN

uc_memory_t  et_sp_region_mem[64]@64:ET_SP_REGION
uc_memory_t  et_scp_unused_region_mem[64]@64:ET_SCP_UNUSED_REGION
uc_memory_t  et_reserved_region_mem[64]@64:ET_RESERVED_REGION

snip_setup
    ; Empty

snip_init
@cpu: x

write_val_to_memblk(regx = rx1, regb = rb1, val = 0, size = TRAP_DATA_MEMBLK.SIZE)

snip_run
@cpu: x
    la reg1, et_sp_region_mem
    SELECT 0 {
        1 : ld  reg0,  0 (reg1)
        1 : ld  reg0, 56 (reg1)

        1 : sd  reg0,  0 (reg1)
        1 : sd  reg0, 56 (reg1)

        1 : lw  reg0, 12 (reg1)
        1 : lw  reg0, 52 (reg1)

        1 : sw  reg0, 12 (reg1)
        1 : sw  reg0, 52 (reg1)

        1 : lh  reg0, 18 (reg1)
        1 : lh  reg0, 46 (reg1)

        1 : sh  reg0, 18 (reg1)
        1 : sh  reg0, 46 (reg1)

        1 : lb  reg0, 25 (reg1)
        1 : lb  reg0, 39 (reg1)

        1 : sb  reg0, 25 (reg1)
        1 : sb  reg0, 39 (reg1)

        1 : flw  rf1, 28 (reg1)
        1 : fsw  rf1, 36 (reg1)
    }

    la reg2, et_scp_unused_region_mem
    SELECT 0 {
        1 : ld  reg0,  0 (reg2)
        1 : ld  reg0, 56 (reg2)

        1 : sd  reg0,  0 (reg2)
        1 : sd  reg0, 56 (reg2)

        1 : lw  reg0, 12 (reg2)
        1 : lw  reg0, 52 (reg2)

        1 : sw  reg0, 12 (reg2)
        1 : sw  reg0, 52 (reg2)

        1 : lh  reg0, 18 (reg2)
        1 : lh  reg0, 46 (reg2)

        1 : sh  reg0, 18 (reg2)
        1 : sh  reg0, 46 (reg2)

        1 : lb  reg0, 25 (reg2)
        1 : lb  reg0, 39 (reg2)

        1 : sb  reg0, 25 (reg2)
        1 : sb  reg0, 39 (reg2)

        1 : flw  rf2, 28 (reg2)
        1 : fsw  rf2, 36 (reg2)

    }

    la reg3, et_reserved_region_mem
    SELECT 1 {
        1 : ld  reg0,  0 (reg3)
        1 : ld  reg0, 56 (reg3)

        1 : sd  reg0,  0 (reg3)
        1 : sd  reg0, 56 (reg3)

        1 : lw  reg0, 12 (reg3)
        1 : lw  reg0, 52 (reg3)

        1 : sw  reg0, 12 (reg3)
        1 : sw  reg0, 52 (reg3)

        1 : lh  reg0, 18 (reg3)
        1 : lh  reg0, 46 (reg3)

        1 : sh  reg0, 18 (reg3)
        1 : sh  reg0, 46 (reg3)

        1 : lb  reg0, 25 (reg3)
        1 : lb  reg0, 39 (reg3)

        1 : sb  reg0, 25 (reg3)
        1 : sb  reg0, 39 (reg3)

        1 : flw  rf3, 28 (reg3)
        1 : fsw  rf3, 36 (reg3)
    }

snip_check

@cpu: x
default_check_excp(regx = rx1, regy = rx2, regb = rb1, tm = TEST_PROPS.TEST_MODE,
                   excp_delegatable_m = RV_PARAMS.CSR_PROPS.MEDELEG.IS_LOAD_ACCESS_FAULT_TRAP_DELEGATABLE,
                   excp_delegatable_s = RV_PARAMS.CSR_PROPS.SEDELEG.IS_LOAD_ACCESS_FAULT_TRAP_DELEGATABLE,
                   excp_deleg_priv_lvl = DELEG_SETTINGS_CONF.LOAD_ACCESS_FAULT_TRAP_DELEGATED,
                   cmode = 0)

snip_handler

mhandler@cpu: x

    default_mhandler(regx = rx1, regy = rx2, regz = rx3, regb = rb1, tm = TEST_PROPS.TEST_MODE)

shandler@cpu: x

    default_shandler(regx = rx1, regy = rx2, regz = rx3, regb = rb1, tm = TEST_PROPS.TEST_MODE)

uhandler@cpu: x

    default_uhandler(regx = rx1, regy = rx2, regz = rx3, regb = rb1, tm = TEST_PROPS.TEST_MODE)
