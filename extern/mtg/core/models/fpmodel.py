from core.models.resource import Resource, VirtualResource
from core.models.test_helper import TestHelper
from core.models.abimodel import *

class pFreg(Resource):

   def __init__(self, reg_num: int = 0) -> None:
      # Sanity check
      assert (reg_num >= 0) and (reg_num < FPModel.RF_SIZE), f"FP Register File size is {FPModel.RF_SIZE}. Provided 'f{reg_num}'."

      # Instance attribute (unique per instance)
      self.reg_num = reg_num

      super().__init__()

   def __repr__(self) -> str:
      return f"f{self.reg_num}: {super().__repr__()}"

   def __str__(self) -> str:
      return f'f{self.reg_num}'

class Freg(VirtualResource):
   def allocate(self, model: 'Model') -> None:
      assert model is not None, "Passed Model is 'None'"
      try:
         phy_resource = model.floatpoint.get_reg(self.reg_num)
         super().allocate(phy_resource)
      except IndexError as exc:
         model.floatpoint.print_all()
         raise Exception(f"VirtualResource of type {self.__class__.__name__} failed to link to a physical resource. Passed Model runned out of free resources") from exc

   def soft_allocate(self, model: 'Model') -> None:
      assert model is not None, "Passed Model is 'None'"
      assert self.reg_num is not None, "Soft allocation must only be used for predefined resources"
      try:
         phy_resource = model.floatpoint.get_reg(self.reg_num)
         super().soft_allocate(phy_resource)
      except IndexError as exc:
         model.floatpoint.print_all()
         raise Exception(f"VirtualResource of type {self.__class__.__name__} failed to link to a physical resource. Passed Model runned out of free resources") from exc

class Frm:

   rm = {'rne': 0b000,
         'rtz': 0b001,
         'rdn': 0b010,
         'rup': 0b011,
         'rmm': 0b100,
         'dyn': 0b111
        }

   def __init__(self, rm: str = None) -> None:
      if rm is not None:
         assert rm in self.rm, f"Valid rounding modes are {str(list(self.rm.keys()))}. Passed '{rm}'."
      self.value = rm
      self.rand_value = None
      if rm is None:
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
            keys = list(self.rm.keys())
            self.rand_value = model.test_helper.pick_one(keys)
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
      return str(f"{self._imm_str} ({format(self.rm[self._imm_str], '#05b')})")

   def __str__(self) -> str:
      return str(self._imm_str)


class FPModel:

   RF_SIZE = 32
   LANE_SIZE = 4
   NUM_LANES = 8

   def __init__(self, test_helper: TestHelper) -> None:
      self.__test_helper = test_helper
      self.__rf = [pFreg(i) for i in range(self.RF_SIZE)]
      self.__log = [set() for i in range(self.RF_SIZE)]

   def get_avail_regs(self) -> list:
      return [x for x in self.__rf if not x.is_used()]

   def exclude_reg(self, reg: int) -> None:
      assert 0 <= reg < self.RF_SIZE
      self.__rf[reg].reserve()

   def reset_reg(self, reg: int) -> None:
      assert 0 <= reg < self.RF_SIZE
      self.__rf[reg]._free()

   def set_ABI_reg(self, abi_reg: ABIreg):
      used = self.__rf[abi_reg.reg_num].is_used()
      assert not used, f"Trying to set {abi_reg.name} but {str(abi_reg)} is already in use"
      self.__rf[abi_reg.reg_num] = abi_reg

   def get_reg(self, num: int = None) -> pFreg:
      if num is None:
         filtered = self.get_avail_regs()
         reg = self.__test_helper.pick_one(filtered)
      elif isinstance(num, list):
         filtered = self.get_avail_regs()
         filtered = [x for x in filtered if x.reg_num in num]
         assert len(filtered) > 0, f"Floating Point candidate registers are {filtered}, but none of them are available."
         return self.__test_helper.pick_one(filtered)
      else:
         assert num < self.RF_SIZE, f"Provided register number ({num}) is bigger than the Floating Point RF size ({self.RF_SIZE})"
         reg = self.__rf[num]
      return reg

   # Tensor related methods
   ##############################################

   def free_resources(self, op_type: str):
      idx = 0
      for i in self.__log:
         if op_type in i:
            assert len(i) == self.__rf[idx]._get_count()
            i.remove(op_type)
            self.__rf[idx].free()
         idx += 1

   def _get_free_regions(self, method):
      regions = list()
      head = None
      for i in self.__rf:
         if method(i):
            if head is None:
               head = i.reg_num
         elif head is not None:
            tail = i.reg_num - 1
            assert head <= tail
            regions.append((head, tail))
            head = None

      if head is not None:
         last = self.__rf[-1]
         tail = last.reg_num
         assert head <= tail
         regions.append((head, tail))

      assert len(regions) > 0
      return regions

   def get_write_free_regions(self):
      try:
         return self._get_free_regions(lambda x: x.is_free())
      except Exception as exc:
         raise Exception(f"{self.__class__.__name__} ran out of write free regions") from exc

   def get_read_free_regions(self):
      try:
         return self._get_free_regions(lambda x: x.is_free() or x.is_read_only())
      except Exception as exc:
         raise Exception(f"{self.__class__.__name__} ran out of read free regions") from exc

   def is_free(self, reg: int) -> bool:
      return self.__rf[reg].is_free()

   def is_read_only(self, reg: int) -> bool:
      return self.__rf[reg].is_read_only()

   def is_write_only(self, reg: int) -> bool:
      return self.__rf[reg].is_write_only()

   def set_read_only(self, reg: int, op_type: str) -> None:
      assert not self.is_write_only(reg)
      assert op_type not in self.__log[reg]
      self.__rf[reg].set_read_only()
      self.__rf[reg]._incr_count()
      self.__log[reg].add(op_type)

   def set_write_only(self, reg: int, op_type: str) -> None:
      assert self.is_free(reg)
      assert op_type not in self.__log[reg]
      self.__rf[reg].set_write_only()
      self.__rf[reg]._incr_count()
      self.__log[reg].add(op_type)

   def print_all(self):
      for i in range(self.RF_SIZE):
         print(f"{repr(self.__rf[i])} : {repr(self.__log[i])}")
