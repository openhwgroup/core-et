
from core.mtg import *
from components.tensor.tensor_load import TensorLoad
from components.tensor.tensor_mask import TensorMask

@register
class DummyTensorLoad(Component, AsmCode):

   def __init__(self, test_helper: TestHelper = None, x31: Xreg = None, x31_val: int = None, mem: Malloc = None):
      # VERIF-3666
      self.dummy_tl = TensorLoad(test_helper = test_helper, tenb = 0, enc = 0x8000000000000000)
      self.tm = TensorMask(test_helper = test_helper, value = 0)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(self.tm)
      content_list.append(self.dummy_tl)

if TEST_CONF.RTLMIN_WORKAROUNDS['rtlmin-6160'] == False:
   TEST_CONF.EXCL_COMPS.add(DummyTensorLoad)
