
from core.mtg import *

from components.randinst.cacheops import UNLOCK_SW, FLUSH_SW, EVICT_SW
from components.tensor_wait import WAIT_CACHEOPS
from components.randinst.rv32i import *
from components.tensor.tensor_mask import TensorMask

@register
class EvictOrFlushSW(Component, AsmCode):

   def __init__(self, test_helper: TestHelper = None, num_sets: int = None, num_ways: int = None):

      self.rx1 = Xreg()
      self.rx2 = Xreg()

      self.csr_val = test_helper.get_urand_num()
      self.tmask = test_helper.get_urand_num()
      self.repeat = test_helper.get_rand_range(0, 1)
      self.tensor_id = test_helper.get_rand_range(0, 1)
      self.do_evict = test_helper.get_rand_range(0, 1)

      self.csr_enc_vals     = [ 1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15]
      self.csr_enc_weights  = [16, 12, 12, 10, 10, 10, 10,  8,  8,  4,  4,  2,  2,  1,  1]

      self.csr_enc = self.csr_val & 0xfffffffffffffff0

      if (self.repeat > 0):
         # Select the repeat count, with a bias on smaller values
         self.csr_enc = (self.csr_enc | test_helper.pick_one_weight(self.csr_enc_vals, self.csr_enc_weights))


   @machine_code
   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:

      # rx1 is the register used to write to CSR
      # Everything can be random because all fields are either WARL or legal
      # Clear bottom 4 bits (repeat count) to have a biased random
      content_list.append(LI(test_helper = test_helper, rd = self.rx1, imm = self.csr_enc))

      # Setup mask if used
      if (self.csr_val >= 0x8000000000000000):
         content_list.append(TensorMask(test_helper = test_helper, value = self.tmask))

      if (self.do_evict > 0):
         content_list.append(EVICT_SW(test_helper = test_helper, rs1 = self.rx1))
      else:
         content_list.append(FLUSH_SW(test_helper = test_helper, rs1 = self.rx1))

if TEST_CONF.EXTRA_CHECKS == 1:
   TEST_CONF.EXCL_COMPS.add(EvictOrFlushSW)
