from core.mtg import *
from components.encoding.enc import *
from components.randinst.randinst_base import RandinstBase

@register
class FCVT_L_S(RD_FS1_RM): pass

@register
class FCVT_LU_S(RD_FS1_RM): pass

@register
class FCVT_S_L(FD_RS1_RM): pass

@register
class FCVT_S_LU(FD_RS1_RM): pass

@register
class RV64F(RandinstBase):

   subcls = []

   # Exclusions: FCVT_L_S, FCVT_LU_S, FCVT_S_L, FCVT_S_LU
