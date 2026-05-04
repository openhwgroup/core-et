
from core.mtg import *
from components.flb_fcc.fcc import FCC, SendFCC
from components.randinst.rv32i import LI, FENCE, SLL, SRLI, ANDI
from components.randinst.rv32m import MUL
from components.et_csr import FLB0
from components.flb_fcc.fcc_sync_threads import FCCSyncThreads
from components.read_hartid import ReadHartID

@register
class FLBFCCSyncShireThreads(FCCSyncThreads, AsmCode):

   exclude = False
   odd_nums  = list(range( 0,  8))
   even_nums = list(range( 8, 16))
   all_nums  = list(range(16, 32))

   def get_barrier_num(self, test_helper) -> int:
      assert self._threads_mask > 0 and self._threads_mask <= 0x3
      if self._threads_mask == 0x1:
         return test_helper.pick_one(self.even_nums)
      elif self._threads_mask == 0x2:
         return test_helper.pick_one(self.odd_nums)
      else:
         return test_helper.pick_one(self.all_nums)

   def __init__(self, test_helper: TestHelper = None, threads_mask = TEST_CONF.THREAD_MASK):
      self.zero = ABIreg('zero')
      self.nid = UXreg()
      self.mid = UXreg()
      self.hid = UXreg()
      self.rx1 = UXreg()
      self.rx2 = UXreg()
      self.rx3 = UXreg()
      self.fcc_cntr = test_helper.get_rand_range(0,1)
      self._threads_mask = threads_mask

      barrier = self.get_barrier_num(test_helper)
      flb_match_val = get_num_harts_local_shire(skip_assert=True, thread_mask=threads_mask)
      self.flb_csr_val = self.get_flb_csr_value(test_helper, barrier, flb_match_val)

      self.even_mask = self.get_fcc_mask(None, 0)
      self.odd_mask  = self.get_fcc_mask(None, 1)
      #                                     shire_id, thread_id,   counter_num
      self.fcc_esr_addr = self.get_fcc_addr(      -1,         0, self.fcc_cntr)

      self.rn0 = test_helper.get_rand_num()
      self.rn1 = test_helper.get_rand_num()
      self.rn2 = test_helper.get_rand_num()
      self.rn3 = test_helper.get_rand_num()
      self.rn4 = test_helper.get_rand_num()
      self.rn5 = test_helper.get_rand_num()

   def _get_hid(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(ReadHartID(test_helper=test_helper, rd=self.hid))

   def _get_mid(self, content_list: list, test_helper: TestHelper = None) -> None:
      insns = [SRLI(rd=self.mid, rs1=self.hid, imm=1),
               ANDI(rd=self.mid, rs1=self.mid, imm=0x1f)]
      content_list += insns

   def _get_nid(self, content_list: list, test_helper: TestHelper = None) -> None:
      insns = [SRLI(rd=self.nid, rs1=self.hid, imm=4),
               ANDI(rd=self.nid, rs1=self.nid, imm=0x3)]
      content_list += insns

   def _set_fcc_mask(self, insns: list, test_helper: TestHelper = None) -> None:
      insns += [
         LI(rd=self.rx1, imm=self.fcc_esr_addr)
      ]
      if TEST_CONF.MINION_AGNOSTIC == 1:
         self._get_hid(insns, test_helper)
         self._get_mid(insns, test_helper)
         insns += [
            LI(rd=self.rx2, imm=1),
            SLL(rd=self.rx2, rs1=self.rx2, rs2=self.mid),
         ]
         if self.even_mask != 0:
            insns.append(SendFCC(rs2=self.rx2, off=Imm12( 0),  rs1=self.rx1))
         if self.odd_mask != 0:
            insns.append(SendFCC(rs2=self.rx2, off=Imm12(16),  rs1=self.rx1))
      elif TEST_CONF.NEIGH_AGNOSTIC == 1:
         self._get_hid(insns, test_helper)
         self._get_nid(insns, test_helper)
         insns += [
            LI(rd=self.rx3, imm=8),
            MUL(rd=self.rx3, rs1=self.nid, rs2=self.rx3),
            LI(rd=self.rx2, imm=0xff),
            SLL(rd=self.rx2, rs1=self.rx2, rs2=self.rx3),
         ]
         if self.even_mask != 0:
            insns.append(SendFCC(rs2=self.rx2, off=Imm12( 0),  rs1=self.rx1))
         if self.odd_mask != 0:
            insns.append(SendFCC(rs2=self.rx2, off=Imm12(16),  rs1=self.rx1))
      elif TEST_CONF.SHIRE_AGNOSTIC == 1:
         insns += [
            LI(rd=self.rx2, imm=0xffffffff),
         ]
         if self.even_mask != 0:
            insns.append(SendFCC(rs2=self.rx2, off=Imm12( 0),  rs1=self.rx1))
         if self.odd_mask != 0:
            insns.append(SendFCC(rs2=self.rx2, off=Imm12(16),  rs1=self.rx1))
      else:
         if self.even_mask != 0:
            insns.append(LI(rd=self.rx2, imm=self.even_mask))
            insns.append(SendFCC(rs2=self.rx2, off=Imm12( 0),  rs1=self.rx1))
         if self.odd_mask != 0:
            insns.append(LI(rd=self.rx2, imm=self.odd_mask))
            insns.append(SendFCC(rs2=self.rx2, off=Imm12(16),  rs1=self.rx1))

   def get_flb_csr_value(self, test_helper: TestHelper, barrier_num: int, match_val: int) -> int:
      rand = test_helper.get_rand_num()

      # Set the Barrier Num
      rand = set_bits(rand, 4, 0, barrier_num)

      # Set the Match Value
      rand = set_bits(rand, 12, 5, match_val - 1)

      return rand

   def _lead_hart_code(self, content_list: list, test_helper: TestHelper = None) -> None:
      pass

   def _follower_hart_code(self, content_list: list, test_helper: TestHelper = None) -> None:
      pass

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      insns = [
         LI(rd=self.rx1, imm=self.flb_csr_val),
         FLB0(rd=self.rx1, rs1=self.rx1),
         "beq {zero}, {rx1}, follower_code"
      ]
      lead_code = ["lead_code:"]
      self._lead_hart_code(lead_code, test_helper)
      insns += lead_code

      self._set_fcc_mask(insns, test_helper)

      follower_code = ["follower_code:"]
      self._follower_hart_code(follower_code, test_helper)

      if len(follower_code) > 1:
         insns += [
            "j wait_for_fcc_credit"
         ]
      insns += follower_code
      insns += [
         "wait_for_fcc_credit:",
         FCC(rd=self.zero, zimm5=self.fcc_cntr),
      ]
      content_list += insns


from components.randinst.amo import AMOADDG_D, AMOADDL_D, AMOSWAPG_D, AMOSWAPL_D

@register
class AMOFLBFCCSyncThreads(FLBFCCSyncShireThreads, AsmCode):

   def __init__(self, test_helper: TestHelper = None):
      super().__init__.__wrapped__(self, test_helper=test_helper)

      self.active_sids = get_active_shire_ids()
      self.reg0 = UXreg()
      self.reg1 = UXreg()
      self.reg2 = UXreg()

      self.amo_lock = StaticGMalloc(f"{self.__class__.__name__}_amo_lock", ZERO_REGION, 8, align=8, align_cl=False)

   def _set_fcc_mask(self, content_list: list, test_helper: TestHelper = None) -> None:
      # This Component cannot be used in Agnostic modes
      assert TEST_CONF.agnostic_mode() == 0

      for sid in self.active_sids:
         #                                shire_id, thread_id,   counter_num
         fcc_esr_addr = self.get_fcc_addr(     sid,         0, self.fcc_cntr)

         # Get active minion's thread mask
         even_mask = self.get_fcc_mask(sid, 0)
         odd_mask  = self.get_fcc_mask(sid, 1)

         content_list.append(LI(rd=self.reg0, imm=fcc_esr_addr))

         if even_mask != 0:
            content_list.append(LI(rd=self.reg2, imm=even_mask))
            content_list.append(SendFCC(rs2=self.reg2, off=Imm12( 0),  rs1=self.reg0))
         if odd_mask != 0:
            content_list.append(LI(rd=self.reg1, imm=odd_mask))
            content_list.append(SendFCC(rs2=self.reg1, off=Imm12(16),  rs1=self.reg0))

   def _lead_hart_code(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list += [
         # Set the AMO Lock address in a register
         LI(rd=self.reg2, imm=self.amo_lock),

         # Atomically increase the AMO Lock
         LI(rd=self.reg0, imm=1)
      ]

      if TEST_CONF.POSTSI_L2_ONLY == True:
         content_list.append(AMOADDL_D(rd=self.reg0, rs2=self.reg0, rs1=self.reg2))
      else:
         content_list.append(AMOADDG_D(rd=self.reg0, rs2=self.reg0, rs1=self.reg2))

      content_list += [
         # Check if I am the last one
         LI(rd=self.reg1, imm=(len(self.active_sids)-1)),
         "bne {reg0}, {reg1}, wait_for_fcc_credit",
      ]

      # Reset AMO Lock to 0
      if TEST_CONF.POSTSI_L2_ONLY == True:
         content_list.append(AMOSWAPL_D(rd=self.zero, rs2=self.zero, rs1=self.reg2))
      else:
         content_list.append(AMOSWAPG_D(rd=self.zero, rs2=self.zero, rs1=self.reg2))

if TEST_CONF.agnostic_mode() != 0:
   TEST_CONF.EXCL_COMPS.add(AMOFLBFCCSyncThreads)
