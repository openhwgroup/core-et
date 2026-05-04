
from core.component import Component, param_block
from core.snippet.asm import AsmCode
from core.models.model import *

class MemHARTOffset:
   def __init__(self, alloc: Malloc):
      self.alloc = alloc

   def __repr__(self) -> str:
      return str(self.alloc.hart_offset)

   def __str__(self) -> str:
      return str(self.alloc.hart_offset)

@param_block
class MTG_LOAD_ADDR(Component, AsmCode):

   def __init__(self, rd: Xreg = None, rx1: Xreg = None, test_helper: TestHelper = None, mem: Malloc = None):
      assert mem is not None
      self.mem = mem

      if rd is None:
         self.rd = Xreg()
      else:
         self.rd = rd

      if rx1 is None:
         self.rx1 = Xreg()
      else:
         self.rx1 = rx1

      if self.mem.slices > 1:
         self.off = MemHARTOffset(self.mem)
         # TODO: properly set an ABI register for the Virtual HART ID
         self.lhid = ABIreg('lhid')

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append("li {rd}, {mem}")
      if self.mem.slices > 1:
         content_list.append("li {rx1}, {off}")
         content_list.append("mul {rx1}, {rx1}, {lhid}")
         content_list.append("add {rd}, {rd}, {rx1}")
