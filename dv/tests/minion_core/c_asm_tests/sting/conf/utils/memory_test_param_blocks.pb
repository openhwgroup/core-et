;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Parametrized blocks for more constrained instruction generation
; for memory operations
;
; The following param block utilities were provided by Valtrix:
; mem_op
; ld_instr_grp
; st_inst_grp
;
; Addition utilities were written the Esperanto DV Team.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; Parameter block for memory operation
;
pb_start mem_op(inst, regx, offst, regb)

    inst regx, offst (regb)

pb_end

; Parameter block for load instruction group
;
pb_start ld_inst_grp(regx, offst, regb)

    SELECT 1 {
        10 : lb  regx, offst (regb)
        10 : lbu regx, offst (regb)
        10 : lh  regx, offst (regb)
        10 : lhu regx, offst (regb)
        10 : lw  regx, offst (regb)
if RV_PARAMS.HAS_RV64I == 1 || RV_PARAMS.HAS_RV128I == 1:
        10 : lwu regx, offst (regb)
        10 : ld  regx, offst (regb)
endif
    }

pb_end


; Parameter block for store instruction group
;
pb_start st_inst_grp(regx, offst, regb)

    SELECT 1 {
        10 : sb  regx, offst (regb)
        10 : sh  regx, offst (regb)
        10 : sw  regx, offst (regb)
if RV_PARAMS.HAS_RV64I == 1 || RV_PARAMS.HAS_RV128I == 1:
        10 : sd  regx, offst (regb)
endif
    }

pb_end

;
; Parameter block for Floating-Point Load instruction group
;
pb_start fp_ld_inst_grp(freg, offset, base_reg)
    SELECT 1 {
        10 : flw freg, offset (base_reg)
if RV_PARAMS.HAS_D_EXT == 1:
        10 : fld freg, offset (base_reg)
endif
    }

pb_end

;
; Parameter block for Floating-Point Store instruction group
;
pb_start fp_st_inst_grp(freg, offset, base_reg)
    SELECT 1 {
        10 : fsw freg, offset (base_reg)
if RV_PARAMS.HAS_D_EXT == 1:
        10 : fsd freg, offset (base_reg)
endif
    }

pb_end

;
; Parameter block for ET Packed-Single Load  instruction group
;
pb_start et_ps_ld_inst_grp(freg, offset, base_reg)
    SELECT 1 {
        10 : flw.ps freg, offset (base_reg)
        10 : flq2   freg, offset (base_reg)
        10 : fbc.ps freg, offset (base_reg)
    }

pb_end

;
; Parameter block for ET Packed-Single Store instruction group
;
pb_start et_ps_st_inst_grp(freg, offset, base_reg)
    SELECT 1 {
        10 : fsw.ps freg, offset (base_reg)
        10 : fsq2   freg, offset (base_reg)
    }

pb_end

;
; Parameter block for ET Atomic Load instruction group
;
pb_start et_atomic_ld_inst_grp(fd, rs1)
    SELECT 1 {
        10 : flwl.ps fd, (rs1)
        10 : flwg.ps fd, (rs1)
    }

pb_end

;
; Parameter block for ET Atomic Store instruction group
;
pb_start et_atomic_st_inst_grp(fs1, rs1)
    SELECT 1 {
        10 : fswl.ps fs1, (rs1)
        10 : fswg.ps fs1, (rs1)
    }

pb_end

;
; Parameter block for ET Atomic Local/Global Scalar 32-bit Atomic operations
; Requirement: Base address specified in rs1 must be 4B aligned
;
pb_start et_atomic_ops_32b_inst_grp(rd, rs2, rs1)
    SELECT 1 {
        10 : amoswapl.w rd, rs2, (rs1)
        10 : amoandl.w  rd, rs2, (rs1)
        10 : amoorl.w   rd, rs2, (rs1)
        10 : amoxorl.w  rd, rs2, (rs1)
        10 : amoaddl.w  rd, rs2, (rs1)
        10 : amominl.w  rd, rs2, (rs1)
        10 : amomaxl.w  rd, rs2, (rs1)
        10 : amominul.w rd, rs2, (rs1)
        10 : amomaxul.w rd, rs2, (rs1)

        10 : amoswapg.w rd, rs2, (rs1)
        10 : amoandg.w  rd, rs2, (rs1)
        10 : amoorg.w   rd, rs2, (rs1)
        10 : amoxorg.w  rd, rs2, (rs1)
        10 : amoaddg.w  rd, rs2, (rs1)
        10 : amoming.w  rd, rs2, (rs1)
        10 : amomaxg.w  rd, rs2, (rs1)
        10 : amominug.w rd, rs2, (rs1)
        10 : amomaxug.w rd, rs2, (rs1)
    }

pb_end

;
; Parameter block for ET Atomic Local/Global Scalar 64-bit Atomic operations
; Requirement: Base address specified in rs1 must be 8B aligned
;
pb_start et_atomic_ops_64b_inst_grp(rd, rs2, rs1)
    SELECT 1 {
        10 : amoswapl.d rd, rs2, (rs1)
        10 : amoandl.d  rd, rs2, (rs1)
        10 : amoorl.d   rd, rs2, (rs1)
        10 : amoxorl.d  rd, rs2, (rs1)
        10 : amoaddl.d  rd, rs2, (rs1)
        10 : amominl.d  rd, rs2, (rs1)
        10 : amomaxl.d  rd, rs2, (rs1)
        10 : amominul.d rd, rs2, (rs1)
        10 : amomaxul.d rd, rs2, (rs1)

        10 : amoswapg.d rd, rs2, (rs1)
        10 : amoandg.d  rd, rs2, (rs1)
        10 : amoorg.d   rd, rs2, (rs1)
        10 : amoxorg.d  rd, rs2, (rs1)
        10 : amoaddg.d  rd, rs2, (rs1)
        10 : amoming.d  rd, rs2, (rs1)
        10 : amomaxg.d  rd, rs2, (rs1)
        10 : amominug.d rd, rs2, (rs1)
        10 : amomaxug.d rd, rs2, (rs1)
    }

pb_end

pb_start set_handler_action(action, action_mem, regx1, regx2)
   la regx1, action_mem
   li regx2, action
   sd regx2, 0(regx1)
pb_end

pb_start get_handler_action(action_mem, regx1, action_reg)
   la regx1, action_mem
   ld action_reg, 0(regx1)
pb_end
