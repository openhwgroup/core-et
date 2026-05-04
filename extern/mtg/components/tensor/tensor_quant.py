
from core.mtg import *
from core.models.tensorquantmodel import TensorQuantEncoding
from components.tensor.tensor_base import TensorBase
from components.tensor.tensor import TENSOR_QUANT
from components.randinst.rv32i import LI, AND, OR
from components.tensor.tensor import TwPrevTsSCP, TwB4L1SCPReads
from components.tensor_wait import IMPLICIT_VPU_TENSOR_WAIT
from components.tensor_wait import WAIT_TENSOR_FMA, WAIT_TENSOR_STORE, WAIT_TENSOR_REDUCE

@register
class TensorQuant(TensorBase):

   def __init__(self, test_helper: TestHelper = None, x31: Xreg = None, x31_val: int = None, enc: int = None):
      self.rx1 = Xreg()
      self.tq_enc = TensorQuantEncoding(value = enc)

   def _tensor_conf(self, content_list: list, test_helper: TestHelper = None) -> None:
      # Setup the Tensor encoding
      content_list.append(LI(rd=self.rx1, imm=self.tq_enc))

   def _tensor_csr(self, content_list: list, test_helper: TestHelper = None) -> None:
      if TEST_CONF.RTLMIN_WORKAROUNDS['fv-260'] == True:
          content_list.append(IMPLICIT_VPU_TENSOR_WAIT(test_helper = test_helper))
          #content_list.append(WAIT_TENSOR_FMA(test_helper = test_helper))
          #content_list.append(WAIT_TENSOR_STORE(test_helper = test_helper))
          #content_list.append(WAIT_TENSOR_REDUCE(test_helper = test_helper))
      content_list.append(TwPrevTsSCP(test_helper = test_helper))
      content_list.append(TwB4L1SCPReads(test_helper = test_helper, tquant = True))
      content_list.append(TENSOR_QUANT(enc=self.tq_enc, rs1=self.rx1))

   def _tensor_post(self, content_list: list, test_helper: TestHelper = None) -> None:
      pass
