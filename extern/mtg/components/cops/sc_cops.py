
from core.mtg import *
from components.randinst.rv32i import LI, FENCE, SRLI, AND
from components.randinst.rv64i import LD, SD
from components.sc_esr.sc_esr import SetShireCacheESR

@param_block
class WaitShireCacheCOP(Component, AsmCode):

   def __init__(self, test_helper: TestHelper = None, bank: int = 0, rega: Xreg = None, regb: Xreg = None):
      self.bank = bank
      self.b0 = StaticGMalloc('WaitShireCacheCOP_b0',  ESR_SC_IDX_COP_SM_CTL_B0, 8, align=8)
      self.b1 = StaticGMalloc('WaitShireCacheCOP_b1',  ESR_SC_IDX_COP_SM_CTL_B1, 8, align=8)
      self.b2 = StaticGMalloc('WaitShireCacheCOP_b2',  ESR_SC_IDX_COP_SM_CTL_B2, 8, align=8)
      self.b3 = StaticGMalloc('WaitShireCacheCOP_b3',  ESR_SC_IDX_COP_SM_CTL_B3, 8, align=8)

      switch_case = { 0: self.b0, 1: self.b1, 2: self.b2, 3: self.b3 }
      self.addr = switch_case[bank]

      self.active = Zimm8(8)
      if rega == None:
         self.rega = Xreg()
      else:
         self.rega = rega

      if regb == None:
         self.regb = UXreg()
      else:
         assert isinstance(regb, UXreg), f"{self.__class__.__name__} regb parameter is not instance of UXreg"
         self.regb = regb

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      #  while (cop_sm_state == SC_COP_SM_ACTIVE)
      wait_insns = [
         # ((*(ADDR) >> 24) && 0xff)
         f"wait_shire_cache_cops_{self.bank}:",
         LI(rd=self.rega, imm=self.addr),
         LD(rd=self.regb, off=Imm12(0), rs1=self.rega),
         SRLI(rd=self.regb, rs1=self.regb, imm=Zimm6(24)),
         LI(rd=self.rega, imm=Zimm8(0xff)),
         AND(rd=self.regb, rs1=self.regb, rs2=self.rega),
         LI(rd=self.rega, imm=self.active),
         f"beq {{regb}}, {{rega}}, wait_shire_cache_cops_{self.bank}",
      ]

      content_list += wait_insns

@base_class
class ShireCacheCOP(SetShireCacheESR):

   sc_read_esr_addr: ESR_REGION = None
   sc_write_esr_addr: ESR_REGION = ESR_SC_IDX_COP_SM_CTL_BCAST

   @abstractmethod
   def get_sc_idx_cop_sm_cmd(self) -> Zimm64:
      pass

   def _read_sc_esr(self, content_list: list, addr_reg: Xreg, read_reg: Xreg) -> None:
      # There is no need to read the previous value
      pass

   def _set_value(self, content_list: list, read_reg: Xreg, write_reg: Xreg, aux_reg: Xreg) -> None:
      self.cmd = self.get_sc_idx_cop_sm_cmd()
      content_list.append(LI(rd=write_reg, imm=self.cmd))

   def _post_set_value(self, content_list: list, aux_reg0: Xreg, aux_reg1: Xreg, aux_reg2: Xreg) -> None:
      assert isinstance(aux_reg1, UXreg), f"{self.__class__.__name__} aux_reg1 parameter is not instance of UXreg. Found {type(aux_reg1)}"
      content_list += [
         WaitShireCacheCOP(bank=0, rega=aux_reg0, regb=aux_reg1),
         WaitShireCacheCOP(bank=1, rega=aux_reg0, regb=aux_reg1),
         WaitShireCacheCOP(bank=2, rega=aux_reg0, regb=aux_reg1),
         WaitShireCacheCOP(bank=3, rega=aux_reg0, regb=aux_reg1)
      ]

@register
class EvictAllL2(ShireCacheCOP):
   def get_sc_idx_cop_sm_cmd(self) -> Zimm64:
      return Zimm64(0x00000301) #cmd

@register
class EvictAllL3(ShireCacheCOP):
   def get_sc_idx_cop_sm_cmd(self) -> Zimm64:
      return Zimm64(0x00000601) #cmd

@register
class ClearL2SCP(ShireCacheCOP):
   def render_if(self, model: 'Model') -> None:
      model.l2scp_initialized = True
      return super().render_if(model)

   def get_sc_idx_cop_sm_cmd(self) -> Zimm64:
      return Zimm64(0x00000901) #cmd

@register
class EvictCoalescingBuffer(ShireCacheCOP):
   def get_sc_idx_cop_sm_cmd(self) -> Zimm64:
      return Zimm64(0x00000a01) #cmd

if TEST_CONF.POSTSI_L2_ONLY == True:
   TEST_CONF.EXCL_COMPS.add(EvictAllL2)
   TEST_CONF.EXCL_COMPS.add(EvictAllL3)
   TEST_CONF.EXCL_COMPS.add(EvictCoalescingBuffer)
