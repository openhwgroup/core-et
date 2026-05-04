from abc import ABCMeta, abstractmethod
from core.models.test_helper import TestHelper
from core.models.tensormodel import set_bits, get_bits, get_bits_signed, TensorEncoding

class TensorMaskEncoding(TensorEncoding):

   def __init__(self, value = None):
      self.name = self.get_name()
      super().__init__(value = value)

   @classmethod
   def get_name(cls) -> str:
      return f'TM'

   @classmethod
   def free_resources(cls, model: 'Model', name: str) -> None:
      pass

   def get_encoding(self, model: 'Model') -> int:
      return model.test_helper.get_rand_kbits(16)

   def post_encoding(self, model: 'Model', csr_val: int):
      model.tensor_mask = 0xffff & csr_val

class TensorConvEncoding(TensorEncoding, metaclass=ABCMeta):

   conv_size = False
   conv_ctrl = False

   def __init__(self, value = None):
      self.name = self.get_name()
      super().__init__(value = value)

   @classmethod
   def free_resources(cls, model: 'Model', name: str) -> None:
      pass

   def get_conv_tensor_mask(self, tensor_conv_size: int, tensor_conv_ctrl: int):
      # Signed integer
      srow     = self.get_srow(tensor_conv_size)
      scol     = self.get_scol(tensor_conv_size)
      rowstart = self.get_rowstart(tensor_conv_ctrl)
      colstart = self.get_colstart(tensor_conv_ctrl)

      # Unsigned integer
      nrow = self.get_nrow(tensor_conv_size)
      ncol = self.get_ncol(tensor_conv_size)

      tensor_mask = 0x0
      for i in range(16):
         row = rowstart + i*srow
         col = colstart + i*scol
         mbit = (row >= 0) and (row < nrow) and (col >= 0) and (col < ncol)
         tensor_mask = set_bits(tensor_mask, i, i, mbit)

      return tensor_mask

   # Signed integers
   def get_rowstart(self, tconv_size_csr_val: int) -> int:
      return get_bits_signed(tconv_size_csr_val, 47, 32)

   def set_rowstart(self, tconv_size_csr_val: int, value: int) -> int:
      return set_bits(tconv_size_csr_val, 47, 32, value)

   def get_colstart(self, tconv_size_csr_val: int) -> int:
      return get_bits_signed(tconv_size_csr_val, 15, 0)

   def set_colstart(self, tconv_size_csr_val: int, value: int) -> int:
      return set_bits(tconv_size_csr_val, 15, 0, value)

   def get_srow(self, tconv_ctrl_csr_val: int) -> int:
      return get_bits_signed(tconv_ctrl_csr_val, 63, 56)

   def set_srow(self, tconv_ctrl_csr_val: int, value: int) -> int:
      return set_bits(tconv_ctrl_csr_val, 63, 56, value)

   def get_scol(self, tconv_ctrl_csr_val: int) -> int:
      return get_bits_signed(tconv_ctrl_csr_val, 31, 24)

   def set_scol(self, tconv_ctrl_csr_val: int, value: int) -> int:
      return set_bits(tconv_ctrl_csr_val, 31, 24, value)

   # Unsigned integers
   def get_nrow(self, tconv_ctrl_csr_val: int) -> int:
      return get_bits(tconv_ctrl_csr_val, 47, 32)

   def set_nrow(self, tconv_ctrl_csr_val: int, value: int) -> int:
      return set_bits(tconv_ctrl_csr_val, 47, 32, value)

   def get_ncol(self, tconv_ctrl_csr_val: int) -> int:
      return get_bits(tconv_ctrl_csr_val, 15, 0)

   def set_ncol(self, tconv_ctrl_csr_val: int, value: int) -> int:
      return set_bits(tconv_ctrl_csr_val, 15, 0, value)

   @abstractmethod
   def get_encoding(self, model: 'Model') -> int:
      pass

   def post_encoding(self, model: 'Model', csr_val: int):
      assert self.conv_size ^ self.conv_ctrl

      if self.conv_size:
         model.tensor_conv_size = csr_val
      else:
         model.tensor_conv_ctrl = csr_val

      tensor_conv_size = model.tensor_conv_size
      tensor_conv_ctrl = model.tensor_conv_ctrl
      tm = self.get_conv_tensor_mask(tensor_conv_size, tensor_conv_ctrl)
      model.tensor_mask = tm

class TensorConvSizeEncoding(TensorConvEncoding):
   conv_size = True

   @classmethod
   def get_name(cls) -> str:
      return f'TCONV_SIZE'

   def get_encoding(self, model: 'Model') -> int:
      test_helper = model.test_helper

      tconv_size = test_helper.get_rand_kbits(64)

      # Get Tensor Convolution Ctrl parameters as reference
      tconv_ctrl = model.tensor_conv_ctrl
      rowstart = self.get_rowstart(tconv_ctrl)
      colstart = self.get_colstart(tconv_ctrl)

      srow = test_helper.get_rand_range(-128, 127)
      scol = test_helper.get_rand_range(-128, 127)
      def get_biased_value(ref):
         ref = max(0, ref)
         select = [(0, ref), (ref, ref*2), (ref*2, 0xffff)]
         bias   = [       3,           85,              25]
         biased_range = test_helper.pick_one_weight(select, bias)
         return test_helper.get_rand_range(biased_range[0], biased_range[1])
      nrow = get_biased_value(rowstart)
      ncol = get_biased_value(colstart)

      assert -128 <= srow and srow <= 127
      assert -128 <= scol and scol <= 127
      assert 0 <= nrow and nrow <= 0xffff
      assert 0 <= ncol and ncol <= 0xffff

      tconv_size = self.set_srow(tconv_size, srow)
      tconv_size = self.set_scol(tconv_size, scol)
      tconv_size = self.set_nrow(tconv_size, nrow)
      tconv_size = self.set_ncol(tconv_size, ncol)

      return tconv_size

class TensorConvCtrlEncoding(TensorConvEncoding):
   conv_ctrl = True

   @classmethod
   def get_name(cls) -> str:
      return f'TCONV_CTRL'

   def get_encoding(self, model: 'Model') -> int:
      test_helper = model.test_helper

      tconv_ctrl = test_helper.get_rand_kbits(64)

      def get_biased_value():
         select = [(-32768, -1921), (-1920, 1919), (1920, 3840), (3841, 32767)]
         bias   = [              3,             3,           85,             3]
         biased_range = test_helper.pick_one_weight(select, bias)
         return test_helper.get_rand_range(biased_range[0], biased_range[1])

      rowstart = get_biased_value()
      colstart = get_biased_value()

      assert -32768 <= rowstart and rowstart <= 32767
      assert -32768 <= colstart and colstart <= 32767

      tconv_ctrl = self.set_rowstart(tconv_ctrl, rowstart)
      tconv_ctrl = self.set_colstart(tconv_ctrl, colstart)

      return tconv_ctrl
