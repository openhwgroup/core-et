
from core.mtg import *
from core.component import Component
from components.read_hartid import ReadHartID
from components.rand_sc.shire_cache_common import *
from components.randinst.rv32i import SW, SH, SB, LI, ADDI, ANDI, SRLI, SLLI, SLL
from components.randinst.rv64i import SLLW
from components.randinst.mask import MOV_M_X

@register
class RandSC(Component, AsmCode):

   def __init__(self, test_helper: TestHelper = None):

      self.size = test_helper.pick_one(["byte", "word"])
      self.glbl = test_helper.pick_one([True, False])
      self.num_ops = test_helper.get_rand_range(10, 50)
      self.write_weight = test_helper.get_rand_range(1, 10)
      self.read_weight = test_helper.get_rand_range(1, 10)
      self.amo_weight = test_helper.get_rand_range(1, 10)

      self.golden = Freg()
      self.fdest = Freg()

      if (self.size == "byte"):
         self.cache_line = Malloc(DRAM_REGION, 1, align=0)
      else:
         self.cache_line = Malloc(DRAM_REGION, 4, align=0)
      self.addr = Xreg()

      self.ps_reg = Xreg()
      self.rx1 = Xreg()
      self.data_shift = Xreg()
      self.check_pass = Xreg()
      self.zero = ABIreg('zero')
      self.m0 = Mreg(0)

      self.data = test_helper.get_rand_num()

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:

      content_list.append(LI(rd=self.addr, imm=self.cache_line))

      # Calculate the position mask and data shift values we will use to make sure 
      # the golden data and read value have the data in the correct location
      content_list.append(SRLI(rd=self.ps_reg, rs1=self.addr, imm=0x2))
      content_list.append(ANDI(rd=self.rx1, rs1=self.ps_reg, imm=0x7))
      content_list.append(ADDI(rd=self.ps_reg, rs1=self.zero, imm=0x1))
      content_list.append(SLLW(rd=self.ps_reg, rs1=self.ps_reg, rs2=self.rx1))

      content_list.append(ANDI(rd=self.data_shift, rs1=self.addr, imm=0x3))
      content_list.append(SLLI(rd=self.data_shift, rs1=self.data_shift, imm=0x3))

      # Initial write so we don't end up reading uninitialized memory
      content_list.append(SCWriteGolden(size = self.size, glbl = self.glbl, addr = self.addr, data = self.data, golden = self.golden, ps_mask = self.ps_reg, data_shift = self.data_shift, m0 = self.m0))

      for i in range(self.num_ops):
         if (self.size == "byte"):
            op = test_helper.pick_one_weight([SCWriteGolden, SCReadAndCheck], [self.write_weight, self.read_weight])
         else:
            op = test_helper.pick_one_weight([SCWriteGolden, SCReadAndCheck, SCAtomicAndCheck], [self.write_weight, self.read_weight, self.amo_weight])
         self.data = test_helper.get_rand_num()
         content_list.append(op(glbl = self.glbl, size = self.size, addr = self.addr, data = self.data, golden = self.golden, fdest = self.fdest, ps_mask = self.ps_reg, data_shift = self.data_shift, check_pass = self.check_pass, m0 = self.m0))

      # Final read so we check don't miss a self-check before writing the known value
      content_list.append(SCReadAndCheck(size = self.size, glbl = self.glbl, addr = self.addr, golden = self.golden, fdest = self.fdest, ps_mask = self.ps_reg, data_shift = self.data_shift, check_pass = self.check_pass, m0 = self.m0))


      content_list.append(LI(rd=self.rx1, imm=0x1))
      content_list.append("bne  {check_pass}, {rx1}, checker_pass")

      content_list.append(TEST_FAIL(test_helper = test_helper))

      content_list.append("checker_pass:")

      # Writing known value to the read FReg, so we don't fail signature generation
      content_list.append(MOV_M_X(md=self.m0, rs1=self.zero, imm8=0xff))
      content_list.append(FBCX_PS(fd = self.fdest, rs1 = self.ps_reg, m0=self.m0))


