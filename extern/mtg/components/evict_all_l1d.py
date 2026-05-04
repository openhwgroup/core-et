
from core.mtg import *
from components.tensor_wait import WAIT_CACHEOPS
from components.randinst.rv import FENCE
from components.randinst.cacheops import EVICT_SW

@register
class EvictAllL1D(Component, AsmCode):

   def __init__(self, test_helper: TestHelper = None):

      self.rega = Xreg()

      WARL = test_helper.get_rand_num()
      self.csr_val = hex(0x040000000000000F | (0x73fffffffffc3f30 & WARL))

   @machine_code
   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:

      content_list += [
         # TensorMask(0), DestLvl(01), set(0), way(0), NumLines(15)
         "li   {rega}, {csr_val}",
         EVICT_SW(rs1=self.rega),
         # TensorMask(0), DestLvl(01), set(0), way(1), NumLines(15)
         "addi {rega}, {rega}, 0x40",
         EVICT_SW(rs1=self.rega),
         # TensorMask(0), DestLvl(01), set(0), way(2), NumLines(15)
         "addi {rega}, {rega}, 0x40",
         EVICT_SW(rs1=self.rega),
         # TensorMask(0), DestLvl(01), set(0), way(3), NumLines(15)
         "addi {rega}, {rega}, 0x40",
         EVICT_SW(rs1=self.rega)
      ]

      content_list.append(WAIT_CACHEOPS(test_helper))
      content_list.append(FENCE(test_helper))
