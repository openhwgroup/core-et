
from core.mtg import *
from components.tensor_wait import *

@register
class CacheOpsWait(Component, AsmCode):

   def __init__(self, test_helper: TestHelper = None, num_sets: int = None, num_ways: int = None):
      ops = [(1,  WAIT_TENSOR_LOAD_0),
             (1,  WAIT_TENSOR_LOAD_1),
             (1,  WAIT_TENSOR_LOAD_L2_0),
             (1,  WAIT_TENSOR_LOAD_L2_1),
             (5,  WAIT_PREFETCH_0),
             (5,  WAIT_PREFETCH_1),
             (50, WAIT_CACHEOPS),
             (1,  WAIT_TENSOR_FMA),
             (1,  WAIT_TENSOR_STORE),
             (1,  WAIT_TENSOR_REDUCE),
             (1,  WAIT_TENSOR_QUANT),
             (1,  WAIT_TENSOR_11),
             (1,  WAIT_TENSOR_12),
             (1,  WAIT_TENSOR_13),
             (1,  WAIT_TENSOR_14),
             (1,  WAIT_TENSOR_15)]

      tw_weights, tw_ops = unzip(ops)
      self.op = test_helper.pick_one_weight(tw_ops, tw_weights)(test_helper = test_helper)
      self.zero =ABIreg('zero')
      self.rd = self.op.rd

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(self.op)

      if TEST_CONF.EXTRA_CHECKS == 1:
         content_list.append("beq {rd}, {zero}, cache_ops_wait_pass")
         content_list.append(TEST_FAIL(test_helper = test_helper))
         content_list.append("cache_ops_wait_pass:")

