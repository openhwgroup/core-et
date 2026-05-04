
from core.mtg import *
from core.models.tensorreducemodel import TensorReduceEncoding
from components.tensor.tensor_base import TensorBase
from components.tensor.tensor import TENSOR_REDUCE
from components.randinst.rv32i import LI
from components.tensor_wait import IMPLICIT_VPU_TENSOR_WAIT

@register
class TensorReduce(TensorBase):

   def __init__(self, test_helper: TestHelper = None, x31: Xreg = None, x31_val: int = None, enc: int = None):
      self.rx1 = Xreg()
      self.tr_enc = TensorReduceEncoding(value = enc)

   def _tensor_conf(self, content_list: list, test_helper: TestHelper = None) -> None:
      # Setup the Tensor encoding
      content_list.append(LI(rd=self.rx1, imm=self.tr_enc))

   def _tensor_csr(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(IMPLICIT_VPU_TENSOR_WAIT(test_helper = test_helper))
      content_list.append(TENSOR_REDUCE(enc=self.tr_enc, rs1=self.rx1))
