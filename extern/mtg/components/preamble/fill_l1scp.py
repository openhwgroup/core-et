
from core.mtg import *
from components.randinst.rv32i import LI, AND, OR
from components.tensor.tensor import FreeL1SCP, FreeTl0SCP, TENSOR_LOAD
from components.tensor.tensor_base import TensorBase
from components.tensor.tensor_load import TensorLoad

@register
class FillL1SCP(Component, AsmCode):

   def __init__(self, test_helper: TestHelper = None):
      self.x31 = Xreg(31)

      x31_val = test_helper.pick_one([0x40, 0x41])

      encs = [0xf, 0x20000000000000F ,0x40000000000000f]
      self.mem = StaticMalloc(DRAM_REGION, 16*64, align=64)
      self.tl_enc = [TensorLoad(x31=self.x31, x31_val=x31_val, enc=x, mem=self.mem, test_helper=test_helper) for x in encs]

      self.free_l1scp = FreeL1SCP(test_helper = test_helper)
      self.free_l1scp.skip_assert = True

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(self.free_l1scp)
      for i in self.tl_enc:
         content_list.append(i)

      content_list.append(FreeTl0SCP(test_helper = test_helper, parent_render_if = lambda x: True))

   def render(self, model: Model) -> Snippet:
      snip = super().render(model)
      for i in self.tl_enc:
         assert i.tl_enc.memory_size <= self.mem.size
      return snip
