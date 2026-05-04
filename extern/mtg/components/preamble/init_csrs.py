
from core.mtg import *
from components.randinst.rv32i import LI, FENCE, SRLI, AND
from components.randinst.rv64i import LD, SD

@register
class InitCSRs(Component, AsmCode):

   def __init__(self, test_helper: TestHelper = None):
      self.zero = ABIreg("zero")
      self.csrs = ["sscratch", "sepc", "scause", "stval", "misa", "mscratch", "mepc", "mcause", "mtval"]

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      for i in self.csrs:
         content_list.append(f"csrrw {{zero}}, {i}, {{zero}}")
