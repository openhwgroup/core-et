
from core.mtg import *
from components.evict_all_l1d import EvictAllL1D
from components.randinst.rv32i import LI
from components.cops.coherency_barrier import CoherencyBarrier

@register
class TargetRandomization(Component, AsmCode):

   def __init__(self, test_helper: TestHelper = None):
      self.rx0 = Xreg(10) # a0 (aka x10) is used to pass arguments to C functions
      self.rx1 = Xreg(11) # a1 (aka x11) is used to pass arguments to C functions
      self.lhid = ABIreg('lhid')
      self.seed0 = test_helper.get_rand_num()
      self.seed1 = test_helper.get_rand_num()

   def _init_rand_data_from_target(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append("addi {rx0}, {lhid}, 0")
      content_list.append(LI(rd=self.rx1, imm=self.seed0))
      content_list.append(JumpToCFunct(test_helper=test_helper, funct_name="init_rand_data"))

   @only_lhid(0)
   def _init_rem_rand_data_from_target(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(LI(rd=self.rx0, imm=self.seed1))
      content_list.append(JumpToCFunct(test_helper=test_helper, funct_name="init_rem_rand_data"))

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      self._init_rand_data_from_target(content_list, test_helper=test_helper)
      self._init_rem_rand_data_from_target(content_list, test_helper=test_helper)

      content_list.append(CoherencyBarrier(test_helper = test_helper))
      if TEST_CONF.POSTSI_L2_ONLY == True or bin(TEST_CONF.SHIRE_MASK).count('1') > 1:
         from components.flb_fcc.fcc_barrier import FCCBarrier
         content_list.append(FCCBarrier(test_helper=test_helper))

if TEST_CONF.TARGET_RANDOMIZATION == False:
   TEST_CONF.EXCL_COMPS.add(TargetRandomization)
