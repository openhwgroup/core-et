from core.models.resource import Resource, VirtualResource
from core.models.test_helper import TestHelper
from core.models.abimodel import *

class pXreg(Resource):
   
   def __init__(self, reg_num: int = 0) -> None:
      # Sanity check
      assert (reg_num >= 0) and (reg_num < IntModel.RF_SIZE), f"Integer Register File size is {IntModel.RF_SIZE}. Provided 'x{reg_num}'."

      # Instance attribute (unique per instance)
      self.reg_num = reg_num

      super().__init__()

   def __repr__(self) -> str:
      return f"x{self.reg_num}: {super().__repr__()}"

   def __str__(self) -> str:
      return f'x{self.reg_num}'

class Xreg(VirtualResource):
   predictable = True
   def allocate(self, model: 'Model') -> None:
      assert model is not None, "Passed Model is 'None'"
      try:
         if self.not_linked():
            phy_resource = model.integer.get_reg(self.reg_num, pred_only=self.predictable)
            super().allocate(phy_resource)
            self.post_allocate(model.integer, phy_resource)
      except IndexError as exc:
         model.integer.print_all()
         raise Exception(f"VirtualResource of type {self.__class__.__name__} failed to link to a physical resource. Passed Model runned out of free resources") from exc

   def soft_allocate(self, model: 'Model') -> None:
      assert model is not None, "Passed Model is 'None'"
      assert self.reg_num is not None, "Soft allocation must only be used for predefined resources"
      try:
         if self.not_linked():
            phy_resource = model.integer.get_reg(self.reg_num, pred_only=self.predictable)
            super().soft_allocate(phy_resource)
      except IndexError as exc:
         model.integer.print_all()
         raise Exception(f"VirtualResource of type {self.__class__.__name__} failed to link to a physical resource. Passed Model runned out of free resources") from exc

   def post_allocate(self, model: 'IntModel', phy_resource: Resource) -> None:
      pass

class UXreg(Xreg):
   predictable = False
   def post_allocate(self, model: 'IntModel', phy_resource: Resource) -> None:
      model.set_unpredictable(phy_resource)

class IntModel:

   RF_SIZE = 32
   unpredictable = set()

   def __init__(self, test_helper: TestHelper) -> None:
      self.__test_helper = test_helper
      self.__rf = [pXreg(i) for i in range(self.RF_SIZE)]

   def set_unpredictable(self, phy_rsrc : pXreg) -> None:
      self.unpredictable.add(phy_rsrc)

   def set_predictable(self, phy_rsrc : pXreg) -> None:
      assert phy_rsrc in self.unpredictable, "The register {phy_rsrc} is already predictable"
      self.unpredictable.remove(phy_rsrc)

   def is_unpredictable(self, phy_rsrc : pXreg) -> bool:
      return phy_rsrc in self.unpredictable

   def get_avail_regs(self, pred_only=True) -> list:
      if not pred_only:
         ret = [x for x in self.unpredictable if not x.is_used()]
         if len(ret) > 0:
            return ret
         else:
            return [x for x in self.__rf if not x.is_used()]
      else:
         return [x for x in self.__rf if not x.is_used() and x not in self.unpredictable]

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

   def get_reg(self, num: int = None, pred_only = True) -> pXreg:
      if num is None:
         filtered = self.get_avail_regs(pred_only=pred_only)
         assert filtered != None
         reg = self.__test_helper.pick_one(filtered)
      elif isinstance(num, list):
         filtered = self.get_avail_regs(pred_only=pred_only)
         filtered = [x for x in filtered if x.reg_num in num]
         assert len(filtered) > 0, f"Integer candidate registers are {filtered}, but none of them are available."
         return self.__test_helper.pick_one(filtered)
      else:
         assert all([x.reg_num != num for x in self.unpredictable]), f"Trying to allocate register x{num} but its content is unpredictable"
         assert num < self.RF_SIZE, f"Provided register number ({num}) is bigger than the Integer RF size ({self.RF_SIZE})"
         reg = self.__rf[num]
      return reg

   def print_all(self):
      for i in self.__rf:
         print(repr(i))
