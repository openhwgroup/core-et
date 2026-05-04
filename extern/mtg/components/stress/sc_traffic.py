
from core.mtg import *
from components.randinst.rv32i import SW, SH, SB, LI, ADDI
from components.randinst.rv64i import SD

@register
class SCTraffic(Component, AsmCode):

   def __init__(self, test_helper: TestHelper = None):
      self.dcache_nsets = 16
      self.dcache_nways = 4
      self.cache_line_size = 64 # Bytes

      mem_size = self.dcache_nsets * self.dcache_nways * self.cache_line_size * 2
      self.mem = GMalloc('SCLdSt_mem', DRAM_REGION, mem_size, align=64)

      self.rx0 = Xreg()
      self.rx1 = Xreg()
      self.cntr = Xreg()
      self.zero = ABIreg('zero')

      self.configs = [(SD, 64-8), (SW, 64-4), (SH, 64-2), (SB, 64-1)]

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(LI(rd=self.rx0, imm=self.mem))

      content_list.append(LI(rd=self.cntr, imm=4))
      content_list.append("loop:")
      for i in range(32):
         (op, tail) = test_helper.pick_one(self.configs)
         self.padding = test_helper.get_rand_range(0, tail)
         content_list.append(op(rs1=self.rx0, rs2=self.rx1, off=((i*64)+self.padding)))
      content_list.append(ADDI(rd=self.cntr, rs1=self.cntr, imm=-1))
      content_list.append(ADDI(rd=self.rx0, rs1=self.rx0, imm=1024))
      content_list.append(ADDI(rd=self.rx0, rs1=self.rx0, imm=1024))
      content_list.append("bne {zero}, {cntr}, loop")
