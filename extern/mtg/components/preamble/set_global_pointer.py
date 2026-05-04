
from core.mtg import *
from components.randinst.rv32i import LI, ADD

@register
class SetGlobalPointer(Component, AsmCode):

   gp_size = (TEST_CONF.MAX_OFF+1)*2 + 128

   def __init__(self, test_helper: TestHelper = None, gp: Xreg = None):
      if gp is None:
         self.gp = ABIreg('gp', ignore_check=True)
      else:
         self.gp = gp
      self.gp_mem = StaticGMalloc('SetGlobalPointer_gp_mem', DRAM_REGION, self.gp_size, align=64)
      self.rx0 = Xreg()
      self.offset = Zimm64(int(self.gp_size/2))

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(LI(rd=self.gp, imm=self.gp_mem))
      content_list.append(LI(rd=self.rx0, imm=self.offset))
      content_list.append(ADD(rd=self.gp, rs1=self.gp, rs2=self.rx0))

   def render_if(self, model: 'Model') -> bool:
      model.gp_is_set = True
      return super().render_if(model)
