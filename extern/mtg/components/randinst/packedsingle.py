import re
from core.mtg import *
from components.encoding.enc import *
from components.randinst.randinst_base import RandinstBase

@register
class FLW_PS(FD_OFF_RS1_M0): pass

@register
class FSW_PS(FS2_OFF_RS1_M0): pass

@register
class FLQ2(FD_OFF_RS1): pass

@register
class FSQ2(FS2_OFF_RS1): pass

@register
class FBC_PS(FD_OFF_RS1_M0): pass

@register
class FBCI_PS(FD_IMM20_M0): pass

@register
class FBCX_PS(FD_RS1_M0): pass

@register
class FGW_PS(FD_FS1_RS2_M0): pass

@register
class FGH_PS(FD_FS1_RS2_M0): pass

@register
class FGB_PS(FD_FS1_RS2_M0): pass

@register
class FSCW_PS(FS3_FS1_RS2_M0): pass

@register
class FSCH_PS(FS3_FS1_RS2_M0): pass

@register
class FSCB_PS(FS3_FS1_RS2_M0): pass

@register
class FG32B_PS(FD_RS1_RS2_M0): pass

@register
class FG32H_PS(FD_RS1_RS2_M0): pass

@register
class FG32W_PS(FD_RS1_RS2_M0): pass

@register
class FSC32B_PS(FS3_RS1_RS2_M0): pass

@register
class FSC32H_PS(FS3_RS1_RS2_M0): pass

@register
class FSC32W_PS(FS3_RS1_RS2_M0): pass

@register
class FADD_PS(FD_FS1_FS2_M0_RM): pass

@register
class FSUB_PS(FD_FS1_FS2_M0_RM): pass

@register
class FMUL_PS(FD_FS1_FS2_M0_RM): pass

@register
class FDIV_PS(FD_FS1_FS2_M0_RM): pass

@register
class FMIN_PS(FD_FS1_FS2_M0): pass

@register
class FMAX_PS(FD_FS1_FS2_M0): pass

@register
class FSQRT_PS(FD_FS1_M0): pass

@register
class FMADD_PS(FD_FS1_FS2_FS3_M0_RM): pass

@register
class FMSUB_PS(FD_FS1_FS2_FS3_M0_RM): pass

@register
class FNMADD_PS(FD_FS1_FS2_FS3_M0_RM): pass

@register
class FNMSUB_PS(FD_FS1_FS2_FS3_M0_RM): pass

@register
class FCMOV_PS(FD_FS1_FS2_FS3_M0): pass

@register
class FCMOVM_PS(FD_FS1_FS2_M0_PI): pass

@register
class FCVT_PW_PS(FD_FS1_M0_RM): pass

@register
class FCVT_PWU_PS(FD_FS1_M0_RM): pass

@register
class FCVT_PS_PW(FD_FS1_M0_RM): pass

@register
class FCVT_PS_PWU(FD_FS1_M0_RM): pass

@register
class FCVT_PS_F16(FD_FS1_M0): pass

@register
class FCVT_F16_PS(FD_FS1_M0): pass

@register
class FSGNJ_PS(FD_FS1_FS2_M0_PI): pass

@register
class FSGNJN_PS(FD_FS1_FS2_M0_PI): pass

@register
class FSGNJX_PS(FD_FS1_FS2_M0_PI): pass

@register
class FMVZ_X_PS(RD_FS1_IDX): pass

@register
class FMVS_X_PS(RD_FS1_IDX): pass

@register
class FSWIZZ_PS(FD_FS1_IMM_M0): pass

@register
class FFRC_PS(FD_FS1_M0): pass

@register
class FROUND_PS(FD_FS1_M0_RM): pass

@register
class FEQM_PS(MD_FS1_FS2): pass

@register
class FEQ_PS(FD_FS1_FS2_M0): pass

@register
class FLEM_PS(MD_FS1_FS2_M0): pass

@register
class FLE_PS(FD_FS1_FS2_M0): pass

@register
class FLTM_PS(MD_FS1_FS2_M0): pass

@register
class FLT_PS(FD_FS1_FS2_M0): pass

@register
class FCLASS_PS(FD_FS1_M0): pass

@register
class FSIN_PS(FD_FS1_M0): pass

@register
class FEXP_PS(FD_FS1_M0_TRANS): pass

@register
class FLOG_PS(FD_FS1_M0_TRANS): pass

@register
class FRCP_PS(FD_FS1_M0_TRANS): pass

@register
class FRSQ_PS(FD_FS1_M0): pass

@register
class PackedSingle(RandinstBase):

   subcls = [FLW_PS, FSW_PS, FLQ2, FSQ2, FBC_PS, FBCI_PS, FBCX_PS, FGW_PS, FGH_PS, FGB_PS,
             FSCW_PS, FSCH_PS, FSCB_PS, FG32B_PS, FG32H_PS, FG32W_PS, FSC32B_PS, FSC32H_PS,
             FSC32W_PS, FADD_PS, FSUB_PS, FMUL_PS, FMIN_PS, FMAX_PS,
             FMADD_PS, FMSUB_PS, FNMADD_PS, FNMSUB_PS, FCMOV_PS, FCMOVM_PS, FCVT_PW_PS,
             FCVT_PWU_PS, FCVT_PS_PW, FCVT_PS_PWU, FCVT_PS_F16, FCVT_F16_PS, FSGNJ_PS,
             FSGNJX_PS, FMVZ_X_PS, FMVS_X_PS, FSWIZZ_PS, FFRC_PS, FROUND_PS, FEQM_PS,
             FEQ_PS, FLEM_PS, FLE_PS, FLTM_PS, FLT_PS, FCLASS_PS, FEXP_PS,
             FLOG_PS, FRCP_PS]

   # Exclusions: FDIV_PS, FSQRT_PS, FSIN_PS, FRSQ_PS
