import re
from core.mtg import *
from components.encoding.enc import *
from components.randinst.randinst_base import RandinstBase

@register
class FBCI_PI(FD_IMM20_M0): pass

@register
class FADD_PI(FD_FS1_FS2_M0_PI): pass

@register
class FADDI_PI(FD_FS1_IMM10_M0): pass

@register
class FSUB_PI(FD_FS1_FS2_M0_PI): pass

@register
class FMUL_PI(FD_FS1_FS2_M0): pass

@register
class FMULH_PI(FD_FS1_FS2_M0): pass

@register
class FMULHU_PI(FD_FS1_FS2_M0): pass

@register
class FDIV_PI(FD_FS1_FS2_M0_PI): pass

@register
class FDIVU_PI(FD_FS1_FS2_M0_PI): pass

@register
class FREM_PI(FD_FS1_FS2_M0_PI): pass

@register
class FREMU_PI(FD_FS1_FS2_M0_PI): pass

@register
class FMAX_PI(FD_FS1_FS2_M0_PI): pass

@register
class FMIN_PI(FD_FS1_FS2_M0_PI): pass

@register
class FMAXU_PI(FD_FS1_FS2_M0_PI): pass

@register
class FMINU_PI(FD_FS1_FS2_M0_PI): pass

@register
class FAND_PI(FD_FS1_FS2_M0_PI): pass

@register
class FANDI_PI(FD_FS1_IMM10_M0): pass

@register
class FOR_PI(FD_FS1_FS2_M0_PI): pass

@register
class FNOT_PI(FD_FS1_M0_PI): pass

@register
class FXOR_PI(FD_FS1_FS2_M0_PI): pass

@register
class FSAT8_PI(FD_FS1_M0_PI): pass

@register
class FSATU8_PI(FD_FS1_M0_PI): pass

@register
class FSLL_PI(FD_FS1_FS2_M0_PI): pass

@register
class FSLLI_PI(FD_FS1_IMM5_M0): pass

@register
class FSRL_PI(FD_FS1_FS2_M0_PI): pass

@register
class FSRLI_PI(FD_FS1_IMM5_M0): pass

@register
class FSRA_PI(FD_FS1_FS2_M0_PI): pass

@register
class FSRAI_PI(FD_FS1_IMM5_M0): pass

@register
class FEQ_PI(FD_FS1_FS2_M0_PI): pass

@register
class FLE_PI(FD_FS1_FS2_M0_PI): pass

@register
class FLT_PI(FD_FS1_FS2_M0_PI): pass

@register
class FLTU_PI(FD_FS1_FS2_M0_PI): pass

@register
class FLTM_PI(MD_FS1_FS2_M0): pass

@register
class FSETM_PI(MD_FS1_M0): pass

@register
class FPACKREPH_PI(FD_FS1_M0_PI): pass

@register
class FPACKREPB_PI(FD_FS1_M0_PI): pass

@register
class PACKB(RD_RS1_RS2_M0): pass

@register
class PackedInteger(RandinstBase):

   subcls = [FBCI_PI, FADD_PI, FADDI_PI, FSUB_PI, FMUL_PI, FMULH_PI, FMULHU_PI,
             FMAX_PI, FMIN_PI, FMAXU_PI,
             FMINU_PI, FAND_PI, FANDI_PI, FOR_PI, FNOT_PI, FXOR_PI, FSAT8_PI,
             FSATU8_PI, FSLL_PI, FSLLI_PI, FSRL_PI, FSRLI_PI, FSRA_PI, FEQ_PI,
             FLE_PI, FLT_PI, FLTU_PI, FLTM_PI, FSETM_PI, FPACKREPH_PI, FPACKREPB_PI,
             PACKB]

   # Exclusions: FDIV_PI, FDIVU_PI, FREM_PI, FREMU_PI
