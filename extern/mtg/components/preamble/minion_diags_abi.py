
from core.mtg import *
from components.randinst.amo import AMOORG_D, AMOORL_D, AMOSWAPG_D, AMOSWAPL_D
from components.randinst.rv32i import LI

@register
class BackupMinionDiagsABI(Component, AsmCode):
   MEM_SIZE = 3*8

   def __init__(self, test_helper: TestHelper = None):
      self.zero   = ABIreg('zero')
      self.sp     = ABIreg('sp')
      self.gp     = ABIreg('gp', ignore_check=True)
      self.tp     = ABIreg('tp', ignore_check=True)
      self.bak_mem = GMalloc('bak_mem', SCRATCH_REGION, self.MEM_SIZE, align=self.MEM_SIZE)
      self.rx1 = Xreg()

   def _amo_write(self, rd: Xreg = None, rs2: Xreg = None, rs1: Xreg = None):
      if TEST_CONF.POSTSI_L2_ONLY == True:
         return AMOSWAPL_D(rd=rd, rs2=rs2, rs1=rs1)
      else:
         return AMOSWAPG_D(rd=rd, rs2=rs2, rs1=rs1)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list += [
         LI(rd=self.rx1, imm=self.bak_mem),

         # Backup SP
         self._amo_write(rd=self.zero, rs2=self.sp, rs1=self.rx1),

         # Backup GP
         ADDI(rd=self.rx1, rs1=self.rx1, imm=8),
         self._amo_write(rd=self.zero, rs2=self.gp, rs1=self.rx1),

         # Backup TP
         ADDI(rd=self.rx1, rs1=self.rx1, imm=8),
         self._amo_write(rd=self.zero, rs2=self.tp, rs1=self.rx1)
      ]


@base_class
class RestoreMinionDiags(Component, AsmCode, metaclass=ABCMeta):
   def __init__(self, test_helper: TestHelper = None):
      self.bak_mem = GMalloc('bak_mem', SCRATCH_REGION, BackupMinionDiagsABI.MEM_SIZE, align=BackupMinionDiagsABI.MEM_SIZE)
      self.zero   = ABIreg('zero')
      self.rx1 = Xreg()

   def _amo_read(self, rd: Xreg = None, rs2: Xreg = None, rs1: Xreg = None):
      if TEST_CONF.POSTSI_L2_ONLY == True:
         return AMOORL_D(rd=rd, rs2=rs2, rs1=rs1)
      else:
         return AMOORG_D(rd=rd, rs2=rs2, rs1=rs1)

   @abstractmethod
   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      pass

@register
class RestoreMinionDiagsSP(RestoreMinionDiags):
   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(LI(rd=self.rx1, imm=self.bak_mem))
      content_list.append(self._amo_read(rd=ABIreg('sp'), rs2=self.zero, rs1=self.rx1))

@register
class RestoreMinionDiagsGP(RestoreMinionDiags):
   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(LI(rd=self.rx1, imm=self.bak_mem))
      content_list.append(ADDI(rd=self.rx1, rs1=self.rx1, imm=8))
      content_list.append(self._amo_read(rd=ABIreg('gp', ignore_check=True), rs2=self.zero, rs1=self.rx1))

@register
class RestoreMinionDiagsTP(RestoreMinionDiags):
   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(LI(rd=self.rx1, imm=self.bak_mem))
      content_list.append(ADDI(rd=self.rx1, rs1=self.rx1, imm=16))
      content_list.append(self._amo_read(rd=ABIreg('tp', ignore_check=True), rs2=self.zero, rs1=self.rx1))
