
from core.mtg import *
from components.randinst.mask import MOVA_M_X

@register
class MovaMX(Component, AsmCode):

   def __init__(self, test_helper: TestHelper = None):
      self.rx0 = Xreg()

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(MOVA_M_X(test_helper = test_helper))
