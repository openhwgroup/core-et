
from core.mtg import *

from components.randinst.cacheops import UNLOCK_VA, LOCK_VA
from components.tensor_wait import WAIT_CACHEOPS
from components.randinst.rv32i import *
from components.tensor.tensor_mask import TensorMask

@register
class LockOrUnlockVA(Component, AsmCode):

   def __init__(self, test_helper: TestHelper = None, num_sets: int = None, num_ways: int = None):

      self.x31 = Xreg(31)
      self.csr_enc = Xreg()
      self.mask_reg = Xreg()
      self.va = Xreg()

      self.rb1 = Xreg()

      self.csr_val = test_helper.get_urand_num()
      self.tmask = test_helper.get_rand_kbits(16)
      self.repeat = test_helper.get_rand_range(0, 1)
      self.tensor_id = test_helper.get_rand_range(0, 1)
      self.unlock = test_helper.get_rand_range(0, 1)
      self.mem_offset = test_helper.get_rand_range(0, 8192)

      self.mem_lock16 = StaticGMalloc('mem_lock16', DRAM_REGION, 16384, align=64)

      # Select the repeat count, with a bias on smaller values
      self.csr_enc_vals     = [ 1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15]
      self.csr_enc_weights  = [16, 12, 12, 10, 10, 10, 10,  8,  8,  4,  4,  2,  2,  1,  1]
      self.csr_enc_imm      = test_helper.pick_one_weight(self.csr_enc_vals, self.csr_enc_weights)

      # Randomize stride to some cache line multiples
      num_lines = self.csr_enc_imm
      self.x31_enc_vals = [64, 128, 256, 512]
      self.x31_enc_vals = [x for x in self.x31_enc_vals if (x*num_lines) <= TEST_CONF.MAX_OFF]
      self.x31_enc      = test_helper.pick_one(self.x31_enc_vals)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:

      content_list.append(LI(test_helper = test_helper, rd = self.rb1, imm = self.mem_lock16))
      content_list.append(LI(test_helper = test_helper, rd = self.va, imm = self.mem_offset))
      content_list.append(ADD(test_helper = test_helper, rd = self.rb1, rs1 = self.rb1, rs2 = self.va))

      # csr_enc is the register used to write to CSR
      # mask_reg is used for masking
      # rb1 contains a VA
      # All equally random, except VA and repeat count
      content_list.append(LI(test_helper = test_helper, rd = self.csr_enc, imm = self.csr_val))
      content_list.append(LI(test_helper = test_helper, rd = self.mask_reg, imm = 0xffff000000000030))
      content_list.append(AND(test_helper = test_helper, rd = self.csr_enc, rs1 = self.csr_enc, rs2 = self.mask_reg))

      # add VA
      content_list.append(LI(test_helper = test_helper, rd = self.mask_reg, imm = 0xffffffffffc0))
      content_list.append(AND(test_helper = test_helper, rd = self.va, rs1 = self.rb1, rs2 = self.mask_reg))
      content_list.append(OR(test_helper = test_helper, rd = self.csr_enc, rs1 = self.csr_enc, rs2 = self.va))

      if (self.repeat > 0):
         content_list.append(ORI(test_helper = test_helper, rd = self.csr_enc, rs1 = self.csr_enc, imm = self.csr_enc_imm))
         content_list.append(LI(test_helper = test_helper, rd = self.x31, imm = self.x31_enc))

      content_list.append(ORI(test_helper = test_helper, rd = self.x31, rs1 = self.x31, imm = self.tensor_id))

      # Load the Tensor Mask CSR if using it
      if (self.csr_val >= 0x8000000000000000):
         content_list.append(TensorMask(test_helper = test_helper, x31 = self.x31, x31_val = self.x31_enc, value = self.tmask))

      # Start the cacheop
      if (self.unlock > 0):
         content_list.append(UNLOCK_VA(test_helper = test_helper, rs1 = self.csr_enc, x31 = self.x31))
      else:
         content_list.append(LOCK_VA(test_helper = test_helper, rs1 = self.csr_enc, x31 = self.x31))
