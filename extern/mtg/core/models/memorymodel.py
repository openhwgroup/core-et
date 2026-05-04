
from abc import ABCMeta, abstractmethod
from functools import reduce
from core.models.test_helper import TestHelper
from core.test_conf import *

class MemoryMeta(ABCMeta):

   _instances = {}

   def __call__(cls, *args, **kwargs):
      if cls not in cls._instances:
         # Check that new regions do not overlap with existing ones
         for i in cls._instances.values():
            i.check_overlap(cls)
         instance = super().__call__(*args, **kwargs)
         cls._instances[cls] = instance
      return cls._instances[cls]

class Memory(metaclass=ABCMeta):

   @abstractmethod
   def __init__(self, test_helper : TestHelper):
      pass

   @abstractmethod
   def get_mem(self, size, align: int = None, slices: int = 1):
      pass

   @property
   def name(self) -> str:
      return self.__class__.__name__

class MemoryRegion(Memory, metaclass=MemoryMeta):
   exclusive: bool = True
   enable:    bool = True
   paddr:     int  = None
   size:      int  = None
   export:    bool = True

   def __init__(self, test_helper : TestHelper):
      self._head = self.paddr
      self._max_addr = self.paddr + self.size
      self.test_helper = test_helper
      self.used = False

   @classmethod
   def dflt_value(cls):
      return None

   def get_mem(self, size, align: int = None, slices: int = 1, align_cl = True):
      assert self.enable is True, f"{self.name} is disabled."
      assert size > 0, f"{self.name} size is 0."

      if not self.exclusive:
         assert slices == 1, f"Non-exclusive memory ({self.__class__.__name__}) must be allocated using StaticMalloc or StaticGMalloc."
      elif align_cl:
         # Align self._head to cacheline such that future memory allocations will not use the same cacheline
         # This is required to avoid coherency issues
         self._get_mem(0, 64);

      if align is None:
         align = self.test_helper.get_rand_range(0, 63)

      base_addr = self._get_mem(size, align)
      slice_size = 0
      for i in range(1, slices):
         addr = self._get_mem(size, align)
         if i == 1:
            slice_size = addr - base_addr

      if self.exclusive and align_cl:
         # Align self._head to cacheline such that future memory allocations will not use the same cacheline
         # This is required to avoid coherency issues
         self._get_mem(0, 64);

      self.used = True
      return base_addr, slice_size

   def _get_mem(self, size, align):

      if align != 0:
         mod = self._head % align
         if mod != 0:
            self._head += (align - mod)

      assert (self._head + size) <= self._max_addr, f"Cannot allocate more memory in {self.name}."
      addr = self._head

      if self.exclusive:
         self._head += size
      return addr

   def get_used_size(self) -> int:
      if self.exclusive:
         return self._head - self.paddr
      else:
         return self.size

   def get_avail_size(self, align = 0):
      head = self._head
      if align != 0:
         mod = head % align
         if mod != 0:
            head += (align - mod)
      return (self.paddr + self.size) - head

   @classmethod
   def check_overlap(cls, other_region: 'MemoryRegion') -> bool:
      my_head = cls.paddr
      my_tail = cls.paddr + cls.size -1
      other_head = other_region.paddr
      other_tail = other_head + other_region.size - 1
      if my_head < other_head:
         assert my_tail < other_head, f"{other_region.__name__}[{hex(other_head)}:{hex(other_tail)}] overlaps with an existing region {cls.__name__}[{hex(my_head)}:{hex(my_tail)}]"
      else:
         assert other_tail < my_head, f"{other_region.__name__}[{hex(other_head)}:{hex(other_tail)}] overlaps with an existing region {cls.__name__}[{hex(my_head)}:{hex(my_tail)}]"

   def get_name(self):
      return self.__class__.__name__

   def sanity_check(self, model: 'Model') -> None:
      pass

class MemorySet(Memory):
   mem_regions = dict()

   def __init__(self, test_helper : TestHelper):
      assert isinstance(self.mem_regions, dict), f"{self.name}.mem_regions must be a dictionary."

      self.test_helper = test_helper
      self._regions = list()
      self._weights = list()
      self._set_regions(test_helper)
      self.region = None
      assert len(self._regions) != 0, f"All the {self.name} regions are disabled or no memory regions have been defined in the set"

   def _set_regions(self, test_helper: TestHelper):
      for k, v in self.mem_regions.items():
         instance = k(test_helper)
         if isinstance(instance, MemorySet):
            mem_set_total_weights = reduce(lambda a, b: a + b, instance._weights)
            mem_set_weight = v/mem_set_total_weights
            relat_weight = [w*mem_set_weight for w in instance._weights]
            self._regions += instance._regions
            self._weights += relat_weight
            expected_weights = round(reduce(lambda a, b: a + b, relat_weight))
            assert expected_weights == v, f"{self.__class__.__name__} weights error: {k.__name__} MemorySet expected total weight is {v}, found {expected_weights}"
         else:
            if instance.enable:
               self._regions.append(instance)
               self._weights.append(v)

   def get_mem(self, size, align: int = None, slices: int = 1, align_cl = True):
      all_slices_size = (size + (size - (size%align))) * slices
      candidates = list()
      candidates_weight = list()
      for region, weight in zip(self._regions, self._weights):
         if region.get_avail_size(align=align) >= all_slices_size:
            candidates.append(region)
            candidates_weight.append(weight)
      assert len(candidates) > 0, f"Not enough space left in any of the memory regions in the memory set {self.name}"
      self.region = self.test_helper.pick_one_weight(candidates, weights = candidates_weight)
      try:
         return self.region.get_mem(size, align, slices, align_cl)
      except Exception as exc:
         raise Exception(f"{self.name} memory set failed to allocate memory in {region.name}[{size}]@{align}") from exc

   def get_name(self):
      return f"{self.__class__.__name__}__{self.region.get_name()}"

   def sanity_check(self, model: 'Model') -> None:
      for reg in self._regions:
         reg.sanity_check(model)

class Malloc:
   def __init__(self, region: Memory, size: int, align: int = None, align_cl:int = True):
      self.mem_region = region
      self.size = size
      self.align = align
      self.addr = None
      self._addr_repr = hex(0xbadc0de)
      self._count = 0
      self.hart_offset = None
      self.slices = self.get_slices()
      self.region_name = None
      self.align_cl = align_cl

   def get_slices(self):
      if TEST_CONF.UNIQ_PC:
         slices = 1
      else:
         slices = TEST_CONF.NUM_HARTS
      return slices

   def inc_refs(self) -> None:
      self._count += 1

   def allocate(self, model: 'Model'):
      region = self.mem_region(model.test_helper)
      assert isinstance(region, Memory), "Memory region must be instance of 'Memory'"
      try:
         if self.addr is None:
            region.sanity_check(model)
            self.addr, self.hart_offset = region.get_mem(self.size, self.align, self.slices, self.align_cl)
            self._addr_repr = hex(self.addr)
            self.region_name = region.get_name()
      except Exception as exc:
         raise Exception(f"{self.__class__.__name__} failed to allocate memory in {region.name}, size: {self.size}, align: {self.align}") from exc

   def free(self):
      assert self.addr is not None, "This memory resource has not been allocated yet"
      self._count -= 1
      if self._count == 0:
         self.addr = None

   def get_region_name(self):
      assert self.region_name is not None
      return self.region_name

   def __repr__(self) -> str:
      return f"{self._addr_repr} /*{self.get_region_name()}[{self.slices}][{self.size}]@{self.align}*/"

   def __str__(self) -> str:
      return f"{self._addr_repr} /*{self.get_region_name()}[{self.slices}][{self.size}]@{self.align}*/"

class CRC16Malloc(Malloc):

   def inc_refs(self) -> None:
      pass

   def free(self):
      assert self.addr is not None, "This memory resource has not been allocated yet"

class StaticMalloc(Malloc):

   # The StaticMalloc class is like Malloc, but with one difference. With a Malloc class, you keep track of the references
   # so if you instantiate one Component and render() it multiple times, you will get different memory addresses every time
   # however, if you declare the memory as a StaticMalloc, every time to call render() for the same Component instance,
   # you will get the same memory address

   def get_slices(self):
      return 1

   def inc_refs(self) -> None:
      pass

   def free(self):
      assert self.addr is not None, "This memory resource has not been allocated yet"

class GMalloc(Malloc):

   _global_allocs = dict()

   def __init__(self, var_name: str, region: Memory, size: int, align: int = None, align_cl = True):
      assert align is not None, "Memory alignment is required for Global Memory"
      self.var_name = var_name
      super().__init__(region, size, align, align_cl = align_cl)

   def allocate(self, model: 'Model') -> None:
      dict_key = self.var_name
      if dict_key in self._global_allocs:
         region = self._global_allocs[dict_key][2]
         size = self._global_allocs[dict_key][3]
         align = self._global_allocs[dict_key][4]

         assert self.mem_region == region, f"Global memory region mismatch for key '{dict_key}'. Previous region was {region}. Current region is {self.mem_region}"
         assert self.size == size, f"Global memory size mismatch for key '{dict_key}'. Previous size was {size}. Current size is {self.size}"
         assert self.align == align, f"Global memory align mismatch for key '{dict_key}'. Previous align was {align}. Current align is {self.align}"

         self.addr = self._global_allocs[dict_key][0]
         self._addr_repr = hex(self.addr)
         self.hart_offset = self._global_allocs[dict_key][1]
         self.region_name = self._global_allocs[dict_key][5]
      else:
         super().allocate(model)
         assert self.region_name is not None
         self._global_allocs[dict_key] = (self.addr, self.hart_offset, self.mem_region, self.size, self.align, self.region_name)

class StaticGMalloc(GMalloc):

   # The StaticGMalloc class is like GMalloc, but with one difference. With a GMalloc class, you keep track of the references
   # so if you instantiate one Component and render() it multiple times, you will get different memory addresses every time
   # however, if you declare the memory as a StaticGMalloc, every time to call render() for the same Component instance,
   # you will get the same memory address.

   def get_slices(self):
      return 1

   def inc_refs(self) -> None:
      pass

   def free(self):
      assert self.addr is not None, "This memory resource has not been allocated yet"
