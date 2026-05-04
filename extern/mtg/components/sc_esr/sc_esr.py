
from core.mtg import *
from components.randinst.rv32i import LI, FENCE
from components.randinst.rv64i import LD, SD
from components.flb_fcc.flb_fcc_sync_threads import FLBFCCSyncShireThreads

@base_class
class SetShireCacheESR(FLBFCCSyncShireThreads):

   sc_read_esr_addr: ESR_REGION = None
   sc_write_esr_addr: ESR_REGION = None

   def _read_sc_esr(self, content_list: list, addr_reg: Xreg, read_reg: Xreg) -> None:
      self.read_addr = StaticGMalloc(f"{self.__class__.__name__}_read",  self.sc_read_esr_addr, 8, align=8)
      content_list += [
                        LI(rd=addr_reg, imm=self.read_addr),
                        LD(rd=read_reg, off=0, rs1=addr_reg)
                      ]

   def _write_sc_esr(self, content_list: list, addr_reg: Xreg, write_reg: Xreg) -> None:
      self.write_addr = StaticGMalloc(f"{self.__class__.__name__}_write",  self.sc_write_esr_addr, 8, align=8)
      content_list += [
                        LI(rd=addr_reg, imm=self.write_addr),
                        SD(rs2=write_reg, off=0, rs1=addr_reg),
                        FENCE()
                      ]

   @abstractmethod
   def _set_value(self, content_list: list, read_reg: Xreg, write_reg: Xreg, aux_reg: Xreg) -> None:
      pass

   def _post_set_value(self, content_list: list, aux_reg0: Xreg, aux_reg1: Xreg, aux_reg2: Xreg) -> None:
      pass

   @machine_code
   def _lead_hart_code(self, content_list: list, test_helper: TestHelper = None) -> None:
      self.addr_reg = UXreg()
      self.read_reg = UXreg()
      self.write_reg = UXreg()

      self._read_sc_esr(content_list, addr_reg=self.addr_reg, read_reg=self.read_reg)
      self._set_value(content_list, read_reg=self.read_reg, write_reg=self.write_reg, aux_reg=self.addr_reg)
      self._write_sc_esr(content_list, addr_reg=self.addr_reg, write_reg=self.write_reg)
      self._post_set_value(content_list, aux_reg0=self.addr_reg, aux_reg1=self.read_reg, aux_reg2=self.write_reg)
