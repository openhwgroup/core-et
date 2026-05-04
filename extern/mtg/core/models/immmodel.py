from core.models.resource import Resource
from core.models.test_helper import TestHelper

class Imm:

   signed = True
   nbits = 64

   def __init__(self, value: int = None) -> None:
      self.value = value
      self.rand_value = None
      if value is None:
         self._count = 0
         self._imm_str = None
      else:
         self._imm_str = str(self.value)

   def inc_refs(self) -> None:
      if self.value is None:
         self._count += 1

   def set_val(self, model: 'Model'):
      if self.value is None:
         assert self._count > 0, f"Cannot set a value to a resource whose references count is <= 0. Current count is {self._count}"
         if self.rand_value is None:
            if self.signed:
               self.rand_value = model.test_helper.get_rand_signed_kbits(self.nbits)
            else:
               self.rand_value = model.test_helper.get_rand_kbits(self.nbits)
            self._imm_str = str(self.rand_value)
      else:
         assert self.rand_value is None

   def free(self):
      if self.value is None:
         self._count -= 1
         assert self._count >= 0, "Reference counter must be >= 0. Double free detected."
         if self._count == 0:
            self.rand_value = None
      else:
         assert self.rand_value is None

   def __repr__(self) -> str:
      return str(self._imm_str)

   def __str__(self) -> str:
      return str(self._imm_str)

class SignedImm(Imm):

   signed = True

   def __init__(self, value: int = None) -> None:
      if value is not None:
         max_val = (2**(self.nbits-1)) - 1
         min_val = -1 * max_val

         # Sanity check
         assert (value >= min_val) and (value <= max_val), f"Value overflows signed {self.nbits}-bits. Provided {value}"

      super().__init__(value)

class UnsignedImm(Imm):

   signed = False

   def __init__(self, value: int = None) -> None:
      if value is not None:
         max_val = (2**(self.nbits)) - 1
         min_val = 0

         # Sanity check
         assert (value >= min_val) and (value <= max_val), f"Value overflows unsigned {self.nbits}-bits. Provided {value}"

      super().__init__(value)

class Zimm64(UnsignedImm):
   nbits = 64

class Zimm20(UnsignedImm):
   nbits = 20

class Zimm8(UnsignedImm):
   nbits = 8

class Zimm6(UnsignedImm):
   nbits = 6

class Zimm5(UnsignedImm):
   nbits = 5

class Zimm3(UnsignedImm):
   nbits = 3

class Imm32(SignedImm):
   nbits = 32

class Imm20(SignedImm):
   nbits = 20

class Imm12(SignedImm):
   nbits = 12

class Imm10(SignedImm):
   nbits = 10

class Imm6(SignedImm):
   nbits = 6

class Imm5(SignedImm):
   nbits = 5
