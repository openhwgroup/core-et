from core.models.model import *
from core.models.tensorcoopmodel import TensorCoopEncoding
from functools import wraps
from components.read_hartid import ReadHartID

def _get_rand_num(test_helper):
   if test_helper != None:
      return test_helper.get_rand_num()
   else:
      return 0

def only_mid(mid: int):
   def wrapper_args(wrapped_method):
      @wraps(wrapped_method)
      def wrapper(self, content_list, test_helper = None):
         if not hasattr(self, '__mid_rx0'):
            self.__mid_rx0 = Xreg()
         if not hasattr(self, '__mid_rx1'):
            self.__mid_rx1 = Xreg()
         if TEST_CONF.agnostic_mode():
            if not hasattr(self, '__rn0'):
               self.__rn0 = _get_rand_num(test_helper)
         label = f"{wrapped_method.__name__}_skip_{self.name}"

         content_list.append(ReadHartID(test_helper=test_helper, rd=self.__mid_rx0))
         content_list.append("srli {__mid_rx0}, {__mid_rx0}, 1")
         content_list.append(f"li {{__mid_rx1}}, {mid}")
         content_list.append(f"bne {{__mid_rx1}}, {{__mid_rx0}}, {label}")
         wrapped_method(self, content_list, test_helper)

         content_list.append(f"{label}:")
         if TEST_CONF.agnostic_mode():
            content_list.append(f"li {{__mid_rx0}}, {self.__rn0}")
      return wrapper
   return wrapper_args

def only_phid(phid: int):
   def wrapper_args(wrapped_method):
      @wraps(wrapped_method)
      def wrapper(self, content_list, test_helper = None):
         if not hasattr(self, '__phid_rx0'):
            self.__phid_rx0 = Xreg()
         if not hasattr(self, '__phid_rx1'):
            self.__phid_rx1 = Xreg()
         if TEST_CONF.agnostic_mode():
            if not hasattr(self, '__rn0'):
               self.__rn0 = _get_rand_num(test_helper)
         label = f"{wrapped_method.__name__}_skip_{self.name}"

         content_list.append(ReadHartID(test_helper=test_helper, rd=self.__phid_rx0))
         content_list.append(f"li {{__phid_rx1}}, {phid}")
         content_list.append(f"bne {{__phid_rx1}}, {{__phid_rx0}}, {label}")
         wrapped_method(self, content_list, test_helper)

         content_list.append(f"{label}:")
         if TEST_CONF.agnostic_mode():
            content_list.append(f"li {{__phid_rx0}}, {self.__rn0}")
      return wrapper
   return wrapper_args

def only_thread(tid: int):
   def wrapper_args(wrapped_method):
      @wraps(wrapped_method)
      def wrapper(self, content_list, test_helper = None):
         self.__zero = ABIreg('zero')
         if not hasattr(self, '__thr_rx0'):
            self.__thr_rx0 = Xreg()
         if TEST_CONF.agnostic_mode():
            if not hasattr(self, '__rn0'):
               self.__rn0 = _get_rand_num(test_helper)
         label = f"{wrapped_method.__name__}_skip_{self.name}"

         content_list.append(ReadHartID(test_helper=test_helper, rd=self.__thr_rx0))
         content_list.append("andi {__thr_rx0}, {__thr_rx0}, 1")
         if tid == 0:
            content_list.append(f"bne {{__zero}}, {{__thr_rx0}}, {label}")
         elif tid == 1:
            content_list.append(f"beq {{__zero}}, {{__thr_rx0}}, {label}")
         else:
            raise ValueError(f"Wrong value in @only_thread decorator ('{wrapped_method.__name__}' from Component '{self.__class__.__name__}'). Thread id must be '0' or '1' but provided '{tid}'")

         wrapped_method(self, content_list, test_helper)

         content_list.append(f"{label}:")
         if TEST_CONF.agnostic_mode():
            content_list.append(f"li {{__thr_rx0}}, {self.__rn0}")
      return wrapper
   return wrapper_args

def only_lhid(lhid: int):
   def wrapper_args(wrapped_method):
      @wraps(wrapped_method)
      def wrapper(self, content_list, test_helper = None):
         assert lhid < TEST_CONF.NUM_HARTS, f"Wrong value in @only_lhid decorator ('{wrapped_method.__name__}' from Component '{self.__class__.__name__}'). Max num HARTs is {TEST_CONF.NUM_HARTS} but provided logical HART ID is {lhid}"
         self.__zero = ABIreg('zero')
         self.__lhid = ABIreg('lhid')
         if not hasattr(self, '__lhid_rx0'):
            self.__lhid_rx0 = Xreg()
         label = f"{wrapped_method.__name__}_skip_{self.name}"

         content_list.append(f"li {{__lhid_rx0}}, {lhid}")
         content_list.append(f"bne {{__lhid}}, {{__lhid_rx0}}, {label}")

         wrapped_method(self, content_list, test_helper)

         content_list.append(f"{label}:")
      return wrapper
   return wrapper_args

def only_mod_lhid(divisor: int, lhid: int):
   def wrapper_args(wrapped_method):
      @wraps(wrapped_method)
      def wrapper(self, content_list, test_helper = None):
         assert lhid < TEST_CONF.NUM_HARTS, f"Wrong value in @only_mod_lhid decorator ('{wrapped_method.__name__}' from Component '{self.__class__.__name__}'). Max num HARTs is {TEST_CONF.NUM_HARTS} but provided logical HART ID is {lhid}"
         assert lhid < divisor, f"Wrong value in @only_mod_lhid decorator ('{wrapped_method.__name__}' from Component '{self.__class__.__name__}'). Divisor is {divisor} but provided logical HART ID is {lhid}"
         self.__zero = ABIreg('zero')
         self.__lhid = ABIreg('lhid')
         if not hasattr(self, '__mod_lhid_rx0'):
            self.__mod_lhid_rx0 = Xreg()
         if not hasattr(self, '__mod_lhid_rx1'):
            self.__mod_lhid_rx1 = Xreg()
         label = f"{wrapped_method.__name__}_skip_{self.name}"

         content_list.append(f"li {{__mod_lhid_rx0}}, {lhid}")
         content_list.append(f"li {{__mod_lhid_rx1}}, {divisor}")
         content_list.append("rem {__mod_lhid_rx1}, {__lhid}, {__mod_lhid_rx1}")
         content_list.append(f"bne {{__mod_lhid_rx1}}, {{__mod_lhid_rx0}}, {label}")

         wrapped_method(self, content_list, test_helper)

         content_list.append(f"{label}:")
      return wrapper
   return wrapper_args

def only_tensor_coop(wrapped_method):
   @wraps(wrapped_method)
   def wrapper(self, content_list, test_helper = None):
      self.__zero = ABIreg('zero')

      if not hasattr(self, '__tcoop_coop_mask'):
         self.__tcoop_coop_mask = Xreg()
      if not hasattr(self, '__tcoop_hid'):
         self.__tcoop_hid = Xreg()
      if not hasattr(self, '__tcoop_mid'):
         self.__tcoop_mid = Xreg()
      if not hasattr(self, '__tcoop_nid'):
         self.__tcoop_nid = Xreg()
      if not hasattr(self, '__tcoop_mask'):
         self.__tcoop_mask = Xreg()
      if not hasattr(self, '__tcoop_one'):
         self.__tcoop_one = Xreg()
      if TEST_CONF.agnostic_mode():
         if not hasattr(self, '__rn0'):
            self.__rn0 = _get_rand_num(test_helper)
         if not hasattr(self, '__rn1'):
            self.__rn1 = _get_rand_num(test_helper)
         if not hasattr(self, '__rn2'):
            self.__rn2 = _get_rand_num(test_helper)
         if not hasattr(self, '__rn3'):
            self.__rn3 = _get_rand_num(test_helper)

      label = f"{wrapped_method.__name__}_coop_skip_{self.name}"

      content_list.append(f"li {{__tcoop_coop_mask}}, {TensorCoopEncoding.tensor_coop}")
      content_list.append(ReadHartID(test_helper=test_helper, rd=self.__tcoop_hid))

      # Get the Minion ID from the Neigh POV
      content_list.append("li {__tcoop_mid}, 16")
      content_list.append("remu {__tcoop_mid}, {__tcoop_hid}, {__tcoop_mid}")
      content_list.append("srli {__tcoop_mid}, {__tcoop_mid}, 1")

      # Get the Neigh ID from the Shire POV
      content_list.append("li {__tcoop_nid}, 64")
      content_list.append("remu {__tcoop_nid}, {__tcoop_hid}, {__tcoop_nid}")
      if TEST_CONF.agnostic_mode():
         content_list.append(f"li {{__tcoop_hid}}, {self.__rn0}")
      content_list.append("srli {__tcoop_nid}, {__tcoop_nid}, 4")

      # Convert ID to bit mask
      content_list.append("li {__tcoop_one}, 1")
      content_list.append("sll {__tcoop_nid}, {__tcoop_one}, {__tcoop_nid}")
      content_list.append("sll {__tcoop_mid}, {__tcoop_one}, {__tcoop_mid}")

      # Compute the mask
      content_list.append("slli {__tcoop_nid}, {__tcoop_nid}, 8")
      content_list.append("or {__tcoop_mask}, {__tcoop_nid}, {__tcoop_mid}")
      if TEST_CONF.agnostic_mode():
         content_list.append(f"li {{__tcoop_mid}}, {self.__rn3}")
         content_list.append(f"li {{__tcoop_nid}}, {self.__rn1}")

      # Check if I am in the mask
      content_list.append("or {__tcoop_mask}, {__tcoop_coop_mask}, {__tcoop_mask}")
      content_list.append(f"bne {{__tcoop_mask}}, {{__tcoop_coop_mask}}, {label}")

      wrapped_method(self, content_list, test_helper)

      content_list.append(f"{label}:")
      if TEST_CONF.agnostic_mode():
         content_list.append(f"li {{__tcoop_mask}}, {self.__rn2}")
   return wrapper

