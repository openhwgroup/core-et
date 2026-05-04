
from core.mtg import *

from components.evict_all_l1d import EvictAllL1D
from components.cacheops import SET_DCACHE_SHARED, SET_D1SPLIT, SET_SCPENABLE
from components.randinst.cacheops import UNLOCK_SW
from components.tensor_wait import WAIT_CACHEOPS
from components.randinst.rv import FENCE
from components.randinst.rv32i import *

@register
class UnlockSW(Component, AsmCode):

   def __init__(self, test_helper: TestHelper = None, num_sets: int = None, num_ways: int = None):

      self.regx1 = Xreg()
      self.regx2 = Xreg()
      self.regx3 = Xreg()
      self.regx4 = Xreg()

      if num_sets is None:
         self.num_sets = 16
      else:
         self.num_sets = num_sets

      if num_ways is None:
         self.num_ways = 4
      else:
         self.num_ways = num_ways


   @machine_code
   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:

      content_list.append(LI(test_helper = test_helper, rd = self.regx3, imm = 0x40))
      content_list.append(LI(test_helper = test_helper, rd = self.regx1, imm = self.num_ways))

      content_list.append("LOOP_WAY:")

      content_list.append("beq {regx1}, x0, UNLOCK_ALL_SW_DONE")
      content_list.append(ADDI(rd = self.regx1, rs1 = self.regx1, imm = -1))
      content_list.append(SLLI(rd = self.regx4, rs1 = self.regx1, imm = 55))

      content_list.append(LI(test_helper = test_helper, rd = self.regx2, imm = self.num_sets))

      content_list.append("LOOP_SET:")

      content_list.append("beq {regx2}, x0, LOOP_WAY")

      content_list.append(ADDI(rd = self.regx2, rs1 = self.regx2, imm = -1))

      content_list.append(ADD(rd = self.regx4, rs1 = self.regx4, rs2 = self.regx3))

      content_list.append(UNLOCK_SW(rs1 = self.regx4))

      content_list.append("j LOOP_SET")

      content_list.append("UNLOCK_ALL_SW_DONE:")
      content_list.append(WAIT_CACHEOPS(test_helper = test_helper))

      #content_list.append(f"add x0, x0, {{regx1}}")
      #content_list.append(f"add x0, x0, {{regx2}}")

      content_list.append("/*{regx1}, {regx2}*/")

