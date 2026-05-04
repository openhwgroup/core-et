
from core.mtg import *
from core.models.tensorstorescpmodel import TensorStoreSCPEncoding
from core.from_future import FromFuture
from components.tensor.tensor_base import TensorBase
from components.tensor.tensor import TENSOR_STORE_SCP, TwB4L1SCPReads
from components.randinst.rv32i import LI, AND, OR
from components.tensor_wait import WAIT_TENSOR_QUANT, WAIT_TENSOR_FMA
from components.cops.sc_cops import EvictCoalescingBuffer

@param_block
class FreeTFMA(CondComponent, AsmCode):
   def render_if(self, model: 'Model') -> bool:
      return model.pending_tensor_fma_tena or model.pending_tensor_fma_tenb
   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(WAIT_TENSOR_FMA())

@param_block
class FreeTQuant(CondComponent, AsmCode):
   def render_if(self, model: 'Model') -> bool:
      return model.pending_tensor_quant
   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(WAIT_TENSOR_QUANT())

@register
class TensorStoreSCP(TensorBase):

   def __init__(self, test_helper: TestHelper = None, x31: Xreg = None, x31_val: int = None, enc: int = None, mem: Malloc = None):
      if x31 is None:
         self.x31 = Xreg(31)
      else:
         self.x31 = x31

      if x31_val is None:
         x31_val = test_helper.pick_one([0x40, 0x80, 0xc0]) + test_helper.pick_one([0, 1])

      self.rx1 = Xreg()
      self.rx2 = Xreg()
      self.rx3 = Xreg()

      self.ts_scp_enc = TensorStoreSCPEncoding(x31_val, value = enc)

      if mem is None:
         self.mem = Malloc(MIN_SHIRE_MEM_SET, self.ts_scp_enc.memory_size, align=64)
      else:
         self.mem = mem

   def _tensor_x31(self, content_list: list, test_helper: TestHelper = None) -> None:
      # Set X31
      content_list.append(LI(rd=self.x31, imm=self.ts_scp_enc.x31_val))

   def _tensor_conf(self, content_list: list, test_helper: TestHelper = None) -> None:
      # Evict Coalescing Buffer
      content_list.append(EvictCoalescingBuffer(test_helper=test_helper, threads_mask=0x1))

      # Load the memory address
      content_list.append(LI(rd=self.rx1, imm=self.mem))
      content_list.append(LI(rd=self.rx2, imm=hex(0xffffffffffc0)))
      content_list.append(AND(rd=self.rx1, rs1=self.rx1, rs2=self.rx2))

      # Setup the Tensor encoding
      content_list.append(LI(rd=self.rx3, imm=self.ts_scp_enc))
      content_list.append(OR(rd=self.rx3, rs1=self.rx3, rs2=self.rx1))

   def _tensor_csr(self, content_list: list, test_helper: TestHelper = None) -> None:
      order = [FreeTFMA, FreeTQuant]
      test_helper.shuffle(order)
      for i in order:
         content_list.append(i(test_helper = test_helper))
      content_list.append(TwB4L1SCPReads(test_helper = test_helper))
      content_list.append(TENSOR_STORE_SCP(enc=self.ts_scp_enc, rs1=self.rx3, x31=self.x31))

   def _tensor_post(self, content_list: list, test_helper: TestHelper = None) -> None:
      pass

   def render(self, model: Model) -> Snippet:
      snip = super().render(model)
      assert self.ts_scp_enc.memory_size <= self.mem.size
      return snip

if TEST_CONF.POSTSI_L2_ONLY == True:
   TEST_CONF.EXCL_COMPS.add(TensorStoreSCP)
