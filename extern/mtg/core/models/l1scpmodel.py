from core.models.resource import Resource
from core.models.test_helper import TestHelper
from core.models.resource import ResourceState

class SCPreg(Resource):
   
   def __init__(self, reg_num: int = 0) -> None:
      # Sanity check
      assert (reg_num >= 0) and (reg_num < L1SCPModel.SCP_SIZE), f"L1 SCP size is {L1SCPModel.RF_SIZE}. Provided 'SCP[{reg_num}]'."

      # Instance attribute (unique per instance)
      self.reg_num = reg_num

      super().__init__()

   def free(self) -> None:
      self._count -= 1
      assert self._count >= 0, "Called free in a Physical Resource before allocating"
      if (self._count == 0):
         self._free()

   def alloc(self) -> None:
      # SCP cannot alloc
      raise ValueError(f"Unexpected call to alloc() in class {self.__class__.__name__}")

   def reserve(self) -> None:
      # SCP cannot reserve
      raise ValueError(f"Unexpected call to reserve() in class {self.__class__.__name__}")

   def set_write_only(self) -> None:
      if self._count == 0:
         super().set_write_only()
      self._count += 1

   def set_read_only(self) -> None:
      if self._count == 0:
         super().set_read_only()
      self._count += 1

   def __repr__(self) -> str:
      return f"SCP{self.reg_num}: {super().__repr__()}"

   def __str__(self) -> str:
      return f'SCP{self.reg_num}'

class L1SCPModel:

   SCP_SIZE = 48 # 48 lines of 64 Bytes each

   def __init__(self, test_helper: TestHelper) -> None:
      self.__test_helper = test_helper
      self.__scp = [SCPreg(i) for i in range(self.SCP_SIZE)]
      self.__log = [set() for i in range(self.SCP_SIZE)]

   def get_usage(self, op_type: str):
      count = 0
      for i in self.__log:
         if op_type in i:
            count += 1
      return count

   def free_resources(self, op_type: str):
      idx = 0
      for i in self.__log:
         assert len(i) == self.__scp[idx]._get_count()
         if op_type in i:
            i.remove(op_type)
            self.__scp[idx].free()
         idx += 1

   def _get_free_regions(self, method):
      regions = list()
      head = None
      for i in self.__scp:
         if method(i):
            if head is None:
               head = i.reg_num
         elif head is not None:
            tail = i.reg_num - 1
            assert head <= tail
            regions.append((head, tail))
            head = None

      if head is not None:
         last = self.__scp[-1]
         tail = last.reg_num
         assert head <= tail
         regions.append((head, tail))

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

   def is_free(self, line: int) -> bool:
      return self.__scp[line].is_free()

   def is_read_only(self, line: int) -> bool:
      return self.__scp[line].is_read_only()

   def is_write_only(self, line: int) -> bool:
      return self.__scp[line].is_write_only()

   def set_read_only(self, line: int, op_type: str) -> None:
      assert op_type not in self.__log[line]
      self.__scp[line].set_read_only()
      self.__log[line].add(op_type)

   def set_write_only(self, line: int, op_type: str) -> None:
      assert self.is_free(line)
      assert op_type not in self.__log[line]
      self.__scp[line].set_write_only()
      self.__log[line].add(op_type)

   def get_state(self) -> list:
      return self.__scp

   def print_all(self):
      for i in range(self.SCP_SIZE):
         print(f"{repr(self.__scp[i])} : {repr(self.__log[i])}")
