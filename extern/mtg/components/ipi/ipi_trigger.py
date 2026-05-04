
from core.mtg import *
from components.flb_fcc.flb_fcc_sync_threads import FLBFCCSyncShireThreads
from components.randinst.rv32i import LI, OR, SLL, AND, XORI
from components.randinst.rv32m import REMU
from components.randinst.rv64i import SD
from components.read_hartid import ReadHartID
from components.preamble.enable_sw_interrupts import EnableSWInterrupts
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
class IPITrigger(FLBFCCSyncShireThreads, ComponentHandler):

   @machine_code
   def _lead_hart_code(self, content_list: list, test_helper: TestHelper = None) -> None:
      self.local_ipi_trigger = StaticGMalloc('IPITrigger_local_ipi_trigger',  ESR_LOCAL_IPI_TRIGGER, 8, align=8)
      self.ipi_rx0 = UXreg()
      self.ipi_rx1 = UXreg()
      self.ipi_rx2 = UXreg()

      self.local_hart_id = UXreg()
      self.my_mask = UXreg()
      self.ipi_trigger_mask = UXreg()

      for _ in range(250):
         content_list.append(NOP())

      content_list += [
         # Get my local HART id
         ReadHartID(test_helper=test_helper, rd=self.local_hart_id),
         LI(rd=self.ipi_rx1, imm=64),
         REMU(rd=self.local_hart_id, rs1=self.local_hart_id, rs2=self.ipi_rx1),

         # Gen the IPI trigger mask
         LI(rd=self.ipi_rx1, imm=1),
         SLL(rd=self.my_mask, rs1=self.ipi_rx1, rs2=self.local_hart_id),
         XORI(rd=self.my_mask, rs1=self.my_mask, imm=-1),
         LI(rd=self.ipi_rx2, imm=_shire_harts_mask),
         AND(rd=self.ipi_trigger_mask, rs1=self.ipi_rx2, rs2=self.my_mask),

         # Write to the IPI Trigger ESR
         LI(rd=self.ipi_rx0, imm=self.local_ipi_trigger),
         SD(rs2=self.ipi_trigger_mask, off=0, rs1=self.ipi_rx0)
      ]

   def _follower_hart_code(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(EnableSWInterrupts(test_helper=test_helper))
      wait_insns = ["wfi", "csrw stall, x0", "wait_ipi:\nj wait_ipi\n"]
      insns = test_helper.pick_one(wait_insns)
      content_list.append(insns)
      content_list.append("ipi_trigger_pass:")

   def ipi_trigger_clear(self, content_list: list, test_helper: TestHelper = None) -> None:
      self.local_ipi_trigger_clear = StaticGMalloc('IPITrigger_local_ipi_trigger_clear',  ESR_LOCAL_IPI_TRIGGER_CLEAR, 8, align=8)
      content_list += [
         # Get my local HART id
         ReadHartID(test_helper=test_helper, rd=self.local_hart_id),
         LI(rd=self.ipi_rx1, imm=64),
         REMU(rd=self.local_hart_id, rs1=self.local_hart_id, rs2=self.ipi_rx1),

         # Gen the IPI clear mask
         LI(rd=self.ipi_rx1, imm=1),
         SLL(rd=self.my_mask, rs1=self.ipi_rx1, rs2=self.local_hart_id),
      
         # Write to the IPI Trigger Clear ESR
         LI(rd=self.ipi_rx0, imm=self.local_ipi_trigger_clear),
         SD(rs2=self.my_mask, off=0, rs1=self.ipi_rx0)
      ]

   @set_mepc_to("ipi_trigger_pass")
   def _mhandler(self, content_list: list, test_helper: TestHelper = None) -> None:
      self.ipi_trigger_clear(content_list, test_helper)
   @set_sepc_to("ipi_trigger_pass")
   def _shandler(self, content_list: list, test_helper: TestHelper = None) -> None:
      self.ipi_trigger_clear(content_list, test_helper)

if TEST_CONF.MINION_AGNOSTIC == 1 or TEST_CONF.NEIGH_AGNOSTIC == 1:
   TEST_CONF.EXCL_COMPS.add(IPITrigger)
else:
   from components.random_csr_read import RandomCSRRead
   TEST_CONF.EXCL_COMPS.add(RandomCSRRead)
