
from core.mtg import *
from core.models.tensormaskmodel import TensorMaskEncoding, TensorConvSizeEncoding, TensorConvCtrlEncoding
from components.tensor.tensor import TENSOR_CSRRW
from components.randinst.rv32i import LI, AND, OR

class X0reg(ABIreg):
   def __init__(self):
      super().__init__('zero')

@register
class CheckTensorMask(Component, AsmCode):
   def __init__(self, test_helper: TestHelper = None, rx0: Xreg = None, rx1: Xreg = None):
      self.tm_val = None # Place holder

      if rx0 is None:
         self.rx0 = Xreg()
      else:
         self.rx0 = rx0

      if rx1 is None:
         self.rx1 = Xreg()
      else:
         self.rx1 = rx1

   def render_if(self, model: Model) -> bool:
      return TEST_CONF.EXTRA_CHECKS == 1

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append("li {rx0}, {tm_val}")
      content_list.append(ReadTensorMask(rd=self.rx1))
      content_list.append("beq {rx0}, {rx1}, tensor_mask_pass")
      content_list.append(TEST_FAIL())
      content_list.append("tensor_mask_pass:")
   def  _alloc_and_free(self, model: Model, context: 'Component', snip_content: list, skip: bool = False) -> None:
      self.tm_val = model.tensor_mask
      super()._alloc_and_free(model, context, snip_content, skip)

@register
@mtg_dataclass
class TENSOR_MASK(TENSOR_CSRRW, AsmCode):
   rd: Xreg  = field(default_factory=X0reg)
   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      assert isinstance(self.enc, TensorMaskEncoding), "Tensor Mask must be set using the TensorMask Component"
      if TEST_CONF.RTLMIN_WORKAROUNDS['rtlmin-6136'] == True:
         content_list.append("csrrs x0, tensor_mask, x0")
      super()._run(content_list, test_helper)

@register
@mtg_dataclass
class TENSOR_CONV_SIZE(TENSOR_CSRRW, AsmCode):
   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      assert isinstance(self.enc, TensorConvSizeEncoding), "Tensor Conv Size must be set using the TensorConvSize Component"
      super()._run(content_list, test_helper)

@register
@mtg_dataclass
class TENSOR_CONV_CTRL(TENSOR_CSRRW, AsmCode):
   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      assert isinstance(self.enc, TensorConvCtrlEncoding), "Tensor Conv Ctrl must be set uisng the TensorConvCtrl Component"
      super()._run(content_list, test_helper)

@register
class TensorMask(Component, AsmCode):
   def __init__(self, test_helper: TestHelper = None, x31: Xreg = None, x31_val: int = None, value: int = None):
      self.enc = TensorMaskEncoding(value = value)
      self.rx0 = Xreg()

   def _tensor_x31(self, content_list: list, test_helper: TestHelper = None) -> None:
      pass

   def _tensor_post(self, content_list: list, test_helper: TestHelper = None) -> None:
      pass

   def _tensor_conf(self, content_list: list, test_helper: TestHelper = None) -> None:
      pass

   def _tensor_mask_conf(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(LI(rd=self.rx0, imm=self.enc))

   def _tensor_csr(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(TENSOR_MASK(enc=self.enc, rs1=self.rx0))

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      self._tensor_mask_conf(content_list, test_helper = test_helper)
      self._tensor_csr(content_list, test_helper = test_helper)
      content_list.append(CheckTensorMask())

@register
class TensorConvSize(TensorMask, AsmCode):
   def __init__(self, test_helper: TestHelper = None, x31: Xreg = None, x31_val: int = None, value: int = None):
      self.enc = TensorConvSizeEncoding(value = value)
      self.rx0 = Xreg()

   def _tensor_csr(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(TENSOR_CONV_SIZE(enc=self.enc, rs1=self.rx0))

@register
class TensorConvCtrl(TensorMask, AsmCode):
   def __init__(self, test_helper: TestHelper = None, x31: Xreg = None, x31_val: int = None, value: int = None):
      self.enc = TensorConvCtrlEncoding(value = value)
      self.rx0 = Xreg()

   def _tensor_csr(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(TENSOR_CONV_CTRL(enc=self.enc, rs1=self.rx0))

@register
class SetRandTensorMask(Component, AsmCode):
   def __init__(self, test_helper: TestHelper = None):
      self.comp = test_helper.pick_one([TensorMask, TensorConvCtrl, TensorConvSize])(test_helper = test_helper)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(self.comp)

@register
class ReadTensorMask(Component, AsmCode):
   def __init__(self, test_helper: TestHelper = None, rd: Xreg = None):
      if rd is None:
         self.rd = Xreg()
      else:
         self.rd = rd
      self.zero = ABIreg('zero')

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append("csrrs {rd}, tensor_mask, {zero}")
