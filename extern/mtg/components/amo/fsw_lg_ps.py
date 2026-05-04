
from core.mtg import *
from components.randinst.amo import FSWL_PS, FSWG_PS
from components.randinst.mask import MOV_M_X
from components.randinst.randinst import RANDINST_RANGE

@register
class FswLGPs(Component, AsmCode):

   def __init__(self, test_helper: TestHelper = None):
      self.m0 = Mreg(0)
      self.zero = ABIreg('zero')
      if TEST_CONF.RTLMIN_WORKAROUNDS['rtlmin-2840'] == True:
         mask = test_helper.pick_one([0, 0xff])
      else:
         mask = test_helper.get_rand_kbits(8)

      self.mov_m_x = MOV_M_X(md=self.m0, rs1=self.zero, imm8=mask)
      self.op = test_helper.pick_one([FSWL_PS, FSWG_PS])(m0=self.m0)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(self.mov_m_x)
      content_list.append(RANDINST_RANGE(0, 12, test_helper=test_helper))
      content_list.append(self.op)
