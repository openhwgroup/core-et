from core.mtg import *
from components.encoding.enc import *
from components.randinst.randinst_base import RandinstBase

@register
class MUL(RD_RS1_RS2): pass

@register
class MULH(RD_RS1_RS2): pass

@register
class MULHSU(RD_RS1_RS2): pass

@register
class MULHU(RD_RS1_RS2): pass

@register
class DIV(RD_RS1_RS2): pass

@register
class DIVU(RD_RS1_RS2): pass

@register
class REM(RD_RS1_RS2): pass

@register
class REMU(RD_RS1_RS2): pass

@register
class RV32M(RandinstBase):

   subcls = [MUL, MULH, MULHSU, MULHU, DIV, DIVU, REM, REMU]
