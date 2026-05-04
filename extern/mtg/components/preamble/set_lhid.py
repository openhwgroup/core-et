
from core.mtg import *
from components.randinst.rv32i import ADD, LI
from components.read_hartid import ReadHartID

@register
class SetLHID(Component, AsmCode):

   def __init__(self, test_helper: TestHelper = None, HARTS = list()):
      self.rx0 = Xreg()
      self.rx1 = Xreg()
      self.lhid = ABIreg('lhid', ignore_check=True)
      self.mem = StaticGMalloc('SetLHID_mem', LHID_MAP_REGION, TEST_CONF.NUM_HARTS*2, align=2)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(ReadHartID(test_helper=test_helper, rd=self.rx0))
      content_list.append("slli {rx0}, {rx0}, 1")
      content_list.append(LI(rd=self.rx1, imm=self.mem))
      content_list.append(ADD(rd=self.rx1, rs1=self.rx0, rs2=self.rx1))
      content_list.append("lh {lhid}, 0({rx1})")

      content_list.append(LI(rd=self.rx0, imm=Imm(-1)))
      content_list.append("bne {lhid}, {rx0}, is_active_hart")
      content_list.append("call mtg_glbl_test_pass")
      content_list.append("is_active_hart:")
      content_list.append("add {rx0}, {lhid}, {lhid}")
      content_list.append("add {rx1}, {lhid}, {lhid}")

   def render_if(self, model: 'Model') -> bool:
      model.lhid_is_set = True
      return super().render_if(model)
