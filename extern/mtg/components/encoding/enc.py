
from components.encoding.single_instruction import SingleInstruction, VPUSingleInstruction
from core.mtg import *

class M0reg(Mreg):
   def __init__(self):
      super().__init__(0)

class M1reg(Mreg):
   def __init__(self):
      super().__init__(1)

class M2reg(Mreg):
   def __init__(self):
      super().__init__(2)

class M3reg(Mreg):
   def __init__(self):
      super().__init__(3)

class M4reg(Mreg):
   def __init__(self):
      super().__init__(4)

class M5reg(Mreg):
   def __init__(self):
      super().__init__(5)

class M6reg(Mreg):
   def __init__(self):
      super().__init__(6)

class M7reg(Mreg):
   def __init__(self):
      super().__init__(7)

class X31reg(Xreg):
   def __init__(self):
      super().__init__(31)

class BAreg(ABIreg):
   def __init__(self):
      super().__init__('tp')

class GAMOreg(ABIreg):
   exclude = bool(TEST_CONF.POSTSI_L2_ONLY)
   def __init__(self):
      super().__init__('gamotp')

class LAMOreg(ABIreg):
   def __init__(self):
      super().__init__('lamotp')

class SPreg(ABIreg):
   def __init__(self):
      super().__init__('sp')

class FPAMOreg(FPABIreg):
   def __init__(self):
      super().__init__('amo')

class FPOFFreg(FPABIreg):
   def __init__(self):
      super().__init__('off')

@mtg_dataclass
class MD_MS1_MS2(SingleInstruction):
   md:  Mreg = field(default_factory=Mreg)
   ms1: Mreg = field(default_factory=Mreg)
   ms2: Mreg = field(default_factory=Mreg)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      return super()._run(content_list, '{md}, {ms1}, {ms2}')

@mtg_dataclass
class MD_MS1(SingleInstruction):
   md:  Mreg = field(default_factory=Mreg)
   ms1: Mreg = field(default_factory=Mreg)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      return super()._run(content_list, '{md}, {ms1}')

@mtg_dataclass
class RD_M0_M1_M2_M3_M4_M5_M6_M7(VPUSingleInstruction):
   rd: Xreg = field(default_factory=Xreg)
   m0:  Mreg = field(default_factory=M0reg)
   m1:  Mreg = field(default_factory=M1reg)
   m2:  Mreg = field(default_factory=M2reg)
   m3:  Mreg = field(default_factory=M3reg)
   m4:  Mreg = field(default_factory=M4reg)
   m5:  Mreg = field(default_factory=M5reg)
   m6:  Mreg = field(default_factory=M6reg)
   m7:  Mreg = field(default_factory=M7reg)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      return super()._run(content_list, '{rd} /*{m0}, {m1}, {m2}, {m3}, {m4}, {m5}, {m6}, {m7}*/')

@mtg_dataclass
class M0_M1_M2_M3_M4_M5_M6_M7_RS1(VPUSingleInstruction):
   rs1: Xreg = field(default_factory=Xreg)
   m0:  Mreg = field(default_factory=M0reg)
   m1:  Mreg = field(default_factory=M1reg)
   m2:  Mreg = field(default_factory=M2reg)
   m3:  Mreg = field(default_factory=M3reg)
   m4:  Mreg = field(default_factory=M4reg)
   m5:  Mreg = field(default_factory=M5reg)
   m6:  Mreg = field(default_factory=M6reg)
   m7:  Mreg = field(default_factory=M7reg)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      return super()._run(content_list, '{rs1} /*{m0}, {m1}, {m2}, {m3}, {m4}, {m5}, {m6}, {m7}*/')

@mtg_dataclass
class MD_RS1_IMM8(VPUSingleInstruction):
   md:   Mreg =  field(default_factory=Mreg)
   rs1:  Xreg =  field(default_factory=Xreg)
   imm8: Zimm8 = field(default_factory=Zimm8)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      return super()._run(content_list, '{md}, {rs1}, {imm8}')

@mtg_dataclass
class RD_MS1(VPUSingleInstruction):
   rd:  Xreg = field(default_factory=Xreg)
   ms1: Mreg = field(default_factory=Mreg)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      return super()._run(content_list, '{rd}, {ms1}')

@mtg_dataclass
class CSRRWI(Component, AsmCode):
   rd:     Xreg  = field(default_factory=Xreg)
   zimm5:  Zimm5 = field(default_factory=Zimm5)

   def __component_post_init__(self, *args, **kwargs):
      self.opcode = self.__class__.__name__.lower()
      assert self.opcode != "csrrwi", "CSRRWI class can only be extended by another class. You must not instantiate a CSRRWI object"
      super().__component_post_init__(*args, **kwargs)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append('csrrwi {rd}, {opcode}, {zimm5}\n')

@mtg_dataclass
class CSRRW(Component, AsmCode):
   rd:     Xreg  = field(default_factory=Xreg)
   rs1:    Xreg  = field(default_factory=Xreg)

   def __component_post_init__(self, *args, **kwargs):
      self.opcode = self.__class__.__name__.lower()
      assert self.opcode != "csrrw", "CSRRW class can only be extended by another class. You must not instantiate a CSRRW object"
      super().__component_post_init__(*args, **kwargs)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append('csrrw {rd}, {opcode}, {rs1}\n')

@mtg_dataclass
class CSRRW_X31(CSRRW):
   x31: Xreg = field(default_factory=X31reg)
   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      assert self.x31.reg_num == 31, f"{self.__class__.__name__} implicitly uses X31, but the 'x31' attribute is {self.x31}"
      content_list.append('csrrw {rd}, {opcode}, {rs1} /*{x31}*/\n')

@mtg_dataclass
class CSR_WRAP(Component, AsmCode):

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(self.insn)

@mtg_dataclass
class FD_OFF_RS1(VPUSingleInstruction, AsmCode):
   fd:  Freg  = field(default_factory=Freg)
   off: Imm12 = field(default_factory=Imm12)
   rs1: ABIreg = field(default_factory=BAreg)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      super()._run(content_list, '{fd}, {off}({rs1})')
      fv260_a_wa(content_list)

@mtg_dataclass
class FD_OFF_RS1_M0(VPUSingleInstruction, AsmCode):
   fd:  Freg  = field(default_factory=Freg)
   off: Imm12 = field(default_factory=Imm12)
   rs1: ABIreg = field(default_factory=BAreg)
   m0:  Mreg = field(default_factory=M0reg)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      super()._run(content_list, '{fd}, {off}({rs1}) /*{m0}*/')
      fv260_a_wa(content_list)

@mtg_dataclass
class FS2_OFF_RS1(VPUSingleInstruction, AsmCode):
   fs2: Freg  = field(default_factory=Freg)
   off: Imm12 = field(default_factory=Imm12)
   rs1: ABIreg = field(default_factory=BAreg)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      return super()._run(content_list, '{fs2}, {off}({rs1})')

@mtg_dataclass
class FS2_OFF_RS1_M0(VPUSingleInstruction, AsmCode):
   fs2: Freg  = field(default_factory=Freg)
   off: Imm12 = field(default_factory=Imm12)
   rs1: ABIreg = field(default_factory=BAreg)
   m0:  Mreg = field(default_factory=M0reg)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      return super()._run(content_list, '{fs2}, {off}({rs1}) /*{m0}*/')

@mtg_dataclass
class FD_IMM20_M0(VPUSingleInstruction, AsmCode):
   fd:    Freg  = field(default_factory=Freg)
   imm20: Zimm20 = field(default_factory=Zimm20)
   m0:  Mreg = field(default_factory=M0reg)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      super()._run(content_list, '{fd}, {imm20} /*{m0}*/')
      fv260_b_wa(content_list)

@mtg_dataclass
class FD_RS1(VPUSingleInstruction, AsmCode):
   fd:  Freg = field(default_factory=Freg)
   rs1: Xreg = field(default_factory=Xreg)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      super()._run(content_list, '{fd}, {rs1}')
      fv260_b_wa(content_list)

@mtg_dataclass
class FD_RS1_M0(VPUSingleInstruction, AsmCode):
   fd:  Freg = field(default_factory=Freg)
   rs1: Xreg = field(default_factory=Xreg)
   m0:  Mreg = field(default_factory=M0reg)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      super()._run(content_list, '{fd}, {rs1} /*{m0}*/')
      fv260_b_wa(content_list)

@mtg_dataclass
class FD_FS1_RS2_M0(VPUSingleInstruction, AsmCode):
   fd:  Freg = field(default_factory=Freg)
   fs1: FPABIreg = field(default_factory=FPOFFreg)
   rs2: ABIreg = field(default_factory=BAreg)
   m0:  Mreg = field(default_factory=M0reg)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      super()._run(content_list, '{fd}, {fs1}({rs2}) /*{m0}*/')
      fv260_a_wa(content_list)

@mtg_dataclass
class GAMO_FD_FS1_RS2_M0(VPUSingleInstruction, AsmCode):
   exclude = bool(TEST_CONF.POSTSI_L2_ONLY)
   fd:  Freg = field(default_factory=Freg)
   fs1: FPABIreg = field(default_factory=FPAMOreg)
   rs2: ABIreg = field(default_factory=GAMOreg)
   m0:  Mreg = field(default_factory=M0reg)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      super()._run(content_list, '{fd}, {fs1}({rs2}) /*{m0}*/')
      fv260_a_wa(content_list)

@mtg_dataclass
class LAMO_FD_FS1_RS2_M0(VPUSingleInstruction, AsmCode):
   fd:  Freg = field(default_factory=Freg)
   fs1: FPABIreg = field(default_factory=FPAMOreg)
   rs2: ABIreg = field(default_factory=LAMOreg)
   m0:  Mreg = field(default_factory=M0reg)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      super()._run(content_list, '{fd}, {fs1}({rs2}) /*{m0}*/')
      fv260_a_wa(content_list)

@mtg_dataclass
class FS3_FS1_RS2_M0(VPUSingleInstruction, AsmCode):
   fs3: Freg = field(default_factory=Freg)
   fs1: FPABIreg = field(default_factory=FPOFFreg)
   rs2: ABIreg = field(default_factory=BAreg)
   m0:  Mreg = field(default_factory=M0reg)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      return super()._run(content_list, '{fs3}, {fs1}({rs2}) /*{m0}*/')

@mtg_dataclass
class FD_RS1_RS2_M0(VPUSingleInstruction, AsmCode):
   fd:  Freg = field(default_factory=Freg)
   rs1: Xreg = field(default_factory=Xreg)
   rs2: ABIreg = field(default_factory=BAreg)
   m0:  Mreg = field(default_factory=M0reg)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      super()._run(content_list, '{fd}, {rs1}({rs2}) /*{m0}*/')
      fv260_a_wa(content_list)

@mtg_dataclass
class FS3_RS1_RS2_M0(VPUSingleInstruction, AsmCode):
   fs3: Freg = field(default_factory=Freg)
   rs1: Xreg = field(default_factory=Xreg)
   rs2: ABIreg = field(default_factory=BAreg)
   m0:  Mreg = field(default_factory=M0reg)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      return super()._run(content_list, '{fs3}, {rs1}({rs2}) /*{m0}*/')

@mtg_dataclass
class FD_FS1_FS2_RM(VPUSingleInstruction, AsmCode):
   fd:  Freg = field(default_factory=Freg)
   fs1: Freg = field(default_factory=Freg)
   fs2: Freg = field(default_factory=Freg)
   rm:  Frm  = field(default_factory=Frm)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      super()._run(content_list, '{fd}, {fs1}, {fs2}, {rm}')
      fv260_c_wa(content_list)

@mtg_dataclass
class FD_FS1_FS2_M0_RM(VPUSingleInstruction, AsmCode):
   fd:  Freg = field(default_factory=Freg)
   fs1: Freg = field(default_factory=Freg)
   fs2: Freg = field(default_factory=Freg)
   rm:  Frm  = field(default_factory=Frm)
   m0:  Mreg = field(default_factory=M0reg)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      super()._run(content_list, '{fd}, {fs1}, {fs2}, {rm} /*{m0}*/')
      fv260_c_wa(content_list)

@mtg_dataclass
class FD_FS1_FS2(VPUSingleInstruction, AsmCode):
   fd:  Freg = field(default_factory=Freg)
   fs1: Freg = field(default_factory=Freg)
   fs2: Freg = field(default_factory=Freg)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      super()._run(content_list, '{fd}, {fs1}, {fs2}')
      fv260_c_wa(content_list)

@mtg_dataclass
class FD_FS1_FS2_M0(VPUSingleInstruction, AsmCode):
   fd:  Freg = field(default_factory=Freg)
   fs1: Freg = field(default_factory=Freg)
   fs2: Freg = field(default_factory=Freg)
   m0:  Mreg = field(default_factory=M0reg)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      super()._run(content_list, '{fd}, {fs1}, {fs2} /*{m0}*/')
      fv260_c_wa(content_list)

@mtg_dataclass
class FD_FS1_FS2_PI(VPUSingleInstruction, AsmCode):
   fd:  Freg = field(default_factory=Freg)
   fs1: Freg = field(default_factory=Freg)
   fs2: Freg = field(default_factory=Freg)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      super()._run(content_list, '{fd}, {fs1}, {fs2}')
      fv260_b_wa(content_list)

@mtg_dataclass
class FD_FS1_FS2_M0_PI(VPUSingleInstruction, AsmCode):
   fd:  Freg = field(default_factory=Freg)
   fs1: Freg = field(default_factory=Freg)
   fs2: Freg = field(default_factory=Freg)
   m0:  Mreg = field(default_factory=M0reg)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      super()._run(content_list, '{fd}, {fs1}, {fs2} /*{m0}*/')
      fv260_b_wa(content_list)

@mtg_dataclass
class FD_FS1_M0(VPUSingleInstruction, AsmCode):
   fd:  Freg = field(default_factory=Freg)
   fs1: Freg = field(default_factory=Freg)
   m0:  Mreg = field(default_factory=M0reg)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      super()._run(content_list, '{fd}, {fs1} /*{m0}*/')
      fv260_c_wa(content_list)

@mtg_dataclass
class FD_FS1_M0_TRANS(VPUSingleInstruction, AsmCode):
   fd:  Freg = field(default_factory=Freg)
   fs1: Freg = field(default_factory=Freg)
   m0:  Mreg = field(default_factory=M0reg)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      super()._run(content_list, '{fd}, {fs1} /*{m0}*/')
      fv260_a_wa(content_list)

@mtg_dataclass
class FD_FS1_M0_PI(VPUSingleInstruction, AsmCode):
   fd:  Freg = field(default_factory=Freg)
   fs1: Freg = field(default_factory=Freg)
   m0:  Mreg = field(default_factory=M0reg)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      super()._run(content_list, '{fd}, {fs1} /*{m0}*/')
      fv260_b_wa(content_list)

@mtg_dataclass
class FD_FS1_FS2_FS3_M0_RM(VPUSingleInstruction, AsmCode):
   fd:  Freg = field(default_factory=Freg)
   fs1: Freg = field(default_factory=Freg)
   fs2: Freg = field(default_factory=Freg)
   fs3: Freg = field(default_factory=Freg)
   rm:  Frm  = field(default_factory=Frm)
   m0:  Mreg = field(default_factory=M0reg)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      super()._run(content_list, '{fd}, {fs1}, {fs2}, {fs3}, {rm} /*{m0}*/')
      fv260_c_wa(content_list)

@mtg_dataclass
class FD_FS1_FS2_FS3_RM(VPUSingleInstruction, AsmCode):
   fd:  Freg = field(default_factory=Freg)
   fs1: Freg = field(default_factory=Freg)
   fs2: Freg = field(default_factory=Freg)
   fs3: Freg = field(default_factory=Freg)
   rm:  Frm  = field(default_factory=Frm)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      super()._run(content_list, '{fd}, {fs1}, {fs2}, {fs3}, {rm}')
      fv260_c_wa(content_list)

@mtg_dataclass
class FD_FS1_FS2_FS3_M0(VPUSingleInstruction, AsmCode):
   fd:  Freg = field(default_factory=Freg)
   fs1: Freg = field(default_factory=Freg)
   fs2: Freg = field(default_factory=Freg)
   fs3: Freg = field(default_factory=Freg)
   m0:  Mreg = field(default_factory=M0reg)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      super()._run(content_list, '{fd}, {fs1}, {fs2}, {fs3} /*{m0}*/')
      fv260_b_wa(content_list)

@mtg_dataclass
class FD_FS1_RM(VPUSingleInstruction, AsmCode):
   fd:  Freg = field(default_factory=Freg)
   fs1: Freg = field(default_factory=Freg)
   rm:  Frm  = field(default_factory=Frm)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      super()._run(content_list, '{fd}, {fs1}, {rm}')
      fv260_c_wa(content_list)

@mtg_dataclass
class FD_FS1_M0_RM(VPUSingleInstruction, AsmCode):
   fd:  Freg = field(default_factory=Freg)
   fs1: Freg = field(default_factory=Freg)
   rm:  Frm  = field(default_factory=Frm)
   m0:  Mreg = field(default_factory=M0reg)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      super()._run(content_list, '{fd}, {fs1}, {rm} /*{m0}*/')
      fv260_c_wa(content_list)

@mtg_dataclass
class RD_FS1_IDX(VPUSingleInstruction, AsmCode):
   rd:  Xreg  = field(default_factory=Xreg)
   fs1: Freg  = field(default_factory=Freg)
   idx: Zimm3 = field(default_factory=Zimm3)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      return super()._run(content_list, '{rd}, {fs1}, {idx}')

@mtg_dataclass
class FD_FS1_IMM_M0(VPUSingleInstruction, AsmCode):
   fd:  Freg  = field(default_factory=Freg)
   fs1: Freg  = field(default_factory=Freg)
   imm: Zimm8 = field(default_factory=Zimm8)
   m0:  Mreg = field(default_factory=M0reg)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      super()._run(content_list, '{fd}, {fs1}, {imm} /*{m0}*/')
      fv260_b_wa(content_list)

@mtg_dataclass
class MD_FS1_FS2(VPUSingleInstruction, AsmCode):
   md:  Mreg  = field(default_factory=Mreg)
   fs1: Freg  = field(default_factory=Freg)
   fs2: Freg  = field(default_factory=Freg)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      return super()._run(content_list, '{md}, {fs1}, {fs2}')

@mtg_dataclass
class MD_FS1_FS2_M0(VPUSingleInstruction, AsmCode):
   md:  Mreg  = field(default_factory=Mreg)
   fs1: Freg  = field(default_factory=Freg)
   fs2: Freg  = field(default_factory=Freg)
   m0:  Mreg = field(default_factory=M0reg)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      return super()._run(content_list, '{md}, {fs1}, {fs2} /*{m0}*/')

@mtg_dataclass
class FD_FS1_IMM10_M0(VPUSingleInstruction, AsmCode):
   fd:  Freg  = field(default_factory=Freg)
   fs1: Freg  = field(default_factory=Freg)
   imm: Imm10 = field(default_factory=Imm10)
   m0:  Mreg = field(default_factory=M0reg)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      super()._run(content_list, '{fd}, {fs1}, {imm} /*{m0}*/')
      fv260_b_wa(content_list)

@mtg_dataclass
class FD_FS1_IMM5_M0(VPUSingleInstruction, AsmCode):
   fd:  Freg  = field(default_factory=Freg)
   fs1: Freg  = field(default_factory=Freg)
   imm: Zimm5 = field(default_factory=Zimm5)
   m0:  Mreg = field(default_factory=M0reg)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      super()._run(content_list, '{fd}, {fs1}, {imm} /*{m0}*/')
      fv260_b_wa(content_list)

@mtg_dataclass
class MD_FS1_M0(VPUSingleInstruction, AsmCode):
   md:  Mreg  = field(default_factory=Mreg)
   fs1: Freg  = field(default_factory=Freg)
   m0:  Mreg = field(default_factory=M0reg)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      return super()._run(content_list, '{md}, {fs1} /*{m0}*/')

@mtg_dataclass
class RD_RS1_RS2(SingleInstruction):
   rd:  Xreg = field(default_factory=Xreg)
   rs1: Xreg = field(default_factory=Xreg)
   rs2: Xreg = field(default_factory=Xreg)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      return super()._run(content_list, '{rd}, {rs1}, {rs2}')

@mtg_dataclass
class RD_RS1_RS2_M0(SingleInstruction):
   rd:  Xreg = field(default_factory=Xreg)
   rs1: Xreg = field(default_factory=Xreg)
   rs2: Xreg = field(default_factory=Xreg)
   m0:  Mreg = field(default_factory=M0reg)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      return super()._run(content_list, '{rd}, {rs1}, {rs2} /*{m0}*/')

@mtg_dataclass
class RD_IMM20(SingleInstruction):
   rd:  Xreg = field(default_factory=Xreg)
   imm: Zimm20 = field(default_factory=Zimm20)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      return super()._run(content_list, '{rd}, {imm}')

@mtg_dataclass
class RD_IMM64(SingleInstruction):
   rd:  Xreg = field(default_factory=Xreg)
   imm: Zimm64 = field(default_factory=Zimm64)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      return super()._run(content_list, '{rd}, {imm}')

@mtg_dataclass
class RD_OFF_RS1(SingleInstruction):
   rd:  Xreg   = field(default_factory=Xreg)
   off: Imm12  = field(default_factory=Imm12)
   rs1: ABIreg = field(default_factory=BAreg)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      return super()._run(content_list, '{rd}, {off}({rs1})')

@mtg_dataclass
class RS2_OFF_RS1(SingleInstruction):
   rs2: Xreg   = field(default_factory=Xreg)
   off: Imm12  = field(default_factory=Imm12)
   rs1: ABIreg = field(default_factory=BAreg)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      return super()._run(content_list, '{rs2}, {off}({rs1})')

@mtg_dataclass
class RD_RS1_IMM12(SingleInstruction):
   rd:  Xreg  = field(default_factory=Xreg)
   rs1: Xreg  = field(default_factory=Xreg)
   imm: Imm12 = field(default_factory=Imm12)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      return super()._run(content_list, '{rd}, {rs1}, {imm}')

@mtg_dataclass
class RD_RS1_IMM6(SingleInstruction):
   rd:  Xreg = field(default_factory=Xreg)
   rs1: Xreg = field(default_factory=Xreg)
   imm: Zimm6 = field(default_factory=Zimm6)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      return super()._run(content_list, '{rd}, {rs1}, {imm}')

@mtg_dataclass
class RD_RS1_IMM5(SingleInstruction):
   rd:  Xreg = field(default_factory=Xreg)
   rs1: Xreg = field(default_factory=Xreg)
   imm: Zimm5 = field(default_factory=Zimm5)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      return super()._run(content_list, '{rd}, {rs1}, {imm}')

@mtg_dataclass
class RD_FS1_RM(SingleInstruction, AsmCode):
   rd:  Xreg  = field(default_factory=Xreg)
   fs1: Freg  = field(default_factory=Freg)
   rm:  Frm  = field(default_factory=Frm)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      return super()._run(content_list, '{rd}, {fs1}, {rm}')

@mtg_dataclass
class RD_FS1(VPUSingleInstruction, AsmCode):
   rd:  Xreg  = field(default_factory=Xreg)
   fs1: Freg  = field(default_factory=Freg)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      return super()._run(content_list, '{rd}, {fs1}')

@mtg_dataclass
class RD_FS1_FS2(VPUSingleInstruction, AsmCode):
   rd:  Xreg  = field(default_factory=Xreg)
   fs1: Freg  = field(default_factory=Freg)
   fs2: Freg  = field(default_factory=Freg)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      return super()._run(content_list, '{rd}, {fs1}, {fs2}')

@mtg_dataclass
class FD_RS1_RM(SingleInstruction, AsmCode):
   fd:  Freg  = field(default_factory=Freg)
   rs1: Xreg  = field(default_factory=Xreg)
   rm:  Frm  = field(default_factory=Frm)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      super()._run(content_list, '{fd}, {rs1}, {rm}')
      fv260_c_wa(content_list)

@mtg_dataclass
class GAMO_FD_NOOFF_RS1_M0(VPUSingleInstruction, AsmCode):
   exclude = bool(TEST_CONF.POSTSI_L2_ONLY)
   fd:  Freg  = field(default_factory=Freg)
   rs1: ABIreg = field(default_factory=GAMOreg)
   m0:  Mreg = field(default_factory=M0reg)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      super()._run(content_list, '{fd}, ({rs1}) /*{m0}*/')
      fv260_a_wa(content_list)

@mtg_dataclass
class LAMO_FD_NOOFF_RS1_M0(VPUSingleInstruction, AsmCode):
   fd:  Freg  = field(default_factory=Freg)
   rs1: ABIreg = field(default_factory=LAMOreg)
   m0:  Mreg = field(default_factory=M0reg)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      super()._run(content_list, '{fd}, ({rs1}) /*{m0}*/')
      fv260_a_wa(content_list)

@mtg_dataclass
class GAMO_FS3_NOOFF_RS1_M0(VPUSingleInstruction, AsmCode):
   exclude = bool(TEST_CONF.POSTSI_L2_ONLY)
   fs3: Freg  = field(default_factory=Freg)
   rs1: ABIreg = field(default_factory=GAMOreg)
   m0:  Mreg = field(default_factory=M0reg)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      return super()._run(content_list, '{fs3}, ({rs1}) /*{m0}*/')

@mtg_dataclass
class LAMO_FS3_NOOFF_RS1_M0(VPUSingleInstruction, AsmCode):
   fs3: Freg  = field(default_factory=Freg)
   rs1: ABIreg = field(default_factory=LAMOreg)
   m0:  Mreg = field(default_factory=M0reg)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      return super()._run(content_list, '{fs3}, ({rs1}) /*{m0}*/')

@mtg_dataclass
class GAMO_FS3_FPOFF_RS2_M0(VPUSingleInstruction, AsmCode):
   exclude = bool(TEST_CONF.POSTSI_L2_ONLY)
   fs3:  Freg     = field(default_factory=Freg)
   fs1: FPABIreg = field(default_factory=FPAMOreg)
   rs2: ABIreg   = field(default_factory=GAMOreg)
   m0:  Mreg = field(default_factory=M0reg)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      return super()._run(content_list, '{fs3}, {fs1}({rs2}) /*{m0}*/')

@mtg_dataclass
class LAMO_FS3_FPOFF_RS2_M0(VPUSingleInstruction, AsmCode):
   fs3:  Freg     = field(default_factory=Freg)
   fs1: FPABIreg = field(default_factory=FPAMOreg)
   rs2: ABIreg   = field(default_factory=LAMOreg)
   m0:  Mreg = field(default_factory=M0reg)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      return super()._run(content_list, '{fs3}, {fs1}({rs2}) /*{m0}*/')

@mtg_dataclass
class GAMO_FD_FPOFF_RS2_M0(VPUSingleInstruction, AsmCode):
   exclude = bool(TEST_CONF.POSTSI_L2_ONLY)
   fd:  Freg     = field(default_factory=Freg)
   fs1: FPABIreg = field(default_factory=FPAMOreg)
   rs2: ABIreg   = field(default_factory=GAMOreg)
   m0:  Mreg = field(default_factory=M0reg)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      super()._run(content_list, '{fd}, {fs1}({rs2}) /*{m0}*/')
      fv260_a_wa(content_list)

@mtg_dataclass
class LAMO_FD_FPOFF_RS2_M0(VPUSingleInstruction, AsmCode):
   fd:  Freg     = field(default_factory=Freg)
   fs1: FPABIreg = field(default_factory=FPAMOreg)
   rs2: ABIreg   = field(default_factory=LAMOreg)
   m0:  Mreg = field(default_factory=M0reg)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      super()._run(content_list, '{fd}, {fs1}({rs2}) /*{m0}*/')
      fv260_a_wa(content_list)

@mtg_dataclass
class GAMO_RS2_NOOFF_RS1(SingleInstruction):
   exclude = bool(TEST_CONF.POSTSI_L2_ONLY)
   rs1: ABIreg = field(default_factory=GAMOreg)
   rs2: Xreg = field(default_factory=Xreg)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      return super()._run(content_list, '{rs2},({rs1})')

@mtg_dataclass
class LAMO_RS2_NOOFF_RS1(SingleInstruction):
   rs1: ABIreg = field(default_factory=LAMOreg)
   rs2: Xreg = field(default_factory=Xreg)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      return super()._run(content_list, '{rs2},({rs1})')

@mtg_dataclass
class GAMO_RD_RS2_NOFF_RS1(SingleInstruction):
   exclude = bool(TEST_CONF.POSTSI_L2_ONLY)
   rd:  Xreg = field(default_factory=Xreg)
   rs2: Xreg = field(default_factory=Xreg)
   rs1: ABIreg = field(default_factory=GAMOreg)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      return super()._run(content_list, '{rd}, {rs2}, ({rs1})')

@mtg_dataclass
class GAMO_RD_RS2_NOFF_RS1_X31(SingleInstruction):
   exclude = bool(TEST_CONF.POSTSI_L2_ONLY)
   rd:  Xreg = field(default_factory=Xreg)
   rs2: Xreg = field(default_factory=Xreg)
   x31: Xreg = field(default_factory=X31reg)
   rs1: ABIreg = field(default_factory=GAMOreg)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      assert self.x31.reg_num == 31, f"{self.__class__.__name__} implicitly uses X31, but the 'x31' attribute is {self.x31}"
      return super()._run(content_list, '{rd}, {rs2}, ({rs1}) /*{x31}*/')

@mtg_dataclass
class LAMO_RD_RS2_NOFF_RS1(SingleInstruction):
   rd:  Xreg = field(default_factory=Xreg)
   rs2: Xreg = field(default_factory=Xreg)
   rs1: ABIreg = field(default_factory=LAMOreg)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      return super()._run(content_list, '{rd}, {rs2}, ({rs1})')

@mtg_dataclass
class LAMO_RD_RS2_NOFF_RS1_X31(SingleInstruction):
   rd:  Xreg = field(default_factory=Xreg)
   rs2: Xreg = field(default_factory=Xreg)
   x31: Xreg = field(default_factory=X31reg)
   rs1: ABIreg = field(default_factory=LAMOreg)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      assert self.x31.reg_num == 31, f"{self.__class__.__name__} implicitly uses X31, but the 'x31' attribute is {self.x31}"
      return super()._run(content_list, '{rd}, {rs2}, ({rs1}) /*{x31}*/')
