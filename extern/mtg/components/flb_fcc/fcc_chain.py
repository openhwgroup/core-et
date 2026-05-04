
from core.mtg import *
from components.flb_fcc.fcc import FCC, SendFCC
from components.randinst.rv32i import LI, SRLI, SLL, ANDI, SLLI, OR, FENCE
from components.randinst.rv32m import MUL, REMU
from components.read_hartid import ReadHartID

_fcc_cntr = 0

@register
class WaitForFccRelay(Component, AsmCode):

   fcc_cntr = _fcc_cntr

   def __init__(self, test_helper: TestHelper = None):
      self.zero = ABIreg('zero')
      self.fcc_addr_t0 = Xreg()
      self.fcc_addr_t1 = Xreg()
      self.fcc_mask_t0 = Xreg()
      self.fcc_mask_t1 = Xreg()
      self.next_phid = Xreg()
      self.rx0 = Xreg()
      self.rx1 = Xreg()
      self.rx2 = Xreg()

   def _get_fcc_addr(self, content_list: list, test_helper: TestHelper = None) -> None:
      # Use ESR classes for FCC addresses (supports both ET-SoC-1 and Erbium)
      if self.fcc_cntr == 0:
         esr_addr_t0 = ESR_FCC_CREDINC_0.paddr
         esr_addr_t1 = ESR_FCC_CREDINC_2.paddr
      else:
         esr_addr_t0 = ESR_FCC_CREDINC_1.paddr
         esr_addr_t1 = ESR_FCC_CREDINC_3.paddr

      if TEST_CONF.THREAD_MASK & 0x1:
         content_list += [# Set the base FCC addr
                          LI(rd=self.fcc_addr_t0, imm=esr_addr_t0),
                         ]
         # For ET-SoC-1, add shire ID offset; for Erbium, no offset needed
         if TEST_CONF.TARGET != 'erbium':
            content_list += [
                          # Set the proper Shire ID in the FCC addr
                          "set_fcc_shire_t0:",
                          SRLI(rd=self.rx2, rs1=self.next_phid, imm=6),
                          SLLI(rd=self.rx2, rs1=self.rx2, imm=22),
                          OR(rd=self.fcc_addr_t0, rs1=self.fcc_addr_t0, rs2=self.rx2)
                         ]

      if TEST_CONF.THREAD_MASK & 0x2:
         content_list += [# Set the base FCC addr
                          LI(rd=self.fcc_addr_t1, imm=esr_addr_t1),
                         ]
         # For ET-SoC-1, add shire ID offset; for Erbium, no offset needed
         if TEST_CONF.TARGET != 'erbium':
            content_list += [
                          # Set the proper Shire ID in the FCC addr
                          "set_fcc_shire_t1:",
                          SRLI(rd=self.rx2, rs1=self.next_phid, imm=6),
                          SLLI(rd=self.rx2, rs1=self.rx2, imm=22),
                          OR(rd=self.fcc_addr_t1, rs1=self.fcc_addr_t1, rs2=self.rx2)
                         ]

   def _get_fcc_mask(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list += [
         LI(rd=self.rx0, imm=64),
         REMU(rd=self.rx0, rs1=self.next_phid, rs2=self.rx0),
         SRLI(rd=self.rx0, rs1=self.rx0, imm=1),
         LI(rd=self.rx1, imm=int(TEST_CONF.MINION_MASK)),
      ]
      if TEST_CONF.THREAD_MASK & 0x1:
         content_list.append(SLL(rd=self.fcc_mask_t0, rs1=self.rx1, rs2=self.rx0))
      if TEST_CONF.THREAD_MASK & 0x2:
         content_list.append(SLL(rd=self.fcc_mask_t1, rs1=self.rx1, rs2=self.rx0))

   @only_phid(TEST_CONF.SEQ_BASE_HART)
   def global_lead_hart_code(self, content_list: list, test_helper: TestHelper = None) -> None:
      # I am the HART that starts the Computation
      content_list.append("j wake_thread")

   @only_mod_lhid(TEST_CONF.NUM_HARTS, 0)
   def minion_lead_hart_code(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(FCC(rd=self.zero, zimm5=self.fcc_cntr))
      # Send FCC to thread 1
      content_list.append('wake_thread:')
      content_list += [ ReadHartID(test_helper=test_helper, rd=self.next_phid),
                        ADDI(rd=self.next_phid, rs1=self.next_phid, imm=1) ]

      self._get_fcc_mask(content_list, test_helper=test_helper)
      self._get_fcc_addr(content_list, test_helper=test_helper)
      content_list.append(SendFCC(rs2=self.fcc_mask_t1, off=Imm12(0),  rs1=self.fcc_addr_t1))
      content_list.append(SendFCC(rs2=self.fcc_mask_t0, off=Imm12(0),  rs1=self.fcc_addr_t0))

   def follower_hart_code(self, content_list: list, test_helper: TestHelper = None) -> None:
      # Wait for an FCC credit
      content_list.append(FCC(rd=self.zero, zimm5=self.fcc_cntr))

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      self.global_lead_hart_code(content_list, test_helper=test_helper)
      self.minion_lead_hart_code(content_list, test_helper=test_helper)
      self.follower_hart_code(content_list, test_helper=test_helper)
      content_list.append("done:")


@register
class PassFccRelay(Component, AsmCode):

   fcc_cntr = _fcc_cntr
   def __init__(self, test_helper: TestHelper = None):
      self.zero = ABIreg('zero')
      self.next_phid = Xreg()

      self.rx0 = Xreg()
      self.rx1 = Xreg()
      self.rx2 = Xreg()
      self.fcc_addr = Xreg()
      self.fcc_mask = Xreg()

      self.max_shires = Xreg()
      self.shire_id = Xreg()
      self.shire_mask = Xreg()

      self.max_minions = Xreg()
      self.minion_id = Xreg()
      self.minion_mask = Xreg()

   def _get_fcc_addr(self, content_list: list, test_helper: TestHelper = None) -> None:
      # Use ESR classes for FCC addresses (supports both ET-SoC-1 and Erbium)
      if (TEST_CONF.SEQ_BASE_HART%2) == 0:
         if self.fcc_cntr == 0:
            esr_addr = ESR_FCC_CREDINC_0.paddr
         else:
            esr_addr = ESR_FCC_CREDINC_1.paddr
      else:
         if self.fcc_cntr == 0:
            esr_addr = ESR_FCC_CREDINC_2.paddr
         else:
            esr_addr = ESR_FCC_CREDINC_3.paddr

      content_list += [# Set the base FCC addr
                       LI(rd=self.fcc_addr, imm=esr_addr),
                      ]
      # For ET-SoC-1, add shire ID offset; for Erbium, no offset needed
      if TEST_CONF.TARGET != 'erbium':
         content_list += [
                       # Set the proper Shire ID in the FCC addr
                       "set_fcc_shire:",
                       SRLI(rd=self.rx1, rs1=self.next_phid, imm=6),
                       SLLI(rd=self.rx1, rs1=self.rx1, imm=22),
                       OR(rd=self.fcc_addr, rs1=self.fcc_addr, rs2=self.rx1)
                      ]

   def _get_fcc_mask(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list += [
         LI(rd=self.rx0, imm=64),
         REMU(rd=self.rx0, rs1=self.next_phid, rs2=self.rx0),
         SRLI(rd=self.rx0, rs1=self.rx0, imm=1),
         LI(rd=self.rx1, imm=1),
         SLL(rd=self.fcc_mask, rs1=self.rx1, rs2=self.rx0)
      ]

   @only_mod_lhid(TEST_CONF.NUM_HARTS, 0)
   def _pass_relay(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list += [ReadHartID(test_helper=test_helper, rd=self.rx0),
                       LI(rd=self.next_phid, imm=0),

                       LI(rd=self.max_shires, imm=34),
                       "add {shire_id}, x0, x0",
                       "shire_loop:",
                       "bge {shire_id}, {max_shires}, end_shire_loop",

                          LI(rd=self.max_minions, imm=32),
                          "add {minion_id}, x0, x0",
                          "minion_loop:",
                          "bge {minion_id}, {max_minions}, end_minion_loop",

                            "bge {rx0}, {next_phid}, skip",

                            "li {shire_mask}, 1",
                            "sll {shire_mask}, {shire_mask}, {shire_id}",
                           f"li {{rx1}}, {TEST_CONF.SEQ_SHIRE_MASK}",
                            "and {rx1}, {shire_mask}, {rx1}",
                            "beq {rx1}, x0, skip",

                            "li {minion_mask}, 1",
                            "sll {minion_mask}, {minion_mask}, {minion_id}",
                           f"li {{rx1}}, {TEST_CONF.SEQ_MINION_MASK}",
                            "and {rx1}, {minion_mask}, {rx1}",
                            "beq {rx1}, x0, skip",

                            "j found",

                            "skip:",
                           f"addi {{next_phid}}, {{next_phid}}, {TEST_CONF.NUM_HARTS}",

                         f"addi {{minion_id}}, {{minion_id}}, {TEST_CONF.NUM_HARTS >> 1}",
                          "j minion_loop",
                          "end_minion_loop:",

                       "addi {shire_id}, {shire_id}, 1",
                       "j shire_loop",
                       "end_shire_loop:",
                       "j last",
                       "found:"
                       "/*{rx0}*/",
                       "/*{rx1}*/",
                       "/*next_phid*/",
                       "/*{max_shires}*/",
                       "/*{shire_id}*/",
                       "/*{shire_mask}*/",
                       "/*{max_minions}*/",
                       "/*{minion_id}*/",
                       "/*{minion_mask}*/"
                      ]

      self._get_fcc_addr(content_list, test_helper = test_helper)
      self._get_fcc_mask(content_list, test_helper = test_helper)
      content_list.append(SendFCC(rs2=self.fcc_mask, off=Imm12(0),  rs1=self.fcc_addr))
      content_list.append("last:")

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      self._pass_relay(content_list, test_helper=test_helper)
      content_list.append(FENCE())

if TEST_CONF.SEQUENTIAL_MODE == 0x0:
   TEST_CONF.EXCL_COMPS.add(WaitForFccRelay)
   TEST_CONF.EXCL_COMPS.add(PassFccRelay)
