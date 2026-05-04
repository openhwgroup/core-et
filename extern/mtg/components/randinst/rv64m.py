from core.mtg import *
from components.encoding.enc import *
from components.randinst.randinst_base import RandinstBase

@register
class MULW(RD_RS1_RS2): pass

@register
class DIVW(RD_RS1_RS2): pass

@register
class DIVUW(RD_RS1_RS2): pass

@register
class REMW(RD_RS1_RS2): pass

@register
class REMUW(RD_RS1_RS2): pass

@register
class RV64M(RandinstBase):

   subcls = [MULW, DIVW, DIVUW, REMW, REMUW]
