from abc import ABCMeta, abstractmethod
from core.models.resource import Resource
from core.models.test_helper import TestHelper
from core.models.immmodel import Imm

def get_bits(value: int, head: int, tail: int):
    # Just in case the programmer does not specify the arguments in the correct order
    if head < tail:
        tmp = head
        head = tail
        tail = tmp

    # we want to extract k-bits
    k = (head - tail) + 1
    return (((1 << k) - 1) & (value >> tail))

def get_bits_signed(value: int, head: int, tail: int):
   # Just in case the programmer does not specify the arguments in the correct order
   if head < tail:
        tmp = head
        head = tail
        tail = tmp
   bits = get_bits(value, head, tail)
   sign = get_bits(value, head, head)

   if sign == 1:
      k = (head - tail) + 1
      mask = ((1 << k) - 1)
      bits = ((~bits) + 1) & mask
      bits = bits * -1
   return bits

def set_bits(value: int, head: int, tail: int, set_value: int):
    # Just in case the programmer does not specify the arguments in the correct order
    if head < tail:
        tmp = head
        head = tail
        tail = tmp

    # we want to set k-bits
    k = (head - tail) + 1
    mask = ((1 << k) - 1)

    set_value = (set_value & mask) << tail
    mask = ~(mask << tail)

    value = value & mask
    value = value | set_value

    return value

def randomize_scp_lines(test_helper: TestHelper, scp_head: int, scp_tail: int, tensor_mask: int = None, scp_size = 48, min_size = 0) -> (int, int):
      zlsb_tmask = 0
      zmsb_tmask = 0
      if tensor_mask is not None:
         for i in range(16):
            if get_bits(tensor_mask, i, i) == 0:
               zlsb_tmask += 1
            else:
               break

         for i in reversed(range(16)):
            if get_bits(tensor_mask, i, i) == 0:
               zmsb_tmask += 1
            else:
               break

      assert ((scp_tail - scp_head) + 1) >= min_size, f"Requesting at least {min_size} SCP lines, but found scp_head({scp_head}) and scp_tail({scp_tail})"
      start_scp_line = test_helper.get_rand_range(scp_head-zlsb_tmask, scp_tail-min_size)
      num_scp_lines  = scp_tail - start_scp_line

      if (start_scp_line + (16 - zmsb_tmask)) <= scp_tail:
         num_scp_lines += zmsb_tmask
         num_scp_lines  = min(num_scp_lines, 15)
         num_scp_lines  = test_helper.get_rand_range(min_size, num_scp_lines)

      if start_scp_line < 0:
         start_scp_line = scp_size + start_scp_line

      assert num_scp_lines >= min_size, f"Requesting at least {min_size} SCP lines, but found {num_scp_lines}"
      return (start_scp_line, num_scp_lines)

class TensorEncoding(Imm, metaclass=ABCMeta):

   def set_val(self, model: 'Model'):
      raise ValueError(f"Unexpected call to set_val() in class {self.__class__.__name__}")

   @abstractmethod
   def get_encoding(self, model: 'Model'):
      pass

   @abstractmethod
   def free_resources(self, model: 'Model', csr_val: int):
      pass

   @abstractmethod
   def post_encoding(self, model: 'Model', csr_val: int):
      pass

   def allocate(self, model: 'Model'):
      if self.__class__.__name__ in ["TensorQuantEncoding","TensorFMAEncoding", "TensorReduceEncoding"]:
         self.free_resources(model, self.name, from_tensor_wait = True)
      else:
         self.free_resources(model, self.name)
      if self.value is None:
         assert self._count > 0, f"Cannot set a value to a resource whose references count is <= 0. Current count is {self._count}"
         if self.rand_value is None:
            self.rand_value = self.get_encoding(model)
            self._imm_str = hex(self.rand_value)
            self.post_encoding(model, self.rand_value)
      else:
         assert self.rand_value is None
         self.post_encoding(model, self.value)
