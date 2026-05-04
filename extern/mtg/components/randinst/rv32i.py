from core.mtg import *
from components.encoding.enc import *
from components.mtg_load_addr import MTG_LOAD_ADDR
from components.randinst.randinst_base import RandinstBase

@register
class FENCE(SingleInstruction):
   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      return super()._run(content_list, 'iorw, iorw')

@register
class FENCE_I(SingleInstruction):
   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      return super()._run(content_list, '')

@register
class LUI(RD_IMM20): pass

@register
class AUIPC(RD_IMM20): pass

# TODO
#@register
#class JAL():

# TODO
#@register
#class JALR():

# TODO
#@register
#class BEQ():

# TODO
#@register
#class BNE():

# TODO
#@register
#class BLT():

# TODO
#@register
#class BGE():

# TODO
#@register
#class BLTU():

# TODO
#@register
#class BGEU():

@register
class LB(RD_OFF_RS1): pass

@register
class LH(RD_OFF_RS1): pass

@register
class LW(RD_OFF_RS1): pass

@register
class LBU(RD_OFF_RS1): pass

@register
class LHU(RD_OFF_RS1): pass

@register
class SB(RS2_OFF_RS1): pass

@register
class SH(RS2_OFF_RS1): pass

@register
class SW(RS2_OFF_RS1): pass

@register
class ADDI(RD_RS1_IMM12): pass

@register
class SLTI(RD_RS1_IMM12): pass

@register
class SLTIU(RD_RS1_IMM12): pass

@register
class XORI(RD_RS1_IMM12): pass

@register
class ORI(RD_RS1_IMM12): pass

@register
class ANDI(RD_RS1_IMM12): pass

@register
class SLLI(RD_RS1_IMM6): pass

@register
class SRLI(RD_RS1_IMM6): pass

@register
class SRAI(RD_RS1_IMM6): pass

@register
class ADD(RD_RS1_RS2): pass

@register
class SUB(RD_RS1_RS2): pass

@register
class SLL(RD_RS1_RS2): pass

@register
class SLT(RD_RS1_RS2): pass

@register
class SLTU(RD_RS1_RS2): pass

@register
class XOR(RD_RS1_RS2): pass

@register
class SRL(RD_RS1_RS2): pass

@register
class SRA(RD_RS1_RS2): pass

@register
class OR(RD_RS1_RS2): pass

@register
class AND(RD_RS1_RS2): pass

# TODO
#@register
#class ECALL()

# TODO
#@register
#class EBREAK()

@register
class LI(RD_IMM64):
   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      if isinstance(self.imm, Malloc):
         content_list.append(MTG_LOAD_ADDR(rd=self.rd, mem=self.imm))
      else:
         super()._run(content_list, '{rd}, {imm}')

@register
class RV32I(RandinstBase):

   subcls = [FENCE, LUI, AUIPC, LB, LH, LW, LBU, LHU, SB, SH, SW,
             ADDI, SLTI, SLTIU, XORI, ORI, ANDI, SLLI, SRLI, SRAI, ADD,
             SUB, SLL, SLT, SLTU, XOR, SRL, SRA, OR, AND]

   # Exclusions: FENCE_I, LI
