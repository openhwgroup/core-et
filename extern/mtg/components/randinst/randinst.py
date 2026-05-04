
from dataclasses import fields
from typing import get_type_hints

from core.mtg import *

from components.randinst.rv import RV

# PRM-4
from components.randinst.mask import Mask

# PRM-5
from components.randinst.packedsingle import PackedSingle

# PRM-6
from components.randinst.packedinteger import PackedInteger

# PRM-7
from components.randinst.amo import Atomic, LocalAtomic, GlobalAtomic, FPLocalAtomic, FPGlobalAtomic

# PRM-8
from components.randinst.cacheops import CacheOps

_components = list()
_components += RV.subcls
_components += Mask.subcls
_components += PackedSingle.subcls
_components += PackedInteger.subcls
_components += LocalAtomic.subcls
if not TEST_CONF.POSTSI_L2_ONLY:
   print("RANDINST: Include Global Atomics")
   _components += GlobalAtomic.subcls
# Disable because cacheops require M-Mode always
#_components += CacheOps.subcls

@register
class RANDINST(Component, AsmCode):

   soft_alloc = True
   subcls = _components

   def __init__(self, test_helper: TestHelper = None, num_inst = 12, skip_res = list()):
      # Filter out any Component that has been marked as excluded
      self.subcls = list(filter(lambda x: x.exclude == False, self.subcls))
      # Filter our any Component that has an attribute with a name that is part of 'skip_res'
      self.subcls = list(filter(lambda x: self._exclude_res_list(x, skip_res) == False, self.subcls))

      self.insns = list()
      for _ in range(num_inst):
         self.insns.append(test_helper.pick_one(self.subcls)())

   def _exclude_res(self, obj_cls: Component, res_name: str) -> bool:
       names = {f.name for f in fields(obj_cls)}
       if res_name in names:
          field_type = get_type_hints(obj_cls)[res_name]
          assert issubclass(field_type, VirtualResource), field_type
          return True
       else:
          return False

   def _exclude_res_list(self, obj: Component, res_list: list) -> bool:
      for res in res_list:
         if self._exclude_res(obj, res):
            return True
      return False

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      for i in self.insns:
         content_list.append(i)

@param_block
class RANDINST_RANGE(Component, AsmCode):

   def __init__(self, min_insns, max_insns, test_helper: TestHelper = None, skip_res = list()):
      self.skip_res = skip_res
      self.num_inst = test_helper.get_rand_range(min_insns, max_insns)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(RANDINST(test_helper=test_helper, num_inst=self.num_inst, skip_res=self.skip_res))

_vpu_components = list()
_vpu_components += PackedSingle.subcls
_vpu_components += PackedInteger.subcls
_vpu_components += FPLocalAtomic.subcls
if not TEST_CONF.POSTSI_L2_ONLY:
   print("VPUINST: Include Global Atomics")
   _vpu_components += FPGlobalAtomic.subcls

@register
class VPUINST(RANDINST):
   subcls = _vpu_components
