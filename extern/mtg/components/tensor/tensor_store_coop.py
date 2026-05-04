
from core.mtg import *
from components.tensor.tensor_store import TensorStore
from components.randinst.rv32i import LI, ADD
from components.randinst.rv32m import MUL
from core.models.model import tr_active_minions
from components.cops.sc_cops import EvictCoalescingBuffer

ts_all_coop_modes = [2, 4]
ts_coop_modes = list()

for coop_mode in ts_all_coop_modes:
   idx = 0
   valid = True
   while idx <= TEST_CONF.MAX_NUM_MINIONS:
      candidates = list(range(idx, idx+coop_mode, 1))
      if any(mid in tr_active_minions for mid in candidates):
         if not all(mid in tr_active_minions for mid in candidates):
            valid = False
            break
      idx += coop_mode
   if valid:
      ts_coop_modes.append(coop_mode)

@register
class TensorStoreCoop(TensorStore):

   coop_modes = {1: [1, 2, 4], 2: [1, 2], 4: [1]}

   def __init__(self, test_helper: TestHelper = None, x31: Xreg = None, x31_val: int = None, enc: int = None, mem: Malloc = None):
      coop_mode = test_helper.pick_one(ts_coop_modes)
      self.__row_size  = test_helper.pick_one(self.coop_modes[coop_mode])
      x31_val = len(tr_active_minions) * (self.__row_size * 16)

      self.__mem_off = Xreg()
      self.__lhid = ABIreg("lhid")

      super().__init__.__wrapped__(self, test_helper, x31, x31_val, enc, mem, coop_mode, self.__row_size)

   def _tensor_conf(self, content_list: list, test_helper: TestHelper = None) -> None:
      # Evict Coalescing Buffer
      content_list.append(EvictCoalescingBuffer(test_helper=test_helper, threads_mask=0x1))

      super()._tensor_conf(content_list, test_helper)
      if TEST_CONF.THREAD_MASK < 0x3:
         imm = 16*self.__row_size
      else:
         imm = 8*self.__row_size
      insns = [ LI(rd=self.__mem_off, imm=imm),
                MUL(rd=self.__mem_off, rs1=self.__lhid, rs2=self.__mem_off),
                ADD(rd=self.rx3, rs1=self.rx3, rs2=self.__mem_off)
              ]
      content_list += insns

if len(ts_coop_modes) == 0 or TEST_CONF.POSTSI_L2_ONLY == True:
   TEST_CONF.EXCL_COMPS.add(TensorStoreCoop)
