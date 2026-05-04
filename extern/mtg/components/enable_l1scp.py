
from core.mtg import *

from components.et_csr import SET_EXCL_MODE, UNSET_EXCL_MODE
from components.evict_all_l1d import EvictAllL1D
from components.cacheops import SET_DCACHE_SHARED, SET_D1SPLIT, SET_SCPENABLE
from components.tensor_wait import WAIT_CACHEOPS
from components.randinst.rv import FENCE

@register
class EnableL1SCP(Component, AsmCode):

   @only_thread(0)
   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(SET_EXCL_MODE(test_helper = test_helper))

      content_list.append(EvictAllL1D(test_helper = test_helper))

      content_list.append(SET_DCACHE_SHARED(test_helper = test_helper))
      content_list.append(FENCE(test_helper = test_helper))
      content_list.append(WAIT_CACHEOPS(test_helper = test_helper))

      content_list.append(SET_D1SPLIT(test_helper = test_helper))
      content_list.append(FENCE(test_helper = test_helper))
      content_list.append(WAIT_CACHEOPS(test_helper = test_helper))

      content_list.append(SET_SCPENABLE(test_helper = test_helper))
      content_list.append(FENCE(test_helper = test_helper))
      content_list.append(WAIT_CACHEOPS(test_helper = test_helper))

      content_list.append(UNSET_EXCL_MODE(test_helper = test_helper))
