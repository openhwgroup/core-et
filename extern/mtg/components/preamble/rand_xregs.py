
from core.mtg import *

@register
class RandXregs(Component, AsmCode):

   def __init__(self, test_helper: TestHelper = None):
      reserved_xregs = [i[0] for i in ABIreg.regs.values()]
      self.avail_regs = [i for i in range(IntModel.RF_SIZE) if i not in reserved_xregs]

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      for i in self.avail_regs:
         rand_num = test_helper.get_rand_num()
         content_list.append(f"li x{i}, {rand_num}")
