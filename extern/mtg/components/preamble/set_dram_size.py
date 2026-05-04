
from core.mtg import *
from components.randinst.rv32i import LI
from components.randinst.rv64i import LD, SD

@register
class SetDRAMSize(Component, AsmCode):

   # Valid DRAM Sizes in GB are 8, 16, 24, 32
   dram_size = 8

   _size_dict = { 8: 0b000000,
                 16: 0b010000,
                 24: 0b100000,
                 32: 0b110000}

   def __init__(self, test_helper: TestHelper = None):
      self.read_mprot  = StaticGMalloc('SetDRAMSize_read_mprot', ESR_MPROT_NEIGH0, 8, align=8)
      self.write_mprot = StaticGMalloc('SetDRAMSize_write_mprot',  ESR_MPROT_BCAST, 8, align=8)
      self.rega = Xreg()
      self.regb = Xreg()
      self.regc = Xreg()

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      valid_sizes = [8, 16, 24, 32]
      assert self.dram_size in valid_sizes, f"Invalid DRAM size ({self.dram_size}). Valid sizes are {valid_sizes}"

      # Read the previous MPROT value
      content_list.append(LI(rd=self.rega, imm=self.read_mprot))
      content_list.append(LD(rd=self.regb, off=Imm12(0), rs1=self.rega))

      # Mask out the DRAM_SIZE bits [5:4]
      self.bits_mask = 0xffffffffffffffcf
      content_list.append(
         "li  {regc}, {bits_mask}\n"
         "and {regb}, {regb}, {regc}\n"
      )

      # Set bits [5:4]
      self.enc = self._size_dict[self.dram_size]
      content_list.append(
         "li {regc}, {enc}\n"
         "or {regb}, {regb}, {regc}\n"
      )

      # Set the new MPROT value
      content_list.append(LI(rd=self.rega, imm=self.write_mprot))
      content_list.append(SD(rs2=self.regb, off=Imm12(0), rs1=self.rega))

      # Wait for the store to be completed
      content_list.append("fence iorw, iorw")
