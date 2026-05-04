from core.models.test_helper import TestHelper
from core.models.tensormodel import set_bits, get_bits, randomize_scp_lines, TensorEncoding
from core.models.l1scpmodel import L1SCPModel
from core.models.resource import ResourceState
from core.from_future import FromFuture
from core.test_conf import TEST_CONF

SET_TENB = 1
SET_NON_TENB = 0

SET_COOP = 1
SET_NON_COOP = 0

TENA  = 0
TENB  = 0
INT8  = 1
INT16 = 2
TRA8  = 5
TRA16 = 6
TRA32 = 7

class TensorLoadEncoding(TensorEncoding):

   valid_trans = [TENA, TENB, INT8, INT16, TRA8, TRA16, TRA32]
   invalid_trans = [0b011, 0b100]

   def __init__(self, x31_val: int, value = None, tenb: int = 0, trans = None, invalid = None, coop = None):

      self.x31_val = x31_val
      self.memory_size = FromFuture()
      tl_id = get_bits(x31_val, 0, 0)
      self.name = self.get_name(tl_id)
      self.coop = coop

      # This variable is used to properly pair the TL TenB with a TFMA TenB
      # non-tenb TL's ignore this variable
      self.__tenb_num_lines = FromFuture()

      self.tenb = tenb
      self.trans = trans

      if invalid is None:
         self.invalid = False
      else:
         self.invalid = True
         assert value is None
         assert tenb == 0

      if value is not None:
         if tenb is not None:
            assert tenb == self.get_tenb(value)
         if trans is not None:
            assert trans == self.get_transformation(value)
         assert invalid is None
         self.tenb_num_lines.set_value(self.get_num_lines(value))

      super().__init__(value = value)

   # Getters / Setters
   ###################################################

   @property
   def tenb_num_lines(self) -> FromFuture:
      return self.__tenb_num_lines

   @tenb_num_lines.setter
   def tenb_num_lines(self, num_lines: FromFuture) -> None:
      if self.value is not None and num_lines != None:
         assert self.get_num_lines(self.value) == num_lines
      self.__tenb_num_lines = num_lines

   @classmethod
   def get_name(cls, tl_id: int) -> str:
      return f'TL{tl_id}'

   def get_use_tensor_mask(self, tl_csr_val) -> int:
      return get_bits(tl_csr_val, 63, 63)

   def set_use_tensor_mask(self, tl_csr_val, value) -> int:
      if value == 1:
         assert self.tensor_mask is not None
      return set_bits(tl_csr_val, 63, 63, value)

   def get_is_coop(self, tl_csr_val) -> int:
      return get_bits(tl_csr_val, 62, 62)

   def set_is_coop(self, tl_csr_val, value) -> int:
      return set_bits(tl_csr_val, 62, 62, value)

   def get_transformation(self, tl_csr_val) -> int:
      return get_bits(tl_csr_val, 61, 59)

   def set_transformation(self, tl_csr_val, value) -> int:
      return set_bits(tl_csr_val, 61, 59, value)

   def get_start_scp_line(self, tl_csr_val) -> int:
      return get_bits(tl_csr_val, 58, 53)

   def set_start_scp_line(self, tl_csr_val, value) -> int:
      return set_bits(tl_csr_val, 58, 53, value)

   def get_tenb(self, tl_csr_val) -> int:
      return get_bits(tl_csr_val, 52, 52)

   def set_tenb(self, tl_csr_val, value) -> int:
      return set_bits(tl_csr_val, 52, 52, value)

   def get_num_lines(self, tl_csr_val) -> int:
      return get_bits(tl_csr_val, 3, 0)

   def set_num_lines(self, tl_csr_val, value) -> int:
      return set_bits(tl_csr_val, 3, 0, value)

   # Get memory size
   ###################################################

   def _get_mem_size_tload_tena_tenb(self, tl_csr_val, x31_val):
      stride = get_bits(x31_val, 47, 6) * 64
      count = self.get_num_lines(tl_csr_val) + 1
      i = count - 1
      mem_size = i*stride + 64
      return mem_size

   def _get_mem_size_tload_int8(self, tl_csr_val, x31_val):
      stride = get_bits(x31_val, 47, 6) * 64
      offset = get_bits(tl_csr_val, 5, 4) * 16
      count = self.get_num_lines(tl_csr_val) + 1
      i = count - 1
      r = 3

      VA = offset
      mem_size = (VA + (i * 4 + r) * stride) + 16
      return mem_size

   def _get_mem_size_tload_int16(self, tl_csr_val, x31_val):
      stride = get_bits(x31_val, 47, 6) * 64
      r = 1
      count = self.get_num_lines(tl_csr_val) + 1
      i = count - 1
      offset = get_bits(tl_csr_val, 5, 5) * 32
      VA = offset
      mem_size = (VA + (i * 2 + r) * stride) + 32
      return mem_size

   def _get_mem_size_tload_tra8(self, tl_csr_val, x31_val):
      count = self.get_num_lines(tl_csr_val) + 1
      i = count - 1
      stride = get_bits(x31_val, 47, 6) * 64
      r = 63
      offset = get_bits(tl_csr_val, 5, 4) * 16
      VA = offset
      mem_size = (VA + r*stride + i) + 1
      return mem_size

   def _get_mem_size_tload_tra16(self, tl_csr_val, x31_val):
      count = self.get_num_lines(tl_csr_val) + 1
      i = count - 1
      stride = get_bits(x31_val, 47, 6) * 64
      r = 31
      offset = get_bits(tl_csr_val, 5, 5) * 32
      VA = offset
      mem_size = (VA + r*stride + i*2) + 2
      return mem_size

   def _get_mem_size_tload_tra32(self, tl_csr_val, x31_val):
      count = self.get_num_lines(tl_csr_val) + 1
      i = count - 1
      stride = get_bits(x31_val, 47, 6) * 64
      r = 15
      VA = 0
      mem_size = (VA + r*stride + i*4) + 4
      return mem_size

   def get_mem_size(self, tl_csr_val):
      mem_size = 0
      tenb = self.get_tenb(tl_csr_val) == 1

      if tenb:
         mem_size = self._get_mem_size_tload_tena_tenb(tl_csr_val, self.x31_val)
      else:
         config = self.get_transformation(tl_csr_val)
         switch_case = {
                          TENA  : self._get_mem_size_tload_tena_tenb,
                          INT8  : self._get_mem_size_tload_int8,
                          INT16 : self._get_mem_size_tload_int16,
                          TRA8  : self._get_mem_size_tload_tra8,
                          TRA16 : self._get_mem_size_tload_tra16,
                          TRA32 : self._get_mem_size_tload_tra32
                       }
         if config in switch_case:
            mem_size = switch_case[config](tl_csr_val, self.x31_val)

      if tenb or config in switch_case:
         # To avoid sharing the same cache line between different operations
         # We align the memory size to 64 bytes
         if (mem_size%64) != 0:
            mem_size = (mem_size & 0xffffffffffffffc0) + 64

      return mem_size

   ###################################################

   def set_valid_transformation(self, test_helper: TestHelper, csr_value: int) -> int:
      rand_value = test_helper.pick_one(self.valid_trans)
      return self.set_transformation(csr_value, rand_value)

   def set_invalid_transformation(self, test_helper: TestHelper, csr_value: int) -> int:
      rand_value = test_helper.pick_one(self.invalid_trans)
      return self.set_transformation(csr_value, rand_value)

   def get_tload_base(self, test_helper: TestHelper, tl_csr_val: int, tenb: int, coop: int, trans: int = None)-> int:
      tl_csr_val = tl_csr_val & 0xbfef00000000003f

      if self.tensor_mask != None:
         tl_csr_val = self.set_use_tensor_mask(tl_csr_val, 1)
      else:
         tl_csr_val = self.set_use_tensor_mask(tl_csr_val, 0)

      if tenb == 1:
         tl_csr_val = self.set_tenb(tl_csr_val, 1)
      else:
         tl_csr_val = self.set_tenb(tl_csr_val, 0)

      if trans != None:
         tl_csr_val = self.set_transformation(tl_csr_val, trans)
      else:
         if self.invalid:
            tl_csr_val = self.set_invalid_transformation(test_helper, tl_csr_val)
         else:
            tl_csr_val = self.set_valid_transformation(test_helper, tl_csr_val)
      return tl_csr_val

   def get_tload_wrapper(self, test_helper: TestHelper, trans: int = None) -> int:
      tl_csr_val_rand = test_helper.get_urand_num()
      tl_csr_val = self.get_tload_base(test_helper, tl_csr_val_rand, SET_NON_TENB, SET_NON_COOP, trans = trans)
      return tl_csr_val

   def get_tload_tenb_wrapper(self, test_helper: TestHelper, trans: int = None) -> int:
      tl_csr_val_rand = test_helper.get_urand_num()
      tl_csr_val = self.get_tload_base(test_helper, tl_csr_val_rand, SET_TENB, SET_NON_COOP, trans = trans)
      return tl_csr_val

   def _map_scp_lines(self, method, tl_csr_val: int) -> int:
      config = self.get_transformation(tl_csr_val)
      tenb   = self.get_tenb(tl_csr_val) == 1

      valid_config = config in self.valid_trans

      if valid_config and not tenb:
         dst   = self.get_start_scp_line(tl_csr_val)
         count = self.get_num_lines(tl_csr_val) + 1
         for i in range(count):
            if self.tensor_mask is None or get_bits(self.tensor_mask, i, i) == 1:
               scp_line = (dst + i) % 48
               method(scp_line)

   def selfcheck_boundaries_tload(self, tl_csr_val: int, scp_head: int, scp_tail: int) -> None:
      def check_boundaries(scp_line: int) -> None:
         assert scp_head <= scp_line and scp_line <= scp_tail
      self._map_scp_lines(check_boundaries, tl_csr_val)

   def selfcheck_set_tload(self, l1_scp: L1SCPModel, tl_csr_val: int, selfcheck: int, setm: int) -> None:
      # Make sure that at least one of the two options are set
      assert selfcheck or setm
      def selfcheck_set(scp_line: int) -> None:
         if selfcheck == 1:
            assert l1_scp.is_free(scp_line)
         if setm == 1:
            l1_scp.set_write_only(scp_line, self.name)
      self._map_scp_lines(selfcheck_set, tl_csr_val)

   def tl_get_empty_l1scp_range(self, test_helper: TestHelper, l1_scp: L1SCPModel) -> (int, int):
      free_regions = l1_scp.get_write_free_regions()
      region = test_helper.pick_one(free_regions)
      return region

   def constrain_tload_write_lines(self, test_helper: TestHelper, tl_csr_val: int, scp_head: int, scp_tail: int, scp_size = 48) -> int:
      ret = tl_csr_val
      config = self.get_transformation(tl_csr_val)
      tenb   = self.get_tenb(tl_csr_val) == 1
      valid_config = config in self.valid_trans

      if valid_config and not tenb:
         (start_scp_line, num_scp_lines) = randomize_scp_lines(test_helper, scp_head, scp_tail, tensor_mask = self.tensor_mask, scp_size = scp_size)

         ret = self.set_start_scp_line(ret, start_scp_line)
         ret = self.set_num_lines(ret, num_scp_lines)
         self.selfcheck_boundaries_tload(ret, scp_head, scp_tail)

      return ret

   @classmethod
   def free_resources(cls, model: 'Model', name: str, tw: bool = False) -> None:
      if tw is True:
         l1_scp = model.l1scp
         # Free all the L1SCP resources used by this same operation as HW cannot execute
         # the same tensor operation at the same time (implicit tensor wait)
         l1_scp.free_resources(name)

   def get_encoding(self, model: 'Model') -> int:

      test_helper = model.test_helper
      l1_scp = model.l1scp

      if TEST_CONF.USE_TENSOR_MASK_MODEL == 1:
         self.tensor_mask = test_helper.pick_one([None, model.tensor_mask])
      else:
         self.tensor_mask = test_helper.pick_one([None, 0xffff])

      if self.tenb == 1:
         tl_csr_val = self.get_tload_tenb_wrapper(test_helper, trans = self.trans)
         if self.__tenb_num_lines != None:
            num_lines = int(str(self.__tenb_num_lines))
            tl_csr_val = self.set_num_lines(tl_csr_val, num_lines)
      else:
         tl_csr_val = self.get_tload_wrapper(test_helper, trans = self.trans)
         if not self.invalid:
            (head, tail) = self.tl_get_empty_l1scp_range(test_helper, l1_scp)
            tl_csr_val = self.constrain_tload_write_lines(test_helper, tl_csr_val, head, tail, l1_scp.SCP_SIZE)

      if self.__tenb_num_lines == None:
         self.__tenb_num_lines.set_value(self.get_num_lines(tl_csr_val))

      if self.coop is True:
         tl_csr_val = self.set_is_coop(tl_csr_val, 1)
      else:
         tl_csr_val = self.set_is_coop(tl_csr_val, 0)

      return tl_csr_val

   def post_encoding(self, model: 'Model', tl_csr_val: int):
      tm = self.get_use_tensor_mask(tl_csr_val)
      if tm == 1:
         if TEST_CONF.USE_TENSOR_MASK_MODEL == 1:
            self.tensor_mask = model.tensor_mask
         else:
            self.tensor_mask = 0xffff
      else:
         self.tensor_mask = None

      l1_scp = model.l1scp
      self.selfcheck_set_tload(l1_scp, tl_csr_val, 1, 1)
      self.memory_size.set_value(self.get_mem_size(tl_csr_val))
