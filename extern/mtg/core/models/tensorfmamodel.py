from core.models.test_helper import TestHelper
from core.models.tensormodel import set_bits, get_bits, randomize_scp_lines, TensorEncoding
from core.models.l1scpmodel import L1SCPModel
from core.models.fpmodel import FPModel
from core.models.resource import ResourceState
from core.from_future import FromFuture
from core.test_conf import TEST_CONF

FV260_TENA_AROWS = 3 # FV-260 TFMA TenA cannot use arrows == 3
FV260_TENB_AROWS = 4 # FV-260 TFMA TenB requires arrows >= 4

TFMA32    = 0b000
TFMA16A32 = 0b001
TIMA8A32  = 0b011

class TensorFMAEncoding(TensorEncoding):

   valid_config = [TFMA32, TFMA16A32, TIMA8A32]
   invalid_config = [0b010, 0b100, 0b101, 0b110, 0b111]

   def __init__(self, value = None, tenb: int = None, op_type = None):
      self.name = self.get_name()

      # This variable is used to properly pair the TFMA TenB with a TL TenB
      # non-tenb TFMA's ignore this variable
      self.__tenb_a_num_cols = FromFuture()

      self.tenb = tenb
      self.op_type = op_type

      if op_type is not None:
         assert op_type in self.valid_config

      if value is not None:
         if tenb is not None:
            assert tenb == self.get_is_tenb(value)
         if op_type is not None:
            assert op_type == self.get_config(value)
         self.__tenb_a_num_cols.set_value(self.get_acols(value))

      self._vpu_regs = set()
      self._scp_lines = set()

      super().__init__(value = value)

   # Getters / Setters
   ###################################################

   @property
   def tenb_a_num_cols(self) -> FromFuture:
      return self.__tenb_a_num_cols

   @tenb_a_num_cols.setter
   def tenb_a_num_cols(self, a_num_cols: FromFuture) -> None:
      if self.value is not None and a_num_cols != None:
         assert self.get_acols(self.value) == a_num_cols
      self.__tenb_a_num_cols = a_num_cols

   @classmethod
   def get_name(cls) -> str:
      return f'TFMA'

   def get_use_tensor_mask(self, tfma_csr_val) -> int:
      return get_bits(tfma_csr_val, 63, 63)

   def set_use_tensor_mask(self, tfma_csr_val, value) -> int:
      if value == 1:
         assert self.tensor_mask is not None
      return set_bits(tfma_csr_val, 63, 63, value)

   def get_bcols(self, tfma_csr_val) -> int:
      return get_bits(tfma_csr_val, 56, 55)

   def set_bcols(self, tfma_csr_val, value) -> int:
      return set_bits(tfma_csr_val, 56, 55, value)

   def get_arows(self, tfma_csr_val) -> int:
      return get_bits(tfma_csr_val, 54, 51)

   def set_arows(self, tfma_csr_val, value) -> int:
      if (TEST_CONF.RTLMIN_WORKAROUNDS['fv-260'] == True) and (self.get_config(tfma_csr_val) != TIMA8A32):
         if self.get_is_tenb(tfma_csr_val) == 1:
            assert value >= FV260_TENB_AROWS, f"FV-260 bug requires arows to be >= {FV260_TENB_AROWS} for TenB. Current arows is {value}."
         else:
            assert value != FV260_TENA_AROWS, f"FV-260 bug requires arows to be != {FV260_TENA_AROWS}. Current arows is {value}."
      return set_bits(tfma_csr_val, 54, 51, value)

   def get_acols(self, tfma_csr_val) -> int:
      return get_bits(tfma_csr_val, 50, 47)

   def set_acols(self, tfma_csr_val, value) -> int:
      return set_bits(tfma_csr_val, 50, 47, value)

   def get_offset(self, tfma_csr_val) -> int:
      return get_bits(tfma_csr_val, 46, 43)

   def set_offset(self, tfma_csr_val, value) -> int:
      return set_bits(tfma_csr_val, 46, 43, value)

   def get_tenc2rf(self, tfma_csr_val) -> int:
      return get_bits(tfma_csr_val, 23, 23)

   def set_tenc2rf(self, tfma_csr_val, value) -> int:
      return set_bits(tfma_csr_val, 23, 23, value)

   def get_is_tenb(self, tfma_csr_val) -> int:
      return get_bits(tfma_csr_val, 20, 20)

   def set_is_tenb(self, tfma_csr_val, value) -> int:
      return set_bits(tfma_csr_val, 20, 20, value)

   def get_bstart(self, tfma_csr_val) -> int:
      return get_bits(tfma_csr_val, 17, 12)

   def set_bstart(self, tfma_csr_val, value) -> int:
      return set_bits(tfma_csr_val, 17, 12, value)

   def get_astart(self, tfma_csr_val) -> int:
      return get_bits(tfma_csr_val, 9, 4)

   def set_astart(self, tfma_csr_val, value) -> int:
      return set_bits(tfma_csr_val, 9, 4, value)

   def get_config(self, tfma_csr_val) -> int:
      return get_bits(tfma_csr_val, 3, 1)

   def set_config(self, tfma_csr_val, value) -> int:
      return set_bits(tfma_csr_val, 3, 1, value)

   def get_firstpass(self, tfma_csr_val) -> int:
      return get_bits(tfma_csr_val, 0, 0)

   def set_firstpass(self, tfma_csr_val, value) -> int:
      return set_bits(tfma_csr_val, 0, 0, value)

   ###################################################

   def tfma_get_empty_l1scp_range(self, test_helper: TestHelper, l1_scp: L1SCPModel, tfma_csr_val: int) -> (int, int):
      region = None
      free_regions = l1_scp.get_read_free_regions()
      if (TEST_CONF.RTLMIN_WORKAROUNDS['fv-260'] == True):
         if self.get_is_tenb(tfma_csr_val) == 1:
            free_regions = [(x, y) for x, y in free_regions if (y-x) >= FV260_TENB_AROWS]
         else:
            free_regions = [(x, y) for x, y in free_regions if (y-x) != FV260_TENA_AROWS]
      try:
         region = test_helper.pick_one(free_regions)
      except IndexError:
         l1_scp.print_all()
         Exception("Not enough space in the L1 SCP model")
      return region

   def tfma_get_empty_vpurf_range(self, test_helper: TestHelper, vpu_rf: FPModel) -> (int, int):
      free_regions = vpu_rf.get_write_free_regions()
      # TFMA always starts from fp register 0, return the first region
      region = free_regions[0]
      assert region[0] == 0
      return region

   def selfcheck_a_scp_boundaries_tfma(self, tfma_csr_val: int, scp_head: int, scp_tail: int):
      def nop(*args):
         pass
      def check_boundaries(scp_line: int):
         assert scp_head <= scp_line and scp_line <= scp_tail
      self._map_scp_vpu_regs(check_boundaries, nop, nop, tfma_csr_val)

   def selfcheck_b_scp_boundaries_tfma(self, tfma_csr_val: int, scp_head: int, scp_tail: int):
      def nop(*args):
         pass
      def check_boundaries(scp_line: int):
         assert scp_head <= scp_line and scp_line <= scp_tail
      self._map_scp_vpu_regs(nop, check_boundaries, nop, tfma_csr_val)

   def selfcheck_vpurf_boundaries_tfma(self, tfma_csr_val: int, rf_head: int, rf_tail: int):
      def nop(*args):
         pass
      def check_boundaries(reg: int):
         assert rf_head <= reg and reg <= rf_tail
      self._map_scp_vpu_regs(nop, nop, check_boundaries, tfma_csr_val)

   def constrain_tfma_a_read_lines(self, test_helper: TestHelper, tfma_csr_val: int, scp_head: int, scp_tail: int, scp_size: int = 48):
      ret = tfma_csr_val
      config = self.get_config(tfma_csr_val)
      valid_config = config in self.valid_config
      if valid_config:
         if (TEST_CONF.RTLMIN_WORKAROUNDS['fv-260'] == True) and (config != TIMA8A32) and (self.get_is_tenb(tfma_csr_val) == 1):
            (astart, arows) = randomize_scp_lines(test_helper, scp_head, scp_tail, tensor_mask = self.tensor_mask, scp_size = scp_size, min_size = FV260_TENB_AROWS)
         else:
            (astart, arows) = randomize_scp_lines(test_helper, scp_head, scp_tail, tensor_mask = self.tensor_mask, scp_size = scp_size)
            if (TEST_CONF.RTLMIN_WORKAROUNDS['fv-260'] == True) and (config != TIMA8A32) and (arows == FV260_TENA_AROWS):
               arows = arows - 1
         arows = min(arows, 10)
         ret = self.set_arows(ret, arows)
         ret = self.set_astart(ret, astart)
         self.selfcheck_a_scp_boundaries_tfma(ret, scp_head, scp_tail)

      return ret

   def constrain_tfma_b_read_lines(self, test_helper: TestHelper, tfma_csr_val: int, scp_head: int, scp_tail: int, scp_size: int = 48):
      ret = tfma_csr_val
      config = self.get_config(tfma_csr_val)
      valid_config = config in self.valid_config
      l1scpb = (self.get_is_tenb(tfma_csr_val) == 0)
      if valid_config and l1scpb:
         bstart = test_helper.get_rand_range(scp_head, scp_tail)
         acols  = test_helper.get_rand_range(0, scp_tail-bstart)
         ret = self.set_acols(ret, acols)
         ret = self.set_bstart(ret, bstart)
         self.selfcheck_b_scp_boundaries_tfma(ret, scp_head, scp_tail)
      return ret

   def constrain_tfma_vpu_write_rf(self, test_helper: TestHelper, tfma_csr_val: int, rf_head: int, rf_tail: int):
      arows = self.get_arows(tfma_csr_val) + 1
      config = self.get_config(tfma_csr_val)
      tenc2rf = self.get_tenc2rf(tfma_csr_val) == 1
   
      rf_min = rf_head + (arows-1)*2
   
      valid_config = config in self.valid_config
      if valid_config:
         if (config == TIMA8A32 and tenc2rf) or config != TIMA8A32:
            if rf_min > rf_tail:
               arows = int(1 + ((rf_tail - rf_head)/2))
               if (TEST_CONF.RTLMIN_WORKAROUNDS['fv-260'] == True) and (config != TIMA8A32) and (arows == FV260_TENB_AROWS):
                  arows = arows - 1
               tfma_csr_val = self.set_arows(tfma_csr_val, arows - 1)
               rf_min = rf_head + (arows-1)*2
               assert rf_min <= rf_tail

         margin = rf_tail - rf_min
         # Only 2 bits, max value is 0b11
         if margin == 0:
            bcols = 0
         elif margin == 1:
            bcols = test_helper.get_rand_range(0, 1)
         else:
            bcols = test_helper.get_rand_range(0, 3)
   
         tfma_csr_val = self.set_bcols(tfma_csr_val, bcols)
         self.selfcheck_vpurf_boundaries_tfma(tfma_csr_val, rf_head, rf_tail)

      return tfma_csr_val

   def _map_scp_vpu_regs(self, scpa_method, scpb_method, vpureg_method, tfma_csr_val: int):
      config = self.get_config(tfma_csr_val)
      l1scpb = (self.get_is_tenb(tfma_csr_val) == 0)
      switch_case = { 0 : 1, 1 : 2, 3 : 4 }
      mult = switch_case[config]
      tenc2rf = self.get_tenc2rf(tfma_csr_val) == 1
   
      valid_config = config in self.valid_config
      if valid_config:
         bcols  = (self.get_bcols(tfma_csr_val) + 1) * 4
         arows  = self.get_arows(tfma_csr_val) + 1
         acols  = (self.get_acols(tfma_csr_val) + 1) * mult
         bstart = self.get_bstart(tfma_csr_val)
         astart = self.get_astart(tfma_csr_val)
   
         k = 0
         while k < acols:
            if l1scpb:
               scp_line = int((bstart+(k/mult))%48)
               scpb_method(scp_line)
            i = 0
            while i < arows:
               if self.tensor_mask is None or get_bits(self.tensor_mask, i, i) == 1:
                  scp_line = (astart+i)%48
                  scpa_method(scp_line)
                  j = 0
                  while j < bcols:
                     if config == TIMA8A32:
                        if tenc2rf and ((k+4) == acols):
                           reg = int(i*2+j/8)
                           vpureg_method(reg)
                     else:
                        reg = int(i*2+j/8)
                        vpureg_method(reg)
                     j += 1
               i += 1
            k += mult

   def selfcheck_set_tfma(self, l1_scp: 'Model', vpu_rf: 'Model', tfma_csr_val: int, selfcheck: int, setm: int):
      # Make sure that at least one of the two options are set
      assert selfcheck or setm

      def scp_method(scp_line):
         if selfcheck == 1:
            assert l1_scp.is_free(scp_line) or l1_scp.is_read_only(scp_line)
         if setm == 1:
            if scp_line not in self._scp_lines:
               l1_scp.set_read_only(scp_line, self.name)
               self._scp_lines.add(scp_line)

      def vpureg_method(reg):
         if selfcheck == 1:
            if reg not in self._vpu_regs:
               assert vpu_rf.is_free(reg)
         if setm == 1:
            if reg not in self._vpu_regs:
               vpu_rf.set_write_only(reg, self.name)
               self._vpu_regs.add(reg)

      self._map_scp_vpu_regs(scp_method, scp_method, vpureg_method, tfma_csr_val)

   def tfma_from_scp(self, value):
      return self.set_is_tenb(value, 0)
   
   def tfma_from_tenb(self, value):
      return self.set_is_tenb(value, 1)

   def set_valid_tfma_op(self, test_helper: TestHelper, rand_val: int, tenb: int):
      if tenb == 0:
         rand_val = self.tfma_from_scp(rand_val)
      else:
         rand_val = self.tfma_from_tenb(rand_val)
      if self.op_type is None:
         tfma_op = test_helper.pick_one(self.valid_config)
         rand_val = self.set_config(rand_val, tfma_op)
      else:
         rand_val = self.set_config(rand_val, self.op_type)
      if self.tensor_mask != None:
         rand_val = self.set_use_tensor_mask(rand_val, 1)
      else:
         rand_val = self.set_use_tensor_mask(rand_val, 0)
      return rand_val

   def get_tfma_wrapper(self, test_helper: TestHelper):
      tfma_rand = test_helper.get_urand_num()
      tfma_csr_val = self.set_valid_tfma_op(test_helper, tfma_rand, 0)
      return tfma_csr_val
   
   def get_tfma_tenb_wrapper(self, test_helper: TestHelper):
      tfma_rand = test_helper.get_urand_num()
      tfma_csr_val = self.set_valid_tfma_op(test_helper, tfma_rand, 1)
      return tfma_csr_val

   @classmethod
   def free_resources(cls, model: 'Model', name: str, from_tensor_wait: bool = False) -> None:
      l1_scp = model.l1scp
      vpu_rf = model.floatpoint
      # Free all the L1SCP resources used by this same operation as HW cannot execute
      # the same tensor operation at the same time (implicit tensor wait)
      if from_tensor_wait:
         l1_scp.free_resources(name)
         model.pending_tensor_fma_tena = False
         model.pending_tensor_fma_tenb = False
         model.pending_tensor_fma_to_rf = False
      vpu_rf.free_resources(name)

   def get_encoding(self, model: 'Model') -> int:
      test_helper = model.test_helper
      l1_scp = model.l1scp
      vpu_rf = model.floatpoint

      self._vpu_regs = set()
      self._scp_lines = set()

      if TEST_CONF.USE_TENSOR_MASK_MODEL == 1:
         self.tensor_mask = test_helper.pick_one([None, model.tensor_mask])
      else:
         self.tensor_mask = test_helper.pick_one([None, 0xffff])

      if self.tenb == 1:
         tfma_csr_val = self.get_tfma_tenb_wrapper(test_helper)
         if self.__tenb_a_num_cols != None:
            acols = int(str(self.__tenb_a_num_cols))
            tfma_csr_val = self.set_acols(tfma_csr_val, acols)
      else:
         tfma_csr_val = self.get_tfma_wrapper(test_helper)

      config = self.get_config(tfma_csr_val)
      l1scpb = (self.get_is_tenb(tfma_csr_val) == 0)
      valid_config = config in self.valid_config
      if valid_config:

         # Make sure there is enough space in the VPU RF
         if vpu_rf.is_free(0):
            (vpu_head, vpu_tail) = self.tfma_get_empty_vpurf_range(test_helper, vpu_rf)
         else :
            #If there is no space left, force TFMA to be TIMA and write into TenC
            tfma_csr_val = self.set_config(tfma_csr_val, TIMA8A32)
            tfma_csr_val = self.set_tenc2rf(tfma_csr_val, 0)
            vpu_head = 0
            vpu_tail = 31

         # Matrix A
         (scp_head, scp_tail) = self.tfma_get_empty_l1scp_range(test_helper, l1_scp, tfma_csr_val)
         tfma_csr_val = self.constrain_tfma_a_read_lines(test_helper, tfma_csr_val, scp_head, scp_tail, scp_size=l1_scp.SCP_SIZE)
         if l1scpb:
            # Matrix B
            (scp_head, scp_tail) = self.tfma_get_empty_l1scp_range(test_helper, l1_scp, tfma_csr_val)
            tfma_csr_val = self.constrain_tfma_b_read_lines(test_helper, tfma_csr_val, scp_head, scp_tail, scp_size=l1_scp.SCP_SIZE)

         tfma_csr_val = self.constrain_tfma_vpu_write_rf(test_helper, tfma_csr_val, vpu_head, vpu_tail)

         if self.__tenb_a_num_cols == None:
            self.__tenb_a_num_cols.set_value(self.get_acols(tfma_csr_val))
         return tfma_csr_val

   def post_encoding(self, model: 'Model', tfma_csr_val: int):
      tm = self.get_use_tensor_mask(tfma_csr_val)
      if tm == 1:
         if TEST_CONF.USE_TENSOR_MASK_MODEL == 1:
            self.tensor_mask = model.tensor_mask
         else:
            self.tensor_mask = 0xffff
      else:
         self.tensor_mask = None

      l1_scp = model.l1scp
      vpu_rf = model.floatpoint
      # Self Check there is no potential L1SCP collisions and set the lines in the L1SCP/VPU RF models
      self.selfcheck_set_tfma(l1_scp, vpu_rf, tfma_csr_val, 1, 1)
      if self.get_is_tenb(tfma_csr_val) == 0:
         assert model.pending_tensor_fma_tenb == False
         model.pending_tensor_fma_tena = True
      else:
         assert model.pending_tensor_fma_tena == False
         model.pending_tensor_fma_tenb = True

      tfma_type = self.get_config(tfma_csr_val)
      if (tfma_type in [TFMA16A32, TFMA32]) or ((tfma_type == TIMA8A32) and (self.get_tenc2rf(tfma_csr_val) == 1)):
         model.pending_tensor_fma_to_rf = True
      else:
         model.pending_tensor_fma_to_rf = False
