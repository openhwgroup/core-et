
from core.mtg import *

from components.et_csr import SET_EXCL_MODE, UNSET_EXCL_MODE
from components.randinst.randinst import *
from components.randinst.rv32i import *

@register
class ExclusiveMode(Component, AsmCode):

   def __init__(self, test_helper: TestHelper = None):

      self.previous = Xreg()
      self.reg_1 = Xreg()

      self.nop1 = test_helper.get_rand_range(0, 100)
      self.nop2 = test_helper.get_rand_range(0, 100)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:

      content_list.append(LI(test_helper = test_helper, rd = self.reg_1, imm = 1))

      content_list.append(RANDINST(test_helper = test_helper, num_inst = test_helper.get_rand_range(0, 6)))

      content_list.append(SET_EXCL_MODE(test_helper = test_helper, rd = self.previous))

      content_list.append(RANDINST(test_helper = test_helper))

      content_list.append(f"bne {{previous}}, x0, fail")

      if (self.nop1 < 10):
         content_list.append(SET_EXCL_MODE(test_helper = test_helper, rd = self.previous))
         content_list.append(f"bne {{previous}}, {{reg_1}}, fail")

      content_list.append(UNSET_EXCL_MODE(test_helper = test_helper, rd = self.previous))
      content_list.append(RANDINST(test_helper = test_helper, num_inst = test_helper.get_rand_range(0, 6)))

      content_list.append(f"bne {{previous}}, {{reg_1}}, fail")

      if (self.nop2 < 10):
         content_list.append(UNSET_EXCL_MODE(test_helper = test_helper, rd = self.previous))
         content_list.append(f"bne {{previous}}, x0, fail")

      content_list.append(f"j pass")

      content_list.append("fail:")
      content_list.append(TEST_FAIL(test_helper = test_helper))

      content_list.append("pass:")


TEST_CONF.EXCL_COMPS.add(ExclusiveMode)
