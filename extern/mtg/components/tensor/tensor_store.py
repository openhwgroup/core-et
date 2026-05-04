
from core.mtg import *
from core.models.tensorstoremodel import TensorStoreEncoding
from core.from_future import FromFuture
from components.tensor.tensor_base import TensorBase
from components.tensor.tensor import TENSOR_STORE
from components.randinst.rv32i import LI, AND, OR
from components.cops.sc_cops import EvictCoalescingBuffer
from components.tensor_wait import IMPLICIT_VPU_TENSOR_WAIT

@register
class TensorStore(TensorBase):

   def __init__(self, test_helper: TestHelper = None, x31: Xreg = None, x31_val: int = None, enc: int = None, mem: Malloc = None, coop_mode: int = 0, row_size: int = None):
      if x31 is None:
         self.x31 = Xreg(31)
      else:
         self.x31 = x31

      if x31_val is None:
         x31_val = test_helper.pick_one([0x40, 0x80, 0xc0]) + test_helper.pick_one([0, 1])

      self.rx1 = Xreg()
      self.rx2 = Xreg()
      self.rx3 = Xreg()

      self.ts_enc = TensorStoreEncoding(x31_val, coop_mode, value = enc, row_size = row_size)

      if mem is None:
         if coop_mode > 0:
            self.mem = StaticMalloc(MIN_SHIRE_MEM_SET, self.ts_enc.memory_size, align=64)
         else:
            self.mem = Malloc(MIN_SHIRE_MEM_SET, self.ts_enc.memory_size, align=64)
      else:
         self.mem = mem

   def _tensor_x31(self, content_list: list, test_helper: TestHelper = None) -> None:
      # Set X31
      content_list.append(LI(rd=self.x31, imm=self.ts_enc.x31_val))

   def _tensor_conf(self, content_list: list, test_helper: TestHelper = None) -> None:
      # Evict Coalescing Buffer
      content_list.append(EvictCoalescingBuffer(test_helper=test_helper, threads_mask=0x1))

      # Load the memory address
      content_list.append(LI(rd=self.rx1, imm=self.mem))
      content_list.append(LI(rd=self.rx2, imm=hex(0xfffffffffff0)))
      content_list.append(AND(rd=self.rx1, rs1=self.rx1, rs2=self.rx2))

      # Setup the Tensor encoding
      content_list.append(LI(rd=self.rx3, imm=self.ts_enc))
      content_list.append(OR(rd=self.rx3, rs1=self.rx3, rs2=self.rx1))

   def _tensor_csr(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(IMPLICIT_VPU_TENSOR_WAIT(test_helper = test_helper))
      content_list.append(TENSOR_STORE(enc=self.ts_enc, rs1=self.rx3, x31=self.x31))

   def _tensor_post(self, content_list: list, test_helper: TestHelper = None) -> None:
      pass

   def render(self, model: Model) -> Snippet:
      snip = super().render(model)
      assert self.ts_enc.memory_size <= self.mem.size
      return snip

if TEST_CONF.POSTSI_L2_ONLY == True:
   TEST_CONF.EXCL_COMPS.add(TensorStore)
