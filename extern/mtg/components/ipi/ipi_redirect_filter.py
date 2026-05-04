
from core.mtg import *
from components.randinst.rv64i import SD
from components.randinst.rv32i import LI

_shire_harts_mask = 0x0
_minion_mask = TEST_CONF.MINION_MASK
_idx = 0
while _minion_mask != 0:
   if _minion_mask & 0x1:
      _shire_harts_mask |= (TEST_CONF.THREAD_MASK << _idx)
   _idx += 2
   _minion_mask >>= 1

@register
class IPIRedirectFilter(Component, AsmCode):
   def __init__(self, test_helper: TestHelper = None):
      self.local_ipi_redirect_filter = StaticGMalloc('IPIRedirectFilter_redirect_filter',  ESR_LOCAL_IPI_REDIRECT_FILTER, 8, align=8)
      self.rx0 = Xreg()
      self.rx1 = Xreg()

   @only_mod_lhid(TEST_CONF.NUM_ACTIVE_HARTS_PER_SHIRE, 0)
   @machine_code
   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list += [ LI(rd=self.rx0, imm=self.local_ipi_redirect_filter),
                        LI(rd=self.rx1, imm=_shire_harts_mask),
                        SD(rs2=self.rx1, off=0, rs1=self.rx0)]

if TEST_CONF.MINION_AGNOSTIC == 1 or TEST_CONF.NEIGH_AGNOSTIC == 1:
   TEST_CONF.EXCL_COMPS.add(IPIRedirectFilter)
