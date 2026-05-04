import "$STING_ROOT/src/snippets/rv/rv_param_blocks.pb"
import "$MINION_DIAGS/sting/conf/utils/memory_test_param_blocks.pb"

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

;uc_memory_t  et_maxion_region_mem[64]@64:ET_MAXION_REGION  avoid trap 0x29 data bus error
uc_memory_t  et_scp_region_mem[64]@64:ET_SCP_REGION
uc_memory_t  et_os_region_mem[64]@64:ET_OS_REGION
uc_memory_t  et_dram_region_mem[64]@64:ET_DRAM_REGION

unum_t rval1 = snp_urandrange(0, MAX_DEFS.MAX_UINT)
unum_t rval2 = snp_urandrange(0, MAX_DEFS.MAX_UINT)
unum_t rval3 = snp_urandrange(0, MAX_DEFS.MAX_UINT)
unum_t rval4 = snp_urandrange(0, MAX_DEFS.MAX_UINT)
unum_t rval5 = snp_urandrange(0, MAX_DEFS.MAX_UINT)

;; Declared here but will be used in the rv_param_blocks.pb file.
unum_t cntr = 0

snip_setup
    ; Empty

snip_init
@cpu: x
    ;la reg1, et_maxion_region_mem
    ;write_val_to_memblk(regx = reg0, regb = reg1, val = rval1, size = 64)

    la reg1, et_scp_region_mem
    write_val_to_memblk(regx = reg0, regb = reg1, val = rval2, size = 64)

    la reg1, et_os_region_mem
    write_val_to_memblk(regx = reg0, regb = reg1, val = rval3, size = 64)

    la reg1, et_dram_region_mem
    write_val_to_memblk(regx = reg0, regb = reg1, val = rval4, size = 64)

snip_run
@cpu: x

    ;; The following workaround is needed/necessary
    ;; See VERIF-851/RTLMIN-3932 for further details
    mov.m.x m0, x0, 0xff

    ;la reg1, et_maxion_region_mem
    ;et_atomic_st_inst_grp(fs1 = rf1, rs1 = reg1)
    ;et_atomic_ld_inst_grp(fd  = rf1, rs1 = reg1)

    la reg2, et_scp_region_mem
    et_atomic_st_inst_grp(fs1 = rf2, rs1 = reg2)
    et_atomic_ld_inst_grp(fd  = rf2, rs1 = reg2)

    la reg3, et_os_region_mem
    et_atomic_st_inst_grp(fs1 = rf3, rs1 = reg3)
    et_atomic_ld_inst_grp(fd  = rf3, rs1 = reg3)

    la reg4, et_dram_region_mem
    et_atomic_st_inst_grp(fs1 = rf4, rs1 = reg4)
    et_atomic_ld_inst_grp(fd  = rf4, rs1 = reg4)


snip_check
    ; Empty
