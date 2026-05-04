from core.mtg import *
from components.evict_all_l1d import EvictAllL1D
from components.tensor_wait import WAIT_TENSOR_STORE

@register
class CoherencyBarrier(Component, AsmCode):
   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(WAIT_TENSOR_STORE())
      content_list.append(EvictAllL1D(test_helper = test_helper))
      if TEST_CONF.POSTSI_L2_ONLY == False: # AKA if Global memory requests can happen
         from components.cops.sc_cops import EvictAllL2, EvictCoalescingBuffer
         content_list.append(EvictAllL2(test_helper = test_helper))
         content_list.append(EvictCoalescingBuffer(test_helper = test_helper))
