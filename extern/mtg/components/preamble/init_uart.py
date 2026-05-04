
from core.mtg import *
from components.randinst.rv32i import LI

@register
class InitUART(Component, AsmCode):

   def __init__(self, test_helper: TestHelper = None):
      self.ra = ABIreg('ra')
      self.rx1 = Xreg(10)

   @only_lhid(0)
   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(LI(rd=self.rx1, imm=0x0012007000))
      content_list.append(JumpToCFunct(test_helper=test_helper, funct_name="SERIAL_init"))
