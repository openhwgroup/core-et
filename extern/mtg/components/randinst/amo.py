import re
from core.mtg import *
from components.encoding.enc import *
from components.randinst.randinst_base import RandinstBase

@register
class FLWL_PS(LAMO_FD_NOOFF_RS1_M0): pass

@register
class FSWL_PS(LAMO_FS3_NOOFF_RS1_M0): pass

@register
class FGBL_PS(LAMO_FD_FS1_RS2_M0): pass

@register
class FGHL_PS(LAMO_FD_FPOFF_RS2_M0): pass

@register
class FGWL_PS(LAMO_FD_FPOFF_RS2_M0): pass

@register
class FSCBL_PS(LAMO_FS3_FPOFF_RS2_M0): pass

@register
class FSCHL_PS(LAMO_FS3_FPOFF_RS2_M0): pass

@register
class FSCWL_PS(LAMO_FS3_FPOFF_RS2_M0): pass

@register
class SBL(LAMO_RS2_NOOFF_RS1): pass

@register
class SHL(LAMO_RS2_NOOFF_RS1): pass

@register
class AMOSWAPL_W(LAMO_RD_RS2_NOFF_RS1): pass

@register
class AMOANDL_W(LAMO_RD_RS2_NOFF_RS1): pass

@register
class AMOORL_W(LAMO_RD_RS2_NOFF_RS1): pass

@register
class AMOXORL_W(LAMO_RD_RS2_NOFF_RS1): pass

@register
class AMOADDL_W(LAMO_RD_RS2_NOFF_RS1): pass

@register
class AMOMINL_W(LAMO_RD_RS2_NOFF_RS1): pass

@register
class AMOMAXL_W(LAMO_RD_RS2_NOFF_RS1): pass

@register
class AMOMINUL_W(LAMO_RD_RS2_NOFF_RS1): pass

@register
class AMOMAXUL_W(LAMO_RD_RS2_NOFF_RS1): pass

@register
class AMOCMPSWAPL_W(LAMO_RD_RS2_NOFF_RS1_X31):
   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      if TEST_CONF.RTLMIN_WORKAROUNDS['rtlmin-6469'] == True:
         content_list.append("addi x31, x31, 0")
      super()._run(content_list)

@register
class AMOSWAPL_D(LAMO_RD_RS2_NOFF_RS1): pass

@register
class AMOANDL_D(LAMO_RD_RS2_NOFF_RS1): pass

@register
class AMOORL_D(LAMO_RD_RS2_NOFF_RS1): pass

@register
class AMOXORL_D(LAMO_RD_RS2_NOFF_RS1): pass

@register
class AMOADDL_D(LAMO_RD_RS2_NOFF_RS1): pass

@register
class AMOMINL_D(LAMO_RD_RS2_NOFF_RS1): pass

@register
class AMOMAXL_D(LAMO_RD_RS2_NOFF_RS1): pass

@register
class AMOMINUL_D(LAMO_RD_RS2_NOFF_RS1): pass

@register
class AMOMAXUL_D(LAMO_RD_RS2_NOFF_RS1): pass

@register
class AMOCMPSWAPL_D(LAMO_RD_RS2_NOFF_RS1_X31):
   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      if TEST_CONF.RTLMIN_WORKAROUNDS['rtlmin-6469'] == True:
         content_list.append("addi x31, x31, 0")
      super()._run(content_list)

@register
class FAMOSWAPL_PI(LAMO_FD_FPOFF_RS2_M0): pass

@register
class FAMOANDL_PI(LAMO_FD_FPOFF_RS2_M0): pass

@register
class FAMOORL_PI(LAMO_FD_FPOFF_RS2_M0): pass

@register
class FAMOXORL_PI(LAMO_FD_FPOFF_RS2_M0): pass

@register
class FAMOADDL_PI(LAMO_FD_FPOFF_RS2_M0): pass

@register
class FAMOMINL_PI(LAMO_FD_FPOFF_RS2_M0): pass

@register
class FAMOMAXL_PI(LAMO_FD_FPOFF_RS2_M0): pass

@register
class FAMOMINUL_PI(LAMO_FD_FPOFF_RS2_M0): pass

@register
class FAMOMAXUL_PI(LAMO_FD_FPOFF_RS2_M0): pass

@register
class FAMOMINL_PS(LAMO_FD_FPOFF_RS2_M0): pass

@register
class FAMOMAXL_PS(LAMO_FD_FPOFF_RS2_M0): pass

@register
class FLWG_PS(GAMO_FD_NOOFF_RS1_M0): pass

@register
class FSWG_PS(GAMO_FS3_NOOFF_RS1_M0): pass

@register
class FGBG_PS(GAMO_FD_FS1_RS2_M0): pass

@register
class FGHG_PS(GAMO_FD_FPOFF_RS2_M0): pass

@register
class FGWG_PS(GAMO_FD_FPOFF_RS2_M0): pass

@register
class FSCBG_PS(GAMO_FS3_FPOFF_RS2_M0): pass

@register
class FSCHG_PS(GAMO_FS3_FPOFF_RS2_M0): pass

@register
class FSCWG_PS(GAMO_FS3_FPOFF_RS2_M0): pass

@register
class SBG(GAMO_RS2_NOOFF_RS1): pass

@register
class SHG(GAMO_RS2_NOOFF_RS1): pass

@register
class AMOSWAPG_W(GAMO_RD_RS2_NOFF_RS1): pass

@register
class AMOANDG_W(GAMO_RD_RS2_NOFF_RS1): pass

@register
class AMOORG_W(GAMO_RD_RS2_NOFF_RS1): pass

@register
class AMOXORG_W(GAMO_RD_RS2_NOFF_RS1): pass

@register
class AMOADDG_W(GAMO_RD_RS2_NOFF_RS1): pass

@register
class AMOMING_W(GAMO_RD_RS2_NOFF_RS1): pass

@register
class AMOMAXG_W(GAMO_RD_RS2_NOFF_RS1): pass

@register
class AMOMINUG_W(GAMO_RD_RS2_NOFF_RS1): pass

@register
class AMOMAXUG_W(GAMO_RD_RS2_NOFF_RS1): pass

@register
class AMOCMPSWAPG_W(GAMO_RD_RS2_NOFF_RS1_X31):
   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      if TEST_CONF.RTLMIN_WORKAROUNDS['rtlmin-6469'] == True:
         content_list.append("addi x31, x31, 0")
      super()._run(content_list)

@register
class AMOSWAPG_D(GAMO_RD_RS2_NOFF_RS1): pass

@register
class AMOANDG_D(GAMO_RD_RS2_NOFF_RS1): pass

@register
class AMOORG_D(GAMO_RD_RS2_NOFF_RS1): pass

@register
class AMOXORG_D(GAMO_RD_RS2_NOFF_RS1): pass

@register
class AMOADDG_D(GAMO_RD_RS2_NOFF_RS1): pass

@register
class AMOMING_D(GAMO_RD_RS2_NOFF_RS1): pass

@register
class AMOMAXG_D(GAMO_RD_RS2_NOFF_RS1): pass

@register
class AMOMINUG_D(GAMO_RD_RS2_NOFF_RS1): pass

@register
class AMOMAXUG_D(GAMO_RD_RS2_NOFF_RS1): pass

@register
class AMOCMPSWAPG_D(GAMO_RD_RS2_NOFF_RS1_X31):
   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      if TEST_CONF.RTLMIN_WORKAROUNDS['rtlmin-6469'] == True:
         content_list.append("addi x31, x31, 0")
      super()._run(content_list)

@register
class FAMOSWAPG_PI(GAMO_FD_FPOFF_RS2_M0): pass

@register
class FAMOANDG_PI(GAMO_FD_FPOFF_RS2_M0): pass

@register
class FAMOORG_PI(GAMO_FD_FPOFF_RS2_M0): pass

@register
class FAMOXORG_PI(GAMO_FD_FPOFF_RS2_M0): pass

@register
class FAMOADDG_PI(GAMO_FD_FPOFF_RS2_M0): pass

@register
class FAMOMING_PI(GAMO_FD_FPOFF_RS2_M0): pass

@register
class FAMOMAXG_PI(GAMO_FD_FPOFF_RS2_M0): pass

@register
class FAMOMINUG_PI(GAMO_FD_FPOFF_RS2_M0): pass

@register
class FAMOMAXUG_PI(GAMO_FD_FPOFF_RS2_M0): pass

@register
class FAMOMING_PS(GAMO_FD_FPOFF_RS2_M0): pass

@register
class FAMOMAXG_PS(GAMO_FD_FPOFF_RS2_M0): pass

@register
class Atomic(RandinstBase):

   subcls = [FLWL_PS, FGBL_PS, FGHL_PS, FGWL_PS, FSCBL_PS, FSCHL_PS, FSCWL_PS, SBL, SHL, AMOSWAPL_W,
             AMOANDL_W, AMOORL_W, AMOXORL_W, AMOADDL_W, AMOMINL_W, AMOMAXL_W, AMOMINUL_W, AMOMAXUL_W,
             AMOSWAPL_D, AMOANDL_D, AMOORL_D, AMOXORL_D, AMOADDL_D, AMOMINL_D, AMOMAXL_D, AMOMINUL_D, AMOMAXUL_D,
             FAMOSWAPL_PI, FAMOANDL_PI, FAMOORL_PI, FAMOXORL_PI, FAMOADDL_PI, FAMOMINL_PI, FAMOMAXL_PI,
             FAMOMINUL_PI, FAMOMAXUL_PI, FAMOMINL_PS, FAMOMAXL_PS, FLWG_PS, FGBG_PS, FGHG_PS, FGWG_PS,
             FSCBG_PS, FSCHG_PS, FSCWG_PS, SBG, SHG, AMOSWAPG_W, AMOANDG_W, AMOORG_W, AMOXORG_W, AMOADDG_W, AMOMING_W,
             AMOMAXG_W, AMOMINUG_W, AMOMAXUG_W, AMOSWAPG_D, AMOANDG_D, AMOORG_D, AMOXORG_D, AMOADDG_D,
             AMOMING_D, AMOMAXG_D, AMOMINUG_D, AMOMAXUG_D, FAMOSWAPG_PI, FAMOANDG_PI, FAMOORG_PI,
             FAMOXORG_PI, FAMOADDG_PI, FAMOMING_PI, FAMOMAXG_PI, FAMOMINUG_PI, FAMOMAXUG_PI, FAMOMING_PS, FAMOMAXG_PS,
             AMOCMPSWAPL_W, AMOCMPSWAPL_D, AMOCMPSWAPG_W, AMOCMPSWAPG_D]

   # Exclusions: FSWG_PS, FSWL_PS

@register
class LocalAtomic(RandinstBase):

   subcls = [FLWL_PS, FGBL_PS, FGHL_PS, FGWL_PS, FSCBL_PS, FSCHL_PS, FSCWL_PS, SBL, SHL, AMOSWAPL_W,
             AMOANDL_W, AMOORL_W, AMOXORL_W, AMOADDL_W, AMOMINL_W, AMOMAXL_W, AMOMINUL_W, AMOMAXUL_W,
             AMOSWAPL_D, AMOANDL_D, AMOORL_D, AMOXORL_D, AMOADDL_D, AMOMINL_D, AMOMAXL_D, AMOMINUL_D, AMOMAXUL_D,
             FAMOSWAPL_PI, FAMOANDL_PI, FAMOORL_PI, FAMOXORL_PI, FAMOADDL_PI, FAMOMINL_PI, FAMOMAXL_PI,
             FAMOMINUL_PI, FAMOMAXUL_PI, FAMOMINL_PS, FAMOMAXL_PS, AMOCMPSWAPL_W, AMOCMPSWAPL_D]

   # Exclusions: FSWL_PS

@register
class GlobalAtomic(RandinstBase):


   subcls = [FLWG_PS, FGBG_PS, FGHG_PS, FGWG_PS, FSCBG_PS, FSCHG_PS, FSCWG_PS, SBG, SHG, AMOSWAPG_W,
             AMOANDG_W, AMOORG_W, AMOXORG_W, AMOADDG_W, AMOMING_W, AMOMAXG_W, AMOMINUG_W, AMOMAXUG_W,
             AMOSWAPG_D, AMOANDG_D, AMOORG_D, AMOXORG_D, AMOADDG_D, AMOMING_D, AMOMAXG_D, AMOMINUG_D,
             AMOMAXUG_D, FAMOSWAPG_PI, FAMOANDG_PI, FAMOORG_PI, FAMOXORG_PI, FAMOADDG_PI, FAMOMING_PI,
             FAMOMAXG_PI, FAMOMINUG_PI, FAMOMAXUG_PI, FAMOMING_PS, FAMOMAXG_PS, AMOCMPSWAPG_W, AMOCMPSWAPG_D]

   # Exclusions: FSWG_PS

def uses_fp_reg(l : list):
   return any(cls_type == Freg for name, cls_type in l.items())


@register
class FPLocalAtomic(RandinstBase):
   subcls = [x for x in LocalAtomic.subcls if uses_fp_reg(x.__annotations__)]


@register
class FPGlobalAtomic(RandinstBase):
   subcls = [x for x in GlobalAtomic.subcls if uses_fp_reg(x.__annotations__)]


if TEST_CONF.POSTSI_L2_ONLY == True:
   TEST_CONF.EXCL_COMPS.add(FLWG_PS)
   TEST_CONF.EXCL_COMPS.add(FSWG_PS)
   TEST_CONF.EXCL_COMPS.add(FGBG_PS)
   TEST_CONF.EXCL_COMPS.add(FGHG_PS)
   TEST_CONF.EXCL_COMPS.add(FGWG_PS)
   TEST_CONF.EXCL_COMPS.add(FSCBG_PS)
   TEST_CONF.EXCL_COMPS.add(FSCHG_PS)
   TEST_CONF.EXCL_COMPS.add(FSCWG_PS)
   TEST_CONF.EXCL_COMPS.add(SBG)
   TEST_CONF.EXCL_COMPS.add(SHG)
   TEST_CONF.EXCL_COMPS.add(AMOSWAPG_W)
   TEST_CONF.EXCL_COMPS.add(AMOANDG_W)
   TEST_CONF.EXCL_COMPS.add(AMOORG_W)
   TEST_CONF.EXCL_COMPS.add(AMOXORG_W)
   TEST_CONF.EXCL_COMPS.add(AMOADDG_W)
   TEST_CONF.EXCL_COMPS.add(AMOMING_W)
   TEST_CONF.EXCL_COMPS.add(AMOMAXG_W)
   TEST_CONF.EXCL_COMPS.add(AMOMINUG_W)
   TEST_CONF.EXCL_COMPS.add(AMOMAXUG_W)
   TEST_CONF.EXCL_COMPS.add(AMOCMPSWAPG_W)
   TEST_CONF.EXCL_COMPS.add(AMOSWAPG_D)
   TEST_CONF.EXCL_COMPS.add(AMOANDG_D)
   TEST_CONF.EXCL_COMPS.add(AMOORG_D)
   TEST_CONF.EXCL_COMPS.add(AMOXORG_D)
   TEST_CONF.EXCL_COMPS.add(AMOADDG_D)
   TEST_CONF.EXCL_COMPS.add(AMOMING_D)
   TEST_CONF.EXCL_COMPS.add(AMOMAXG_D)
   TEST_CONF.EXCL_COMPS.add(AMOMINUG_D)
   TEST_CONF.EXCL_COMPS.add(AMOMAXUG_D)
   TEST_CONF.EXCL_COMPS.add(AMOCMPSWAPG_D)
   TEST_CONF.EXCL_COMPS.add(FAMOSWAPG_PI)
   TEST_CONF.EXCL_COMPS.add(FAMOANDG_PI)
   TEST_CONF.EXCL_COMPS.add(FAMOORG_PI)
   TEST_CONF.EXCL_COMPS.add(FAMOXORG_PI)
   TEST_CONF.EXCL_COMPS.add(FAMOADDG_PI)
   TEST_CONF.EXCL_COMPS.add(FAMOMING_PI)
   TEST_CONF.EXCL_COMPS.add(FAMOMAXG_PI)
   TEST_CONF.EXCL_COMPS.add(FAMOMINUG_PI)
   TEST_CONF.EXCL_COMPS.add(FAMOMAXUG_PI)
   TEST_CONF.EXCL_COMPS.add(FAMOMING_PS)
   TEST_CONF.EXCL_COMPS.add(FAMOMAXG_PS)
