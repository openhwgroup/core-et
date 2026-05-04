
from core.mtg import *
from components.preamble.set_thread_pointer import SetThreadPointer
from components.preamble.set_global_pointer import SetGlobalPointer
from components.preamble.set_lamo_tp import SetLAMOThreadPointer
from components.preamble.set_gamo_tp import SetGAMOThreadPointer
from components.preamble.set_stack_pointer import SetStackPointer
from components.randinst.rv32i import LI

@register
class SPChecks(Component, AsmCode):

   def __init__(self, test_helper: TestHelper = None):
      # Stack pointer
      self.sp = ABIreg('sp')

      self.rx0 = Xreg()

   def render_if(self, model: Model) -> bool:
      return TEST_CONF.POSTSI_L2_ONLY == True

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      sp_chk = [
         SetStackPointer(sp=self.rx0),
         "beq {rx0}, {sp}, sp_pass",
         TEST_FAIL(),
         "sp_pass:"
      ]
      content_list += sp_chk

@param_block
class ReadFCC(Component, AsmCode):

   def __init__(self, test_helper: TestHelper = None, rd: Xreg = None):
      if rd is None:
         self.rx0 = Xreg()
      else:
         self.rx0 = rd
      self.zero = ABIreg('zero')

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append("csrrs {rx0}, fccnb, {zero}")

@register
class EOTChecks(Component, AsmCode):

   def __init__(self, test_helper: TestHelper = None):
      # Thread pointer
      self.tp = ABIreg('tp')
      # Global pointer
      self.gp = ABIreg('gp')
      # Local AMO thread pointer
      self.lamotp = ABIreg('lamotp')
      # Global AMO thread pointer
      self.gamotp = ABIreg('gamotp')

      self.rx0 = Xreg()
      self.rx1 = Xreg()

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(SPChecks(test_helper = test_helper))

      fcc_chk = [
         ReadFCC(rd=self.rx0),
         "beq x0, {rx0}, fcc_pass",
         TEST_FAIL(),
         "fcc_pass:"
      ]
      content_list += fcc_chk

      tp_chk = [
         SetThreadPointer(tp=self.rx0),
         "beq {rx0}, {tp}, tp_pass",
         TEST_FAIL(),
         "tp_pass:"
      ]
      content_list += tp_chk

      gp_chk = [
         SetGlobalPointer(gp=self.rx0),
         "beq {rx0}, {gp}, gp_pass",
         TEST_FAIL(),
         "gp_pass:"
      ]
      content_list += gp_chk

      lamotp_chk = [
         SetLAMOThreadPointer(lamotp=self.rx0),
         "beq {rx0}, {lamotp}, lamotp_pass",
         TEST_FAIL(),
         "lamotp_pass:"
      ]
      content_list += lamotp_chk

      gamotp_chk = [
         SetGAMOThreadPointer(gamotp=self.rx0),
         "beq {rx0}, {gamotp}, gamotp_pass",
         TEST_FAIL(),
         "gamotp_pass:"
      ]
      content_list += gamotp_chk
