
from core.mtg import *
from components.tensor.tensor_load import TensorLoad
from components.tensor.tensor_fma import TensorFMA
from components.tensor.tensor_load_fma_tenb import TensorLoadFMATenB
from components.tensor.tensor_store import TensorStore
from components.tensor.tensor_store_scp import TensorStoreSCP
from components.tensor_wait import WAIT_TENSOR_LOAD_0, WAIT_TENSOR_LOAD_1, WAIT_TENSOR_STORE, WAIT_TENSOR_FMA

@register
class TensorMixed(Component, AsmCode):
   tensor_comps = [TensorLoad, TensorFMA, TensorLoadFMATenB, TensorStore, TensorStoreSCP]
   comp_weights = [         2,         1,                 2,           6,              1]
   last_comp = None
   last_id = None

   def __init__(self, test_helper: TestHelper = None):
      self.x31 = Xreg(31)
      self.x31_val = test_helper.get_rand_kbits(8)
      self.rx0 = Xreg()

      self.mem = GMalloc('TensorMixed_mem', MIN_SHIRE_MEM_SET, 16*1024, align=64)

      self.comp = test_helper.pick_one_weight(self.tensor_comps, self.comp_weights)
      if self.comp == TensorFMA:
         self.comp = self.comp(test_helper = test_helper, x31 = self.x31, x31_val = self.x31_val)
      else:
         if self.comp == TensorStore or self.comp == TensorStoreSCP:
            self.x31_val = test_helper.pick_one([0x40, 0x80, 0xc0]) + test_helper.pick_one([0, 1])
         self.comp = self.comp(test_helper = test_helper, x31 = self.x31, x31_val = self.x31_val, mem = self.mem)

   def add_tw_prev_comp(self, content_list: list, test_helper: TestHelper = None) -> None:
      if self.last_comp is not None:
         if isinstance(self.last_comp, TensorLoad):
            if self.last_id == 0:
               content_list.append(WAIT_TENSOR_LOAD_0())
            else:
               content_list.append(WAIT_TENSOR_LOAD_1())
         elif isinstance(self.last_comp, TensorStore) or isinstance(self.last_comp, TensorStoreSCP):
            content_list.append(WAIT_TENSOR_STORE())
         elif isinstance(self.last_comp, TensorLoadFMATenB):
            content_list.append(WAIT_TENSOR_FMA())
         else:
            assert isinstance(self.last_comp, TensorFMA), f"{self.last_comp}"

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      self.add_tw_prev_comp(content_list, test_helper)
      content_list.append(self.comp)

      TensorMixed.last_comp = self.comp
      TensorMixed.last_id = self.x31_val & 0x1

TEST_CONF.EXCL_COMPS.add(TensorMixed)
