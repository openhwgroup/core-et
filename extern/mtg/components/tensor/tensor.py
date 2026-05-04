
from core.mtg import *
from core.models.tensorloadmodel import TensorLoadEncoding
from core.models.tensorfmamodel import TensorFMAEncoding
from core.models.tensorquantmodel import TensorQuantEncoding
from core.models.tensorstorescpmodel import TensorStoreSCPEncoding
from components.encoding.enc import *
from components.tensor_wait import WAIT_TENSOR_LOAD_0, WAIT_TENSOR_LOAD_1, WAIT_TENSOR_FMA, WAIT_TENSOR_STORE, WAIT_TENSOR_QUANT

@param_block
class FreeTl0SCP(CondComponent, AsmCode):
   def __init__(self, test_helper: TestHelper = None, parent_render_if = None):
      self.parent_render_if = parent_render_if
   def render_if(self, model: 'Model') -> bool:
      return self.parent_render_if(model) and model.l1scp.get_usage(TensorLoadEncoding.get_name(0)) > 0
   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(WAIT_TENSOR_LOAD_0())

@param_block
class FreeTl1SCP(CondComponent, AsmCode):
   def __init__(self, test_helper: TestHelper = None, parent_render_if = None):
      self.parent_render_if = parent_render_if
   def render_if(self, model: 'Model') -> bool:
      return self.parent_render_if(model) and model.l1scp.get_usage(TensorLoadEncoding.get_name(1)) > 0
   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(WAIT_TENSOR_LOAD_1())

@param_block
class FreeFMASCP(CondComponent, AsmCode):
   def __init__(self, test_helper: TestHelper = None, parent_render_if = None):
      self.parent_render_if = parent_render_if
   def render_if(self, model: 'Model') -> bool:
      return self.parent_render_if(model) and model.l1scp.get_usage(TensorFMAEncoding.get_name()) > 0
   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(WAIT_TENSOR_FMA())

@param_block
class FreeQuantSCP(CondComponent, AsmCode):
   def __init__(self, test_helper: TestHelper = None, parent_render_if = None):
      self.parent_render_if = parent_render_if
   def render_if(self, model: 'Model') -> bool:
      return self.parent_render_if(model) and model.l1scp.get_usage(TensorQuantEncoding.get_name()) > 0
   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(WAIT_TENSOR_QUANT())

@param_block
class FreeTSScpSCP(CondComponent, AsmCode):
   def __init__(self, test_helper: TestHelper = None, parent_render_if = None):
      self.parent_render_if = parent_render_if
   def render_if(self, model: 'Model') -> bool:
      return self.parent_render_if(model) and model.l1scp.get_usage(TensorStoreSCPEncoding.get_name()) > 0
   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(WAIT_TENSOR_STORE())

@param_block
class FreeL1SCP(CondComponent, AsmCode):
   def render_if(self, model: 'Model') -> bool:
      return True
   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      order = [FreeTl0SCP, FreeTl1SCP, FreeFMASCP, FreeQuantSCP, FreeTSScpSCP]
      test_helper.shuffle(order)
      for i in order:
         content_list.append(i(parent_render_if = self.render_if))

@param_block
class TwB4L1SCPReads(FreeL1SCP, AsmCode):
   def __init__(self, test_helper: TestHelper = None, tquant = False):
      self.tquant = tquant

   def render_if(self, model: 'Model') -> bool:
      free_regions = model.l1scp.get_read_free_regions()
      if self.tquant:
         # RTL does some L1SCP prefetching, even if the data is not consumed.
         # Consider only L1SCP regions with at least 2 free lines
         free_regions = [(x, y) for (x, y) in free_regions if x != y]
      return len(free_regions) == 0

@param_block
class TwB4L1SCPWrites(FreeL1SCP, AsmCode):
   def __init__(self, test_helper: TestHelper = None, tl_tenb = 0):
      self.tl_tenb = tl_tenb

   def render_if(self, model: 'Model') -> bool:
      if self.tl_tenb == 0:
         free_regions = model.l1scp.get_write_free_regions()
         return len(free_regions) == 0
      else:
         return False

@param_block
class TwPrevTsSCP(CondComponent, AsmCode):
   def render_if(self, model: 'Model') -> bool:
      return model.pending_tensor_store_scp
   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(WAIT_TENSOR_STORE())

@mtg_dataclass
class TENSOR_CSRRW(CSRRW):
   _tensor = True
   enc: TensorEncoding = None
   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      assert self.enc is not None, "Tensor CSRRW Component encoding is 'None'"
      content_list.append('csrrw {rd}, {opcode}, {rs1} /*{enc}*/\n')

@mtg_dataclass
class TENSOR_CSRRW_X31(TENSOR_CSRRW):
   x31: Xreg = field(default_factory=X31reg)
   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      assert self.enc is not None, "Tensor CSRRW X31 Component encoding is 'None'"
      assert self.x31.reg_num == 31, f"{self.__class__.__name__} implicitly uses X31, but the 'x31' attribute is {self.x31}"
      content_list.append('csrrw {rd}, {opcode}, {rs1} /*{enc}, {x31}*/\n')

@register
class TENSOR_LOAD(TENSOR_CSRRW_X31): pass

@register
class TENSOR_LOAD_L2(TENSOR_CSRRW_X31): pass

@register
class TENSOR_FMA(TENSOR_CSRRW): pass

@register
class TENSOR_STORE(TENSOR_CSRRW_X31): pass

@register
class TENSOR_STORE_SCP(TENSOR_CSRRW_X31):
   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      self.opcode = "tensor_store"
      return super()._run(content_list, test_helper = test_helper)

@register
class TENSOR_QUANT(TENSOR_CSRRW): pass

@register
class TENSOR_REDUCE(TENSOR_CSRRW): pass

@register
class TENSOR_COOP(TENSOR_CSRRW): pass
