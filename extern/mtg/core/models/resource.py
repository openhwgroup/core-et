from enum import Enum
from abc import ABCMeta, abstractmethod

class ResourceState(Enum):
   INVALID   = 1
   ALLOCATED = 2
   RESERVED  = 3
   READONLY  = 4
   WRITEONLY = 5
   SOFT_ALLOC = 6

class ResourcePredict(Enum):
   PREDICTABLE = 11
   UNPREDICTABLE = 12

class Resource(metaclass=ABCMeta):

   def __init__(self):
      self._count = 0
      self._soft_alloc_count = 0
      self.__state = ResourceState.INVALID
      self.set_predictable()

   def _set_count(self, num: int) -> None:
      assert self._count == 0, f"Cannot set the count for a soft-allocated resource that still allocated." if self._soft_alloc_count > 0 else f"This Resource count value is not 0 (curr val is {self._count}). Provided count is {num}."
      self._count = num

   def _incr_count(self) -> None:
      self._count += 1

   def _get_count(self) -> int:
      return self._count

   def reserve(self) -> None:
      assert self.__state == ResourceState.INVALID
      self.__state = ResourceState.RESERVED

   def alloc(self) -> None:
      assert self.__state == ResourceState.INVALID or self.__state == ResourceState.SOFT_ALLOC, f"Failed to allocate the resource ({self.__repr__()})"
      if self.__state == ResourceState.SOFT_ALLOC:
         assert self._soft_alloc_count > 0
      self.__state = ResourceState.ALLOCATED

   def soft_alloc(self) -> None:
      assert self.__state == ResourceState.INVALID or self.__state == ResourceState.SOFT_ALLOC, f"Failed to soft-allocate the resource ({self.__repr__()})"
      self.__state = ResourceState.SOFT_ALLOC
      self._soft_alloc_count += 1

   def set_read_only(self) -> None:
      assert self.__state == ResourceState.INVALID or self.__state == ResourceState.READONLY
      self.__state = ResourceState.READONLY

   def set_write_only(self) -> None:
      assert self.__state == ResourceState.INVALID
      self.__state = ResourceState.WRITEONLY

   def is_read_only(self) -> None:
      return self.__state == ResourceState.READONLY

   def is_write_only(self) -> None:
      return self.__state == ResourceState.WRITEONLY

   def get_state(self) -> int:
      return self.__state

   def is_used(self) -> bool:
      return self.get_state() is not ResourceState.INVALID

   def is_free(self) -> None:
      return not self.is_used()

   def _free(self) -> None:
      assert self._count == 0
      self.__state = ResourceState.INVALID

   def free(self) -> None:
      assert self.__state != ResourceState.RESERVED, "Cannot free a RESERVED resource"
      self._count -= 1
      assert self._count >= 0, "Called free in a Physical Resource before allocating"
      if self._soft_alloc_count > 0:
         self._soft_alloc_count -= 1
      if (self._count == 0):
         if (self._soft_alloc_count == 0):
            self.__state = ResourceState.INVALID
         else:
            self.__state = ResourceState.SOFT_ALLOC

   def set_predictable(self) -> None:
      self.__predict = ResourcePredict.PREDICTABLE

   def set_unpredictable(self) -> None:
      self.__predict = ResourcePredict.UNPREDICTABLE

   def is_predictable(self) -> ResourcePredict:
      return self.__predict is ResourcePredict.PREDICTABLE

   @abstractmethod
   def __str__(self):
      return self.__state.name

   @abstractmethod
   def __repr__(self):
      if self.__state == ResourceState.SOFT_ALLOC:
         return f"{self.__state.name}[{self._soft_alloc_count}] ({self._count})"
      else:
         return f"{self.__state.name} ({self._count})"


class VirtualResource(metaclass=ABCMeta):

   def __init__(self, reg_num: int = None) -> None:
      self.link(None)
      self._count = 0
      self._phy_resource_str = "<virtual>"
      self.reg_num = reg_num

   def link(self, phy_resource: Resource):
      # When you link a resource, you set the self._phy_resource with a reference of a physical resource
      if phy_resource is not None:
         assert isinstance(phy_resource, Resource), "This physical resource is not a Resource instance"
         # And update the physical resource with the count number of the virtual resource
         if self.reg_num is None:
            assert phy_resource.get_state() == ResourceState.INVALID, f"The physical resource you are trying to link is already in use/linked. Physical resource is {repr(phy_resource)}"
         phy_resource._set_count(self._count)
         self._count = 0
      self._phy_resource = phy_resource

   def free(self):
      assert self._phy_resource is not None, "This virtual resource has not been linked to a physical resource yet"
      self._phy_resource.free()
      if not self._phy_resource.is_used():
         self._phy_resource = None

   def __repr__(self):
      return f"Virtual Resource: {self._phy_resource_str}"

   def __str__(self):
      return self._phy_resource_str

   def inc_refs(self) -> None:
      self._count += 1

   def not_linked(self) -> bool:
      return self._phy_resource is None

   @abstractmethod
   def allocate(self, phy_resource: Resource) -> None:
      if self.not_linked():
         assert phy_resource is not None, "Passed Resource is 'None'"
         self.link(phy_resource)
         phy_resource.alloc()
         self._phy_resource_str = str(phy_resource)

   @abstractmethod
   def soft_allocate(self, phy_resource: Resource) -> None:
      assert phy_resource is not None, "Passed Resource is 'None'"
      phy_resource.soft_alloc()
