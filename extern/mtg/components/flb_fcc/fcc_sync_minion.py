
from core.mtg import *
from components.flb_fcc.fcc import FCC, SendFCC
from components.randinst.rv32i import LI, SRLI, SLL, ANDI
from components.randinst.rv32m import MUL
from components.read_hartid import ReadHartID

@register
class FCCSyncMinion(Component, AsmCode):

   expected_fcc_credits_per_hart = 2

   def __init__(self, test_helper: TestHelper = None):
      self.hid = Xreg()
      self.sid = Xreg()
      self.mid = Xreg()

      self.rx0 = Xreg()

      self.fcc_addr_t0 = Xreg()
      self.fcc_addr_t1 = Xreg()
      self.fcc_mask = Xreg()

      self.zero = ABIreg('zero')

      self.fcc_cntr = test_helper.get_rand_range(0,1)

   def _get_hid(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(ReadHartID(test_helper=test_helper, rd=self.hid))

   def _get_sid(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(SRLI(rd=self.sid, rs1=self.hid, imm=6))

   def _get_mid(self, content_list: list, test_helper: TestHelper = None) -> None:
      insns = [SRLI(rd=self.mid, rs1=self.hid, imm=1),
               ANDI(rd=self.mid, rs1=self.mid, imm=0x1f)]
      content_list += insns

   def _get_fcc_addr(self, content_list: list, test_helper: TestHelper = None) -> None:
      # Use ESR classes for FCC addresses (supports both ET-SoC-1 and Erbium)
      if self.fcc_cntr == 0:
         esr_addr_t0 = ESR_FCC_CREDINC_0.paddr
         esr_addr_t1 = ESR_FCC_CREDINC_2.paddr
      else:
         esr_addr_t0 = ESR_FCC_CREDINC_1.paddr
         esr_addr_t1 = ESR_FCC_CREDINC_3.paddr

      insns = [
         LI(rd=self.fcc_addr_t0, imm=esr_addr_t0),
         LI(rd=self.fcc_addr_t1, imm=esr_addr_t1),
      ]
      # For ET-SoC-1, add shire ID offset; for Erbium, no offset needed
      if TEST_CONF.TARGET != 'erbium':
         insns += [
            LI(rd=self.rx0, imm=0x400000),
            MUL(rd=self.rx0, rs1=self.rx0, rs2=self.sid),
            ADD(rd=self.fcc_addr_t0, rs1=self.fcc_addr_t0, rs2=self.rx0),
            ADD(rd=self.fcc_addr_t1, rs1=self.fcc_addr_t1, rs2=self.rx0)
         ]
      content_list += insns

   def _get_fcc_mask(self, content_list: list, test_helper: TestHelper = None) -> None:
      insns = [
         LI(rd=self.rx0, imm=1),
         SLL(rd=self.fcc_mask, rs1=self.rx0, rs2=self.mid)
      ]
      content_list += insns

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      if TEST_CONF.THREAD_MASK == 0x3:
         self._get_hid(content_list, test_helper = test_helper)
         self._get_sid(content_list, test_helper = test_helper)
         self._get_mid(content_list, test_helper = test_helper)

         self._get_fcc_addr(content_list, test_helper = test_helper)
         self._get_fcc_mask(content_list, test_helper = test_helper)

         # Send credit to TH0
         content_list.append(SendFCC(rs2=self.fcc_mask, off=Imm12(0),  rs1=self.fcc_addr_t0))
         # Send credit to TH1
         content_list.append(SendFCC(rs2=self.fcc_mask, off=Imm12(0),  rs1=self.fcc_addr_t1))

         for _ in range(self.expected_fcc_credits_per_hart):
            content_list.append(FCC(rd=self.zero, zimm5=self.fcc_cntr))

         content_list.append(ADD(rd=self.hid, rs1=self.zero, rs2=self.zero))
         content_list.append(ADD(rd=self.mid, rs1=self.zero, rs2=self.zero))
         content_list.append(ADD(rd=self.sid, rs1=self.zero, rs2=self.zero))
         content_list.append(ADD(rd=self.rx0, rs1=self.zero, rs2=self.zero))
         content_list.append(ADD(rd=self.fcc_addr_t0, rs1=self.zero, rs2=self.zero))
         content_list.append(ADD(rd=self.fcc_addr_t1, rs1=self.zero, rs2=self.zero))
         content_list.append(ADD(rd=self.fcc_mask, rs1=self.zero, rs2=self.zero))

@register
class FCCSyncNeigh(FCCSyncMinion, AsmCode):

   expected_fcc_credits_per_hart = 16

   def _get_nid(self, content_list: list, test_helper: TestHelper = None) -> None:
      self.nid = Xreg()
      insns = [SRLI(rd=self.nid, rs1=self.hid, imm=4),
               ANDI(rd=self.nid, rs1=self.nid, imm=0x3)]
      content_list += insns

   def _get_fcc_mask(self, content_list: list, test_helper: TestHelper = None) -> None:
      self.rx1 = Xreg()
      self._get_nid(content_list, test_helper=test_helper)
      insns = [
         LI(rd=self.rx1, imm=8),
         MUL(rd=self.rx1, rs1=self.nid, rs2=self.rx1),
         LI(rd=self.rx0, imm=0xff),
         SLL(rd=self.fcc_mask, rs1=self.rx0, rs2=self.rx1)
      ]
      content_list += insns

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      super()._run(content_list, test_helper=test_helper)
      if TEST_CONF.THREAD_MASK == 0x3:
         content_list.append(ADD(rd=self.nid, rs1=self.zero, rs2=self.zero))
         content_list.append(ADD(rd=self.rx1, rs1=self.zero, rs2=self.zero))

@register
class FCCSyncShire(FCCSyncMinion, AsmCode):

   expected_fcc_credits_per_hart = 64

   def _get_fcc_mask(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(LI(rd=self.fcc_mask, imm=0xffffffff))

# Exclude these components because they conflict with FLB FCC barriers or FCC Barriers where not all
# the active HARTs are participating
TEST_CONF.EXCL_COMPS.add(FCCSyncMinion)
TEST_CONF.EXCL_COMPS.add(FCCSyncNeigh)
TEST_CONF.EXCL_COMPS.add(FCCSyncShire)
