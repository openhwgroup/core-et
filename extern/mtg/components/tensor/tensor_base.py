
from core.mtg import *
from components.randinst.randinst import RANDINST

@register
class TensorBase(Component, AsmCode):

   MAX_RANDINST = 12

   def __init__(self, test_helper: TestHelper = None, x31: Xreg = None, x31_val: int = None):
      pass

   def _tensor_x31(self, content_list: list, test_helper: TestHelper = None) -> None:
      pass

   def _tensor_conf(self, content_list: list, test_helper: TestHelper = None) -> None:
      pass

   def _tensor_csr(self, content_list: list, test_helper: TestHelper = None) -> None:
      pass

   def _tensor_post(self, content_list: list, test_helper: TestHelper = None) -> None:
      pass

   @only_thread(0)
   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      rand_order = [self._tensor_conf, self._tensor_x31]
      test_helper.shuffle(rand_order)
      for i in rand_order:
         i(content_list, test_helper = test_helper)
      content_list.append(RANDINST(test_helper = test_helper, num_inst = test_helper.get_rand_range(0, self.MAX_RANDINST), skip_res=['x31']))
      self._tensor_csr(content_list, test_helper = test_helper)
      self._tensor_post(content_list, test_helper = test_helper)
