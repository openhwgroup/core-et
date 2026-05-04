
from core.mtg import *
from components.randinst.rv32i import LI, ADD

@register
class SetThreadPointer(Component, AsmCode):

   tp_size = (TEST_CONF.MAX_OFF+1)*2 + 128

   def __init__(self, test_helper: TestHelper = None, tp: Xreg = None):
      if tp is None:
         self.tp = ABIreg('tp', ignore_check=True)
      else:
         self.tp = tp
      self.tp_mem = GMalloc('tp_mem', DRAM_REGION, self.tp_size, align=64)
      self.rx0 = Xreg()
      self.offset = Zimm64(int(self.tp_size/2))

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(LI(rd=self.tp, imm=self.tp_mem))
      content_list.append(LI(rd=self.rx0, imm=self.offset))
      content_list.append(ADD(rd=self.tp, rs1=self.tp, rs2=self.rx0))

   def render_if(self, model: 'Model') -> bool:
      model.tp_is_set = True
      return super().render_if(model)
