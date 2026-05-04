import "$STING_ROOT/src/snippets/rv/rv_param_blocks.pb"
; import "$STING_ROOT/src/snippets/rv/algorithms/memory/memory_test_param_blocks.pb"

resource
cpu_t        x
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

;uc_memory_t  et_maxion_region_mem[64]@64:ET_MAXION_REGION  ;avoid 0x29 Trap data bus error
uc_memory_t  et_scp_region_mem[64]@64:ET_SCP_REGION
uc_memory_t  et_os_region_mem[64]@64:ET_OS_REGION
uc_memory_t  et_dram_region_mem[64]@64:ET_DRAM_REGION

unum_t rval0 = snp_urandrange(0, MAX_DEFS.MAX_UINT)
unum_t rval1 = snp_urandrange(0, MAX_DEFS.MAX_UINT)
unum_t rval2 = snp_urandrange(0, MAX_DEFS.MAX_UINT)
unum_t rval3 = snp_urandrange(0, MAX_DEFS.MAX_UINT)
unum_t rval4 = snp_urandrange(0, MAX_DEFS.MAX_UINT)
unum_t rval5 = snp_urandrange(0, MAX_DEFS.MAX_UINT)
unum_t rval6 = snp_urandrange(0, MAX_DEFS.MAX_UINT)
unum_t rval7 = snp_urandrange(0, MAX_DEFS.MAX_UINT)

snip_setup
    ; Empty

snip_init
@cpu: x
;    li reg0, rval0
;    ;;; TBD: This seems to work for now, but is it correct? Not sure.
;    ;;; See PRM-19 note: It should only be accessible from the Maxions and the service processor.
;    la reg1, et_maxion_region_mem
;    sd reg0,  0 (reg1)
;    sd reg0,  8 (reg1)
;    sd reg0, 16 (reg1)
;    sd reg0, 24 (reg1)
;    sd reg0, 32 (reg1)
;    sd reg0, 40 (reg1)
;    sd reg0, 48 (reg1)
;    sd reg0, 56 (reg1)
;
    ; TBD: Unclear why this did not work
    ; la reg1, et_maxion_region_mem
    ; write_val_to_memblk(regx = reg0, regb = reg1, val = rval, size = 64)

    li reg0, rval1
    la reg2, et_scp_region_mem
    sd reg0,  0 (reg2)
    sd reg0,  8 (reg2)
    sd reg0, 16 (reg2)
    sd reg0, 24 (reg2)
    sd reg0, 32 (reg2)
    sd reg0, 40 (reg2)
    sd reg0, 48 (reg2)
    sd reg0, 56 (reg2)

    li reg0, rval2
    la reg3, et_os_region_mem
    sd reg0,  0 (reg3)
    sd reg0,  8 (reg3)
    sd reg0, 16 (reg3)
    sd reg0, 24 (reg3)
    sd reg0, 32 (reg3)
    sd reg0, 40 (reg3)
    sd reg0, 48 (reg3)
    sd reg0, 56 (reg3)

    li reg0, rval3
    la reg4, et_dram_region_mem
    sd reg0,  0 (reg4)
    sd reg0,  8 (reg4)
    sd reg0, 16 (reg4)
    sd reg0, 24 (reg4)
    sd reg0, 32 (reg4)
    sd reg0, 40 (reg4)
    sd reg0, 48 (reg4)
    sd reg0, 56 (reg4)

snip_run
@cpu: x
;    la reg1, et_maxion_region_mem
;    SELECT 1 {
;        1 : ld  reg0,  0 (reg1)
;        1 : ld  reg0, 56 (reg1)
;
;        1 : sd  reg0,  0 (reg1)
;        1 : sd  reg0, 56 (reg1)
;
;        1 : lw  reg0, 12 (reg1)
;        1 : lw  reg0, 52 (reg1)
;
;        1 : sw  reg0, 12 (reg1)
;        1 : sw  reg0, 52 (reg1)
;
;        1 : lh  reg0, 18 (reg1)
;        1 : lh  reg0, 46 (reg1)
;
;        1 : sh  reg0, 18 (reg1)
;        1 : sh  reg0, 46 (reg1)
;
;        1 : lb  reg0, 25 (reg1)
;        1 : lb  reg0, 39 (reg1)
;
;        1 : sb  reg0, 25 (reg1)
;        1 : sb  reg0, 39 (reg1)
;
;        1 : flw  rf1, 28 (reg1)
;        1 : fsw  rf1, 36 (reg1)
;    }

    la reg2, et_scp_region_mem
    SELECT 1 {
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

    la reg3, et_os_region_mem
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

    la reg4, et_dram_region_mem
    SELECT 1 {
        1 : ld  reg0,  0 (reg4)
        1 : ld  reg0, 56 (reg4)

        1 : sd  reg0,  0 (reg4)
        1 : sd  reg0, 56 (reg4)

        1 : lw  reg0, 12 (reg4)
        1 : lw  reg0, 52 (reg4)

        1 : sw  reg0, 12 (reg4)
        1 : sw  reg0, 52 (reg4)

        1 : lh  reg0, 18 (reg4)
        1 : lh  reg0, 46 (reg4)

        1 : sh  reg0, 18 (reg4)
        1 : sh  reg0, 46 (reg4)

        1 : lb  reg0, 25 (reg4)
        1 : lb  reg0, 39 (reg4)

        1 : sb  reg0, 25 (reg4)
        1 : sb  reg0, 39 (reg4)

        1 : flw  rf4, 28 (reg4)
        1 : fsw  rf4, 36 (reg4)
    }

snip_check
    ; Empty
