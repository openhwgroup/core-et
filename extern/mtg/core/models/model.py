
from core.models.maskmodel import *
from core.models.intmodel import *
from core.models.immmodel import *
from core.models.abimodel import *
from core.models.fpmodel import *
from core.models.memorymodel import *
from core.models.l1scpmodel import L1SCPModel
from core.models.tensormodel import TensorEncoding
from core.models.test_helper import TestHelper

class Stack:
   def __init__(self):
      self.elements = []
   def push(self, data):
      self.elements.append(data)
      return data
   def pop(self):
      return self.elements.pop()
   def top(self):
      return self.elements[-1]
   def is_empty(self):
      return len(self.elements) == 0

def get_mask_bits(mask: int) -> list:
   bits = list()
   idx = 0
   while mask > 0:
      if mask & 0x1:
         bits.append(idx)
      idx += 1
      mask >>= 1
   return bits

def get_mask_from_bits(bits: list) -> int:
   mask = 0x0
   for bit in bits:
      mask += (1 << bit)
   return mask

tr_active_shire_minions = get_mask_bits(TEST_CONF.MINION_MASK)
tr_active_shires = get_mask_bits(TEST_CONF.SHIRE_MASK)
tr_active_minions =  [minion + (shire*32) for shire in tr_active_shires for minion in tr_active_shire_minions]

MAX_TREE_LVL = 10

class Model:

   gp_is_set = False
   tp_is_set = False

   gamotp_is_set = False
   lamotp_is_set = False

   lhid_is_set = False

   fp_amo_is_set = False
   fp_off_is_set = False

   l2scp_initialized = False

   def __init__(self, seed: int = 0) -> None:
      self.test_helper = TestHelper(seed=seed)
      self.mask = MaskModel(self.test_helper)
      self.integer = IntModel(self.test_helper)
      self.floatpoint = FPModel(self.test_helper)
      self.l1scp = L1SCPModel(self.test_helper)

      self.tensor_mask = 0x0
      self.tensor_conv_size = 0x0
      self.tensor_conv_ctrl = 0x0

      self.integer.set_ABI_reg(ABIreg('zero'))
      self.integer.set_ABI_reg(ABIreg('ra'))
      self.integer.set_ABI_reg(ABIreg('sp'))
      self.integer.set_ABI_reg(ABIreg('gp'))
      self.integer.set_ABI_reg(ABIreg('tp'))
      self.integer.set_ABI_reg(ABIreg('gamotp'))
      self.integer.set_ABI_reg(ABIreg('lamotp'))
      self.integer.set_ABI_reg(ABIreg('lhid'))
      self.integer.set_ABI_reg(ABIreg('mhandler'))
      self.integer.set_ABI_reg(ABIreg('shandler'))
      self.integer.set_ABI_reg(ABIreg('uhandler'))

      self.floatpoint.set_ABI_reg(FPABIreg('amo'))
      self.floatpoint.set_ABI_reg(FPABIreg('off'))

      self.pending_coop_tensor_load_0 = False
      self.pending_coop_tensor_load_1 = False
      self.pending_tensor_fma_tena = False
      self.pending_tensor_fma_tenb = False
      self.pending_tensor_fma_to_rf = False
      self.pending_tensor_quant = False
      self.pending_tensor_store_scp = False

      self.tr_last_reg = None

      self.treduce_tree_lvls = list()
      for depth in range(MAX_TREE_LVL+1):
         valid_depth = True
         nop = True
         for minionid in tr_active_minions:
            cond = minionid % (2**(depth+1))
            if cond == 0:
               partner = minionid + 2**depth
            elif cond == (2**depth):
               partner = minionid - 2**depth
            else:
               continue
            nop = False

            if partner not in tr_active_minions:
               valid_depth = False
               break

         if valid_depth and not nop:
            self.treduce_tree_lvls.append(depth)

         self.treduce_tree_lvls_weights = list(range(len(self.treduce_tree_lvls)))
         self.treduce_tree_lvls_weights.reverse()
         self.treduce_tree_lvls_weights = [(x+1)*2 for x in self.treduce_tree_lvls_weights]

         self.curr_priv_mode = "m"

   def isABISet(self) -> bool:
      return self.gp_is_set and self.tp_is_set and self.gamotp_is_set and self.lamotp_is_set and self.lhid_is_set and self.fp_amo_is_set and self.fp_off_is_set
