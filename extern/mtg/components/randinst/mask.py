import re
from core.mtg import *
from components.encoding.enc import *
from components.randinst.randinst_base import RandinstBase

@register
class MASKAND(MD_MS1_MS2): pass

@register
class MASKOR(MD_MS1_MS2): pass

@register
class MASKXOR(MD_MS1_MS2): pass

@register
class MASKNOT(MD_MS1): pass

@register
class MOVA_X_M(RD_M0_M1_M2_M3_M4_M5_M6_M7): pass

@register
class MOVA_M_X(M0_M1_M2_M3_M4_M5_M6_M7_RS1): pass

@register
class MOV_M_X(MD_RS1_IMM8): pass

@register
class MASKPOPC(RD_MS1): pass

@register
class MASKPOPCZ(RD_MS1): pass

@register
class Mask(RandinstBase):

   subcls = [MASKAND, MASKOR, MASKXOR, MASKNOT, MOV_M_X, MASKPOPC, MASKPOPCZ]
   # MOVA_M_X, MOVA_X_M implicitly uses all the mask registers
   # Exclusions: MOVA_M_X, MOVA_X_M
