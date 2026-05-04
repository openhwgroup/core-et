from core.models.test_helper import TestHelper
from core.models.tensormodel import set_bits, get_bits, randomize_scp_lines, TensorEncoding
from core.models.l1scpmodel import L1SCPModel
from core.models.fpmodel import FPModel
from core.models.resource import ResourceState
from core.test_conf import TEST_CONF

LAST = 0
INT32_TO_FP32 = 1
FP32_TO_INT32 = 2
RELU = 3
INT32_ADD_ROW = 4
INT32_ADD_COL = 5
FP32_MUL_ROW = 6
FP32_MUL_COL = 7
SATINT8 = 8
SATUINT8 = 9
PACK_128B = 10

class TensorQuantEncoding(TensorEncoding):

   valid_trans = [LAST, INT32_TO_FP32, FP32_TO_INT32, RELU, INT32_ADD_ROW, INT32_ADD_COL, FP32_MUL_ROW, FP32_MUL_COL, SATINT8, SATUINT8, PACK_128B]
   invalid_trans = [11, 12, 13, 14, 15]

   def __init__(self, value = None):
      self.name = self.get_name()
      self._vpu_regs = set()
      self._scp_lines = set()
      super().__init__(value = value)

   # Getters / Setters
   ###################################################

   @classmethod
   def get_name(cls) -> str:
      return f'TQUANT'

   def get_fstart(self, tq_csr_val: int) -> int:
      return get_bits(tq_csr_val, 61, 57)

   def set_fstart(self, tq_csr_val: int, value: int) -> int:
      return set_bits(tq_csr_val, 61, 57, value)

   def get_cols(self, tq_csr_val: int) -> int:
      return get_bits(tq_csr_val, 56, 55)

   def set_cols(self, tq_csr_val: int, value: int) -> int:
      return set_bits(tq_csr_val, 56, 55, value)

   def get_rows(self, tq_csr_val: int) -> int:
      return get_bits(tq_csr_val, 54, 51)

   def set_rows(self, tq_csr_val: int, value: int) -> int:
      if TEST_CONF.RTLMIN_WORKAROUNDS['fv-260'] == True:
         assert value > 7, f"FV-260 bug requires rows to be > 7. Current rows is {value}."
      return set_bits(tq_csr_val, 54, 51, value)

   def get_line(self, tq_csr_val: int) -> int:
      return get_bits(tq_csr_val, 50, 45)

   def set_line(self, tq_csr_val: int, value: int) -> int:
      return set_bits(tq_csr_val, 50, 45, value)

   def get_n_trans(self, tq_csr_val: int, n: int) -> int:
      assert 0 <= n and n < 10
      return get_bits(tq_csr_val, (n*4)+3, n*4)

   def set_n_trans(self, tq_csr_val: int, n: int, value: int) -> int:
      assert 0 <= n and n < 10
      return set_bits(tq_csr_val, (n*4)+3, n*4, value)

   ###################################################

   @classmethod
   def free_resources(cls, model: 'Model', name: str, from_tensor_wait: bool = False) -> None:
      l1_scp = model.l1scp
      vpu_rf = model.floatpoint
      # Free all the resources used by this same operation as HW cannot execute
      # the same tensor operation at the same time (implicit tensor wait)
      if from_tensor_wait:
         l1_scp.free_resources(name)
         model.pending_tensor_quant = False
      vpu_rf.free_resources(name)

   def set_valid_tquant_op(self, test_helper: TestHelper, rand_val: int):
       n_trans = test_helper.get_rand_range(0, 10)
       for i in range(10):
           if i < n_trans:
               rand_trans_val = test_helper.pick_one(self.valid_trans[1:])
               rand_val = self.set_n_trans(rand_val, i, rand_trans_val)
           elif i == n_trans:
               rand_trans_val = LAST
               rand_val = self.set_n_trans(rand_val, i, rand_trans_val)
               break
       return rand_val

   def get_tquant_wrapper(self, test_helper: TestHelper):
      csr_val_rand = test_helper.get_urand_num()
      tq_csr_val = self.set_valid_tquant_op(test_helper, csr_val_rand)
      return tq_csr_val

   def get_empty_vpurf_range(self, test_helper: TestHelper, vpu_rf: FPModel) -> (int, int):
      free_regions = vpu_rf.get_write_free_regions()
      if (TEST_CONF.RTLMIN_WORKAROUNDS['fv-260'] == True):
         free_regions = [(x, y) for x, y in free_regions if (y-x) >= 16]
      try:
         region = test_helper.pick_one(free_regions)
      except IndexError as exc:
         vpu_rf.print_all()
         raise Exception(f"TensorQuant failed to allocate VPU RF resources") from exc
      return region

   def get_empty_l1scp_range(self, test_helper: TestHelper, l1_scp: L1SCPModel) -> (int, int):
      try:
         free_regions = l1_scp.get_read_free_regions()
         # RTL does some L1SCP prefetching, even if the data is not consumed.
         # Consider only L1SCP regions with at least 2 free lines
         tquant_free_regions = [(x, y) for (x, y) in free_regions if x != y]
         region = test_helper.pick_one(tquant_free_regions)
         return region
      except IndexError as exc:
         l1_scp.print_all()
         raise Exception(f"{self.__class__.__name__} failed to get free L1 SCP regions.") from exc

   def _map_scp_vpu_regs(self, scp_method, vpurf_method, pack128b_method, tq_csr_val: int) -> None:
      fstart = self.get_fstart(tq_csr_val)
      cols   = (self.get_cols(tq_csr_val) + 1) * 4
      rows   = self.get_rows(tq_csr_val) + 1
      line   = self.get_line(tq_csr_val)

      for k in range(10):
          trans = self.get_n_trans(tq_csr_val, k)

          if trans == LAST:
             break

          elif trans in [INT32_TO_FP32, FP32_TO_INT32, RELU, SATINT8, SATUINT8]:
             for i in range(rows):
                for j in range(cols):
                   reg = (fstart + i*2 + int(j/8)) % 32
                   vpurf_method(reg)

          elif trans in [INT32_ADD_ROW, INT32_ADD_COL, FP32_MUL_ROW, FP32_MUL_COL]:
             for i in range(rows):
                for j in range(cols):
                   reg = (fstart + i*2 + int(j/8)) % 32
                   vpurf_method(reg)
                   scp_method(line)
             line = (line + 1) % 48

          elif trans == PACK_128B:
             for i in range(rows):
                for j in range(cols):
                   reg0 = (fstart + i*2) % 32
                   reg1 = (fstart + i*2 + int(j/8)) % 32
                   pack128b_method(reg0, reg1)
      if k > 0:
         scp_method(line)

   def selfcheck_vpurf_boundaries_tquant(self, tq_csr_val: int, rf_head: int, rf_tail: int) -> None:
      def scp_method(line: int) -> None:
         pass

      def vpurf_method(reg: int) -> None:
         assert rf_head <= reg and reg <= rf_tail

      def pack128b_method(reg0: int, reg1: int) -> None:
         assert rf_head <= reg0 and reg0 <= rf_tail
         assert rf_head <= reg1 and reg1 <= rf_tail

      self._map_scp_vpu_regs(scp_method, vpurf_method, pack128b_method, tq_csr_val)

   def constrain_tquant_write_vpu_rf(self, test_helper: TestHelper, tq_csr_val: int, rf_head: int, rf_tail: int) -> int:
      ret = tq_csr_val
      trans0 = self.get_n_trans(tq_csr_val, 0)
      if trans0 != LAST:
         if TEST_CONF.RTLMIN_WORKAROUNDS['fv-260'] == True:
            fstart   = test_helper.get_rand_range(rf_head, rf_tail-16)
            if self.tr_last_reg is not None and fstart == (self.tr_last_reg - 2):
               regs = list()
               if rf_head < (self.tr_last_reg - 2):
                  regs.append(test_helper.get_rand_range(rf_head, min(self.tr_last_reg - 3, rf_tail-16)))
               if self.tr_last_reg <= (rf_tail-16):
                  regs.append(test_helper.get_rand_range(self.tr_last_reg, rf_tail-16))
               fstart = test_helper.pick_one(regs)
            num_regs = rf_tail - fstart
            num_regs = min(num_regs, 22)
            num_regs = test_helper.get_rand_range(16, num_regs)
         else:
            fstart   = test_helper.get_rand_range(rf_head, rf_tail)
            num_regs = rf_tail - fstart
            num_regs = min(num_regs, 15)
            num_regs = test_helper.get_rand_range(0, num_regs)

         rows = int(num_regs/2)

         if (fstart + (rows*2)) < rf_tail:
            cols = test_helper.get_rand_range(0, 3)
         else:
            cols = test_helper.get_rand_range(0, 1)

         ret = self.set_fstart(ret, fstart)
         ret = self.set_cols(ret, cols)
         ret = self.set_rows(ret, rows)

      self.selfcheck_vpurf_boundaries_tquant(ret, rf_head, rf_tail)
      return ret

   def selfcheck_scp_boundaries_tquant(self, tq_csr_val: int, scp_head: int, scp_tail: int) -> None:
      def scp_method(line: int) -> None:
         assert scp_head <= line and line <= scp_tail

      def vpurf_method(reg: int) -> None:
         pass

      def pack128b_method(reg0: int, reg1: int) -> None:
         pass

      self._map_scp_vpu_regs(scp_method, vpurf_method, pack128b_method, tq_csr_val)

   def constrain_tquant_read_l1scp(self, test_helper: TestHelper, tq_csr_val: int, scp_head: int, scp_tail: int) -> int:
      ret = tq_csr_val
      # RTL does some L1SCP prefetching, even if the data is not consumed. This triggers L1SB errors
      # To avoid this, we reduce scp_tail by 1
      scp_tail = scp_tail - 1
      line = test_helper.get_rand_range(scp_head, scp_tail)
      ret = self.set_line(ret, line)

      vpu_only_trans = [INT32_TO_FP32, FP32_TO_INT32, RELU, SATINT8, SATUINT8]

      for k in range(10):
         trans = self.get_n_trans(tq_csr_val, k)

         if trans == LAST:
            break

         elif trans in [INT32_ADD_ROW, FP32_MUL_ROW, INT32_ADD_COL, FP32_MUL_COL]:
            if line > scp_tail:
               # If the L1SCP line is above the L1SCP limit, then we change it to a
               # trans that only reads/write the VPU RF
               trans = test_helper.pick_one(vpu_only_trans)
               ret = self.set_n_trans(ret, k, trans)

            # PRM-10 wraps around 48, but we are not considering L1SCP regions that can wrap around
            line = line + 1

         else:
            pass

      # Pass the real SCP tail (scp_tail+1)
      self.selfcheck_scp_boundaries_tquant(ret, scp_head, scp_tail + 1)
      return ret

   def selfcheck_set_tquant(self, l1_scp: 'Model', vpu_rf: 'Model', tq_csr_val: int, selfcheck: int, setm: int) -> None:
      assert selfcheck or setm

      def scp_method(line: int) -> None:
         if selfcheck == 1:
            assert l1_scp.is_free(line) or l1_scp.is_read_only(line)
         if setm == 1:
            if line not in self._scp_lines:
               l1_scp.set_read_only(line, self.name)
               self._scp_lines.add(line)

      def vpurf_method(reg: int) -> None:
         if selfcheck == 1:
            if reg not in self._vpu_regs:
               assert vpu_rf.is_free(reg)
         if setm == 1:
            if reg not in self._vpu_regs:
               vpu_rf.set_write_only(reg, self.name)
               self._vpu_regs.add(reg)

      def pack128b_method(reg0: int, reg1: int) -> None:
         if selfcheck == 1:
            if reg0 not in self._vpu_regs:
               assert vpu_rf.is_free(reg0)

            if reg1 not in self._vpu_regs:
               assert vpu_rf.is_free(reg1) or vpu_rf.is_read_only(reg1)

         if setm == 1:
            if reg0 not in self._vpu_regs:
               vpu_rf.set_write_only(reg0, self.name)
               self._vpu_regs.add(reg0)

            if reg1 not in self._vpu_regs:
               vpu_rf.set_read_only(reg1, self.name)
               self._vpu_regs.add(reg1)

      self._map_scp_vpu_regs(scp_method, vpurf_method, pack128b_method, tq_csr_val)

   def get_encoding(self, model: 'Model') -> int:

      test_helper = model.test_helper
      l1_scp = model.l1scp
      vpu_rf = model.floatpoint

      self._vpu_regs = set()
      self._scp_lines = set()

      tq_csr_val = self.get_tquant_wrapper(test_helper)
      trans0 = self.get_n_trans(tq_csr_val, 0)

      if trans0 != LAST:
         self.tr_last_reg = model.tr_last_reg
         (vpurf_head, vpurf_tail) = self.get_empty_vpurf_range(test_helper, vpu_rf)
         (  scp_head,   scp_tail) = self.get_empty_l1scp_range(test_helper, l1_scp)
         tq_csr_val = self.constrain_tquant_write_vpu_rf(test_helper, tq_csr_val, vpurf_head, vpurf_tail)
         tq_csr_val = self.constrain_tquant_read_l1scp(test_helper, tq_csr_val, scp_head, scp_tail)

      return tq_csr_val

   def post_encoding(self, model: 'Model', tq_csr_val: int):
      l1_scp = model.l1scp
      vpu_rf = model.floatpoint
      # Self Check there is no potential L1SCP collisions and set the lines in the L1SCP/VPU RF models
      self.selfcheck_set_tquant(l1_scp, vpu_rf, tq_csr_val, 1, 1)
      model.pending_tensor_quant = True
