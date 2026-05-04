
from core.mtg import *
from components.randinst.rv32i import LI, ADD

@register
class SetLAMOThreadPointer(Component, AsmCode):

   lamo_size = (TEST_CONF.MAX_AMO_OFF+1)*2 + 128

   def __init__(self, test_helper: TestHelper = None, lamotp: Xreg = None):
      if lamotp is None:
         self.lamotp = ABIreg('lamotp', ignore_check=True)
      else:
         self.lamotp = lamotp
      self.lamotp_mem = GMalloc('lamotp_mem', DRAM_REGION, self.lamo_size, align=64)
      self.rx0 = Xreg()
      self.offset = Zimm64(int(self.lamo_size/2))

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(LI(rd=self.lamotp, imm=self.lamotp_mem))
      content_list.append(LI(rd=self.rx0, imm=self.offset))
      content_list.append(ADD(rd=self.lamotp, rs1=self.lamotp, rs2=self.rx0))

   def render_if(self, model: 'Model') -> bool:
      model.lamotp_is_set = True
      return super().render_if(model)
