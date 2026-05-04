
from core.mtg import *

from components.randinst.cacheops import UNLOCK_SW, LOCK_SW
from components.tensor_wait import WAIT_CACHEOPS
from components.randinst.rv32i import *
from components.randinst.randinst import RANDINST

@param_block
class ClearLockSWTensorError(Component, AsmCode):
   def __init__(self, test_helper: TestHelper = None, num_sets: int = None, num_ways: int = None):
      self.rx0 = Xreg()
      self.zero = ABIreg('zero')
      self.num_inst = test_helper.get_rand_range(0, 12)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      insns = [RANDINST(test_helper = test_helper, num_inst = self.num_inst),
               WAIT_CACHEOPS(test_helper = test_helper),
               LI(rd=self.rx0, imm=0x20),
               "csrrc {zero}, tensor_error, {rx0}\n"
              ]
      content_list += insns

@register
class LockOrUnlockSW(Component, AsmCode):

   def __init__(self, test_helper: TestHelper = None, num_sets: int = None, num_ways: int = None):

      self.rx1 = Xreg()
      self.rx2 = Xreg()
      self.base_addr = Xreg()

      self.rb1 = Xreg()

      self.csr_val = test_helper.get_urand_num()
      self.do_unlock = test_helper.get_rand_range(0, 1)
      self.mem_offset = test_helper.get_rand_range(0, 8192)

      self.mem_lock = StaticGMalloc('mem_lock', DRAM_REGION, 8192, align=64)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:

      content_list.append(LI(test_helper = test_helper, rd = self.rb1, imm = self.mem_lock))
      content_list.append(LI(test_helper = test_helper, rd = self.rx1, imm = self.mem_offset))
      content_list.append(ADD(test_helper = test_helper, rd = self.rb1, rs1 = self.rb1, rs2 = self.rx1))

      # rx1 is the register used to write to CSR
      # rx2 is used for masking
      # rb1 contains a PA
      # All equally random, except PA (or just set for unlock)
      content_list.append(LI(test_helper = test_helper, rd = self.rx1, imm = self.csr_val))

      # Mask Set or PA
      if (self.do_unlock > 0):
         content_list.append(LI(test_helper = test_helper, rd = self.rx2, imm = 0xfffffffffffffc3f))
      else:
         content_list.append(LI(test_helper = test_helper, rd = self.rx2, imm = 0xffffff000000003f))

      content_list.append(AND(test_helper = test_helper, rd = self.rx1, rs1 = self.rx1, rs2 = self.rx2))

      if (self.do_unlock > 0):
         content_list.append(LI(test_helper = test_helper, rd = self.rx2, imm = 0x00000000000003c0))
      else:
         content_list.append(LI(test_helper = test_helper, rd = self.rx2, imm = 0x000000ffffffffc0))

      content_list.append(AND(test_helper = test_helper, rd = self.base_addr, rs1 = self.rb1, rs2 = self.rx2))
      content_list.append(OR(test_helper = test_helper, rd = self.rx1, rs1 = self.rx1, rs2 = self.base_addr))

      if (self.do_unlock > 0):
         content_list.append(UNLOCK_SW(test_helper = test_helper, rs1 = self.rx1))
      else:
         content_list.append(LOCK_SW(test_helper = test_helper, rs1 = self.rx1))
         content_list.append(ClearLockSWTensorError(test_helper = test_helper))



