
from core.mtg import *
from components.randinst.rv32i import LI, FENCE, ORI
from components.randinst.rv64i import LD, SD

from components.sc_esr.sc_esr import SetShireCacheESR

@register
class SCL3YieldPriority(SetShireCacheESR):
   sc_read_esr_addr: ESR_REGION = ESR_SC_REQQ_CTL_B0
   sc_write_esr_addr: ESR_REGION = ESR_SC_REQQ_CTL_BCAST

   def _set_value(self, content_list: list, read_reg: Xreg, write_reg: Xreg, aux_reg: Xreg) -> None:
      # 'esr_sc_l3_yield_priority' correspond to the bits 'esr_sc_reqq_ctl[4:0]'
      # By default requests from the L3 slave have priority for the pipeline over requests from the L2.
      # If this register is set to a non-zero value, the priority will shift to L2 if the number of L3
      # requests picked over valid L2 requests meets or exceeds the value in this register.
      if TEST_CONF.DEBUG_PRINTS == True:
         num_l3_req = 0xf
      else:
         num_l3_req = 0x1f
      content_list.append(ORI(rd=write_reg, rs1=read_reg, imm=0xf))
