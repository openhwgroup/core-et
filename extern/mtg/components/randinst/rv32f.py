from core.mtg import *
from components.encoding.enc import *
from components.randinst.randinst_base import RandinstBase

@register
class FLW(FD_OFF_RS1): pass

@register
class FSW(FS2_OFF_RS1): pass

@register
class FMADD_S(FD_FS1_FS2_FS3_RM): pass

@register
class FMSUB_S(FD_FS1_FS2_FS3_RM): pass

@register
class FNMADD_S(FD_FS1_FS2_FS3_RM): pass

@register
class FNMSUB_S(FD_FS1_FS2_FS3_RM): pass

@register
class FADD_S(FD_FS1_FS2_RM): pass

@register
class FSUB_S(FD_FS1_FS2_RM): pass

@register
class FMUL_S(FD_FS1_FS2_RM): pass

@register
class FDIV_S(FD_FS1_FS2_RM): pass

@register
class FSQRT_S(FD_FS1_RM): pass

@register
class FSGNJ_S(FD_FS1_FS2_PI): pass

@register
class FSGNJN_S(FD_FS1_FS2_PI): pass

@register
class FSGNJX_S(FD_FS1_FS2_PI): pass

@register
class FMIN_S(FD_FS1_FS2): pass

@register
class FMAX_S(FD_FS1_FS2): pass

@register
class FCVT_W_S(RD_FS1_RM): pass

@register
class FCVT_WU_S(RD_FS1_RM): pass

@register
class FMV_X_W(RD_FS1): pass

@register
class FEQ_S(RD_FS1_FS2): pass

@register
class FLT_S(RD_FS1_FS2): pass

@register
class FLE_S(RD_FS1_FS2): pass

@register
class FCLASS_S(RD_FS1): pass

@register
class FCVT_S_W(FD_RS1_RM): pass

@register
class FCVT_S_WU(FD_RS1_RM): pass

@register
class FMV_W_X(FD_RS1): pass

@register
class RV32F(RandinstBase):

   subcls = [FLW, FSW, FMADD_S, FMSUB_S, FNMADD_S, FADD_S, FSUB_S, FMUL_S,
             FSGNJ_S, FSGNJN_S, FSGNJX_S, FMIN_S, FMAX_S, FCVT_W_S, FCVT_WU_S,
             FMV_X_W, FEQ_S, FLT_S, FLE_S, FCLASS_S, FCVT_S_W, FCVT_S_WU, FMV_W_X]

   # Exclusions: FDIV_S, FSQRT_S
