
from core.mtg import *
from components.randinst.rv32i import LI, ADD

@register
class SetGAMOThreadPointer(Component, AsmCode):

   gamo_size = (TEST_CONF.MAX_AMO_OFF+1)*2 + 128

   def __init__(self, test_helper: TestHelper = None, gamotp: Xreg = None):
      if gamotp is None:
         self.gamotp = ABIreg('gamotp', ignore_check=True)
      else:
         self.gamotp = gamotp
      self.gamotp_mem = GMalloc('gamotp_mem', DRAM_REGION, self.gamo_size, align=64)
      self.rx0 = Xreg()
      self.offset = Zimm64(int(self.gamo_size/2))

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(LI(rd=self.gamotp, imm=self.gamotp_mem))
      content_list.append(LI(rd=self.rx0, imm=self.offset))
      content_list.append(ADD(rd=self.gamotp, rs1=self.gamotp, rs2=self.rx0))

   def render_if(self, model: 'Model') -> bool:
      model.gamotp_is_set = True
      return super().render_if(model)
