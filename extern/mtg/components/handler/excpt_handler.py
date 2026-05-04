from core.mtg import *
from core.priv.priv import MRET, SRET, URET
from components.checking.gen_signature import BackupIntRF, RestoreIntRF, BackupFPRF, RestoreFPRF

@register
class MTrapVector(Component, AsmCode):

   label = "mtg_mtrap_vector"

   def __init__(self, test_helper: TestHelper = None):
      self.mexcp = ABIreg('mhandler')

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      insns = [  "j skip\n",
                 ".align 12\n",
                f"{self.label}:\n",
                 BackupIntRF(test_helper=test_helper, excl_regs=[]),
                 BackupFPRF(test_helper=test_helper),
                 "jalr {mexcp}, {mexcp}, 0\n",
                 RestoreFPRF(test_helper=test_helper),
                 RestoreIntRF(test_helper=test_helper),
                 MRET(),
                 "skip:"
              ]
      content_list += insns

@register
class STrapVector(Component, AsmCode):

   label = "mtg_strap_vector"

   def __init__(self, test_helper: TestHelper = None):
      self.sexcp = ABIreg('shandler')

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      insns = [  "j skip\n",
                 ".align 12\n",
                f"{self.label}:\n",
                 BackupIntRF(test_helper=test_helper, excl_regs=[]),
                 BackupFPRF(test_helper=test_helper),
                 "jalr {sexcp}, {sexcp}, 0\n",
                 RestoreFPRF(test_helper=test_helper),
                 RestoreIntRF(test_helper=test_helper),
                 SRET(),
                 "skip:"
              ]
      content_list += insns

@register
class UTrapVector(Component, AsmCode):

   label = "mtg_utrap_vector"

   def __init__(self, test_helper: TestHelper = None):
      self.uexcp = ABIreg('uhandler')

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      insns = [  "j skip\n",
                 ".align 12\n",
                f"{self.label}:\n",
                 BackupIntRF(test_helper=test_helper, excl_regs=[]),
                 BackupFPRF(test_helper=test_helper),
                 "jalr {uexcp}, {uexcp}, 0\n",
                 RestoreFPRF(test_helper=test_helper),
                 RestoreIntRF(test_helper=test_helper),
                 URET(),
                 "skip:"
              ]
      content_list += insns

TEST_CONF.STATIC_LABELS.append(MTrapVector.label)
TEST_CONF.STATIC_LABELS.append(STrapVector.label)
TEST_CONF.STATIC_LABELS.append(UTrapVector.label)

@register
class SetupMTrapVector(ComponentHandler):

   _backup_excp_ptr = False

   def __init__(self, test_helper: TestHelper = None):
      self.mtvec = Xreg()
      self.mtval = Xreg()
      self.mcause = Xreg()
      self.mepc = Xreg()

   @machine_code
   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      insns = [f"la {{mtvec}}, {MTrapVector.label}\n",
                "csrw mtvec, {mtvec}\n"
              ]
      content_list += insns

   def _mhandler(self, content_list: list, test_helper: TestHelper = None) -> None:
      insns = [  "csrr {mtval}, mtval\n",
                 "csrr {mcause}, mcause\n",
                 "csrr {mepc}, mepc\n",
                 "addi {mepc}, {mepc}, 4\n",
                 TEST_FAIL(test_helper=test_helper)
              ]
      content_list += insns

   def _shandler(self, content_list: list, test_helper: TestHelper = None) -> None:
      pass

@register
class SetupSTrapVector(ComponentHandler):

   _backup_excp_ptr = False

   def __init__(self, test_helper: TestHelper = None):
      self.stvec = Xreg()
      self.stval = Xreg()
      self.scause = Xreg()
      self.sepc = Xreg()

   @supervisor_code
   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      insns = [f"la {{stvec}}, {STrapVector.label}\n",
                "csrw stvec, {stvec}\n"
              ]
      content_list += insns

   def _mhandler(self, content_list: list, test_helper: TestHelper = None) -> None:
      pass

   def _shandler(self, content_list: list, test_helper: TestHelper = None) -> None:
      insns = [  "csrr {stval}, stval\n",
                 "csrr {scause}, scause\n",
                 "csrr {sepc}, sepc\n",
                 "addi {sepc}, {sepc}, 4\n",
                 TEST_FAIL(test_helper=test_helper)
              ]
      content_list += insns

@register
class SetupUTrapVector(ComponentHandler):

   _backup_excp_ptr = False

   def __init__(self, test_helper: TestHelper = None):
      self.utvec = Xreg()
      self.utval = Xreg()
      self.ucause = Xreg()
      self.uepc = Xreg()

   @user_code
   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      insns = [f"la {{utvec}}, {UTrapVector.label}\n",
                "csrw utvec, {utvec}\n"
              ]
      content_list += insns

   def _mhandler(self, content_list: list, test_helper: TestHelper = None) -> None:
      pass

   def _shandler(self, content_list: list, test_helper: TestHelper = None) -> None:
      pass

   def _uhandler(self, content_list: list, test_helper: TestHelper = None) -> None:
      insns = [  "csrr {utval}, utval\n",
                 "csrr {ucause}, ucause\n",
                 "csrr {uepc}, uepc\n",
                 "addi {uepc}, {uepc}, 4\n",
                 TEST_FAIL(test_helper=test_helper)
              ]
      content_list += insns
