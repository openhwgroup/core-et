
from core.mtg import *
from core.models.model import *
from components.tensor_wait import *
from components.randinst.rv32i import ADDI, FENCE, LI, SLTI
from components.randinst.rv64i import LD, SD
from components.randinst.mask import MOVA_X_M, MOVA_M_X
from components.common import NOP
from components.mtg_load_addr import MTG_LOAD_ADDR
from components.tensor.tensor_mask import CheckTensorMask
from components.tensor_wait import WaitAllTensors
from components.encoding.enc import SPreg
from components.preamble.set_stack_pointer import SetStackPointer

@register
class BackupCSRList(Component, AsmCode):
   csrs = list()

   def __init__(self, test_helper: TestHelper = None):
      self.sp = ABIreg('sp')
      self.zero = ABIreg('zero')
      self.rx0 = Xreg()
      self.csrs_size = 8*len(self.csrs)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(ADDI(rd=self.sp, rs1=self.sp, imm=Imm12(-1*self.csrs_size)))
      for i in range(len(self.csrs)):
         content_list.append(f"csrrs {{rx0}}, {self.csrs[i]}, {{zero}}")
         content_list.append(SD(rs2=self.rx0, off=Imm12(i*8), rs1=self.sp))

@register
class RestoreCSRList(Component, AsmCode):
   csrs = list()

   def __init__(self, test_helper: TestHelper = None):
      self.sp = ABIreg('sp')
      self.zero = ABIreg('zero')
      self.rx0 = Xreg()
      self.csrs_size = 8*len(self.csrs)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      for i in range(len(self.csrs)):
         content_list.append(LD(rd=self.rx0, off=Imm12(i*8), rs1=self.sp))
         content_list.append(f"csrrw {{zero}}, {self.csrs[i]}, {{rx0}}")
      content_list.append(ADDI(rd=self.sp, rs1=self.sp, imm=Imm12(self.csrs_size)))

@register
class WaitLongLatencyOps(Component, AsmCode):

   def __init__(self, test_helper: TestHelper = None):
      self.zero = ABIreg('zero')

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(FENCE())
      content_list.append(WaitAllTensors())

@param_block
class BackupRX(Component, AsmCode):
   def __init__(self, test_helper: TestHelper = None):
      self.xreg_size = 8
      self.sp = ABIreg('sp')

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      excl_regs = [5, 10]
      for i in excl_regs:
         content_list.append(f"sd x{i}, {i*self.xreg_size-256}({{sp}})")

@register
class BackupIntRF(Component, AsmCode):
   def __init__(self, test_helper: TestHelper = None, excl_regs = [5, 10]):
      self.sp = ABIreg('sp')
      self.xreg_size = 8
      self.rf_size = IntModel.RF_SIZE * self.xreg_size
      self.excl_regs = excl_regs

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(ADDI(rd=self.sp, rs1=self.sp, imm=Imm12(-1*self.rf_size)))
      for i in range(IntModel.RF_SIZE):
         if i not in self.excl_regs:
            content_list.append(f"sd x{i}, {i*self.xreg_size}({{sp}})")

@register
class RestoreIntRF(Component, AsmCode):
   def __init__(self, test_helper: TestHelper = None):
      self.sp = ABIreg('sp')
      self.xreg_size = 8
      self.rf_size = IntModel.RF_SIZE * self.xreg_size

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      excl_regs = []
      if TEST_CONF.agnostic_mode() == True:
         excl_regs.append(2)
      for i in range(IntModel.RF_SIZE):
         if i not in excl_regs:
            content_list.append(f"ld x{i}, {i*self.xreg_size}({{sp}})")
      content_list.append(ADDI(rd=self.sp, rs1=self.sp, imm=Imm12(self.rf_size)))

@register
class BackupFPRF(Component, AsmCode):
   def __init__(self, test_helper: TestHelper = None):
      self.sp = ABIreg('sp')
      self.freg_size = FPModel.NUM_LANES * FPModel.LANE_SIZE
      self.rf_size = FPModel.RF_SIZE * self.freg_size

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(ADDI(rd=self.sp, rs1=self.sp, imm=Imm12(-1*self.rf_size)))
      for i in range(FPModel.RF_SIZE):
         content_list.append(f"fsq2 f{i}, {i*self.freg_size}({{sp}})")

@register
class RestoreFPRF(Component, AsmCode):
   def __init__(self, test_helper: TestHelper = None):
      self.sp = ABIreg('sp')
      self.freg_size = FPModel.NUM_LANES * FPModel.LANE_SIZE
      self.rf_size = FPModel.RF_SIZE * self.freg_size

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      for i in range(FPModel.RF_SIZE):
         content_list.append(f"flq2 f{i}, {i*self.freg_size}({{sp}})")
         fv260_a_wa(content_list, idx = i)
      content_list.append(ADDI(rd=self.sp, rs1=self.sp, imm=Imm12(self.rf_size)))

@register
class BackupTensorCSR(BackupCSRList, AsmCode):
   csrs = ['tensor_error', 'tensor_mask']

@register
class RestoreTensorCSR(RestoreCSRList, AsmCode):
   csrs = ['tensor_error', 'tensor_mask']

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      if TEST_CONF.RTLMIN_WORKAROUNDS['rtlmin-6136'] == True:
         content_list.append("csrrs x0, tensor_mask, x0")
      super()._run(content_list, test_helper=test_helper)

@mtg_dataclass
class BackupMaskRF(Component, AsmCode):

   m0:  Mreg = field(default_factory=M0reg)
   m1:  Mreg = field(default_factory=M1reg)
   m2:  Mreg = field(default_factory=M2reg)
   m3:  Mreg = field(default_factory=M3reg)
   m4:  Mreg = field(default_factory=M4reg)
   m5:  Mreg = field(default_factory=M5reg)
   m6:  Mreg = field(default_factory=M6reg)
   m7:  Mreg = field(default_factory=M7reg)

   sp: ABIreg = field(default_factory=SPreg)
   rx0: Xreg = field(default_factory=Xreg)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(ADDI(rd=self.sp, rs1=self.sp, imm=Imm12(-8)))
      content_list.append(MOVA_X_M(rd=self.rx0, m0=self.m0, m1=self.m1, m2=self.m2, m3=self.m3, m4=self.m4, m5=self.m5, m6=self.m6, m7=self.m7))
      content_list.append(SD(rs2=self.rx0, off=Imm12(0), rs1=self.sp))

@mtg_dataclass
class RestoreMaskRF(Component, AsmCode):

   m0:  Mreg = field(default_factory=M0reg)
   m1:  Mreg = field(default_factory=M1reg)
   m2:  Mreg = field(default_factory=M2reg)
   m3:  Mreg = field(default_factory=M3reg)
   m4:  Mreg = field(default_factory=M4reg)
   m5:  Mreg = field(default_factory=M5reg)
   m6:  Mreg = field(default_factory=M6reg)
   m7:  Mreg = field(default_factory=M7reg)

   sp: ABIreg = field(default_factory=SPreg)
   rx0: Xreg = field(default_factory=Xreg)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(LD(rd=self.rx0, off=Imm12(0), rs1=self.sp))
      content_list.append(MOVA_M_X(rs1=self.rx0, m0=self.m0, m1=self.m1, m2=self.m2, m3=self.m3, m4=self.m4, m5=self.m5, m6=self.m6, m7=self.m7))
      content_list.append(ADDI(rd=self.sp, rs1=self.sp, imm=Imm12(8)))

@register
class BackupSIMDCSR(BackupCSRList, AsmCode):
   csrs = ['gsc_progress', 'fcsr', 'fflags']

@register
class RestoreSIMDCSR(RestoreCSRList, AsmCode):
   csrs = ['gsc_progress', 'fcsr', 'fflags']

@register
class BackupVPUTenc(Component, AsmCode):
   # TODO: Only if the L1SCP is enabled
   def __init__(self, test_helper: TestHelper = None):
      self.sp = ABIreg('sp')
      self.zero = ABIreg('zero')
      self.rx0 = Xreg()
      self.skip_label = 'skip_tenc_bak'

   @only_thread(0)
   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      if TEST_CONF.RTLMIN_WORKAROUNDS['rtlmin-6136'] == True:
         content_list.append("csrrs x0, tensor_mask, x0")
      content_list.append("csrrw {zero}, tensor_mask, {zero}")

      # Uses tensor_mask. Write result to the VPU RF
      # Config is TensorIMA8A32. Max a_rows and b_cols value
      content_list.append(LI(rd=self.rx0, imm=Zimm64(0x81f8000000800006)))
      content_list.append("csrrw {zero}, tensor_fma, {rx0}")
      content_list.append(WAIT_TENSOR_FMA())
      content_list.append(BackupFPRF())

@register
class RestoreVPUTenc(Component, AsmCode):
   # TODO: Only if the L1SCP is enabled
   def __init__(self, test_helper: TestHelper = None):
      self.sp = ABIreg('sp')
      self.skip_label = 'skip_tenc_rest'

   @only_thread(0)
   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      # BackupFPRF() increases the SP by 1024
      # we will not restore the data but we want to
      # restore the SP position
      content_list.append(ADDI(rd=self.sp, rs1=self.sp, imm=Imm12(1024)))

@register
class GenSignature(Component, AsmCode):

   def __init__(self, test_helper: TestHelper = None, crc16_mem: StaticMalloc = None, info_mem: StaticMalloc = None, check = False):
      # CRC16 -> 16 bits (2 Bytes)
      if crc16_mem:
         self.crc16_mem = crc16_mem
      else:
         self.crc16_mem = CRC16Malloc(CRC16_REGION, 2, align=2, align_cl=False)

      if info_mem:
         self.info_mem = info_mem
      else:
         self.info_mem = GMalloc('GenSignature_info_mem', DRAM_REGION, 16, align=64)

      self.a1  = Xreg(11) # a1 (aka x11) is used to pass arguments to C functions
      self.rx0 = Xreg(10) # a0 (aka x10) is used to pass arguments to C functions
      self.rx1 = Xreg(5)
      self.rx2 = Xreg(6)
      self.ra = ABIreg('ra')
      self.sp = ABIreg('sp')
      self.zero = ABIreg('zero')
      self.check = check

      self.m0 = Mreg(0)
      self.m1 = Mreg(1)
      self.m2 = Mreg(2)
      self.m3 = Mreg(3)
      self.m4 = Mreg(4)
      self.m5 = Mreg(5)
      self.m6 = Mreg(6)
      self.m7 = Mreg(7)

   def _add_self_check_code(self, content_list: list, insns_list: list) -> None:
      if self.check:
         content_list.append(NOP())
      else:
         content_list.append("j pass")
      content_list += insns_list

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(WaitLongLatencyOps())

      # Pre-backup the general purpose registers
      content_list.append(BackupRX())

      # Set the Checkpoint tail info
      content_list.append(MTG_LOAD_ADDR(rd=self.rx0, rx1=self.rx1, mem=self.info_mem))
      content_list.append(SD(rs2=self.sp, off=Imm12(8), rs1=self.rx0))

      # Create a snapshot of the current architectural state
      content_list.append(BackupIntRF())
      content_list.append(BackupFPRF())
      content_list.append(BackupTensorCSR())
      content_list.append(BackupMaskRF(m0=self.m0, m1=self.m1, m2=self.m2, m3=self.m3, m4=self.m4, m5=self.m5, m6=self.m6, m7=self.m7))
      content_list.append(BackupSIMDCSR())
      if BackupVPUTenc not in TEST_CONF.EXCL_COMPS:
         content_list.append(BackupVPUTenc())

      # Set the Checkpoint head info
      content_list.append(SD(rs2=self.sp, off=Imm12(0), rs1=self.rx0))

      # Jump to C code
      content_list.append(MTG_LOAD_ADDR(rd=self.a1, mem=self.crc16_mem))
      content_list.append("la {ra}, et_gen_crc16")
      content_list.append("jalr {ra}, {ra}, 0")

      # C function return value is set on x10
      # In this case, the CRC16 checksum is on x10

      # RV Hint
      content_list.append(SLTI(rd=self.zero, rs1=self.rx0, imm=Imm12(0x516)))
      content_list.append(MTG_LOAD_ADDR(rd=self.rx2, mem=self.crc16_mem))
      if self.check:
         content_list.append("lhu {rx1}, 0({rx2})")
      else:
         content_list.append("shg {rx0}, ({rx2})")

      self._do_check(content_list, test_helper)

      # Restore the previous architectural state from the snapshot
      if RestoreVPUTenc not in TEST_CONF.EXCL_COMPS:
         content_list.append(RestoreVPUTenc())
      content_list.append(RestoreSIMDCSR())
      content_list.append(RestoreMaskRF(m0=self.m0, m1=self.m1, m2=self.m2, m3=self.m3, m4=self.m4, m5=self.m5, m6=self.m6, m7=self.m7))
      content_list.append(RestoreTensorCSR())
      content_list.append(RestoreFPRF())
      content_list.append(RestoreIntRF())

      content_list.append(CheckTensorMask(rx0 = self.rx0, rx1 = self.rx1))

   def _add_debug_info(self, content_list: list):
      self.dbg_reg0 = Xreg(31)
      self.dbg_reg1 = Xreg()
      content_list.append(MTG_LOAD_ADDR(rd=self.dbg_reg0, rx1=self.dbg_reg1, mem=self.info_mem))
      content_list.append("ld x13, 0({dbg_reg0})")
      content_list.append("ld x14, 8({dbg_reg0})")
      TEST_FAIL.add_debug_info(content_list)

   def _do_check(self, content_list: list, test_helper: TestHelper = None) -> None:
      chk_insns = ["beq {rx0}, {rx1}, pass",
                   "fence"]
      if TEST_CONF.POST_SILICON == 0:
         self._add_debug_info(chk_insns)
         chk_insns += ["lui x31, 0x50BAD",
                       "csrw validation0, x31"]
      else:
         self._add_debug_info(chk_insns)
         chk_insns += ["lui x31, 0x50BAD",
                       "csrwi validation2, 1"]
         chk_insns.append("call postsi_ust_test_end")
      chk_insns += ["wfi",
                    "pass:"]
      self._add_self_check_code(content_list, chk_insns)
