
from core.mtg import *
from components.tensor_wait import *

@register
class RandomTensorWait(Component, AsmCode):

   def __init__(self, test_helper: TestHelper = None, num_sets: int = None, num_ways: int = None):
      ops = [(10, WAIT_TENSOR_LOAD_0),
             (10, WAIT_TENSOR_LOAD_1),
             (10, WAIT_TENSOR_LOAD_L2_0),
             (10, WAIT_TENSOR_LOAD_L2_1),
             (1,  WAIT_PREFETCH_0),
             (1,  WAIT_PREFETCH_1),
             (1,  WAIT_CACHEOPS),
             (10, WAIT_TENSOR_FMA),
             (10, WAIT_TENSOR_STORE),
             (10, WAIT_TENSOR_REDUCE),
             (10, WAIT_TENSOR_QUANT),
             (10, WAIT_TENSOR_11),
             (10, WAIT_TENSOR_12),
             (10, WAIT_TENSOR_13),
             (10, WAIT_TENSOR_14),
             (10, WAIT_TENSOR_15)]

      tw_weights, tw_ops = unzip(ops)
      self.op = test_helper.pick_one_weight(tw_ops, tw_weights)(test_helper = test_helper)
      self.zero =ABIreg('zero')
      self.rd = self.op.rd

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(self.op)

      if TEST_CONF.EXTRA_CHECKS == 1:
         content_list.append("beq {rd}, {zero}, tensor_wait_pass")
         content_list.append(TEST_FAIL(test_helper = test_helper))
         content_list.append("tensor_wait_pass:")

