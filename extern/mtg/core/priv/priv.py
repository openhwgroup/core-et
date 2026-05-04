
from core.mtg import *
from core.priv.defines import *

@param_block
class MRET(Component, AsmCode):
   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      if TEST_CONF.RTLMIN_WORKAROUNDS['rtlmin-6398'] == True:
         content_list.append('fence iorw, iorw')
      content_list.append("mret")

@param_block
class SRET(Component, AsmCode):
   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append("sret")

@param_block
class URET(Component, AsmCode):
   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append("uret")

@param_block
class BackupXreg(Component, AsmCode):
   def __init__(self, xreg: Xreg, test_helper: TestHelper = None, glbl_op = False):
      self.sp = ABIreg('sp')
      self.xreg = xreg
      self.glbl_op = glbl_op
      assert isinstance(xreg, Xreg)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append("addi {sp}, {sp}, -8")
      if self.glbl_op:
         self.zero = ABIreg('zero')
         content_list.append("amoswapg.d {zero}, {xreg}, ({sp})")
      else:
         content_list.append("sd {xreg}, 0({sp})")

@param_block
class RestoreXreg(Component, AsmCode):
   def __init__(self, xreg: Xreg, test_helper: TestHelper = None, glbl_op = False):
      self.sp = ABIreg('sp')
      self.xreg = xreg
      self.glbl_op = glbl_op
      assert isinstance(xreg, Xreg)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      if self.glbl_op:
         self.zero = ABIreg('zero')
         content_list.append("amoorg.d {xreg}, {zero}, ({sp})")
      else:
         content_list.append("ld {xreg}, 0({sp})")
      content_list.append("addi {sp}, {sp}, 8")

@base_class
class XtoYMode(Component, AsmCode):

   __from_mode_dict = {"m" : ("mstatus", 0x01900, 11, 0x3, "mepc", MRET),
                       "s" : ("sstatus", 0x00100,  8, 0x1, "sepc", SRET),
                       "u" : ("ustatus", 0x00000,  5, 0x0, "uepc", URET)}

   __to_mode_dict = {"m" : (CSR_PRV_M, "m_mode"),
                     "s" : (CSR_PRV_S, "s_mode"),
                     "u" : (CSR_PRV_U, "u_mode")}

   from_mode = None
   to_mode = None

   def __init__(self, test_helper: TestHelper = None):
      assert self.from_mode is not None
      assert self.to_mode is not None

      self.rx0 = Xreg()
      self.zero = ABIreg('zero')

      # From mode (x)
      self.xstatus = self.__from_mode_dict[self.from_mode][0]
      self.clear_xstatus_mask = self.__from_mode_dict[self.from_mode][1]
      self.xepc = self.__from_mode_dict[self.from_mode][4]
      self.xret = self.__from_mode_dict[self.from_mode][5]()

      assert 0 <= self.__to_mode_dict[self.to_mode][0] and self.__to_mode_dict[self.to_mode][0] <= self.__from_mode_dict[self.from_mode][3], f"It is not possible to transition from {self.from_mode} to {self.to_mode}"

      # To mode (y)
      self.ypp = self.__to_mode_dict[self.to_mode][0] << self.__from_mode_dict[self.from_mode][2]
      self.ylabel = self.__to_mode_dict[self.to_mode][1]

      self.link = None
      self.condition = None

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list += [
         BackupXreg(xreg=self.rx0),
         # Clear the XSTATUS register
        f"li {{rx0}}, {self.clear_xstatus_mask}",
        f"csrrc {{zero}}, {self.xstatus}, {{rx0}}",

         # Set the XPP field (MSTATUS[12:11, SSTATUS[8]])
        f"li {{rx0}}, {self.ypp}",
        f"csrrs {{zero}}, {self.xstatus}, {{rx0}}",

         # Set the XEPC to the Y-Mode code
        f"la {{rx0}}, {self.ylabel}",
        f"csrw {self.xepc}, {{rx0}}",
        self.xret,
        f"{self.ylabel}:",
         RestoreXreg(xreg=self.rx0)
      ]

   def render_if(self, model: Model) -> bool:
      if self.link is None:
         self.condition = model.curr_priv_mode == self.from_mode
      else:
         self.condition = self.link.condition

      if self.condition:
         model.curr_priv_mode = self.to_mode

      return self.condition

#####################################
# From Machine Mode
@register
class MtoMMode(XtoYMode):
   from_mode = "m"
   to_mode = "m"

@register
class MtoSMode(XtoYMode):
   from_mode = "m"
   to_mode = "s"

@register
class MtoUMode(XtoYMode):
   from_mode = "m"
   to_mode = "u"

#####################################
# From Supervisor Mode
@register
class StoSMode(XtoYMode):
   from_mode = "s"
   to_mode = "s"

@register
class StoUMode(XtoYMode):
   from_mode = "s"
   to_mode = "u"

@register
class StoMMode(ComponentHandler):
   # This Component first jumpts to Machine Mode and then sets the Machine Previous Privilege (MPP) field from MSTATUS
   # to Machine. This is necessary to not break the MTG trap handler flow. The MTG trap handler flow always adds an MRET
   # at the very end of the mhandler. When MPP is set to Machine, when doing an MRET, the core will not change the priv mode
   # as the current mode is the same as MPP, but the core will execute the instructions after the '.word 0' (which always causes a trap to M mode"
   def __init__(self, test_helper: TestHelper = None):
      self.rx0 = Xreg()
      self.condition = None
      self.link = None

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(".word 0")

   @mepc_plus(4)
   def _mhandler(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list += [
        BackupXreg(xreg=self.rx0),
         # Set the MPP field (MSTATUS[12:11)
        "li {rx0}, 0x1800",
        "csrrs {rx0}, mstatus, {rx0}",
        RestoreXreg(xreg=self.rx0)
      ]

   def _shandler(self, content_list: list, test_helper: TestHelper = None) -> None:
      pass

   def _uhandler(self, content_list: list, test_helper: TestHelper = None) -> None:
      pass

   def render_if(self, model: Model) -> bool:
      if self.link is None:
         self.condition = model.curr_priv_mode == "s"
      else:
         self.condition = self.link.condition

      if self.condition:
         model.curr_priv_mode = "m"

      return self.condition

#####################################
# From User Mode
@register
class UtoUMode(XtoYMode):
   from_mode = "u"
   to_mode = "u"
   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      # SYS_EMU does not implement ustatus
      content_list.append("u_to_u_mode:")

@register
class UtoMMode(StoMMode):
   def render_if(self, model: Model) -> bool:
      if self.link is None:
         self.condition = model.curr_priv_mode == "u"
      else:
         self.condition = self.link.condition

      if self.condition:
         model.curr_priv_mode = "m"

      return self.condition

@register
class UtoSMode(StoMMode):
   # This Component first jumpts to Machine Mode and then sets the Machine Previous Privilege (MPP) field from MSTATUS
   # to Supervisor. When MPP is set to Supervisor, when doing an MRET, the core will change to Supervisor Mode
   @mepc_plus(4)
   def _mhandler(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list += [
        BackupXreg(xreg=self.rx0),
         # Set the MPP field (MSTATUS[12:11)
        "li {rx0}, 0x1800",
        "csrrc {rx0}, mstatus, {rx0}",
        "li {rx0}, 0x0800",
        "csrrs {rx0}, mstatus, {rx0}",
        RestoreXreg(xreg=self.rx0)
      ]

   def render_if(self, model: Model) -> bool:
      if self.link is None:
         self.condition = model.curr_priv_mode == "u"
      else:
         self.condition = self.link.condition

      if self.condition:
         model.curr_priv_mode = "s"

      return self.condition
