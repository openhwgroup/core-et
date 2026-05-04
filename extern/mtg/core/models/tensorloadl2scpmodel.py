from core.models.test_helper import TestHelper
from core.models.tensormodel import set_bits, get_bits, TensorEncoding
from core.from_future import FromFuture
from core.test_conf import L2SCP_CONF
from core.test_conf import TEST_CONF

class TensorLoadL2SCPEncoding(TensorEncoding):

   def __init__(self, x31_val: int, value = None):

      self.x31_val = x31_val
      self.memory_size = FromFuture()
      self.l2scp_dst = FromFuture()
      tl_id = get_bits(x31_val, 0, 0)
      self.name = self.get_name(tl_id)
      super().__init__(value = value)

   # Getters / Setters
   ###################################################

   @classmethod
   def get_name(cls, tl_id: int) -> str:
      return f'TL_L2SCP{tl_id}'

   def get_usemsk(self, tl_l2scp_csr_val: int) -> int:
      return get_bits(tl_l2scp_csr_val, 63, 63)

   def set_usemsk(self, tl_l2scp_csr_val: int, value: int) -> int:
      return set_bits(tl_l2scp_csr_val, 63, 63, value)

   def get_dst(self, tl_l2scp_csr_val: int) -> int:
      msb = get_bits(tl_l2scp_csr_val, 62, 48)
      lsb = get_bits(tl_l2scp_csr_val,  5,  4)
      return (msb << 2) | lsb

   def set_dst(self, tl_l2scp_csr_val: int, value: int) -> int:
      msb = get_bits(value, 16, 2)
      lsb = get_bits(value,  1, 0)
      tl_l2scp_csr_val = set_bits(tl_l2scp_csr_val, 62, 48, msb)
      tl_l2scp_csr_val = set_bits(tl_l2scp_csr_val,  5,  4, lsb)
      return tl_l2scp_csr_val

   def get_count(self, tl_l2scp_csr_val: int) -> int:
      return get_bits(tl_l2scp_csr_val, 3, 0)

   def set_count(self, tl_l2scp_csr_val: int, value: int) -> int:
      return set_bits(tl_l2scp_csr_val, 3, 0, value)

   # Get memory size
   ###################################################

   def get_mem_size(self, tl_l2scp_csr_val):
      count = self.get_count(tl_l2scp_csr_val) + 1
      stride = get_bits(self.x31_val, 47, 6) * 64
      return count*stride + 64

   ###################################################

   @classmethod
   def free_resources(cls, model: 'Model', name: str) -> None:
      # Nothing to be freed
      pass

   def selfcheck_boundaries_tload_l2(self, tl_l2scp_csr_val, head, tail):
      count = self.get_count(tl_l2scp_csr_val)
      dst = self.get_dst(tl_l2scp_csr_val)
      for i in range(count):
         if self.tensor_mask is None or get_bits(self.tensor_mask, i, i) == 1:
            l2scp_line = (dst + i) % L2SCP_CONF.nlines
            assert head <= l2scp_line and l2scp_line <= tail, f"{head} <= {l2scp_line} and {l2scp_line} <= {tail}"


   def get_encoding(self, model: 'Model') -> int:

      test_helper = model.test_helper

      if TEST_CONF.USE_TENSOR_MASK_MODEL == 1:
         self.tensor_mask = test_helper.pick_one([None, model.tensor_mask])
      else:
         self.tensor_mask = test_helper.pick_one([None, 0xffff])

      slice_size = L2SCP_CONF.tensor_slice_size
      tl_l2scp_csr_base = test_helper.get_urand_num()

      if self.tensor_mask != None:
         tl_l2scp_csr_base = self.set_usemsk(tl_l2scp_csr_base, 1)
      else:
         tl_l2scp_csr_base = self.set_usemsk(tl_l2scp_csr_base, 0)

      count = test_helper.get_rand_range(0, min(0xf, slice_size))
      tl_l2scp_csr_val = self.set_count(tl_l2scp_csr_base, count)

      dst = test_helper.get_rand_range(0, slice_size-count) + L2SCP_CONF.tensor_head
      assert dst <= 0x1ffff
      tl_l2scp_csr_val = self.set_dst(tl_l2scp_csr_val, dst)

      tl_l2scp_csr_val = tl_l2scp_csr_val & 0xffff00000000003f

      return tl_l2scp_csr_val

   def post_encoding(self, model: 'Model', tl_l2scp_csr_val: int):
      tm = self.get_usemsk(tl_l2scp_csr_val)
      if tm == 1:
         if TEST_CONF.USE_TENSOR_MASK_MODEL == 1:
            self.tensor_mask = model.tensor_mask
         else:
            self.tensor_mask = 0xffff
      else:
         self.tensor_mask = None

      self.selfcheck_boundaries_tload_l2(tl_l2scp_csr_val, L2SCP_CONF.tensor_head, L2SCP_CONF.tensor_tail)
      self.memory_size.set_value(self.get_mem_size(tl_l2scp_csr_val))

      dst = self.get_dst(tl_l2scp_csr_val)
      self.l2scp_dst.set_value(dst)

      if self.value is None:
        self.rand_value = self.set_dst(tl_l2scp_csr_val, 0)
        self._imm_str = hex(self.rand_value)
      else:
        self.value = self.set_dst(tl_l2scp_csr_val, 0)
        self._imm_str = hex(self.value)
