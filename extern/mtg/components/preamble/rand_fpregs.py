
from core.mtg import *
from components.randinst.rv32i import LI
from components.randinst.rv64i import SD

@param_block
class RandFPreg(Component, AsmCode):

   def __init__(self, idx, test_helper: TestHelper = None):
      self.idx = idx
      self.rx0 = Xreg()
      self.sp = ABIreg('sp')

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(ADDI(rd=self.sp, rs1=self.sp, imm=-32))
      # 256 bits per VPU register (8 lanes, 32 bits per lane)
      for off in range(0, 32, 8):
         content_list.append(LI(rd=self.rx0, imm=test_helper.get_rand_num()))
         content_list.append(SD(rs2=self.rx0, off=off, rs1=self.sp))
      content_list.append(f"flq2 f{self.idx}, 0({{sp}})")
      fv260_a_wa(content_list, idx=self.idx)
      content_list.append(ADDI(rd=self.sp, rs1=self.sp, imm=32))

@register
class RandFPregs(Component, AsmCode):

   def __init__(self, test_helper: TestHelper = None):
      reserved_fpregs = [i[0] for i in FPABIreg.regs.values()]
      self.avail_regs = [i for i in range(FPModel.RF_SIZE) if i not in reserved_fpregs]
      self.regx0 = Xreg()

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      for i in self.avail_regs:
         content_list.append(RandFPreg(i, test_helper=test_helper))
