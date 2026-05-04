
from core.mtg import *

@register
class InitMstatus(Component, AsmCode):
   def __init__(self, test_helper: TestHelper = None):
      self.rx0 = Xreg()
      self.zero = ABIreg('zero')

   @machine_code
   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append("li {rx0}, 0x8000000a000078b0")
      content_list.append("csrrw {zero}, mstatus, {rx0}")
