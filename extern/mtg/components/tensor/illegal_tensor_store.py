
from core.mtg import *
from core.models.tensorstoremodel import TensorStoreEncoding
from components.tensor.tensor_base import TensorBase
from components.tensor.tensor import TENSOR_STORE
from components.randinst.rv32i import LI
from components.preamble.set_coop_mode import SetCoopMode

@register
class IllegalTensorStore(TensorBase):

   def __init__(self, test_helper: TestHelper = None, x31: Xreg = None, x31_val: int = None):
      if x31 is None:
         self.x31 = Xreg(31)
      else:
         self.x31 = x31

      if x31_val is None:
         x31_val = test_helper.get_rand_kbits(8)

      self.rx1 = Xreg()

      if SetCoopMode.exclude == True:
         coop_mode = 1
      else:
         coop_mode = test_helper.pick_one([1, 2, 4])
      self.ts_enc = TensorStoreEncoding(x31_val, invalid = True, coop_mode = coop_mode)

   def _tensor_conf(self, content_list: list, test_helper: TestHelper = None) -> None:
      # Setup the Tensor encoding
      content_list.append(LI(rd=self.rx1, imm=self.ts_enc))

   def _tensor_csr(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(TENSOR_STORE(enc=self.ts_enc, rs1=self.rx1, x31=self.x31))
