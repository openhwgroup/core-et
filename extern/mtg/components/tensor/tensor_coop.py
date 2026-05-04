
from core.mtg import *
from core.models.tensorcoopmodel import TensorCoopEncoding
from components.tensor.tensor import TENSOR_COOP
from components.randinst.rv32i import LI
from components.tensor.tensor_base import TensorBase
from components.flb_fcc.flb_fcc_sync_threads import FLBFCCSyncShireThreads
from components.tensor_wait import WAIT_TENSOR_LOAD_0, WAIT_TENSOR_LOAD_1

@param_block
class WaitCoopTensorLoad0(WAIT_TENSOR_LOAD_0):
   def render_if(self, model: 'Model') -> bool:
      return model.pending_coop_tensor_load_0

@param_block
class WaitCoopTensorLoad1(WAIT_TENSOR_LOAD_1):
   def render_if(self, model: 'Model') -> bool:
      return model.pending_coop_tensor_load_1

@register
class ResetTensorCoopIds(Component, AsmCode):

   # Concurrently executing cooperative TensorLoads with the same coopid inside
   # a neighborhood leads to undefined behavior. This means that inside a shire,
   # multiple TensorLoads with the same coopid can execute concurrently only if
   # their corresponding coopneighmask values do not overlap, otherwise the
   # behavior of the system becomes undefined.

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      if TensorCoopEncoding.must_reset_ids():
         if TEST_CONF.MINION_AGNOSTIC == 0:
            content_list.append(FLBFCCSyncShireThreads(test_helper=test_helper))
         TensorCoopEncoding.reset_ids()

         twaits = [WaitCoopTensorLoad0, WaitCoopTensorLoad1]
         test_helper.shuffle(twaits)
         for tw in twaits:
            content_list.append(tw(test_helper=test_helper))

@register
class TensorCoop(TensorBase):

   def __init__(self, test_helper: TestHelper = None, x31: Xreg = None, x31_val: int = None, enc: int = None):
      self.rx1 = Xreg()
      self.tensor_coop = TensorCoopEncoding()

   def _tensor_conf(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(LI(rd=self.rx1, imm=self.tensor_coop))

   def _tensor_csr(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(TENSOR_COOP(enc=self.tensor_coop, rs1=self.rx1))

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(ResetTensorCoopIds(test_helper=test_helper))
      super()._run(content_list, test_helper)

if TEST_CONF.MINION_AGNOSTIC == 1 or TEST_CONF.NEIGH_AGNOSTIC == 1:
   TEST_CONF.EXCL_COMPS.add(TensorCoop)
