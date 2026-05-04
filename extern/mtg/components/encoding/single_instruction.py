
from core.mtg import *

class SingleInstruction(Component, AsmCode, metaclass=ABCMeta):

   def _run(self, content_list: list, fmt: str, test_helper: TestHelper = None) -> Snippet:
      self.mnemonic = self.__class__.__name__.lower().replace('_', '.')

      if hasattr(self, 'm0'):
         assert self.m0.reg_num == 0, f"{self.__class__.__name__} expects 'm0' but passed resource is 'm{self.m0.reg_num}'"

      if hasattr(self, 'x31'):
         assert self.x31.reg_num == 31, f"{self.__class__.__name__} expects 'x31' but passed resource is 'x{self.x31.reg_num}'"

      # Get the operand values from the object instance
      values = self.__dict__
      content_list.append('{mnemonic} '+fmt+'\n')

from core.models.tensorfmamodel import TensorFMAEncoding
from core.models.tensorquantmodel import TensorQuantEncoding
from core.models.tensorstoremodel import TensorStoreEncoding
from core.models.tensorreducemodel import TensorReduceEncoding
from core.component import Component, base_class

@base_class
class VPUSingleInstruction(SingleInstruction):

   def  _alloc_and_free(self, model: 'Model', context: 'Component', snip_content: list, skip: bool = False) -> None:
      if (TEST_CONF.RTLMIN_WORKAROUNDS['fv-260'] == False) and (TEST_CONF.EXTRA_CHECKS == 0):
         if not skip:
            ops = [TensorQuantEncoding, TensorStoreEncoding, TensorReduceEncoding]
            for op in ops:
               name = op.get_name()
               op.free_resources(model, name = name)

            if (model.pending_tensor_fma_tena == True or model.pending_tensor_fma_tenb == True) and model.pending_tensor_fma_to_rf == True:
               name = TensorFMAEncoding.get_name()
               TensorFMAEncoding.free_resources(model, name = name)
      super()._alloc_and_free(model, context, snip_content, skip = skip)
