
from core.mtg import *
from core.models.tensorloadmodel import TensorLoadEncoding
from components.tensor.tensor_base import TensorBase
from components.tensor.tensor import TENSOR_LOAD
from components.randinst.rv32i import LI

@register
class IllegalTensorLoad(TensorBase):

   def __init__(self, test_helper: TestHelper = None, x31: Xreg = None, x31_val: int = None):
      if x31 is None:
         self.x31 = Xreg(31)
      else:
         self.x31 = x31

      self.rx0 = Xreg()

      if x31_val is None:
         x31_val = test_helper.get_rand_kbits(8)

      self.tl_enc = TensorLoadEncoding(x31_val, invalid = True)

   def _tensor_conf(self, content_list: list, test_helper: TestHelper = None) -> None:
      # Setup the Tensor encoding
      content_list.append(LI(rd=self.rx0, imm=self.tl_enc))

   def _tensor_csr(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(TENSOR_LOAD(enc=self.tl_enc, rs1=self.rx0, x31=self.x31))
