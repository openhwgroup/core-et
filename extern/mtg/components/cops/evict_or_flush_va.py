
from core.mtg import *

from components.randinst.cacheops import UNLOCK_SW, FLUSH_VA, EVICT_VA
from components.tensor_wait import WAIT_CACHEOPS
from components.randinst.rv32i import *
from components.tensor.tensor_mask import TensorMask

@register
class EvictOrFlushVA(Component, AsmCode):

   def __init__(self, test_helper: TestHelper = None, num_sets: int = None, num_ways: int = None):

      self.x31 = Xreg(31)
      self.csr_reg = Xreg()
      self.mask_reg = Xreg()
      #self.saved_x31 = Xreg()
      self.va = Xreg()

      self.base_addr = ABIreg('tp')

      self.csr_enc = test_helper.get_urand_num()
      self.tmask = test_helper.get_rand_kbits(16)
      self.repeat = test_helper.get_rand_range(0, 1)
      self.tensor_id = test_helper.get_rand_range(0, 1)
      self.do_evict = test_helper.get_rand_range(0, 1)
      self.x31_enc = test_helper.get_urand_num() & 0xffff00000000003e

      self.csr_enc_vals     = [ 1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15]
      self.csr_enc_weights  = [16, 12, 12, 10, 10, 10, 10,  8,  8,  4,  4,  2,  2,  1,  1]

      self.x31_enc_vals = [64, 128, 256, 512]

      self.csr_enc = self.csr_enc & 0xffff000000000030

      if (self.repeat > 0):
         # Select the repeat count, with a bias on smaller values
         num_lines = test_helper.pick_one_weight(self.csr_enc_vals, self.csr_enc_weights)
         self.csr_enc = (self.csr_enc | num_lines)
         # Randomize stride to some cache line multiples
         self.x31_enc_vals = [x for x in self.x31_enc_vals if (x*num_lines) <= TEST_CONF.MAX_OFF]
         self.x31_enc = (self.x31_enc | test_helper.pick_one(self.x31_enc_vals))

         # Sanity check
         sc_stride = self.x31_enc & 0xffffffffffc0
         assert sc_stride in self.x31_enc_vals
         sc_num_lines = self.csr_enc & 0xf
         assert (sc_num_lines*sc_stride) <= TEST_CONF.MAX_OFF

      self.x31_enc = (self.x31_enc | self.tensor_id)
      assert (self.x31_enc & 0x1) == self.tensor_id


   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:

      # csr_reg is the register used to write to CSR
      # mask_reg is used for masking
      # base_addr contains a VA
      # All equally random, except VA and bottom 4 bits (repeat count)
      # to avoid destroying too much cache state too often
      content_list.append(LI(test_helper = test_helper, rd = self.x31, imm = self.x31_enc))
      content_list.append(LI(test_helper = test_helper, rd = self.csr_reg, imm = self.csr_enc))

      # add VA
      content_list.append(LI(test_helper = test_helper, rd = self.mask_reg, imm = 0xffffffffffc0))
      content_list.append(AND(test_helper = test_helper, rd = self.va, rs1 = self.base_addr, rs2 = self.mask_reg))
      content_list.append(OR(test_helper = test_helper, rd = self.csr_reg, rs1 = self.csr_reg, rs2 = self.va))

      # Load the Tensor Mask CSR if using it
      if (self.csr_enc >= 0x8000000000000000):
         content_list.append(TensorMask(test_helper = test_helper, x31 = self.x31, x31_val = self.x31_enc, value = self.tmask))

      # Start the cacheop
      if (self.do_evict > 0):
         content_list.append(EVICT_VA(test_helper = test_helper, rs1 = self.csr_reg, x31 = self.x31))
      else:
         content_list.append(FLUSH_VA(test_helper = test_helper, rs1 = self.csr_reg, x31 = self.x31))

if TEST_CONF.EXTRA_CHECKS == 1:
   TEST_CONF.EXCL_COMPS.add(EvictOrFlushVA)
