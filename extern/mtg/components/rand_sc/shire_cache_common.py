
from core.mtg import *
from core.component import Component
from components.randinst.amo import *
from components.randinst.packedsingle import *
from components.randinst.packedinteger import *
from components.randinst.rv32i import SW, SH, SB, LI, ADDI, ANDI, SRLI, SLLI, SLL
from components.randinst.rv64i import SLLW
from components.randinst.mask import MOV_M_X, MASKPOPC


@base_class
class SCBase(Component, AsmCode):

   def __init__(self, test_helper: TestHelper = None, glbl = False, size = 'byte', addr: Xreg = None, data: int = None, golden: Freg = None, ps_mask: Xreg = None, data_shift: Xreg = None, fdest: Freg = None, check_pass: Xreg = None, m0: Mreg = None):

      self.glbl = glbl
      self.size = size
      self.addr = addr
      self.data = data
      self.golden = golden
      self.data_shift = data_shift
      self.ps_mask = ps_mask
      self.check_pass = check_pass
      self.m0 = m0
      self.fdest = fdest

      self.golden_value = Xreg()
      self.data_reg = Xreg()
      self.rf1 = Freg()
      self.lhid = ABIreg('lhid')
      self.zero = ABIreg("zero")
      self.tgt_addr = Xreg()
      self.rf_zero = Freg()
      self.amo_ret = Xreg()

      self.rx1 = Xreg()
      self.word_match_cnt = Xreg()
      self.rf3 = Freg()
      self.rf4 = Freg()


@param_block
class SCWriteGolden(SCBase):

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(LI(rd=self.data_reg, imm=self.data))
      # Make data unique to the HART by adding the LHID (might be more interesting to add more randomization here)
      content_list.append(ADD(rd=self.data_reg, rs1=self.data_reg, rs2=self.lhid))

      content_list.append(SCWrite(glbl = self.glbl, size = self.size, addr = self.addr, data = self.data_reg, ps_mask = self.ps_mask, m0 = self.m0))
      content_list.append(MOV_M_X(md=self.m0, rs1=self.ps_mask, imm8=0x0))

      # Update the golden value
      if (self.size == "byte"):
         content_list.append(ANDI(rd=self.data_reg, rs1=self.data_reg, imm=0xff))
         content_list.append(SLL(rd=self.golden_value, rs1=self.data_reg, rs2=self.data_shift))
         content_list.append(FBCX_PS(fd = self.golden, rs1 = self.golden_value, m0=self.m0))
      else:
         content_list.append(FBCX_PS(fd = self.golden, rs1 = self.data_reg, m0=self.m0))

@param_block
class SCReadAndCheck(SCBase):

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(SCRead(glbl = self.glbl, size = self.size, addr = self.addr, fdest = self.fdest, m0 = self.m0))
      content_list.append(SCCheck(size = self.size, addr = self.addr, golden = self.golden, fdest = self.fdest, check_pass = self.check_pass, ps_mask = self.ps_mask, data_shift = self.data_shift, m0 = self.m0))


@param_block
class SCAtomicAndCheck(SCBase):

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:

      content_list.append(LI(rd=self.data_reg, imm=self.data))
      content_list.append(ANDI(rd=self.data_reg, rs1=self.data_reg, imm=0xff))
      # Make data unique to the HART by adding the LHID (might be more interesting to add more randomization here)
      content_list.append(ADD(rd=self.data_reg, rs1=self.data_reg, rs2=self.lhid))

      content_list.append(SCAtomic(glbl = self.glbl, size = self.size, addr = self.addr, data = self.data_reg, ps_mask = self.ps_mask, fdest = self.fdest, m0 = self.m0))
      content_list.append(SCCheck(size = self.size, addr = self.addr, golden = self.golden, fdest = self.fdest, check_pass = self.check_pass, ps_mask = self.ps_mask, m0 = self.m0))

      # Update the golden value
      content_list.append(MOV_M_X(md=self.m0, rs1=self.ps_mask, imm8=0x0))
      content_list.append(FBCX_PS(fd = self.rf1, rs1 = self.data_reg, m0=self.m0))
      content_list.append(FADD_PI(fd = self.golden, fs1 = self.golden, fs2 = self.rf1, m0=self.m0))


@param_block
class SCWrite(SCBase):

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:

      if (self.size == "byte"):
         if (self.glbl == True):
            content_list.append(SBG(rs1 = self.addr, rs2 = self.data))
         else:
            content_list.append(SBL(rs1 = self.addr, rs2 = self.data))
      else:
         content_list.append(ANDI(rd=self.tgt_addr, rs1=self.addr, imm=-4))
         content_list.append(MOV_M_X(md=self.m0, rs1=self.ps_mask, imm8=0x0))
         content_list.append(FBCX_PS(fd = self.rf1, rs1 = self.data, m0=self.m0))
         content_list.append(FBCX_PS(fd = self.rf_zero, rs1 = self.zero, m0=self.m0))
         if (self.glbl == True):
            content_list.append(FSCWG_PS(fs3 = self.rf1, fs1 = self.rf_zero, rs2 = self.tgt_addr))
         else:
            content_list.append(FSCWL_PS(fs3 = self.rf1, fs1 = self.rf_zero, rs2 = self.tgt_addr))


@param_block
class SCRead(SCBase):

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(ANDI(rd=self.tgt_addr, rs1=self.addr, imm=-32))
      content_list.append(MOV_M_X(md=self.m0, rs1=self.zero, imm8=0xff))
      if (self.glbl == True):
         content_list.append(FLWG_PS(fd = self.fdest, rs1 = self.tgt_addr))
      else:
         content_list.append(FLWL_PS(fd = self.fdest, rs1 = self.tgt_addr))

@param_block
class SCAtomic(SCBase):

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(ANDI(rd=self.tgt_addr, rs1=self.addr, imm=-4))
      content_list.append(MOV_M_X(md=self.m0, rs1=self.ps_mask, imm8=0x0))

      if (self.glbl == True):
         content_list.append(AMOADDG_W(rd = self.amo_ret, rs1 = self.tgt_addr, rs2 = self.data))
      else:
         content_list.append(AMOADDL_W(rd = self.amo_ret, rs1 = self.tgt_addr, rs2 = self.data))

      content_list.append(FBCX_PS(fd = self.fdest, rs1 = self.amo_ret, m0=self.m0))


@param_block
class SCCheck(SCBase):

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      if (self.size == "byte"):
         content_list.append(ADDI(rd=self.rx1, rs1=self.zero, imm=0xff))
         content_list.append(SLLW(rd=self.rx1, rs1=self.rx1, rs2=self.data_shift))

      content_list.append(MOV_M_X(md=self.m0, rs1=self.zero, imm8=0xff))
      if (self.size == "byte"):
         content_list.append(FBCX_PS(fd = self.rf4, rs1 = self.zero, m0=self.m0))
      else:
         content_list.append(FBCX_PS(fd = self.rf3, rs1 = self.zero, m0=self.m0))
      content_list.append(MOV_M_X(md=self.m0, rs1=self.ps_mask, imm8=0x0))

      if (self.size == "byte"):
         content_list.append(FBCX_PS(fd = self.rf4, rs1 = self.rx1, m0=self.m0))

         content_list.append(MOV_M_X(md=self.m0, rs1=self.zero, imm8=0xff))
         content_list.append(FAND_PI(fd = self.fdest, fs1 = self.fdest, fs2 = self.rf4, m0=self.m0))

      content_list.append(FEQ_PI(fd = self.rf3, fs1 = self.golden, fs2 = self.fdest, m0=self.m0))

      content_list.append(MOV_M_X(md=self.m0, rs1=self.ps_mask, imm8=0x0))
      content_list.append(FSETM_PI(md = self.m0, fs1 = self.rf3, m0=self.m0))

      content_list.append(MASKPOPC(rd = self.word_match_cnt, ms1 = self.m0))

      content_list.append(LI(rd=self.check_pass, imm=0x0))

      content_list.append(LI(rd=self.rx1, imm=0x1))
      content_list.append("beq  {word_match_cnt}, {rx1}, found_match")

      content_list.append(LI(rd=self.check_pass, imm=0x1))

      content_list.append("found_match:")

