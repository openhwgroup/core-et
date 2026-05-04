
from core.mtg import *
from components.encoding.enc import *

## TODO: Should we replace the specific CSRRWI/CSRRW components here with some kind of mechanism to choose between them randomly?

@register
class EXCL_MODE(CSRRWI):
   pass

@register
class CACHE_INVALIDATE(CSRRWI):
   pass

@mtg_dataclass
class EXCL_MODE_WRAP(CSR_WRAP):
   rd: Xreg = field(default_factory=Xreg)
   def __component_post_init__(self, *args, **kwargs):
      self.insn = EXCL_MODE(zimm5 = self.action, rd = self.rd)
      super().__component_post_init__(*args, **kwargs)

   @machine_code
   def _run(self, content_list: list, test_helper: TestHelper = None):
      super()._run(content_list, test_helper)

@register
class SET_EXCL_MODE(EXCL_MODE_WRAP):
   action: Zimm5 = Zimm5(1)

@register
class UNSET_EXCL_MODE(EXCL_MODE_WRAP):
   action: Zimm5 = Zimm5(0)

@register
class FLB0(CSRRW):
   pass
