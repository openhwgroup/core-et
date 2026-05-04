
from core.mtg import *
from components.tensor.tensor_load import TensorLoad
from components.tensor.tensor import TwB4L1SCPWrites

@register
class TensorLoadCoop(TensorLoad):

   coop = True

   @only_tensor_coop
   def __run(self, content_list: list, test_helper: TestHelper = None) -> None:
      super()._run(content_list, test_helper)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(TwB4L1SCPWrites(test_helper = test_helper, tl_tenb = self.tenb))
      self.__run(content_list, test_helper)

if TEST_CONF.MINION_AGNOSTIC == 1 or TEST_CONF.NEIGH_AGNOSTIC == 1:
   TEST_CONF.EXCL_COMPS.add(TensorLoadCoop)
