
from core.mtg import *

from components.et_csr import CACHE_INVALIDATE
from components.randinst.randinst import *
from components.randinst.rv32i import *


@register
class CacheInvalidate(Component, AsmCode):

   def __init__(self, test_helper: TestHelper = None, num_sets: int = None, num_ways: int = None):

      ## The following variables are for use in a csrrw instruction. For now FLUSH_ICACHE
      ## uses only csrrwi. Leaving these commented until we change the implementation
      #self.ins_sel = test_helper.get_rand_range(0,3)
      #self.csr_reg = Xreg()

      self.warl = (test_helper.get_urand_num() << 2)
      self.cinvalidate_event = test_helper.get_rand_range(0,3)
      self.csr_enc = self.warl | self.cinvalidate_event
      self.csr_imm = self.csr_enc & 0x3

      self.return0 = Xreg()


   @machine_code
   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:

      content_list.append(CACHE_INVALIDATE(rd = self.return0, zimm5 = self.csr_imm))

      content_list.append("beq {return0}, x0, pass")

      content_list.append(TEST_FAIL(test_helper = test_helper))

      content_list.append("pass:")

