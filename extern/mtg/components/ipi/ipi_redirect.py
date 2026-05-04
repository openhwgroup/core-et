
from core.mtg import *
from components.flb_fcc.flb_fcc_sync_threads import FLBFCCSyncShireThreads
from components.randinst.rv32i import LI, OR, SLL, AND, XORI, FENCE
from components.randinst.rv32m import REMU
from components.randinst.rv64i import SD
from components.read_hartid import ReadHartID
from components.common import NOP

assert TEST_CONF.THREAD_MASK <= 3

_shire_harts_mask = 0x0
_minion_mask = TEST_CONF.MINION_MASK
_idx = 0
while _minion_mask != 0:
   if _minion_mask & 0x1:
      _shire_harts_mask |= (TEST_CONF.THREAD_MASK << _idx)
   _idx += 2
   _minion_mask >>= 1

@register
class IPIRedirect(FLBFCCSyncShireThreads):

   def _set_ipi_redirect_pc(self, content_list: list, test_helper: TestHelper = None) -> None:
      self.local_ipi_redirect_pc = StaticGMalloc('IPIRedirect_local_ipi_redirect_pc',  ESR_LOCAL_IPI_REDIRECT_PC_BCAST, 8, align=8)
      self.ipi_rx3 = UXreg()
      self.ipi_rx4 = UXreg()
      content_list += [
          LI(rd=self.ipi_rx4, imm=self.local_ipi_redirect_pc),
          "la {ipi_rx3}, ipi_redirect_pass",
          SD(rs2=self.ipi_rx3, off=0, rs1=self.ipi_rx4),
          FENCE()
      ]

   def _lead_hart_code(self, content_list: list, test_helper: TestHelper = None) -> None:
      self.local_ipi_redirect = StaticGMalloc('IPIRedirect_local_ipi_redirect',  ESR_LOCAL_IPI_REDIRECT, 8, align=8)
      self.ipi_rx0 = UXreg()
      self.ipi_rx1 = UXreg()
      self.ipi_rx2 = UXreg()

      self.local_hart_id = UXreg()
      self.my_mask = UXreg()
      self.ipi_redirect_mask = UXreg()

      # Set the IPI Redirect PC
      self._set_ipi_redirect_pc(content_list, test_helper)

      for _ in range(200):
         content_list.append(NOP())

      # Trigger IPI Redirect
      content_list += [
         # Get my local HART id
         ReadHartID(test_helper=test_helper, rd=self.local_hart_id),
         LI(rd=self.ipi_rx1, imm=64),
         REMU(rd=self.local_hart_id, rs1=self.local_hart_id, rs2=self.ipi_rx1),

         # Gen the IPI Redirect mask
         LI(rd=self.ipi_rx1, imm=1),
         SLL(rd=self.my_mask, rs1=self.ipi_rx1, rs2=self.local_hart_id),
         XORI(rd=self.my_mask, rs1=self.my_mask, imm=-1),
         LI(rd=self.ipi_rx2, imm=_shire_harts_mask),
         AND(rd=self.ipi_redirect_mask, rs1=self.ipi_rx2, rs2=self.my_mask),

         # Write to the IPI Redirect ESR
         LI(rd=self.ipi_rx0, imm=self.local_ipi_redirect),
         SD(rs2=self.ipi_redirect_mask, off=0, rs1=self.ipi_rx0)
      ]


   @user_code
   def _follower_hart_code(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append("csrw stall, x0")
      content_list.append(TEST_FAIL())
      content_list.append("ipi_redirect_pass:")

if TEST_CONF.MINION_AGNOSTIC == 1 or TEST_CONF.NEIGH_AGNOSTIC == 1:
   TEST_CONF.EXCL_COMPS.add(IPIRedirect)
else:
   from components.random_csr_read import RandomCSRRead
   TEST_CONF.EXCL_COMPS.add(RandomCSRRead)
