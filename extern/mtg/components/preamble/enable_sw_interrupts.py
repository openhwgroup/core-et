
from core.mtg import *
from components.randinst.rv32i import LI, OR
from core.priv.priv import BackupXreg, RestoreXreg

@register
class EnableSWInterrupts(Component, AsmCode):

   def __init__(self, test_helper: TestHelper = None):
      self.enc = 0x08

      self.rx0 = Xreg()
      self.rx1 = Xreg()
      self.rx2 = Xreg()

   @machine_code
   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      insns = [
         BackupXreg(xreg=self.rx0),
         BackupXreg(xreg=self.rx1),
         BackupXreg(xreg=self.rx2),
         LI(rd=self.rx0, imm=self.enc),
         "csrr {rx1}, mie\n",
         OR(rd=self.rx2, rs1=self.rx1, rs2=self.rx0),
         "csrw mie, {rx2}\n",
         "csrr {rx2}, mstatus\n",
         OR(rd=self.rx2, rs1=self.rx2, rs2=self.rx0),
         "csrw mstatus, {rx2}\n",
         RestoreXreg(xreg=self.rx2),
         RestoreXreg(xreg=self.rx1),
         RestoreXreg(xreg=self.rx0)
      ]
      content_list += insns
