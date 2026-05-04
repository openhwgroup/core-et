
from core.mtg import *
from components.flb_fcc.fcc import FCC, SendFCC
from components.randinst.rv32i import LI

@register
class FCCSyncThreads(Component, AsmCode):

   _threads_mask = None

   def get_fcc_addr(self, shire_id: int, thread_id: int, counter_num: int):

      assert (counter_num >= 0) and (counter_num <= 1)
      assert (thread_id   >= 0) and (thread_id   <= 1)

      # Use ESR classes for FCC addresses (supports both ET-SoC-1 and Erbium)
      # ESR_FCC_CREDINC_0: thread 0, counter 0
      # ESR_FCC_CREDINC_1: thread 0, counter 1
      # ESR_FCC_CREDINC_2: thread 1, counter 0
      # ESR_FCC_CREDINC_3: thread 1, counter 1
      if thread_id == 0:
         if counter_num == 0:
            esr_addr = ESR_FCC_CREDINC_0.paddr
         else:
            esr_addr = ESR_FCC_CREDINC_1.paddr
      else:
         if counter_num == 0:
            esr_addr = ESR_FCC_CREDINC_2.paddr
         else:
            esr_addr = ESR_FCC_CREDINC_3.paddr

      # For Erbium (single shire), no offset needed
      if TEST_CONF.TARGET == 'erbium':
         return esr_addr

      if (shire_id == -1):
         incr = 0x3fc00000 # local shire
      else:
         incr = shire_id * 0x400000

      return esr_addr + incr

   def get_fcc_mask(self, shire_id: int, thread_id: int):
      if (1 << thread_id) & self._threads_mask == 0:
         return 0
      mask = TEST_CONF.MINION_MASK
      assert (0xffffffff00000000 & mask) == 0
      return mask

   def __init__(self, test_helper: TestHelper = None):
      if self._threads_mask == None:
         self._threads_mask = TEST_CONF.THREAD_MASK
      self.active_sids = get_active_shire_ids()
      self.fcc_cntr = test_helper.get_rand_range(0,1)

      self.ax1 = Xreg()
      self.ax2 = Xreg()
      self.ax3 = Xreg()
      self.zero = ABIreg('zero')

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:

      for sid in self.active_sids:
         #                                shire_id, thread_id,   counter_num
         fcc_esr_addr = self.get_fcc_addr(     sid,         0, self.fcc_cntr)

         # Get active minion's thread mask
         even_mask = self.get_fcc_mask(sid, 0)
         odd_mask  = self.get_fcc_mask(sid, 1)

         content_list.append(LI(rd=self.ax1, imm=fcc_esr_addr))

         if even_mask != 0:
            content_list.append(LI(rd=self.ax2, imm=even_mask))
            content_list.append(SendFCC(rs2=self.ax2, off=Imm12( 0),  rs1=self.ax1))
         if odd_mask != 0:
            content_list.append(LI(rd=self.ax3, imm=odd_mask))
            content_list.append(SendFCC(rs2=self.ax3, off=Imm12(16),  rs1=self.ax1))

      for _ in range(TEST_CONF.NUM_HARTS):
         content_list.append(FCC(rd=self.zero, zimm5=self.fcc_cntr))

if TEST_CONF.agnostic_mode() == True:
   TEST_CONF.EXCL_COMPS.add(FCCSyncThreads)
