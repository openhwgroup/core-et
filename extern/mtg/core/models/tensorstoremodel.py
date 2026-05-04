from core.models.test_helper import TestHelper
from core.models.tensormodel import set_bits, get_bits, randomize_scp_lines, TensorEncoding
from core.models.fpmodel import FPModel
from core.models.resource import ResourceState
from core.from_future import FromFuture

class TensorStoreEncoding(TensorEncoding):

   def __init__(self, x31_val: int, coop_mode: int, value = None, invalid = None, row_size = None):
      self.x31_val = x31_val
      self.memory_size = FromFuture()
      self.name = self.get_name()
      self.coop_mode = coop_mode
      self.row_size = row_size

      if invalid is None:
         self.invalid = False
      else:
         self.invalid = True
         assert value is None

      if value is not None:
         assert invalid is None

      super().__init__(value = value)

   # Getters / Setters
   ###################################################

   @classmethod
   def get_name(cls) -> str:
      return f'TS'

   def get_stride(self, x31_val):
      return get_bits(x31_val, 47, 4)

   def get_fstep(self, ts_csr_val: int) -> int:
      return get_bits(ts_csr_val, 63, 62)

   def set_fstep(self, ts_csr_val: int, value: int) -> int:
      return set_bits(ts_csr_val, 63, 62, value)
 
   def get_fstart(self, ts_csr_val: int) -> int:
      return get_bits(ts_csr_val, 61, 57)

   def set_fstart(self, ts_csr_val: int, value: int) -> int:
      return set_bits(ts_csr_val, 61, 57, value)
 
   def get_ts_bytes(self, ts_csr_val: int) -> int:
      return get_bits(ts_csr_val, 56, 55)

   def set_ts_bytes(self, ts_csr_val: int, value: int) -> int:
      return set_bits(ts_csr_val, 56, 55, value)
 
   def get_rows(self, ts_csr_val: int) -> int:
      return get_bits(ts_csr_val, 54, 51)

   def set_rows(self, ts_csr_val: int, value: int) -> int:
      return set_bits(ts_csr_val, 54, 51, value)

   def get_coop(self, ts_csr_val: int) -> int:
      return get_bits(ts_csr_val, 50, 49)

   def set_coop(self, ts_csr_val: int, value: int) -> int:
      return set_bits(ts_csr_val, 50, 49, value)
 
   def get_VA(self, ts_csr_val: int) -> int:
      return get_bits(ts_csr_val, 47, 4)

   def set_VA(self, ts_csr_val: int, value: int) -> int:
      return set_bits(ts_csr_val, 47, 4, value)

   ###################################################

   def tstore_is_valid_config(self, csr_value: int) -> bool:
      coop_mode = self.get_coop(csr_value)
      ts_bytes  = self.get_ts_bytes(csr_value)

      if coop_mode == 0:
         if ts_bytes == 0b00 or ts_bytes == 0b01 or ts_bytes == 0b11:
            return True
      elif coop_mode == 1:
         if ts_bytes == 0b00 or ts_bytes == 0b01:
            return True
      elif coop_mode == 3:
         if ts_bytes == 0b00:
            return True
      return False

   def set_valid_config(self, csr_value: int, rand_init: int) -> int:
      csr_value  = self.set_fstart(csr_value, rand_init)
      coop_mode  = self.get_coop(csr_value)
      rand_value = self.get_ts_bytes(csr_value)

      if coop_mode == 0:
         rand_value = rand_value % 3
         switch_case = { 0: 0b00, 1: 0b01, 2: 0b11 }
         rand_value = switch_case[rand_value]

      elif coop_mode == 1:
         rand_value = rand_value % 2;
         switch_case = {0: 0b00, 1: 0b01 }
         rand_value = switch_case[rand_value]

      elif coop_mode == 3:
         rand_value = 0

      return self.set_ts_bytes(csr_value, rand_value)

   def set_invalid_config(self, csr_value: int, rand_init: int) -> int:
      csr_value = self.set_fstart(csr_value, rand_init)
      coop_mode  = self.get_coop(csr_value)
      rand_value = self.get_ts_bytes(csr_value)

      if coop_mode == 0:
         rand_value = 2

      elif coop_mode == 1:
         rand_value = rand_value % 2;
         switch_case = {0: 0b10, 1: 0b11 }
         rand_value = switch_case[rand_value]

      elif coop_mode == 3:
         rand_value = rand_value % 3;
         switch_case = {0: 0b01, 1: 0b10, 2: 0b11 }
         rand_value = switch_case[rand_value]

      return self.set_ts_bytes(csr_value, rand_value)

   def get_biased_fstep(self, test_helper: TestHelper, head: int = 0, tail: int = 3):
      candidates = list(range(head, tail+1))
      weights = candidates.copy()
      weights.reverse()
      fstep = test_helper.pick_one_weight(candidates, weights)
      return fstep

   def constrain_tstore_vpu_read_rf(self, test_helper: TestHelper, ts_csr_val: int, rf_head: int, rf_tail: int) -> int:
      ret = ts_csr_val
      valid_config = self.tstore_is_valid_config(ts_csr_val)
      if valid_config == True:
         fstep = self.get_fstep(ts_csr_val) + 1
         ts_bytes = (self.get_ts_bytes(ts_csr_val) + 1) * 16
   
         fstart = test_helper.get_rand_range(rf_head, rf_tail)
   
         if ts_bytes == 16:
            rf_tail = min(rf_tail, fstart + 15 * fstep)
         elif ts_bytes == 32:
            rf_tail = min(rf_tail, fstart + 15 * fstep)
         elif ts_bytes == 64:
            rf_tail = min(rf_tail, fstart + 15 * 2 * fstep + fstep)
   
         rf_tail = test_helper.get_rand_range(fstart, rf_tail)
         if ts_bytes == 64:
            margin = rf_tail-fstart
            if margin == 0:
               # If there is only one register available and the config modifies
               # two register (64 ts_bytes), we must constrain the config
               tmp = test_helper.get_rand_range(0, 1)
               switch_case = {0: 16, 1: 32}
               ts_bytes = switch_case[tmp]
               ret = self.set_ts_bytes(ret, tmp)

            elif margin < fstep:
               # If the fstep is smaller than the number of registers we are working on
               # we need to constrain fstep, as in 64ts_bytes, we read two registers, one at X
               # and the next one at X+fstep every loop iteration
               fstep = self.get_biased_fstep(test_helper, 1, margin)
               ret = self.set_fstep(ret, fstep-1)
   
         if ts_bytes == 16:
            rows = int((rf_tail - fstart)/fstep)
         elif ts_bytes == 32:
            rows = int((rf_tail - fstart)/fstep)
         elif ts_bytes == 64:
            rows = int((rf_tail - fstart - fstep)/(fstep*2))

         ret = self.set_fstart(ret, fstart)
         ret = self.set_rows(ret, rows)
         self.selfcheck_boundaries_tstore(ret, fstart, rf_tail)
      return ret

   def get_tstore_wrapper(self, test_helper: TestHelper) -> int:
      ts_csr_val_rand = test_helper.get_urand_num()
      ts_csr_val_base = ts_csr_val_rand & 0xfff800000000000f

      if self.coop_mode > 0:
         if not self.invalid:
            assert self.row_size is not None
         ts_csr_val_base = self.set_coop(ts_csr_val_base, self.coop_mode - 1)

      if self.row_size is None:
         rand_init = test_helper.get_rand_range(0, 31)
         if self.invalid:
            ts_csr_val = self.set_invalid_config(ts_csr_val_base, rand_init)
         else:
            ts_csr_val = self.set_valid_config(ts_csr_val_base, rand_init)
      else:
         ts_csr_val = self.set_ts_bytes(ts_csr_val_base, self.row_size - 1)

      fstep = self.get_biased_fstep(test_helper)
      ts_csr_val = self.set_fstep(ts_csr_val, fstep)
   
      return ts_csr_val

   def get_empty_vpu_rf_range(self, test_helper: TestHelper, vpu_rf: FPModel) -> (int, int):
      free_regions = vpu_rf.get_read_free_regions()
      region = test_helper.pick_one(free_regions)
      return region

   def _map_vpu_regs(self, method, ts_csr_val: int) -> None:
      valid_config = self.tstore_is_valid_config(ts_csr_val)
      if valid_config == True:
         fstep    = self.get_fstep(ts_csr_val) + 1
         fstart   = self.get_fstart(ts_csr_val)
         ts_bytes = (self.get_ts_bytes(ts_csr_val) + 1) * 16
         rows     = self.get_rows(ts_csr_val) + 1

         if ts_bytes == 16:
            for i in range(rows):
               reg = (fstart + i*fstep) % 32
               method(reg)
         elif ts_bytes == 32:
            for i in range(rows):
               reg = (fstart + i*fstep) % 32
               method(reg)
         elif ts_bytes == 64:
            for i in range(rows):
               reg0 = (fstart + 2*i*fstep) % 32
               reg1 = (fstart + (2*i + 1)*fstep) % 32
               method(reg0)
               method(reg1)

   def selfcheck_boundaries_tstore(self, ts_csr_val: int, head: int, tail: int):
      def method(reg: int):
         assert reg >= head
         assert reg <= tail

      self._map_vpu_regs(method, ts_csr_val)

   def selfcheck_set_tstore(self, vpu_rf: 'Model', ts_csr_val: int, selfcheck: int, setm: int):
      # Make sure that at least one of the two options are set
      assert selfcheck or setm

      def method(reg: int):
         if selfcheck == 1:
            assert vpu_rf.is_free(reg) or vpu_rf.is_read_only(reg)
         if setm == 1:
            vpu_rf.set_read_only(reg, self.name)

      self._map_vpu_regs(method, ts_csr_val)

   @classmethod
   def free_resources(cls, model: 'Model', name: str) -> None:
      vpu_rf = model.floatpoint
      # Free all the VPU resources used by this same operation as HW cannot execute
      # the same tensor operation at the same time (implicit tensor wait)
      vpu_rf.free_resources(name)

   def get_mem_size(self, ts_csr_val: int, x31_val: int) -> int:
      mem_size = 0
      valid_config = self.tstore_is_valid_config(ts_csr_val)
      if valid_config is True:
         ts_bytes  = (self.get_ts_bytes(ts_csr_val) + 1) * 16
         rows   = self.get_rows(ts_csr_val) + 1
         stride = self.get_stride(x31_val) * 16
         # Max 'i' value from the loop
         i = rows - 1
         if ts_bytes == 16:
            mem_size = ((i*stride) & (0xfffffffffffffff0)) + 16
         elif ts_bytes == 32:
            mem_size = ((i*stride) & (0xffffffffffffffe0)) + 32
         elif ts_bytes == 64:
            mem_size = (((i*stride) + 32) & (0xffffffffffffffc0)) + 32

         if self.coop_mode > 0:
            mem_size = mem_size + (x31_val & 0xfffffffffff0)

         # To avoid sharing the same cache line between different operations
         # We align the memory size to 64 ts_bytes
         if (mem_size%64) != 0:
            mem_size = (mem_size & 0xffffffffffffffc0) + 64

         # As the memory address could not be aligned to 64 ts_bytes, we add 64 extra ts_bytes just in case
         mem_size = mem_size + 64
      return mem_size

   def get_encoding(self, model: 'Model') -> int:
      test_helper = model.test_helper
      vpu_rf = model.floatpoint

      ts_csr_val = self.get_tstore_wrapper(test_helper)
      valid_config = self.tstore_is_valid_config(ts_csr_val)
      if valid_config == True:
         (head, tail) = self.get_empty_vpu_rf_range(test_helper, vpu_rf)
         ts_csr_val = self.constrain_tstore_vpu_read_rf(test_helper, ts_csr_val, head, tail)
   
      return ts_csr_val

   def post_encoding(self, model: 'Model', ts_csr_val: int):
      vpu_rf = model.floatpoint
      # Self Check there is no potential VPU reg collisions and set the lines in the VPU RF models
      self.selfcheck_set_tstore(vpu_rf, ts_csr_val, 1, 1)
      self.memory_size.set_value(self.get_mem_size(ts_csr_val, self.x31_val))
