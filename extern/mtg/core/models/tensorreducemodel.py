from core.models.test_helper import TestHelper
from core.models.tensormodel import set_bits, get_bits, TensorEncoding
from core.models.fpmodel import FPModel
from core.test_conf import TEST_CONF

FADD = 0b0000
FMAX = 0b0010
FMIN = 0b0011
IADD = 0b0100
IMAX = 0b0110
IMIN = 0b0111
FGET = 0b1000

SEND = 0b00
RECV = 0b01
REDUCE = 0b11
BROADCAST = 0b10

class TensorReduceEncoding(TensorEncoding):

   valid_op = [FADD, FMAX, FMIN, IADD, IMAX, IMIN, FGET]
   invalid_op = [0b0001, 0b0101, 0b1001, 0b1010, 0b1011, 0b1100, 0b1101, 0b1110, 0b1111]

   def __init__(self, value = None):
      self.name = self.get_name()
      super().__init__(value = value)

   # Getters / Setters
   ###################################################

   @classmethod
   def get_name(cls) -> str:
      return f'TR'

   def get_fstart(self, tr_csr_val: int) -> int:
      return get_bits(tr_csr_val, 61, 57)

   def set_fstart(self, tr_csr_val: int, value: int) -> int:
      return set_bits(tr_csr_val, 61, 57, value)

   def get_op(self, tr_csr_val: int) -> int:
      return get_bits(tr_csr_val, 27, 24)

   def set_op(self, tr_csr_val: int, value: int) -> int:
      return set_bits(tr_csr_val, 27, 24, value)

   def get_fnum(self, tr_csr_val: int) -> int:
      return get_bits(tr_csr_val, 22, 16)

   def set_fnum(self, tr_csr_val: int, value: int) -> int:
      return set_bits(tr_csr_val, 22, 16, value)

   def get_partner(self, tr_csr_val: int) -> int:
      return get_bits(tr_csr_val, 15, 3)

   def set_partner(self, tr_csr_val: int, value: int) -> int:
      assert self.get_variant(tr_csr_val) in [SEND, RECV]
      return set_bits(tr_csr_val, 15, 3, value)

   def get_tree_depth(self, tr_csr_val: int) -> int:
      return get_bits(tr_csr_val, 6, 3)

   def set_tree_depth(self, tr_csr_val: int, value: int) -> int:
      assert self.get_variant(tr_csr_val) in [REDUCE, BROADCAST]
      return set_bits(tr_csr_val, 6, 3, value)

   def get_variant(self, tr_csr_val: int) -> int:
      return get_bits(tr_csr_val, 1, 0)

   def set_variant(self, tr_csr_val: int, value: int) -> int:
      return set_bits(tr_csr_val, 1, 0, value)

   ###################################################

   def get_empty_vpurf_range(self, test_helper: TestHelper, vpu_rf: FPModel) -> (int, int):
      free_regions = vpu_rf.get_write_free_regions()
      free_region = test_helper.pick_one(free_regions)
      return free_region

   def _map_vpu_regs(self, vpureg_method, tr_csr_val: int) -> None:
      depth = self.get_tree_depth(tr_csr_val)
      fstart = self.get_fstart(tr_csr_val)
      fnum = self.get_fnum(tr_csr_val)
      for i in range(fnum):
         k = (fstart+i)%32
         vpureg_method(k)

   def selfcheck_boundaries_treduce(self, tr_csr_val: int, head: int, tail: int) -> None:
      def vpureg_method(reg):
         assert (head <= reg) and (reg <= tail)

      self._map_vpu_regs(vpureg_method, tr_csr_val)

   def selfcheck_set_treduce(self, vpu_rf: 'Model', tr_csr_val: int, selfcheck: int, setm: int):
      # Make sure that at least one of the two options are set
      assert selfcheck or setm

      def vpureg_method(reg):
         if selfcheck == 1:
            assert vpu_rf.is_free(reg)
         if setm == 1:
            vpu_rf.set_write_only(reg, self.name)

      self._map_vpu_regs(vpureg_method, tr_csr_val)

   def constrain_fp_regs(self, test_helper: TestHelper, vpu_rf: FPModel, tr_csr_val: int, tree_lvls: list) -> int:
      if len(tree_lvls) > 0:
         (head, tail) = self.get_empty_vpurf_range(test_helper, vpu_rf)

         # Set starting floating-point register
         fstart = test_helper.get_rand_range(head, tail)

         # Set the number of floating-point registers
         max_fnum = (tail - fstart) + 1
         fnum = test_helper.get_rand_range(0, max_fnum)

      else:
         # If no tree levels are available, do a reduce with 0 fp regs (aka nop)
         head = 0
         tail = 31
         fstart = test_helper.get_rand_range(head, tail)
         fnum = 0

      tr_csr_val = self.set_fstart(tr_csr_val, fstart)
      tr_csr_val = self.set_fnum(tr_csr_val, fnum)
      self.selfcheck_boundaries_treduce(tr_csr_val, head, tail)

      return tr_csr_val

   @classmethod
   def free_resources(cls, model: 'Model', name: str, from_tensor_wait: bool = False) -> None:
      vpu_rf = model.floatpoint
      # Free all the VPU RF resources used by this same operation as HW cannot execute
      # the same tensor operation at the same time (implicit tensor wait)
      vpu_rf.free_resources(name)
      if from_tensor_wait:
         model.tr_last_reg = None

   def set_tensor_error(self, test_helper: TestHelper, tr_csr_val: int):
         # To trigger the error we want to set the op to Reduce Recv and use an illegal operation
         tr_csr_val = self.set_variant(tr_csr_val, RECV)
         il_op = test_helper.pick_one(self.invalid_op)
         tr_csr_val = self.set_op(tr_csr_val, il_op)
         return tr_csr_val

   def get_treduce_auto_bcast_wrapper(self, test_helper: TestHelper, tree_lvls: list, tree_lvls_weights: list):
      tr_csr_val = test_helper.get_rand_num()
   
      if len(tree_lvls) > 0:
         # 50/50 between reduce auto and reduce broadcast
         variant = test_helper.pick_one([REDUCE, BROADCAST])
         tr_csr_val = self.set_variant(tr_csr_val, variant)

         # Select a legal tensor reduce tree depth to avoid hangs
         tree_depth = test_helper.pick_one_weight(tree_lvls, tree_lvls_weights)
         tr_csr_val = self.set_tree_depth(tr_csr_val, tree_depth)

         # Select a valid operation to avoid hangs
         op = test_helper.pick_one(self.valid_op)
         tr_csr_val = self.set_op(tr_csr_val, op)
      else:
         tr_csr_val = self.set_tensor_error(test_helper, tr_csr_val)
      return tr_csr_val

   def get_encoding(self, model: 'Model') -> int:
      test_helper = model.test_helper
      vpu_rf = model.floatpoint
      tree_lvls = model.treduce_tree_lvls
      tree_lvls_weights = model.treduce_tree_lvls_weights

      tr_csr_val = self.get_treduce_auto_bcast_wrapper(test_helper, tree_lvls, tree_lvls_weights)
      tr_csr_val = self.constrain_fp_regs(test_helper, vpu_rf, tr_csr_val, tree_lvls)

      return tr_csr_val

   def post_encoding(self, model: 'Model', tr_csr_val: int):
      vpu_rf = model.floatpoint
      self.selfcheck_set_treduce(vpu_rf, tr_csr_val, 1, 1)
      model.tr_last_reg =(self.get_fstart(tr_csr_val) + self.get_fnum(tr_csr_val) - 1)%32
