
from core.mtg import *
from core.from_future import FromFuture
from components.tensor.tensor_base import TensorBase
from components.tensor.tensor_load import TensorLoad
from components.tensor.tensor_fma import TensorFMA
from components.randinst.rv32i import LI
from components.tensor_wait import WAIT_TENSOR_LOAD_0, WAIT_TENSOR_LOAD_1

@register
class TensorLoadFMATenB(TensorBase):

   def __init__(self, test_helper: TestHelper = None, x31: Xreg = None, x31_val: int = None, mem: Malloc = None):
      self.tenb_attr = FromFuture()
      self.tl_tenb = TensorLoad(test_helper = test_helper, tenb = 1,  tenb_num_lines = self.tenb_attr, x31 = x31, x31_val = x31_val, mem = mem)
      self.tfma_tenb = TensorFMA(test_helper = test_helper, tenb = 1, tenb_acols = self.tenb_attr)

   def _tensor_x31(self, content_list: list, test_helper: TestHelper = None) -> None:
      self.tl_tenb._tensor_x31(content_list, test_helper = test_helper)

   def _tensor_conf(self, content_list: list, test_helper: TestHelper = None) -> None:
      rand_order = [self.tl_tenb._tensor_conf, self.tfma_tenb._tensor_conf]
      test_helper.shuffle(rand_order)
      for i in rand_order:
         i(content_list, test_helper = test_helper)

   def _tensor_csr(self, content_list: list, test_helper: TestHelper = None) -> None:
      if TEST_CONF.RTLMIN_WORKAROUNDS['fv-260'] == True:
          content_list.append(WAIT_TENSOR_LOAD_0(test_helper = test_helper))
          content_list.append(WAIT_TENSOR_LOAD_1(test_helper = test_helper))
      self.tl_tenb._tensor_csr(content_list, test_helper)
      self.tfma_tenb._tensor_csr(content_list, test_helper)

   def _tensor_post(self, content_list: list, test_helper: TestHelper = None) -> None:
      pass
