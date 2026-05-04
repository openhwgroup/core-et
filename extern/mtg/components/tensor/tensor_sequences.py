
from core.mtg import *
from components.tensor.tensor_base import TensorBase
from components.tensor.tensor_load import TensorLoad
from components.tensor.tensor_load_l2scp import TensorLoadL2SCP
from components.tensor.tensor_fma import TensorFMA
from components.tensor.tensor_store import TensorStore
from components.tensor.tensor_quant import TensorQuant
from components.tensor.tensor_reduce import TensorReduce
from components.tensor.tensor_store_scp import TensorStoreSCP
from components.tensor.tensor_load_fma_tenb import TensorLoadFMATenB
from components.tensor.illegal_tensor_load import IllegalTensorLoad
from components.tensor.illegal_tensor_store import IllegalTensorStore
from components.randinst.rv32i import LI
from components.randinst.randinst import RANDINST
from components.tensor.tensor_mask import CheckTensorMask, TensorMask, TensorConvSize, TensorConvCtrl

@register
class TensorSequenceBase(TensorBase, metaclass=ABCMeta):

   comps = list()

   @abstractmethod
   def _tensor_x31(self, content_list: list, test_helper: TestHelper = None) -> None:
      pass

   def _tensor_conf(self, content_list: list, test_helper: TestHelper = None) -> None:
      for tensor_comp in self.comps:
         tensor_comp._tensor_conf(content_list, test_helper = test_helper)

   def _tensor_csr(self, content_list: list, test_helper: TestHelper = None) -> None:
      for tensor_comp in self.comps:
         tensor_comp._tensor_csr(content_list, test_helper = test_helper)

   def _tensor_post(self, content_list: list, test_helper: TestHelper = None) -> None:
      for tensor_comp in self.comps:
         tensor_comp._tensor_post(content_list, test_helper = test_helper)

@register
class TensorSequence(TensorSequenceBase):

   tensor_comps = [TensorLoad, TensorLoadL2SCP, TensorFMA, TensorLoadFMATenB, TensorStore, TensorQuant, TensorStoreSCP, TensorReduce, IllegalTensorLoad, IllegalTensorStore, TensorMask, TensorConvSize, TensorConvCtrl]
   comp_weights = [        20,              20,        20,                20,          20,          20,             20,           20,                 1,                  1,          2,              2,              2]
   seq_size = 5

   def __init__(self, test_helper: TestHelper = None):
      self.x31 = Xreg(31)
      tensor_seq = self.get_sequence(test_helper)
      if TensorStore in tensor_seq or TensorStoreSCP in tensor_seq:
         self.x31_val = test_helper.pick_one([0x40, 0x80, 0xc0]) + test_helper.pick_one([0, 1])
      else:
         self.x31_val = test_helper.get_rand_kbits(8)
      self.rx0 = Xreg()
      self.comps = [comp(test_helper = test_helper, x31 = self.x31, x31_val = self.x31_val) for comp in tensor_seq]
      self.tm_comps = [comp for comp in self.comps if isinstance(comp, TensorMask)]

   def get_sequence(self, test_helper: TestHelper):
      comps = list()
      excl = list()
      if TEST_CONF.POSTSI_L2_ONLY == True:
         excl = [TensorStoreSCP, TensorStore, TensorLoadL2SCP]
      # Erbium has no L2 Shire Cache
      elif TEST_CONF.TARGET == 'erbium':
         excl = [TensorLoadL2SCP]
      for _ in range(self.seq_size):
         tensor_comp = test_helper.pick_one_weight(self.tensor_comps, self.comp_weights, exclude=excl)
         comps.append(tensor_comp)
      return comps

   def _tensor_x31(self, content_list: list, test_helper: TestHelper = None) -> None:
      # Set X31
      content_list.append(LI(rd=self.x31, imm=self.x31_val))

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      for comp in self.tm_comps:
         comp._tensor_mask_conf(content_list, test_helper = test_helper)

      super()._run(content_list, test_helper)

      for comp in self.tm_comps:
         comp._tensor_csr(content_list, test_helper = test_helper)
      if len(self.tm_comps) > 0:
         content_list.append(CheckTensorMask(test_helper = test_helper))

@register
class AnySequence(TensorSequence):

   def _tensor_csr(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(RANDINST(test_helper = test_helper, num_inst = 1, skip_res=['x31']))
      for tensor_comp in self.comps:
         tensor_comp._tensor_csr(content_list, test_helper = test_helper)
         content_list.append(RANDINST(test_helper = test_helper, num_inst = 1, skip_res=['x31']))

@register
class TFMATenaTLTenb(TensorSequence):

   def get_sequence(self, test_helper: TestHelper):
      return [TensorFMA, TensorLoadFMATenB]

@register
class TsScpTFMA(TensorSequence):

   def get_sequence(self, test_helper: TestHelper):
      return [TensorStoreSCP, TensorFMA]

@register
class TsScpTQuant(TensorSequence):

   def get_sequence(self, test_helper: TestHelper):
      return [TensorStoreSCP, TensorQuant]

@register
class TFMATsScp(TensorSequence):

   def get_sequence(self, test_helper: TestHelper):
      return [TensorFMA, TensorStoreSCP]

@register
class TQuantTsScp(TensorSequence):

   def get_sequence(self, test_helper: TestHelper):
      return [TensorQuant, TensorStoreSCP]

if TEST_CONF.POSTSI_L2_ONLY == True:
   TEST_CONF.EXCL_COMPS.add(TsScpTFMA)
   TEST_CONF.EXCL_COMPS.add(TsScpTQuant)
   TEST_CONF.EXCL_COMPS.add(TFMATsScp)
   TEST_CONF.EXCL_COMPS.add(TQuantTsScp)
