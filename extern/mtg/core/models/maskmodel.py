from core.models.resource import Resource, VirtualResource
from core.models.test_helper import TestHelper

class pMreg(Resource):
   def __init__(self, reg_num: int = 0) -> None:
      # Sanity check
      assert (reg_num >= 0) and (reg_num < MaskModel.RF_SIZE), f"Mask Register File size is {MaskModel.RF_SIZE}. Provided 'm{reg_num}'."

      # Instance attribute (unique per instance)
      self.reg_num = reg_num

      super().__init__()

   def __repr__(self) -> str:
      return f"m{self.reg_num}: {super().__repr__()}"

   def __str__(self) -> str:
      return f'm{self.reg_num}'

class Mreg(VirtualResource):
   def allocate(self, model: 'Model') -> None:
      assert model is not None, "Passed Model is 'None'"
      try:
         phy_resource = model.mask.get_reg(self.reg_num)
         super().allocate(phy_resource)
      except IndexError as exc:
         model.mask.print_all()
         raise Exception(f"VirtualResource of type {self.__class__.__name__} failed to link to a physical resource. Passed Model runned out of free resources") from exc

   def soft_allocate(self, model: 'Model') -> None:
      assert model is not None, "Passed Model is 'None'"
      assert self.reg_num is not None, "Soft allocation must only be used for predefined resources"
      try:
         phy_resource = model.mask.get_reg(self.reg_num)
         super().soft_allocate(phy_resource)
      except IndexError as exc:
         model.mask.print_all()
         raise Exception(f"VirtualResource of type {self.__class__.__name__} failed to link to a physical resource. Passed Model runned out of free resources") from exc

class MaskModel:

   RF_SIZE = 8

   def __init__(self, test_helper: TestHelper) -> None:
      self.__test_helper = test_helper
      self.__rf = [pMreg(i) for i in range(self.RF_SIZE)]

   def get_avail_regs(self) -> list:
      return [x for x in self.__rf if not x.is_used()]

   def exclude_reg(self, reg: int) -> None:
      assert 0 <= reg < self.RF_SIZE
      self.__rf[reg].reserve()

   def reset_reg(self, reg: int) -> None:
      assert 0 <= reg < self.RF_SIZE
      self.__rf[reg]._free()

   def get_reg(self, num: int = None) -> pMreg:
      if num is None:
         filtered = self.get_avail_regs()
         reg = self.__test_helper.pick_one(filtered)
      else:
         assert num < self.RF_SIZE, f"Provided register number ({num}) is bigger than the Mask RF size ({self.RF_SIZE})"
         reg = self.__rf[num]
      return reg

   def print_all(self):
      for i in self.__rf:
         print(repr(i))
