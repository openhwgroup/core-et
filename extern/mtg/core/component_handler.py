from abc import ABCMeta, abstractmethod
from core.component import Component, base_class
from core.models.test_helper import TestHelper
from core.snippet.asm import AsmCode
from core.models.model import *
from functools import wraps

def csr_plus(off: int, csr_reg: str = None):
   def wrapper_args(wrapped_method):
      @wraps(wrapped_method)
      def wrapper(self, content_list, test_helper = None):
         wrapped_method(self, content_list, test_helper)
         self._rx0_ = Xreg()
         self._zero_ = ABIreg('zero')
         content_list.append(f"csrrs {{_rx0_}}, {csr_reg}, {{_zero_}}")
         content_list.append(f"addi {{_rx0_}}, {{_rx0_}}, {off}")
         content_list.append(f"csrrw {{_zero_}}, {csr_reg}, {{_rx0_}}")
      return wrapper
   return wrapper_args

def mepc_plus(off: int):
   return csr_plus(off, "mepc")

def sepc_plus(off: int):
   return csr_plus(off, "sepc")

def uepc_plus(off: int):
   return csr_plus(off, "uepc")

def set_csr_to(label: str, csr_reg = None):
   def wrapper_args(wrapped_method):
      @wraps(wrapped_method)
      def wrapper(self, content_list, test_helper = None):
         wrapped_method(self, content_list, test_helper)
         self._rx0_ = Xreg()
         self._zero_ = ABIreg('zero')
         content_list.append(f"la {{_rx0_}}, {label}")
         content_list.append(f"csrrw {{_zero_}}, {csr_reg}, {{_rx0_}}")
      return wrapper
   return wrapper_args

def set_mepc_to(label: str):
   return set_csr_to(label, csr_reg="mepc")

def set_sepc_to(label: str):
   return set_csr_to(label, csr_reg="sepc")

def set_uepc_to(label: str):
   return set_csr_to(label, csr_reg="uepc")

@base_class
class ComponentHandler(Component, AsmCode, metaclass=ABCMeta):

   add_handlers = True
   _backup_excp_ptr = True

   @abstractmethod
   def _mhandler(self, content_list: list, test_helper: TestHelper = None) -> None:
      pass

   @abstractmethod
   def _shandler(self, content_list: list, test_helper: TestHelper = None) -> None:
      pass

   def _uhandler(self, content_list: list, test_helper: TestHelper = None) -> None:
      pass

   def _add_mhandler(self, content_list: list, test_helper: TestHelper = None) -> None:
      self._sp_ = ABIreg('sp')
      self._mexcp_ = ABIreg('mhandler')
      handler_content_list = list()
      try:
         self._mhandler(handler_content_list, test_helper = test_helper)
      except TypeError:
         self._mhandler(handler_content_list)
      if len(handler_content_list) > 0:
         insns = list()
         if self._backup_excp_ptr:
            insns += ["addi {_sp_}, {_sp_}, -8\n",
                      "sd {_mexcp_}, 0({_sp_})\n"]
         insns.append("la {_mexcp_}, comp_mhandler\n")
         content_list[:0] += insns
         if self._backup_excp_ptr:
            content_list += ["ld {_mexcp_}, 0({_sp_})\n",
                             "addi {_sp_}, {_sp_}, 8\n"]
         handler_content_list[:0] += [
            f"start {MTG_HANDLER.M_HINT}\n",
            "comp_mhandler:\n"
         ]
         handler_content_list += [
            "jalr  x0, {_mexcp_}, 0\n",
            f"end {MTG_HANDLER.M_HINT}\n"
         ]
      return handler_content_list

   def _add_shandler(self, content_list: list, test_helper: TestHelper = None) -> None:
      self._sp_ = ABIreg('sp')
      self._sexcp_ = ABIreg('shandler')
      handler_content_list = list()
      try:
         self._shandler(handler_content_list, test_helper = test_helper)
      except TypeError:
         self._shandler(handler_content_list)
      if len(handler_content_list) > 0:
         insns = list()
         if self._backup_excp_ptr:
            insns += ["addi {_sp_}, {_sp_}, -8\n",
                      "sd {_sexcp_}, 0({_sp_})\n"]
         insns.append("la {_sexcp_}, comp_shandler\n")
         content_list[:0] += insns
         if self._backup_excp_ptr:
            content_list += ["ld {_sexcp_}, 0({_sp_})\n",
                             "addi {_sp_}, {_sp_}, 8\n"]
         handler_content_list[:0] += [
            f"start {MTG_HANDLER.S_HINT}\n",
            "comp_shandler:\n"
         ]
         handler_content_list += [
            "jalr  x0, {_sexcp_}, 0\n",
            f"end {MTG_HANDLER.S_HINT}\n"
         ]
      return handler_content_list

   def _add_uhandler(self, content_list: list, test_helper: TestHelper = None) -> None:
      self._sp_ = ABIreg('sp')
      self._uexcp_ = ABIreg('uhandler')
      handler_content_list = list()
      try:
         self._uhandler(handler_content_list, test_helper = test_helper)
      except TypeError:
         self._uhandler(handler_content_list)
      if len(handler_content_list) > 0:
         insns = list()
         if self._backup_excp_ptr:
            insns += ["addi {_sp_}, {_sp_}, -8\n",
                      "sd {_uexcp_}, 0({_sp_})\n"]
         insns.append("la {_uexcp_}, comp_uhandler\n")
         content_list[:0] += insns
         if self._backup_excp_ptr:
            content_list += ["ld {_uexcp_}, 0({_sp_})\n",
                             "addi {_sp_}, {_sp_}, 8\n"]
         handler_content_list[:0] += [
            f"start {MTG_HANDLER.U_HINT}\n",
            "comp_uhandler:\n"
         ]
         handler_content_list += [
            "jalr  x0, {_uexcp_}, 0\n",
            f"end {MTG_HANDLER.U_HINT}\n"
         ]
      return handler_content_list

   def _component_handler(self, content_list: list, test_helper: TestHelper = None) -> None:
      functs = [self._add_mhandler, self._add_shandler, self._add_uhandler]
      if test_helper != None:
         test_helper.shuffle(functs)

      h0 = functs[0](content_list, test_helper=test_helper)
      h1 = functs[1](content_list, test_helper=test_helper)
      h2 = functs[2](content_list, test_helper=test_helper)

      content_list += h0
      content_list += h1
      content_list += h2

   def __component_post_init__(self, *args, **kwargs):
      super().__component_post_init__(*args, **kwargs)

      if self.add_handlers:
         test_helper = None
         # Try to find a TestHelper instance in args
         for i in args:
            if isinstance(i, TestHelper):
               test_helper = i
               break
         if test_helper is None:
            # Try to find a TestHelper instance in keyword args
            for i in kwargs:
               if isinstance(kwargs[i], TestHelper):
                  test_helper = kwargs[i]
                  break

         self._component_handler(self._component_list, test_helper=test_helper)
