
from core.mtg import *

from components.randinst.rv32i import *
from components.tensor_wait import WaitAllTensors

@register
class ClearTensorError(Component, AsmCode):

   def __init__(self, test_helper: TestHelper = None, num_sets: int = None, num_ways: int = None):

      self.zero = ABIreg('zero')


   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:

      content_list.append(WaitAllTensors())
      content_list.append('csrrw {zero}, tensor_error, {zero}')

