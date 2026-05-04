
from core.mtg import *
from core.models.tensorloadmodel import TensorLoadEncoding
from core.from_future import FromFuture
from components.tensor.tensor_base import TensorBase
from components.tensor.tensor import TENSOR_LOAD, TwB4L1SCPWrites
from components.randinst.rv32i import LI, AND, OR
from components.tensor_wait import WAIT_TENSOR_FMA

@param_block
class TFMATenaTLTenb(Component, AsmCode):

   def __init__(self, test_helper: TestHelper = None):
      self.count = 0

   def render_if(self, model: 'Model') -> None:
      return model.pending_tensor_fma_tena

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(WAIT_TENSOR_FMA())

@register
class TensorLoad(TensorBase):

   coop = False

   def __init__(self, test_helper: TestHelper = None, x31: Xreg = None, x31_val: int = None, enc: int = None, tenb: int = 0, tenb_num_lines: FromFuture = None, mem: Malloc = None):
      if x31 is None:
         self.x31 = Xreg(31)
      else:
         self.x31 = x31

      self.rx1 = Xreg()
      self.rx2 = Xreg()
      self.rx3 = Xreg()

      if x31_val is None:
         x31_val = test_helper.get_rand_kbits(8)

      self.tenb = tenb

      self.tl_enc = TensorLoadEncoding(x31_val, value = enc, tenb = tenb, coop = self.coop)
      if tenb_num_lines is None:
         self.tl_enc.tenb_num_lines = FromFuture()
      else:
         self.tl_enc.tenb_num_lines = tenb_num_lines

      if mem is None:
         self.mem = StaticMalloc(MIN_SHIRE_MEM_SET, self.tl_enc.memory_size, align=64)
      else:
         self.mem = mem

   def _tensor_x31(self, content_list: list, test_helper: TestHelper = None) -> None:
      # Set X31
      content_list.append(LI(rd=self.x31, imm=self.tl_enc.x31_val))

   def _tensor_conf(self, content_list: list, test_helper: TestHelper = None) -> None:
      # Load the memory address
      content_list.append(LI(rd=self.rx1, imm=self.mem))
      content_list.append(LI(rd=self.rx2, imm=hex(0xffffffffffc0)))
      content_list.append(AND(rd=self.rx1, rs1=self.rx1, rs2=self.rx2))

      # Setup the Tensor encoding
      content_list.append(LI(rd=self.rx3, imm=self.tl_enc))
      content_list.append(OR(rd=self.rx3, rs1=self.rx3, rs2=self.rx1))

   def _tensor_csr(self, content_list: list, test_helper: TestHelper = None) -> None:
      if self.tenb == 1:
         content_list.append(TFMATenaTLTenb())
      content_list.append(TwB4L1SCPWrites(test_helper = test_helper, tl_tenb = self.tenb))
      content_list.append(TENSOR_LOAD(enc=self.tl_enc, rs1=self.rx3, x31=self.x31))

   def _tensor_post(self, content_list: list, test_helper: TestHelper = None) -> None:
      pass

   def render(self, model: Model) -> Snippet:
      snip = super().render(model)
      assert self.tl_enc.memory_size <= self.mem.size
      return snip
