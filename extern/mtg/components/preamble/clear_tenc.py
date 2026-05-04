
from core.mtg import *
from components.randinst.rv32i import LI
from components.tensor.tensor_mask import TensorMask
from components.tensor_wait import WAIT_TENSOR_LOAD_0, WAIT_TENSOR_LOAD_1, WAIT_TENSOR_FMA
from components.tensor.tensor_load import TensorLoad

class _TensorLoad(TensorLoad):
   MAX_RANDINST = 0

@register
class ClearTenC(Component, AsmCode):

   def set_tfma_from_scp(self, value: int):
      return set_bits(value, 20, 20, False)

   def set_tfma_from_tenb(self, value: int):
      return set_bits(value, 20, 20, True)

   def set_valid_tfma_op(self, rand_val: int, tenb: bool):
      if tenb:
          rand_val = self.set_tfma_from_tenb(rand_val)
      else:
          rand_val = self.set_tfma_from_scp(rand_val)

      tfma_op = (get_bits(rand_val, 3, 1))%3
      tfma_op = {
         0 : 0b000, # TensorFMA32
         1 : 0b001, # TensorFMA16A32
         2 : 0b011  # TensorIMA8A32
      }[tfma_op]
      rand_val = set_bits(rand_val, 3, 1, tfma_op)
      return rand_val

   def set_ima8a32(self, csr_value: int):
      csr_value = set_bits(csr_value,  3,  1, 0b011)
      csr_value = set_bits(csr_value, 63, 63,     0)
      return csr_value;

   def set_l1scp_start(self, csr_value: int):
      # matrix B
      csr_value = set_bits(csr_value,  17, 12, 0b000000)
      # matrix A
      csr_value = set_bits(csr_value,   9,  4, 0b000000)
      return csr_value

   def __init__(self, test_helper: TestHelper = None):
      csr_val_rand = test_helper.get_rand_num() & 0x7e0007ffff0c0c00
      tfma_scp_csr_base = self.set_valid_tfma_op(csr_val_rand, False)
      tfma_scp_csr_ima8 = self.set_ima8a32(tfma_scp_csr_base)
      tfma_scp_csr_ima8 = self.set_l1scp_start(tfma_scp_csr_ima8)
      self.tfma_scp_csr_val  = tfma_scp_csr_ima8 | 0x81F8800000000001

      self.rega = Xreg()
      self.regb = Xreg()
      self.zero = ABIreg('zero')

   @only_thread(0)
   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:

      # Dummy Tensor Load to avoid triggering sys_emu tensor load check error
      content_list.append(_TensorLoad(test_helper, enc = 0xf, x31_val = 0))

      # Tensor Wait for any previous tensor load
      content_list.append(WAIT_TENSOR_LOAD_0())
      content_list.append(WAIT_TENSOR_LOAD_1())

      content_list.append(TensorMask(test_helper=test_helper, value=0))
      content_list.append(LI(rd=self.rega, imm=self.tfma_scp_csr_val))
      # Write to the CSR to trigger the Tensor Load Operation 
      content_list.append("csrrw {zero}, tensor_fma, {rega}")

      # Tensor Wait for the TFMA
      content_list.append(WAIT_TENSOR_FMA())
