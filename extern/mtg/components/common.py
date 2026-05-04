
from abc import ABCMeta, abstractmethod
from dataclasses import dataclass, field, InitVar

from core.models.model import *
from core.snippet.asm import Asm, AsmCode
from core.snippet.snippet import Snippet
from core.component import Component, param_block, base_class
from core.skip_harts import only_thread, only_lhid, only_mod_lhid
from core.component_factory import register, get_component, mtg_dataclass
from core.test_conf import TEST_CONF
from core.priv.decorators import machine_code

from components.encoding.enc import SPreg, M0reg, M1reg, M2reg, M3reg, M4reg, M5reg, M6reg, M7reg
from components.randinst.rv32i import ADD, ADDI
from components.randinst.rv64i import SD, LD
from components.randinst.packedsingle import FSQ2, FLQ2

def unzip(l: list):
   ul = list(zip(*l))
   return ul[0], ul[1]

def get_active_shire_ids():
   assert TEST_CONF.agnostic_mode() == False
   active_shires = list()
   shire_mask = TEST_CONF.SHIRE_MASK
   sid = 0
   while shire_mask != 0:
      if (shire_mask & 0x1) != 0:
         active_shires.append(sid)
      sid = sid + 1
      shire_mask = shire_mask >> 1
   return active_shires

def get_active_minion_ids():
   assert TEST_CONF.agnostic_mode() == False
   active_minions = list()
   minion_mask = TEST_CONF.MINION_MASK
   mid = 0
   while minion_mask != 0:
      if (minion_mask & 0x1) != 0:
         active_minions.append(mid)
      mid = mid + 1
      minion_mask = minion_mask >> 1
   return active_minions

def get_num_harts_local_shire(skip_assert=False, thread_mask=TEST_CONF.THREAD_MASK):

   num_active_minions = bin(TEST_CONF.MINION_MASK).count('1')
   if thread_mask == 0x1:
      num_active_threads = 1
   elif thread_mask == 0x2:
      num_active_threads = 1
   elif thread_mask == 0x3:
      num_active_threads = 2
   else:
      raise ValueError('thread_mask must be 0x1, 0x2 or 0x3')

   return num_active_minions * num_active_threads

def get_bits(value: int, head: int, tail: int):
    # Just in case the programmer does not specify the arguments in the correct order
    if head < tail:
       tmp = head
       head = tail
       tail = tmp
    # we want to extract k-bits
    k = (head - tail) + 1;
    return (((1 << k) - 1) & (value >> tail))

def set_bits(value: int, head: int, tail: int, set_value: int):
    # Just in case the programmer does not specify the arguments in the correct order
    if head < tail:
       tmp = head
       head = tail
       tail = tmp
    # we want to set k-bits
    k = (head - tail) + 1
    mask = ((1 << k) - 1)
    set_value = (set_value & mask) << tail
    mask = ~(mask << tail)
    value = value & mask
    value = value | set_value
    return value

@register
@mtg_dataclass
class BackupToSP(Component, AsmCode):
   sp  : SPreg = field(default_factory=SPreg)
   rs1 : Xreg  = field(default_factory=Xreg)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append("addi {sp}, {sp}, -8")
      content_list.append("sd {rs1}, 0({sp})")

@register
@mtg_dataclass
class RestoreFromSP(Component, AsmCode):
   sp : SPreg = field(default_factory=SPreg)
   rd : Xreg  = field(default_factory=Xreg)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append("ld {rd}, 0({sp})")
      content_list.append("addi {sp}, {sp}, 8")

@register
@mtg_dataclass
class BackupMregs(Component, AsmCode):
   rx : Xreg  = field(default_factory=Xreg)
   m0 : Mreg  = field(default_factory=M0reg)
   m1:  Mreg = field(default_factory=M1reg)
   m2:  Mreg = field(default_factory=M2reg)
   m3:  Mreg = field(default_factory=M3reg)
   m4:  Mreg = field(default_factory=M4reg)
   m5:  Mreg = field(default_factory=M5reg)
   m6:  Mreg = field(default_factory=M6reg)
   m7:  Mreg = field(default_factory=M7reg)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      from components.randinst.mask import MOVA_X_M
      content_list.append(MOVA_X_M(rd = self.rx, m0 = self.m0, m1 = self.m1, m2 = self.m2, m3 = self.m3, m4 = self.m4, m5 = self.m5, m6 = self.m6, m7 = self.m7))
      content_list.append(BackupToSP(rs1 = self.rx))

@register
@mtg_dataclass
class RestoreMregs(Component, AsmCode):
   rx : Xreg  = field(default_factory=Xreg)
   m0 : Mreg  = field(default_factory=M0reg)
   m1:  Mreg = field(default_factory=M1reg)
   m2:  Mreg = field(default_factory=M2reg)
   m3:  Mreg = field(default_factory=M3reg)
   m4:  Mreg = field(default_factory=M4reg)
   m5:  Mreg = field(default_factory=M5reg)
   m6:  Mreg = field(default_factory=M6reg)
   m7:  Mreg = field(default_factory=M7reg)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      from components.randinst.mask import MOVA_M_X
      content_list.append(RestoreFromSP(rd = self.rx))
      content_list.append(MOVA_M_X(rs1 = self.rx, m0 = self.m0, m1 = self.m1, m2 = self.m2, m3 = self.m3, m4 = self.m4, m5 = self.m5, m6 = self.m6, m7 = self.m7))

@register
class NOP(Component, AsmCode):
   zero : ABIreg = ABIreg('zero')

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(ADD(rd=self.zero, rs1=self.zero, rs2=self.zero))

@param_block
class TEST_PASS(Component, AsmCode):
   def __init__(self, test_helper: TestHelper = None):
      self.rx0 = Xreg()

   @machine_code
   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      insns = ["fence"]
      if TEST_CONF.POST_SILICON == 0:
         insns += ["lui {rx0}, 0x1FEED",
                   "csrw validation0, {rx0}"]
      else:
         insns += ["lui x31, 0x1FEED",
                   "csrwi validation2, 1",
                   "call postsi_ust_test_end"]
      insns += ["test_pass:",
                "wfi",
                "j test_pass"]
      content_list += insns

@param_block
class TEST_FAIL(Component, AsmCode):
   @classmethod
   def add_debug_info(cls, content_list: list):
      content_list.append("auipc x12, 0")

   @machine_code
   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      insns = ["fence"]
      if TEST_CONF.POST_SILICON == 0:
         self.add_debug_info(insns)
         insns += ["lui x31, 0x50BAD",
                   "csrw validation0, x31"]
         insns += ["test_fail:",
                   "wfi",
                   "j test_fail"]
      else:
         self.add_debug_info(insns)
         insns += ["lui x31, 0x50BAD",
                   "csrwi validation2, 1"]
         insns.append("call postsi_ust_test_end")
      content_list += insns

@base_class
class MTG_ABI(Component, AsmCode, metaclass=ABCMeta):

   def __init__(self, test_helper: TestHelper = None):
      self.xregs = ["gamotp", "lamotp", "lhid", "ra"]
      self.xreg_size = 8
      self.bak_xregs = [ABIreg(x) for x in self.xregs]

      self.fregs = ["amo", "off"]
      self.freg_size = FPModel.NUM_LANES * FPModel.LANE_SIZE
      self.bak_fregs = [FPABIreg(x) for x in self.fregs]

      self.sp = ABIreg('sp')

   def int_regs(self, content_list: list):
      idx = 0
      for reg in self.bak_xregs:
         self._XregOp(content_list, reg, (idx*self.xreg_size), self.sp)
         idx = idx + 1

   def fp_regs(self, content_list: list):
      idx = 0
      for reg in self.bak_fregs:
         self._FregOp(content_list, reg, (idx*self.freg_size), self.sp)
         idx = idx + 1

   @abstractmethod
   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      pass

   @abstractmethod
   def _XregOp(self, content_list: list, rs2: ABIreg, off: int, rs1: ABIreg):
      pass

   @abstractmethod
   def _FregOp(self, content_list: list, fs2: FPABIreg, off: int, rs1: ABIreg):
      pass

@param_block
class BackupABI(MTG_ABI, AsmCode):
   def _XregOp(self, content_list: list, rs2: ABIreg, off: int, rs1: ABIreg):
      content_list.append(SD(rs2=rs2, off=off, rs1=self.sp))

   def _FregOp(self, content_list: list, fs2: FPABIreg, off: int, rs1: ABIreg):
      content_list.append(FSQ2(fs2=fs2, off=off, rs1=self.sp))

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(ADDI(rd=self.sp, rs1=self.sp, imm=Imm12(-1*len(self.xregs)*self.xreg_size)))
      self.int_regs(content_list)
      content_list.append(ADDI(rd=self.sp, rs1=self.sp, imm=Imm12(-1*len(self.fregs)*self.freg_size)))
      self.fp_regs(content_list)

@param_block
class RestoreABI(MTG_ABI, AsmCode):
   def _XregOp(self, content_list: list, rd: ABIreg, off: int, rs1: ABIreg):
      content_list.append(LD(rd=rd, off=off, rs1=self.sp))

   def _FregOp(self, content_list: list, fd: FPABIreg, off: int, rs1: ABIreg):
      content_list.append(FLQ2(fd=fd, off=off, rs1=self.sp))

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      self.fp_regs(content_list)
      content_list.append(ADDI(rd=self.sp, rs1=self.sp, imm=Imm12(len(self.fregs)*self.freg_size)))
      self.int_regs(content_list)
      content_list.append(ADDI(rd=self.sp, rs1=self.sp, imm=Imm12(len(self.xregs)*self.xreg_size)))

@param_block
class JumpToCFunct(Component, AsmCode):
   def __init__(self, funct_name: str, test_helper: TestHelper = None):
      self.ra = ABIreg('ra')
      self.funct_name = funct_name

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(BackupABI())
      content_list.append(f"la {{ra}}, {self.funct_name}")
      content_list.append("jalr {ra}, {ra}, 0")
      content_list.append(RestoreABI())
