
from core.mtg import *
from components.randinst.rv32i import LI, ADD

@register
class SetStackPointer(Component, AsmCode):

   sp_size = TEST_CONF.HART_STACK_SIZE

   def __init__(self, test_helper: TestHelper = None, sp: Xreg = None):
      if sp is None:
         self.sp = ABIreg('sp')
      else:
         self.sp = sp
      self.sp_mem = GMalloc('sp_mem', DRAM_REGION, self.sp_size, align=64)
      self.rx0 = Xreg()

   def render_if(self, model: Model) -> bool:
      return TEST_CONF.POSTSI_L2_ONLY == True or TEST_CONF.agnostic_mode() == True

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(LI(rd=self.sp, imm=self.sp_mem))
      content_list.append(LI(rd=self.rx0, imm=self.sp_size-64))
      content_list.append(ADD(rd=self.sp, rs1=self.sp, rs2=self.rx0))
