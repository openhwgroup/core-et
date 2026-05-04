
from core.mtg import *
from components.encoding.enc import *
from components.randinst.cacheops import MCACHE_CONTROL

@mtg_dataclass
class MCACHE_CONTROL_WRAP(CSR_WRAP):
   rd: Xreg = field(default_factory=Xreg)
   def __component_post_init__(self, *args, **kwargs):
      if TEST_CONF.REPEAT_COUNT != -1:
         self.rd = ABIreg('zero')
      self.insn = MCACHE_CONTROL(zimm5 = self.state, rd = self.rd)
      super().__component_post_init__(*args, **kwargs)

@register
class SET_DCACHE_SHARED(MCACHE_CONTROL_WRAP):
   state: Zimm5 = Zimm5(0)

@register
class SET_D1SPLIT(MCACHE_CONTROL_WRAP):
   state: Zimm5 = Zimm5(1)

@register
class SET_SCPENABLE(MCACHE_CONTROL_WRAP):
   state: Zimm5 = Zimm5(3)
