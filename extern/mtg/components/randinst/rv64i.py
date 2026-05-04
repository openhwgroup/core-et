from core.mtg import *
from components.encoding.enc import *
from components.randinst.randinst_base import RandinstBase

@register
class LWU(RD_OFF_RS1): pass

@register
class LD(RD_OFF_RS1): pass

@register
class SD(RS2_OFF_RS1): pass

@register
class SLLIW(RD_RS1_IMM5): pass

@register
class SRLIW(RD_RS1_IMM5): pass

@register
class SRAIW(RD_RS1_IMM5): pass

@register
class ADDIW(RD_RS1_IMM12): pass

@register
class ADDW(RD_RS1_RS2): pass

@register
class SUBW(RD_RS1_RS2): pass

@register
class SLLW(RD_RS1_RS2): pass

@register
class SRLW(RD_RS1_RS2): pass

@register
class SRAW(RD_RS1_RS2): pass

@register
class RV64I(RandinstBase):

   subcls = [LWU, LD, SLLIW, SRLIW, SRAIW, ADDIW, ADDW, SUBW, SLLW, SRLW, SRAW]
