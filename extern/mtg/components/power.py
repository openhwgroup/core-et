
from core.mtg import *
from components.tensor.tensor_sequences import TensorSequence
from components.randinst.rv32i import LI
from components.randinst.randinst import VPUINST

@register
class PowerSequence(Component, AsmCode):

   LOOP_SIZE = 1000
   NUM_COMPS = 256

   def __init__(self, test_helper: TestHelper = None):
      self.zero = ABIreg('zero')
      self.loop_cnt = Xreg()

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(LI(rd=self.loop_cnt, imm=self.LOOP_SIZE))
      content_list.append("power_loop:")

      content_list.append(VPUINST(test_helper=test_helper, num_inst=self.NUM_COMPS))

      content_list.append(ADDI(rd=self.loop_cnt, rs1=self.loop_cnt, imm=-1))
      content_list.append("bne {zero}, {loop_cnt}, power_loop")
