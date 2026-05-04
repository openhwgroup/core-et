
from core.mtg import *
from core.models.tensorfmamodel import TensorFMAEncoding
from core.from_future import FromFuture
from components.tensor.tensor_base import TensorBase
from components.tensor.tensor import TENSOR_FMA
from components.randinst.rv32i import LI, AND, OR
from components.tensor.tensor import TwB4L1SCPReads, TwPrevTsSCP
from components.tensor_wait import IMPLICIT_VPU_TENSOR_WAIT
from components.tensor_wait import WAIT_TENSOR_QUANT, WAIT_TENSOR_STORE, WAIT_TENSOR_REDUCE

@param_block
class TR_TFMA_WA(CondComponent, AsmCode):
   def render_if(self, model: 'Model') -> bool:
      return (TEST_CONF.RTLMIN_WORKAROUNDS['fv-260'] == True) and (model.tr_last_reg == 0)
   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(f"/*WA for TR->TFMA where last register written by TR is f0*/")
      content_list.append(WAIT_TENSOR_REDUCE())

@register
class TensorFMA(TensorBase):

   def __init__(self, test_helper: TestHelper = None, x31: Xreg = None, x31_val: int = None, enc: int = None, tenb: int = 0, tenb_acols: FromFuture = None):
      self.rx1 = Xreg()

      self.tfma_enc = TensorFMAEncoding(value = enc, tenb = tenb)
      if tenb_acols is None:
         self.tfma_enc.tenb_a_num_cols = FromFuture()
      else:
         self.tfma_enc.tenb_a_num_cols = tenb_acols

   def _tensor_conf(self, content_list: list, test_helper: TestHelper = None) -> None:
      # Setup the Tensor encoding
      content_list.append(LI(rd=self.rx1, imm=self.tfma_enc))

   def _tensor_csr(self, content_list: list, test_helper: TestHelper = None) -> None:
      if TEST_CONF.RTLMIN_WORKAROUNDS['fv-260'] == True:
          content_list.append(IMPLICIT_VPU_TENSOR_WAIT(test_helper = test_helper))
          content_list.append(TR_TFMA_WA(test_helper = test_helper))
          #content_list.append(WAIT_TENSOR_QUANT(test_helper = test_helper))
          #content_list.append(WAIT_TENSOR_STORE(test_helper = test_helper))
          #content_list.append(WAIT_TENSOR_REDUCE(test_helper = test_helper))
      content_list.append(TwPrevTsSCP(test_helper = test_helper))
      content_list.append(TwB4L1SCPReads(test_helper = test_helper))
      content_list.append(TENSOR_FMA(enc=self.tfma_enc, rs1=self.rx1))

   def _tensor_post(self, content_list: list, test_helper: TestHelper = None) -> None:
      pass
