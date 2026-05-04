
from core.mtg import *
from components.randinst.rv32i import LI
from components.randinst.rv64i import LD, SD

@register
class SetCoopMode(Component, AsmCode):

   def __init__(self, test_helper: TestHelper = None):
      self.esr_addr = StaticGMalloc('SetCoopMode_esr_addr', ESR_LOCAL_SHIRE_COOP_MODE, 8, align=8)
      self.write_val = test_helper.get_rand_num() | 0x1
      self.rega = Xreg()
      self.regb = Xreg()

   @only_mod_lhid(TEST_CONF.NUM_ACTIVE_HARTS_PER_SHIRE, 0)
   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(LI(rd=self.regb, imm=self.write_val))
      content_list.append(LI(rd=self.rega, imm=self.esr_addr))
      content_list.append(SD(rs2=self.regb, off=Imm12(0), rs1=self.rega))

      # Wait for the store to be completed
      content_list.append("fence iorw, iorw")

if TEST_CONF.MINION_AGNOSTIC:
   TEST_CONF.EXCL_COMPS.add(SetCoopMode)
