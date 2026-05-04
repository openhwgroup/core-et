
from core.mtg import *
from components.encoding.enc import *
from components.randinst.rv32i import LI
from core.models.tensorloadmodel import TensorLoadEncoding
from core.models.tensorfmamodel import TensorFMAEncoding
from core.models.tensorstoremodel import TensorStoreEncoding
from core.models.tensorquantmodel import TensorQuantEncoding
from core.models.tensorreducemodel import TensorReduceEncoding
from core.models.tensorstorescpmodel import TensorStoreSCPEncoding
from core.models.tensorloadl2scpmodel import TensorLoadL2SCPEncoding

@register
class TENSOR_WAIT(CSRRWI):
   pass

@mtg_dataclass
class TENSOR_WAIT_WRAP(CSR_WRAP):
   rd: Xreg = field(default_factory=Xreg)
   csr_types = ['csrrw', 'csrrs', 'csrrwi', 'csrrsi']

   def __init__(self, test_helper: TestHelper = None, rd: Xreg = None):
      if rd is None:
         self.rd = Xreg()
      else:
         self.rd = rd

      self.rs1 = Xreg()

      if test_helper is None:
         self.mnemonic = 'csrrwi'
      else:
         self.mnemonic = test_helper.pick_one(self.csr_types)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      if 'i' in self.mnemonic:
         content_list.append(f'{self.mnemonic} {{rd}}, tensor_wait, {self.event_id}\n')
      else:
         warl_id = (test_helper.get_rand_kbits(60) << 4) | self.event_id.value
         content_list.append(LI(rd=self.rs1, imm=warl_id))
         content_list.append(f'{self.mnemonic} {{rd}}, tensor_wait, {{rs1}}\n')

@register
class IMPLICIT_VPU_TENSOR_WAIT(Component, AsmCode):
   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
       content_list.append("/*MTG free TFMA resources*/")
       content_list.append("/*MTG free TSTORE (non-scp only) resources*/")
       content_list.append("/*MTG free TREDUCE resources*/")
       content_list.append("/*MTG free TQUANT resources*/")

   def  _alloc_and_free(self, model: Model, context: 'Component', snip_content: list, skip: bool = False) -> None:
      if not skip:
         # TFMA
         name = TensorFMAEncoding.get_name()
         TensorFMAEncoding.free_resources(model, name = name)

         # TSTORE (non-scp only)
         name = TensorStoreEncoding.get_name()
         TensorStoreEncoding.free_resources(model, name = name)

         # TREDUCE
         name = TensorReduceEncoding.get_name()
         TensorReduceEncoding.free_resources(model, name = name)

         # TQUANT
         name = TensorQuantEncoding.get_name()
         TensorQuantEncoding.free_resources(model, name = name)
      super()._alloc_and_free(model, context, snip_content, skip = skip)

@register
class WAIT_TENSOR_LOAD_0(TENSOR_WAIT_WRAP):
   event_id: Zimm5 = Zimm5(0)
   csr_types = ['csrrw', 'csrrwi']
   def  _alloc_and_free(self, model: Model, context: 'Component', snip_content: list, skip: bool = False) -> None:
      if not skip:
         name = TensorLoadEncoding.get_name(0)
         TensorLoadEncoding.free_resources(model, name = name, tw = True)
      super()._alloc_and_free(model, context, snip_content, skip = skip)

@register
class WAIT_TENSOR_LOAD_1(TENSOR_WAIT_WRAP):
   event_id: Zimm5 = Zimm5(1)
   def  _alloc_and_free(self, model: Model, context: 'Component', snip_content: list, skip: bool = False) -> None:
      if not skip:
         name = TensorLoadEncoding.get_name(1)
         TensorLoadEncoding.free_resources(model, name = name, tw = True)
      super()._alloc_and_free(model, context, snip_content, skip = skip)

@register
class WAIT_TENSOR_LOAD_L2_0(TENSOR_WAIT_WRAP):
   event_id: Zimm5 = Zimm5(2)
   def  _alloc_and_free(self, model: Model, context: 'Component', snip_content: list, skip: bool = False) -> None:
      if not skip:
         name = TensorLoadL2SCPEncoding.get_name(0)
         TensorLoadL2SCPEncoding.free_resources(model, name = name)
      super()._alloc_and_free(model, context, snip_content, skip = skip)

@register
class WAIT_TENSOR_LOAD_L2_1(TENSOR_WAIT_WRAP):
   event_id: Zimm5 = Zimm5(3)
   def  _alloc_and_free(self, model: Model, context: 'Component', snip_content: list, skip: bool = False) -> None:
      if not skip:
         name = TensorLoadL2SCPEncoding.get_name(1)
         TensorLoadL2SCPEncoding.free_resources(model, name = name)
      super()._alloc_and_free(model, context, snip_content, skip = skip)

@register
class WAIT_PREFETCH_0(TENSOR_WAIT_WRAP):
   event_id: Zimm5 = Zimm5(4)
   def  _alloc_and_free(self, model: Model, context: 'Component', snip_content: list, skip: bool = False) -> None:
      if not skip:
         # TODO: free the resources in the model
         pass
      super()._alloc_and_free(model, context, snip_content, skip = skip)

@register
class WAIT_PREFETCH_1(TENSOR_WAIT_WRAP):
   event_id: Zimm5 = Zimm5(5)
   def  _alloc_and_free(self, model: Model, context: 'Component', snip_content: list, skip: bool = False) -> None:
      if not skip:
         # TODO: free the resources in the model
         pass
      super()._alloc_and_free(model, context, snip_content, skip = skip)

@register
class WAIT_CACHEOPS(TENSOR_WAIT_WRAP):
   event_id: Zimm5 = Zimm5(6)
   def  _alloc_and_free(self, model: Model, context: 'Component', snip_content: list, skip: bool = False) -> None:
      if not skip:
         # TODO: free the resources in the model
         pass
      super()._alloc_and_free(model, context, snip_content, skip = skip)

@register
class WAIT_TENSOR_FMA(TENSOR_WAIT_WRAP):
   event_id: Zimm5 = Zimm5(7)
   def  _alloc_and_free(self, model: Model, context: 'Component', snip_content: list, skip: bool = False) -> None:
      if not skip:
         name = TensorFMAEncoding.get_name()
         TensorFMAEncoding.free_resources(model, name = name, from_tensor_wait = True)
      super()._alloc_and_free(model, context, snip_content, skip = skip)

@register
class WAIT_TENSOR_STORE(TENSOR_WAIT_WRAP):
   event_id: Zimm5 = Zimm5(8)
   def  _alloc_and_free(self, model: Model, context: 'Component', snip_content: list, skip: bool = False) -> None:
      if not skip:
         name = TensorStoreEncoding.get_name()
         TensorStoreEncoding.free_resources(model, name = name)
         name = TensorStoreSCPEncoding.get_name()
         TensorStoreSCPEncoding.free_resources(model, name = name)
      super()._alloc_and_free(model, context, snip_content, skip = skip)

@register
class WAIT_TENSOR_REDUCE(TENSOR_WAIT_WRAP):
   event_id: Zimm5 = Zimm5(9)
   def  _alloc_and_free(self, model: Model, context: 'Component', snip_content: list, skip: bool = False) -> None:
      if not skip:
         name = TensorReduceEncoding.get_name()
         TensorReduceEncoding.free_resources(model, name = name, from_tensor_wait = True)
      super()._alloc_and_free(model, context, snip_content, skip = skip)

@register
class WAIT_TENSOR_QUANT(TENSOR_WAIT_WRAP):
   event_id: Zimm5 = Zimm5(10)
   def  _alloc_and_free(self, model: Model, context: 'Component', snip_content: list, skip: bool = False) -> None:
      if not skip:
         name = TensorQuantEncoding.get_name()
         TensorQuantEncoding.free_resources(model, name = name, from_tensor_wait = True)
      super()._alloc_and_free(model, context, snip_content, skip = skip)

@param_block
class WAIT_TENSOR_11(TENSOR_WAIT_WRAP):
   event_id: Zimm5 = Zimm5(11)

@param_block
class WAIT_TENSOR_12(TENSOR_WAIT_WRAP):
   event_id: Zimm5 = Zimm5(12)

@param_block
class WAIT_TENSOR_13(TENSOR_WAIT_WRAP):
   event_id: Zimm5 = Zimm5(13)

@param_block
class WAIT_TENSOR_14(TENSOR_WAIT_WRAP):
   event_id: Zimm5 = Zimm5(14)

@param_block
class WAIT_TENSOR_15(TENSOR_WAIT_WRAP):
   event_id: Zimm5 = Zimm5(15)

@param_block
class WaitAllTensors(Component, AsmCode):
   def __init__(self, test_helper: TestHelper = None):
      self.zero = ABIreg('zero')

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(WAIT_TENSOR_LOAD_0(rd=self.zero))
      content_list.append(WAIT_TENSOR_LOAD_1(rd=self.zero))
      content_list.append(WAIT_TENSOR_LOAD_L2_0(rd=self.zero))
      content_list.append(WAIT_TENSOR_LOAD_L2_1(rd=self.zero))
      content_list.append(WAIT_PREFETCH_0(rd=self.zero))
      content_list.append(WAIT_PREFETCH_1(rd=self.zero))
      content_list.append(WAIT_CACHEOPS(rd=self.zero))
      content_list.append(WAIT_TENSOR_FMA(rd=self.zero))
      content_list.append(WAIT_TENSOR_STORE(rd=self.zero))
      content_list.append(WAIT_TENSOR_REDUCE(rd=self.zero))
      content_list.append(WAIT_TENSOR_QUANT(rd=self.zero))
