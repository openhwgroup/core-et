from core.models.test_helper import TestHelper
from core.models.tensormodel import set_bits, get_bits, randomize_scp_lines, TensorEncoding
from core.models.l1scpmodel import L1SCPModel
from core.models.resource import ResourceState
from core.from_future import FromFuture

class TensorStoreSCPEncoding(TensorEncoding):

   def __init__(self, x31_val: int, value = None):
      self.x31_val = x31_val
      self.memory_size = FromFuture()
      self.name = self.get_name()

      super().__init__(value = value)

   # Getters / Setters
   ###################################################

   @classmethod
   def get_name(cls) -> str:
      return f'TSSCP'

   def get_step(self, ts_scp_csr_val: int) -> int:
      return get_bits(ts_scp_csr_val, 63, 62)

   def set_step(self, ts_scp_csr_val: int, value: int) -> int:
      return set_bits(ts_scp_csr_val, 63, 62, value)

   def get_src(self, ts_scp_csr_val: int) -> int:
      return get_bits(ts_scp_csr_val, 61, 56)

   def set_src(self, ts_scp_csr_val: int, value: int) -> int:
      return set_bits(ts_scp_csr_val, 61, 56, value)

   def get_rows(self, ts_scp_csr_val: int) -> int:
      return get_bits(ts_scp_csr_val, 54, 51)

   def set_rows(self, ts_scp_csr_val: int, value: int) -> int:
      return set_bits(ts_scp_csr_val, 54, 51, value)

   def get_VA(self, ts_scp_csr_val: int) -> int:
      return get_bits(ts_scp_csr_val, 47, 6)

   def set_VA(self, ts_scp_csr_val: int, value: int) -> int:
      return set_bits(ts_scp_csr_val, 47, 6, value)

   ###################################################

   def get_tstore_scp_wrapper(self, test_helper: TestHelper) -> int:
      ts_csr_val_rand = test_helper.get_urand_num()
      ts_csr_val      = ts_csr_val_rand & 0xfffe00000000003f
      ts_csr_val      = set_bits(ts_csr_val, 48, 48, 1)
      return ts_csr_val

   def get_empty_l1scp_range(self, test_helper: TestHelper, l1_scp: L1SCPModel) -> (int, int):
      free_regions = l1_scp.get_read_free_regions()
      region = test_helper.pick_one(free_regions)
      return region

   def _map_scp_lines(self, ts_scp_csr_val: int, method) -> None:
      step = self.get_step(ts_scp_csr_val) + 1
      src  = self.get_src(ts_scp_csr_val)
      rows = self.get_rows(ts_scp_csr_val) + 1

      for i in range(rows):
         scp_line = (src + (i*step))%48
         method(scp_line)

   def selfcheck_boundaries_ts_scp(self, ts_scp_csr_val: int, head: int, tail: int) -> None:
      def method(scp_line):
         assert head <= scp_line and scp_line <= tail
      self._map_scp_lines(ts_scp_csr_val, method)

   def constrain_ts_scp_read(self, test_helper: TestHelper, ts_scp_csr_val: int, scp_head: int, scp_tail: int) -> None:
      src = test_helper.get_rand_range(scp_head, scp_tail)
      max_rows = max(min(scp_tail-src, 16), 1)
      rows = test_helper.get_rand_range(1, max_rows)

      if rows > 1:
         max_step = max(min(int((scp_tail - src)/(rows-1)), 4), 1)
         step = test_helper.get_rand_range(1, max_step)
         ts_scp_csr_val = self.set_step(ts_scp_csr_val, step-1)

      ts_scp_csr_val = self.set_src(ts_scp_csr_val, src)
      ts_scp_csr_val = self.set_rows(ts_scp_csr_val, rows-1)
      self.selfcheck_boundaries_ts_scp(ts_scp_csr_val, scp_head, scp_tail)
      return ts_scp_csr_val

   def selfcheck_set_ts_scp(self, l1_scp: 'Model', ts_scp_csr_val: int, selfcheck: int, setm: int):
      # Make sure that at least one of the two options are set
      assert selfcheck or setm

      def method(scp_line):
         if selfcheck == 1:
            assert l1_scp.is_free(scp_line) or l1_scp.is_read_only(scp_line)
         if setm == 1:
            l1_scp.set_read_only(scp_line, self.name)

      self._map_scp_lines(ts_scp_csr_val, method)

   def get_mem_size(self, ts_scp_csr_val: int, x31_val: int) -> int:
      rows = self.get_rows(ts_scp_csr_val) + 1
      stride = get_bits(x31_val, 47, 6) * 64
      mem_size = (rows-1)*stride + 64

      # To avoid sharing the same cache line between different operations
      # We align the memory size to 64 bytes
      if (mem_size%64) != 0:
         mem_size = (mem_size & 0xffffffffffffffc0) + 64

      # As the memory address could not be aligned to 64 bytes, we add 64 extra bytes just in case
      mem_size = mem_size + 64

      return mem_size

   @classmethod
   def free_resources(cls, model: 'Model', name: str) -> None:
      l1_scp = model.l1scp
      # Free all the L1SCP resources used by this same operation as HW cannot execute
      # the same tensor operation at the same time (implicit tensor wait)
      l1_scp.free_resources(name)
      model.pending_tensor_store_scp = False

   def get_encoding(self, model: 'Model') -> int:
      test_helper = model.test_helper
      l1_scp = model.l1scp

      ts_scp_csr_val = self.get_tstore_scp_wrapper(test_helper)

      (head, tail) = self.get_empty_l1scp_range(test_helper, l1_scp)
      ts_scp_csr_val = self.constrain_ts_scp_read(test_helper, ts_scp_csr_val, head, tail)

      return ts_scp_csr_val

   def post_encoding(self, model: 'Model', ts_scp_csr_val: int):
      l1_scp = model.l1scp
      # Self Check there is no potential L1SCP collisions and set the lines in the L1SCP model
      self.selfcheck_set_ts_scp(l1_scp, ts_scp_csr_val, 1, 1)
      self.memory_size.set_value(self.get_mem_size(ts_scp_csr_val, self.x31_val))
      model.pending_tensor_store_scp = True
